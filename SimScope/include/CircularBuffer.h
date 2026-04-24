#pragma once
#include <vector>
#include <mutex> // Required for thread synchronization

/**
 * PILLAR 2: DETERMINISTIC MEMORY MANAGEMENT (Thread-Safe)
 * This class acts as the "Bridge" between the high-speed Producer (Hardware)
 * and the asynchronous Consumer (Math Engine).
 */
class CircularBuffer {
private:
    // DATA STORAGE: Fixed-size vector to prevent runtime allocations/deallocations.
    std::vector<double> buffer;

    // INDEX MANAGEMENT: Standard "Head and Tail" pointers for O(1) performance.
    size_t head = 0;     // Write pointer (where new data enters)
    size_t tail = 0;     // Read pointer (where oldest data is retrieved)
    size_t count = 0;    // Tracks the current occupancy of the buffer
    size_t capacity;     // The total maximum samples allowed

    /**
     * MUTEX: The Gatekeeper
     * In a multi-threaded environment, this prevents "Data Races" where
     * the Producer and Consumer might access the same memory address simultaneously.
     */
    std::mutex mtx;

public:
    /**
     * CONSTRUCTOR: Pre-allocates memory to ensure deterministic performance.
     * @param size The total number of samples to hold in memory.
     */
    CircularBuffer(size_t size) : capacity(size) {
        buffer.resize(size, 0.0);
    }

    /**
     * PRODUCER METHOD: push
     * Adds a new sample to the buffer.
     * Uses std::lock_guard for "Resource Acquisition Is Initialization" (RAII)
     * thread safety—automatically unlocking when the function scope ends.
     */
    void push(double value) {
        std::lock_guard<std::mutex> lock(mtx);

        buffer[head] = value;
        head = (head + 1) % capacity; // Modulo wrap-around logic

        if (count < capacity) {
            count++;
        }
        else {
            /**
             * OVERWRITE LOGIC:
             * If the buffer is full, we must move the 'tail' forward.
             * This ensures the tail always points to the "Chronological Oldest" sample.
             */
            tail = (tail + 1) % capacity;
        }
    }

    /**
     * CONSUMER METHOD: pop
     * Safely retrieves one sample from the buffer for processing.
     * @param value A reference where the sample will be stored if available.
     * @return True if a sample was retrieved; False if the buffer was empty.
     */
    bool pop(double& value) {
        std::lock_guard<std::mutex> lock(mtx);

        // Safety check to prevent reading from an empty queue
        if (count == 0) return false;

        value = buffer[tail];
        tail = (tail + 1) % capacity; // Advance the read pointer
        count--;

        return true;
    }

    /**
     * ANALYSIS METHOD: getAverage
     * Calculates the mean of the entire current buffer state.
     * Note: This is an O(n) operation and should be called sparingly.
     */
    double getAverage() {
        std::lock_guard<std::mutex> lock(mtx);

        if (count == 0) return 0.0;

        double sum = 0;
        for (double val : buffer) sum += val;

        return sum / capacity;
    }

    /**
     * UTILITY: Returns the fixed capacity of the buffer.
     */
    size_t getSize() const { return capacity; }
};
