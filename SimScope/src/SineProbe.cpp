#include "SineProbe.h"
#include <cmath> // For std::sin

namespace SimScope {

    // Initialize m_time in the constructor
    SineProbe::SineProbe() : m_time(0.0) {}

    double SineProbe::readVoltage() {
        m_time += m_frequency;
        return std::sin(m_time);
    }

    const char* SineProbe::getProbeName() const {
        return "Simulated Sine Wave Probe";
    }

}
