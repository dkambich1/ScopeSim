#include <iostream>
#include "Probe.h"
#include "SineProbe.h"

int main() {
    // Polymorphism: Using a Base pointer for a Child object
    Probe* myProbe = new SineProbe();

    std::cout << "--- " << myProbe->getProbeName() << " Online ---" << std::endl;

    // Simulate 10 readings
    for (int i = 0; i < 10; i++) {
        std::cout << "Reading " << i << ": " << myProbe->readVoltage() << " V" << std::endl;
    }

    delete myProbe; // Manual cleanup (we'll use Smart Pointers later)
    return 0;
}
