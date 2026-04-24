/**
 * SineProbe.cpp - Implementation of the Simulated Hardware
 *
 * This file contains the "Business Logic" of the signal generation.
 * By keeping this in a .cpp file, we reduce compilation dependencies
 * and follow the principle of Encapsulation.
 */

#include "SineProbe.h"
#include <cmath> // Required for trigonometric functions (std::sin)

namespace SimScope {

    /**
     * CONSTRUCTOR: Initialization
     * Resets the internal simulation clock to zero to ensure
     * predictable signal start-up.
     */
    SineProbe::SineProbe() : m_time(0.0) {}

    /**
     * CORE SIMULATION LOGIC: readVoltage
     * Mimics an Analog-to-Digital Converter (ADC) reading.
     *
     * Logic:
     * 1. Increments the phase (time) by the predefined frequency step.
     * 2. Calculates the Sine of that phase.
     * 3. Returns a value typically ranging from -1.0 to +1.0.
     */
    double SineProbe::readVoltage() {
        // Advance the internal 'clock' of the wave
        m_time += m_frequency;

        // Return the calculated instantaneous voltage
        return std::sin(m_time);
    }

    /**
     * HARDWARE IDENTIFICATION: getProbeName
     * Returns a human-readable identifier. In professional instrumentation,
     * this helps the system map data to the correct channel or probe type.
     */
    const char* SineProbe::getProbeName() const {
        return "Simulated Sine Wave Probe";
    }

}
