#pragma once
#include "Probe.h"
#include <cmath>

// This class simulates a real physical probe
class SineProbe : public Probe {
private:
    double time = 0.0;

public:
    double readVoltage() override {
        time += 0.1;
        return std::sin(time); // Generates a smooth waveform
    }

    std::string getProbeName() const override {
        return "Internal Sine Wave Simulator";
    }
};
#pragma once
