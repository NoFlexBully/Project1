#ifndef GRAPH_MANIFOLD_HPP
#define GRAPH_MANIFOLD_HPP

#include "Rational.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>
#include <algorithm>

struct GraphNode {
    uint32_t id{0};
    std::string label;
    Rational base_resistance{1, 1};
    bool is_committed{false};

    GraphNode() = default;
    GraphNode(uint32_t i, std::string l, Rational r, bool committed = false)
        : id(i), label(std::move(l)), base_resistance(r), is_committed(committed) {}
};

struct GraphEdge {
    uint32_t target_node_id{0};
    Rational admittance_cost{1, 1};

    GraphEdge() = default;
    GraphEdge(uint32_t target, Rational cost)
        : target_node_id(target), admittance_cost(cost) {}
};

class GraphManifold {
public:
    GraphManifold() = default;

    void AddNode(uint32_t id, const std::string& label, Rational resistance = Rational(1, 1)) {
        nodes_[id] = GraphNode(id, label, resistance, false);
    }

    void Connect(uint32_t from, uint32_t to, Rational cost) {
        adjacency_[from].emplace_back(to, cost);
    }

    [[nodiscard]] const GraphNode* GetNode(uint32_t id) const noexcept {
        auto it = nodes_.find(id);
        if (it != nodes_.end()) return &it->second;
        return nullptr;
    }

    [[nodiscard]] const std::vector<GraphEdge>* GetEdges(uint32_t from) const noexcept {
        auto it = adjacency_.find(from);
        if (it != adjacency_.end()) return &it->second;
        return nullptr;
    }

    [[nodiscard]] std::optional<Rational> Traverse(uint32_t from, uint32_t to) const noexcept {
        auto edges = GetEdges(from);
        if (!edges) return std::nullopt;
        for (const auto& edge : *edges) {
            if (edge.target_node_id == to) {
                return edge.admittance_cost;
            }
        }
        return std::nullopt;
    }

    bool SeverEdge(uint32_t from, uint32_t to) {
        auto it = adjacency_.find(from);
        if (it == adjacency_.end()) return false;

        auto& edges = it->second;
        auto edge_it = std::remove_if(edges.begin(), edges.end(),
            [to](const GraphEdge& e) { return e.target_node_id == to; });

        if (edge_it != edges.end()) {
            edges.erase(edge_it, edges.end());
            return true;
        }
        return false;
    }

    bool ForgeEdge(uint32_t from, uint32_t to, Rational cost) {
        if (Traverse(from, to).has_value()) {
            return false;
        }
        adjacency_[from].emplace_back(to, cost);
        return true;
    }

    void MarkCommitted(uint32_t id) noexcept {
        auto it = nodes_.find(id);
        if (it != nodes_.end()) {
            it->second.is_committed = true;
        }
    }

    [[nodiscard]] bool IsCommitted(uint32_t id) const noexcept {
        auto it = nodes_.find(id);
        if (it != nodes_.end()) {
            return it->second.is_committed;
        }
        return false;
    }

private:
    std::unordered_map<uint32_t, GraphNode> nodes_;
    std::unordered_map<uint32_t, std::vector<GraphEdge>> adjacency_;
};

#endif // GRAPH_MANIFOLD_HPP
