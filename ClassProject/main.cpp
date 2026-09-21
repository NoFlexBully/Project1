#include "CognitiveEngine.h"
#include <iostream>

int main() {
    std::cout << "[DRIVER] Instantiating primary and auxiliary engine instances...\n";

    // 1. Instantiating two objects (default & parameterized)
    CognitiveEngine primaryEngine;
    CognitiveEngine secondaryEngine("CE-ALPHA", 1, Triad{.T = 2.0, .S = 1.0, .E = 0.85});

    // 2. Invoke setters on primary
    primaryEngine.setEngineTag("CE-RESONANCE-01");
    
    // 3. Exercise domain member functions
    std::cout << "[DRIVER] Applying kinematic rotation to primary...\n";
    primaryEngine.applyKinematicRotation();

    std::cout << "[DRIVER] Applying dissipation decay to primary...\n";
    primaryEngine.applyDissipationDecay(0.12);

    std::cout << "[DRIVER] Testing manifold edge traversal (0 -> 1)...\n";
    primaryEngine.transitionTo(1);

    std::cout << "[DRIVER] Testing invalid traversal (1 -> 0, unlinked)...\n";
    primaryEngine.transitionTo(0);

    // 4. Access state via Getters
    std::cout << "\n--- Direct Getters Inspection ---\n";
    std::cout << "Primary Tag:        " << primaryEngine.getEngineTag() << "\n";
    std::cout << "Current Node Locus: " << primaryEngine.getCurrentLocus() << "\n";
    std::cout << "Current Label:      " << primaryEngine.getCurrentNodeLabel() << "\n";
    std::cout << "Energy Metric:      " << primaryEngine.getMetricEnergy() << "\n";

    // 5. Emit formatted telemetry reports for both objects
    primaryEngine.emitTelemetryReport();
    secondaryEngine.emitTelemetryReport();

    return 0;
}