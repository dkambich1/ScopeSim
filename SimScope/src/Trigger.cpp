#include "Trigger.h"

namespace SimScope {

    // FIX: Must match the header signature (double, TriggerEdge)
    Trigger::Trigger(double threshold, TriggerEdge edge)
        : m_threshold(threshold), m_edge(edge), m_lastSample(0.0)
    {
    }

    bool Trigger::isFired(double currentSample) {
        bool fired = false;

        if (m_edge == TriggerEdge::Rising) {
            fired = (currentSample > m_threshold && m_lastSample <= m_threshold);
        }
        else {
            fired = (currentSample < m_threshold && m_lastSample >= m_threshold);
        }

        m_lastSample = currentSample;
        return fired;
    }

    void Trigger::setThreshold(double level) { m_threshold = level; }
    double Trigger::getThreshold() const { return m_threshold; }
    void Trigger::setEdge(TriggerEdge edge) { m_edge = edge; }
    TriggerEdge Trigger::getEdge() const { return m_edge; }
}
