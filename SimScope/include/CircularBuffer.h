#pragma once
#include <vector>
#include <mutex>

class CircularBuffer {
private:
    std::vector<double> buffer;
    size_t head = 0;
    size_t tail = 0;    // Added: Tracks where the consumer is reading
    size_t count = 0;   // Added: Tracks how many samples are currently in the buffer
    size_t capacity;
    std::mutex mtx;

public:
    CircularBuffer(size_t size) : capacity(size) {
        buffer.resize(size, 0.0);
    }

    void push(double value) {
        std::lock_guard<std::mutex> lock(mtx);
        buffer[head] = value;
        head = (head + 1) % capacity;

        if (count < capacity) {
            count++;
        }
        else {
            // Overwriting oldest data: move tail forward to stay at the "new" oldest
            tail = (tail + 1) % capacity;
        }
    }

    // NEW: Pops a single sample out for the Math Engine
    bool pop(double& value) {
        std::lock_guard<std::mutex> lock(mtx);
        if (count == 0) return false; // Buffer is empty

        value = buffer[tail];
        tail = (tail + 1) % capacity;
        count--;
        return true;
    }

    double getAverage() {
        std::lock_guard<std::mutex> lock(mtx);
        if (count == 0) return 0.0;
        double sum = 0;
        for (double val : buffer) sum += val;
        return sum / capacity;
    }

    size_t getSize() const { return capacity; }
};