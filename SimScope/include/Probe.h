#pragma once
namespace SimScope {
    class Probe {
    public:
        virtual ~Probe() = default;
        virtual double readVoltage() = 0;
        virtual const char* getProbeName() const = 0; // Added because SineProbe uses it
    };
}
