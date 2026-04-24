#pragma once
#include <string>

// The Abstract Base Class (The Interface)
class Probe {
public:
    virtual ~Probe() {} // Virtual destructor for safe cleanup

    // Pure Virtual Functions (= 0) make this class Abstract
    virtual double readVoltage() = 0;
    virtual std::string getProbeName() const = 0;
};
#pragma once
