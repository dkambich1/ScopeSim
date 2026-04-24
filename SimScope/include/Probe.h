#pragma once

/**
 * PILLAR 1: HARDWARE ABSTRACTION LAYER (HAL)
 * The Probe class serves as an Abstract Base Class (Interface).
 * It defines the "contract" that any physical or simulated probe must fulfill.
 */
namespace SimScope {

    class Probe {
    public:
        /**
         * VIRTUAL DESTRUCTOR:
         * Essential for polymorphic cleanup. Since main() handles Probe* pointers,
         * a virtual destructor ensures that the derived class (like SineProbe)
         * is properly destroyed to prevent memory leaks.
         */
        virtual ~Probe() = default;

        /**
         * PURE VIRTUAL METHOD: readVoltage
         * This is the primary data source for the acquisition pipeline.
         * The '= 0' makes this class Abstract; it cannot be instantiated directly.
         * @return The current voltage level as a double.
         */
        virtual double readVoltage() = 0;

        /**
         * METADATA INTERFACE: getProbeName
         * Provides identification for the measurement system. In a professional
         * oscilloscope, this would return details like Model Number or Serial.
         * @return A C-style string representing the probe's identity.
         */
        virtual const char* getProbeName() const = 0;
    };

}