#include <iostream>
#include <thread>
#include <vector>
#include <functional> 
#include <sstream> 
#include <atomic>
#include <fstream> 
#include "Probe.h"
#include "SineProbe.h"
#include "CircularBuffer.h"
#include "Trigger.h"

using namespace SimScope; // This connects all the dots

std::atomic<bool> keepRunning(true);

// Use the clean names now that 'using namespace' is active
void captureData(Probe* probe, CircularBuffer& buffer) {
    while (keepRunning) {
        buffer.push(probe->readVoltage());
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void processData(CircularBuffer& buffer, Trigger& trigger) {
    const int FRAME_SIZE = 40;
    std::vector<double> frame;
    bool isCapturing = false;
    int frameCount = 0; // To name our files

    while (keepRunning) {
        double val;
        if (buffer.pop(val)) {
            if (!isCapturing) {
                if (trigger.isFired(val)) {
                    isCapturing = true;
                    frame.push_back(val);
                }
            }
            else {
                frame.push_back(val);
                if (frame.size() >= FRAME_SIZE) {
                    // 1. Calculate Math
                    double sum = 0;
                    for (double s : frame) sum += s;

                    std::cout << "[Math Engine] Triggered! | Avg: " << (sum / FRAME_SIZE) << " V" << std::endl;

                    // 2. EXPORT TO CSV (Only for the first 5 frames so we don't spam the disk)
                    if (frameCount < 5) {
                        std::string name = "data/wave_capture_" + std::to_string(frameCount) + ".csv";
                        std::ofstream file(name);
                        file << "SampleIndex,Voltage\n";
                        for (int i = 0; i < frame.size(); ++i) file << i << "," << frame[i] << "\n";
                        std::cout << ">>> Saved Waveform to " << name << std::endl;
                        frameCount++;
                    }

                    frame.clear();
                    isCapturing = false;
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                }
            }
        }
    }

    std::cout << "Analysis Thread Stopped.\n";
}

int main() {
    Probe* myProbe = new SineProbe();
    CircularBuffer myBuffer(100);
    Trigger myTrigger(0.0);

    std::thread producer(captureData, myProbe, std::ref(myBuffer));
    std::thread consumer(processData, std::ref(myBuffer), std::ref(myTrigger));

    std::cout << "SimScope Running. Press Enter to Stop." << std::endl;
    std::cin.get();

    keepRunning = false;
    producer.join();
    consumer.join();
    delete myProbe;
    return 0;
}
