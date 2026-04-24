/**
 * Project: SimScope - C++ Multi-threaded Oscilloscope Simulator
 * Purpose: Demonstrates high-performance data acquisition using a
 *          Producer-Consumer architecture and edge-triggering logic.
 */

#include <iostream>
#include <thread>
#include <vector>
#include <functional>
#include <sstream>
#include <atomic>   // Required for thread-safe flag signaling
#include <fstream>  // Required for CSV file persistence
#include <filesystem> // (Optional) For automated directory management

 // Project-specific headers
#include "Probe.h"
#include "SineProbe.h"
#include "CircularBuffer.h"
#include "Trigger.h"

using namespace SimScope;

/**
 * ATOMIC FLAG: keepRunning
 * Standard practice for multi-threaded systems to ensure a graceful shutdown.
 * Using std::atomic prevents memory visibility issues (data races) between threads.
 */
std::atomic<bool> keepRunning(true);

/**
 * PILLAR 3: THE PRODUCER (Data Acquisition)
 * Mimics high-speed hardware sampling. This thread's only job is to move data
 * from the HAL (Probe) to the Memory Layer (CircularBuffer) as fast as possible.
 */
void captureData(Probe* probe, CircularBuffer& buffer) {
    while (keepRunning) {
        // High-speed polling of the simulated hardware
        double val = probe->readVoltage();

        // Thread-safe push into the shared memory buffer
        buffer.push(val);

        // Simulate a 100Hz sampling rate (10ms intervals)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << "[System] Capture Thread Stopped.\n";
}

/**
 * PILLAR 3: THE CONSUMER (Math Engine & Data Logging)
 * Acts as the 'Brain' of the scope. It processes raw data asynchronously
 * from the capture thread to ensure the UI/Analysis remains non-blocking.
 */
void processData(CircularBuffer& buffer, Trigger& trigger) {
    // Record Length: Number of samples representing one 'screen' of data
    const int FRAME_SIZE = 40;
    std::vector<double> frame;

    // State Machine Flags: Manages the transition from 'Hunting' to 'Capturing'
    bool isCapturing = false;
    int frameCount = 0;

    while (keepRunning) {
        double val;

        // PILLAR 2: Thread-Safe Pop
        // Attempt to extract the oldest sample from the memory layer
        if (buffer.pop(val)) {

            if (!isCapturing) {
                /**
                 * TRIGGER LOGIC: Hunting Mode
                 * The system ignores all data until the Trigger identifies a Rising/Falling edge.
                 * This is what creates a 'stable' waveform on an oscilloscope display.
                 */
                if (trigger.isFired(val)) {
                    isCapturing = true;
                    frame.push_back(val);
                }
            }
            else {
                /**
                 * CAPTURE MODE: Packetizing Data
                 * Once triggered, the system fills a fixed-size frame for analysis.
                 */
                frame.push_back(val);

                if (frame.size() >= FRAME_SIZE) {
                    // STEP 1: MATH PROCESSING
                    // Calculate the Mean/DC Offset of the current waveform snapshot
                    double sum = 0;
                    for (double s : frame) sum += s;
                    std::cout << "[Math Engine] Triggered! | Avg: " << (sum / FRAME_SIZE) << " V" << std::endl;

                    // STEP 2: DATA PERSISTENCE (I/O)
                    // Exporting to CSV allows for external verification (e.g., Excel/MATLAB)
                    if (frameCount < 5) {
                        std::string name = "data/wave_capture_" + std::to_string(frameCount) + ".csv";
                        std::ofstream file(name);

                        file << "SampleIndex,Voltage\n"; // CSV Header
                        for (int i = 0; i < (int)frame.size(); ++i) {
                            file << i << "," << frame[i] << "\n";
                        }

                        std::cout << ">>> Saved Waveform to " << name << std::endl;
                        frameCount++;
                    }

                    // RESET STATE: Go back to 'Hunting' for the next trigger event
                    frame.clear();
                    isCapturing = false;

                    // Simulate a lower-priority UI refresh rate (10Hz)
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
        else {
            // Efficiency: If the buffer is empty, yield the CPU to the Producer thread
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
    std::cout << "[System] Analysis Thread Stopped.\n";
}

/**
 * APPLICATION ENTRY POINT
 * Responsible for Object Orchestration and Thread Lifecycle Management.
 */
int main() {
    // PILLAR 1: Instantiate simulated hardware via the HAL interface
    Probe* myProbe = new SineProbe();

    // PILLAR 2: Initialize deterministic memory (Circular Buffer)
    CircularBuffer myBuffer(100);

    // Initialize the Triggering Engine (Threshold = 0.0V, Rising Edge)
    Trigger myTrigger(0.0);

    /**
     * THREAD LAUNCH: std::thread
     * We pass shared objects by reference using std::ref() because they
     * contain non-copyable members like std::mutex.
     */
    std::thread producer(captureData, myProbe, std::ref(myBuffer));
    std::thread consumer(processData, std::ref(myBuffer), std::ref(myTrigger));

    std::cout << "SimScope Running. Press Enter to Stop." << std::endl;
    std::cin.get(); // Blocking call: Wait for user to hit Enter

    // SIGNAL SHUTDOWN: Set the atomic flag to false
    keepRunning = false;

    /**
     * THREAD JOINING
     * Ensures all background tasks are finished before the main program exits.
     * This prevents access violations and memory corruption.
     */
    if (producer.joinable()) producer.join();
    if (consumer.joinable()) consumer.join();

    // CLEANUP: Manual memory management of the HAL probe
    delete myProbe;

    return 0;
}
