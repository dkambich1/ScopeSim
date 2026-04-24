#pragma once

namespace SimScope {

    // Define the trigger modes clearly
    enum class TriggerEdge {
        Rising,
        Falling
    };

    class Trigger {
    public:
        explicit Trigger(double threshold = 0.0, TriggerEdge edge = TriggerEdge::Rising);

        // The core logic that now handles both directions
        bool isFired(double currentSample);

        // Getters and Setters
        void setThreshold(double level);
        double getThreshold() const;

        void setEdge(TriggerEdge edge);
        TriggerEdge getEdge() const;

    private:
        double m_threshold;
        double m_lastSample;
        TriggerEdge m_edge;
    };

}