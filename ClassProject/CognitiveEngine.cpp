#include "CognitiveEngine.h"
#include <iostream>
#include <iomanip>
#include <cmath>

void CognitiveEngine::recordTrace(const std::string& entry) {
    sessionLog.push_back(entry);
}

// Default Constructor
CognitiveEngine::CognitiveEngine()
    : engineTag("CE-DEFAULT"),
      currentLocus(0),
      currentState(Triad{.T = 1.0, .S = 0.5, .E = 0.4}) {
    
    // Seed standard base manifold
    manifold.AddNode(0, "Stillness / Open Observation", Rational(1, 1));
    manifold.AddNode(1, "Deliberation / The Crucible", Rational(3, 2));
    manifold.AddNode(2, "Committed Action / The Crossing", Rational(1, 2));

    manifold.Connect(0, 1, Rational(1, 2));
    manifold.Connect(1, 2, Rational(3, 4));
    manifold.Connect(2, 0, Rational(1, 1));

    recordTrace("[INIT] CognitiveEngine default instance constructed.");
}

// Parameterized Constructor
CognitiveEngine::CognitiveEngine(const std::string& tag, uint32_t initialLocus, const Triad& initialState)
    : engineTag(tag),
      currentLocus(initialLocus),
      currentState(initialState) {

    manifold.AddNode(0, "Stillness / Open Observation", Rational(1, 1));
    manifold.AddNode(1, "Deliberation / The Crucible", Rational(3, 2));
    manifold.AddNode(2, "Committed Action / The Crossing", Rational(1, 2));

    manifold.Connect(0, 1, Rational(1, 2));
    manifold.Connect(1, 2, Rational(3, 4));
    manifold.Connect(2, 0, Rational(1, 1));

    recordTrace("[INIT] CognitiveEngine parameterized instance: " + tag);
}

// Getters
std::string CognitiveEngine::getEngineTag() const {
    return engineTag;
}

uint32_t CognitiveEngine::getCurrentLocus() const {
    return currentLocus;
}

Triad CognitiveEngine::getCurrentState() const {
    return currentState;
}

double CognitiveEngine::getMetricEnergy() const {
    return currentState.E;
}

std::string CognitiveEngine::getCurrentNodeLabel() const {
    auto nodeOpt = manifold.GetNode(currentLocus);
    if (nodeOpt.has_value()) {
        return nodeOpt->label;
    }
    return "UNKNOWN_LOCUS";
}

std::vector<std::string> CognitiveEngine::getSessionLog() const {
    return sessionLog;
}

// Setters
void CognitiveEngine::setEngineTag(const std::string& newTag) {
    engineTag = newTag;
    recordTrace("[MUTATE] Engine tag updated to: " + newTag);
}

bool CognitiveEngine::setCurrentLocus(uint32_t targetNodeId) {
    if (manifold.HasNode(targetNodeId)) {
        currentLocus = targetNodeId;
        recordTrace("[MUTATE] Locus forced to node ID: " + std::to_string(targetNodeId));
        return true;
    }
    recordTrace("[ERR] Failed to set invalid locus: " + std::to_string(targetNodeId));
    return false;
}

void CognitiveEngine::setCurrentState(const Triad& nextState) {
    currentState = nextState;
    recordTrace("[MUTATE] Triad coordinate modified directly.");
}

// Domain Operations
void CognitiveEngine::applyKinematicRotation() {
    currentState = rotator.Rotate(currentState);
    recordTrace("[KINEMATICS] SO(3) conservative rotation applied.");
}

void CognitiveEngine::applyDissipationDecay(double rate) {
    double decay = rate * std::tanh(currentState.E + 0.05);
    currentState.E = std::max(0.0, currentState.E - decay);
    currentState.T += 0.05; // temporal step
    recordTrace("[DISSIPATION] Tension dissipated by rate " + std::to_string(rate));
}

bool CognitiveEngine::transitionTo(uint32_t targetNodeId) {
    if (!manifold.HasNode(targetNodeId)) {
        recordTrace("[TRAVERSAL_REJECTED] Destination does not exist: " + std::to_string(targetNodeId));
        return false;
    }

    auto edges = manifold.GetEdges(currentLocus);
    bool connected = false;
    for (const auto& edge : edges) {
        if (edge.target_node_id == targetNodeId) {
            connected = true;
            break;
        }
    }

    if (connected) {
        currentLocus = targetNodeId;
        currentState.T += 0.10;
        currentState.E += 0.20; // transition energy cost
        recordTrace("[TRAVERSAL] Transitioned to locus: " + getCurrentNodeLabel());
        return true;
    }

    recordTrace("[TRAVERSAL_BLOCKED] No admittance edge from " + std::to_string(currentLocus) + " to " + std::to_string(targetNodeId));
    return false;
}

void CognitiveEngine::emitTelemetryReport() const {
    std::cout << "\n================================================================================\n"
              << "COGNITIVE ENGINE INSTANCE REPORT: " << engineTag << "\n"
              << "================================================================================\n"
              << "Current Node:    [" << currentLocus << "] " << getCurrentNodeLabel() << "\n"
              << "State Vector:    T: " << std::fixed << std::setprecision(2) << currentState.T
              << " | S: " << currentState.S << " | E: " << currentState.E << "\n"
              << "Total Energy E:  " << getMetricEnergy() << "\n"
              << "L2 Norm:         " << currentState.Norm() << "\n"
              << "--------------------------------------------------------------------------------\n"
              << "Journal Entries (" << sessionLog.size() << " logged):\n";
    for (const auto& line : sessionLog) {
        std::cout << "  " << line << "\n";
    }
    std::cout << "================================================================================\n\n";
}