#pragma once
#include <vector>
#include <mutex>

// PILLAR 2: Deterministic Memory Management
class CircularBuffer {
private:
    std::vector<double> buffer;
    size_t head = 0;
    size_t capacity;
    std::mutex mtx; // PILLAR 3: Thread Safety

public:
    CircularBuffer(size_t size) : capacity(size) {
        buffer.resize(size, 0.0);
    }

    // Adds a new sample, overwriting the oldest one
    void push(double value) {
        std::lock_guard<std::mutex> lock(mtx); // Protects the write
        buffer[head] = value;
        head = (head + 1) % capacity; // The "Wrap-around" logic
    }

    // Gets a sample at a specific index
    double get(size_t index) const {
        // Note: In a real scope, we'd use more complex indexing here, 
        // but for now, we'll just read raw positions.
        return buffer[index % capacity];
    }

    double getAverage() {
        std::lock_guard<std::mutex> lock(mtx); // Protect the read
        double sum = 0;
        for (double val : buffer) {
            sum += val;
        }
        return sum / capacity;
    }


    size_t getSize() const { return capacity; }
};
