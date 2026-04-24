#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include "SineProbe.h"
#include "CircularBuffer.h"

// Flag to safely stop threads
std::atomic<bool> keepRunning(true);

// THE PRODUCER: High-priority data capture
void captureData(Probe* probe, CircularBuffer& buffer) {
    while (keepRunning) {
        double val = probe->readVoltage();
        buffer.push(val);

        // Simulate hardware sampling rate (e.g., 100Hz)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << "Capture Thread Stopped.\n";
}

// THE CONSUMER: Lower-priority analysis/UI
void processData(CircularBuffer& buffer) {
    while (keepRunning) {
        // Just read the most recent sample for now
        double val = buffer.get(0);
        std::cout << "[Analysis] Current Voltage: " << val << " V" << std::endl;

        // Analysis runs slower than capture (e.g., 2Hz)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    std::cout << "Analysis Thread Stopped.\n";
}

int main() {
    Probe* myProbe = new SineProbe();
    CircularBuffer myBuffer(100);

    // Launch Threads
    std::thread producer(captureData, myProbe, std::ref(myBuffer));
    std::thread consumer(processData, std::ref(myBuffer));

    std::cout << "Oscilloscope Running... Press Enter to stop." << std::endl;
    std::cin.get(); // Wait for user to hit enter

    // Stop threads and cleanup
    keepRunning = false;
    producer.join();
    consumer.join();

    delete myProbe;
    return 0;
}
