#include <iostream>
#include "SineProbe.h"
#include "CircularBuffer.h"

int main() {
    Probe* myProbe = new SineProbe();
    CircularBuffer myBuffer(5); // Small buffer to easily see it "wrap"

    std::cout << "--- Circular Buffer Test ---" << std::endl;

    // We will push 10 samples into a 5-slot buffer
    for (int i = 0; i < 10; i++) {
        double val = myProbe->readVoltage();
        myBuffer.push(val);
        std::cout << "Pushed: " << val << std::endl;
    }

    std::cout << "\nBuffer Contents (should be the LAST 5 samples):" << std::endl;
    for (int i = 0; i < 5; i++) {
        std::cout << "Slot " << i << ": " << myBuffer.get(i) << " V" << std::endl;
    }

    delete myProbe;
    return 0;
}
