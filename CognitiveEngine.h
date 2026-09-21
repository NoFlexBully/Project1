#ifndef COGNITIVE_ENGINE_H
#define COGNITIVE_ENGINE_H

#include <string>
#include <vector>
#include <cstdint>
#include "Types.hpp"
#include "GraphManifold.hpp"
#include "Rotator.hpp"

class CognitiveEngine {
private:
    std::string engineTag;
    uint32_t currentLocus;
    Triad currentState;
    GraphManifold manifold;
    TriadicRotator rotator;
    std::vector<std::string> sessionLog;

    void recordTrace(const std::string& entry);

public:
    // Constructors
    CognitiveEngine();
    CognitiveEngine(const std::string& tag, uint32_t initialLocus, const Triad& initialState);

    // Getters / Accessors
    std::string getEngineTag() const;
    uint32_t getCurrentLocus() const;
    Triad getCurrentState() const;
    double getMetricEnergy() const;
    std::string getCurrentNodeLabel() const;
    std::vector<std::string> getSessionLog() const;

    // Setters / Mutators
    void setEngineTag(const std::string& newTag);
    bool setCurrentLocus(uint32_t targetNodeId);
    void setCurrentState(const Triad& nextState);

    // Domain Operations
    void applyKinematicRotation();
    void applyDissipationDecay(double rate = 0.08);
    bool transitionTo(uint32_t targetNodeId);
    void emitTelemetryReport() const;
};

#endif // COGNITIVE_ENGINE_H