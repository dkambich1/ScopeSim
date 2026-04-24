#pragma once
#include "Probe.h"

namespace SimScope {

    class SineProbe : public Probe {
    public:
        SineProbe(); // Constructor declaration

        // Method declarations (no bodies here!)
        double readVoltage() override;
        const char* getProbeName() const override;

    private:
        double m_time;
        const double m_frequency = 0.1; // Increment step for the wave
    };

}
