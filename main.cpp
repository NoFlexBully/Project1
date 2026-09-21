#include "CognitiveEngine.h"
#include <iostream>

int main() {
    std::cout << "[DRIVER] Instantiating primary and auxiliary engine instances..." << std::endl;

    CognitiveEngine primaryEngine;
    CognitiveEngine secondaryEngine("CE-ALPHA", 1, Triad{.T = 2.0, .S = 1.0, .E = 0.85});

    primaryEngine.setEngineTag("CE-RESONANCE-01");

    std::cout << "[DRIVER] Applying kinematic rotation to primary..." << std::endl;
    primaryEngine.applyKinematicRotation();

    std::cout << "[DRIVER] Applying dissipation decay to primary..." << std::endl;
    primaryEngine.applyDissipationDecay(0.12);

    std::cout << "[DRIVER] Testing manifold edge traversal (0 -> 1)..." << std::endl;
    primaryEngine.transitionTo(1);

    std::cout << "[DRIVER] Testing blocked traversal (1 -> 0)..." << std::endl;
    primaryEngine.transitionTo(0);

    std::cout << std::endl << "--- Direct Getters Inspection ---" << std::endl;
    std::cout << "Primary Tag:        " << primaryEngine.getEngineTag() << std::endl;
    std::cout << "Current Node Locus: " << primaryEngine.getCurrentLocus() << std::endl;
    std::cout << "Current Label:      " << primaryEngine.getCurrentNodeLabel() << std::endl;
    std::cout << "Energy Metric:      " << primaryEngine.getMetricEnergy() << std::endl;

    primaryEngine.emitTelemetryReport();
    secondaryEngine.emitTelemetryReport();

    return 0;
}
