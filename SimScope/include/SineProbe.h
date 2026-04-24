#pragma once

#include "Probe.h"

namespace SimScope {

    /**
     * CONCRETE IMPLEMENTATION: SineProbe
     * Inherits from the Probe interface to simulate a periodic voltage signal.
     * This allows the rest of the system to be tested without physical hardware.
     */
    class SineProbe : public Probe {
    public:
        /**
         * CONSTRUCTOR:
         * Initializes the simulation state, specifically the time-tracking variable.
         */
        SineProbe();

        /**
         * POLYMORPHIC OVERRIDE: readVoltage
         * Calculates the next voltage point in a sine wave.
         * Labeled 'override' to ensure the compiler verifies it matches the base class signature.
         * @return Simulated voltage between -1.0V and 1.0V.
         */
        double readVoltage() override;

        /**
         * POLYMORPHIC OVERRIDE: getProbeName
         * Identifies this specific simulation instance to the data acquisition system.
         */
        const char* getProbeName() const override;

    private:
        // Internal clock state to track the phase of the sine wave
        double m_time;

        // The simulation step size; effectively controls the frequency of the wave
        const double m_frequency = 0.1;
    };

}
