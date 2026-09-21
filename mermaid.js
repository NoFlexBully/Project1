classDiagram
    class CognitiveEngine {
        -string engineTag
        -uint32_t currentLocus
        -Triad currentState
        -GraphManifold manifold
        -TriadicRotator rotator
        -vector~string~ sessionLog
        -recordTrace(string entry) void
        +CognitiveEngine()
        +CognitiveEngine(string tag, uint32_t initialLocus, Triad initialState)
        +getEngineTag() string
        +getCurrentLocus() uint32_t
        +getCurrentState() Triad
        +getMetricEnergy() double
        +getCurrentNodeLabel() string
        +getSessionLog() vector~string~
        +setEngineTag(string newTag) void
        +setCurrentLocus(uint32_t targetNodeId) bool
        +setCurrentState(Triad nextState) void
        +applyKinematicRotation() void
        +applyDissipationDecay(double rate) void
        +transitionTo(uint32_t targetNodeId) bool
        +emitTelemetryReport() void
    }

    class Triad {
        +double T
        +double S
        +double E
        +NormSquared() double
        +Norm() double
        +L1Norm() double
    }

    class GraphManifold {
        -unordered_map~uint32_t, GraphNode~ nodes
        -unordered_map~uint32_t, vector~GraphEdge~~ adjacency_list
        +AddNode(uint32_t id, string label, Rational res) void
        +Connect(uint32_t src, uint32_t dst, Rational cost) void
        +GetNode(uint32_t id) const GraphNode*
        +GetEdges(uint32_t id) const vector~GraphEdge~*
    }

    class TriadicRotator {
        +Rotate(Triad v) Triad
    }

    class GraphNode {
        +uint32_t id
        +string label
        +Rational base_resistance
        +bool is_committed
    }

    class GraphEdge {
        +uint32_t target_node_id
        +Rational admittance_cost
    }

    CognitiveEngine *-- Triad : contains
    CognitiveEngine *-- GraphManifold : contains
    CognitiveEngine *-- TriadicRotator : contains
    GraphManifold o-- GraphNode : aggregates
    GraphManifold o-- GraphEdge : aggregates