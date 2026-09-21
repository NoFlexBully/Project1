#ifndef TYPES_HPP
#define TYPES_HPP

#include <cmath>
#include <string>
#include <cstdint>

struct Triad {
    double T{0.0}; // Temporal Stride / Progression
    double S{0.0}; // Symbolic / Spatial Coherence
    double E{0.0}; // Tension / Residual Strain

    [[nodiscard]] constexpr double NormSquared() const noexcept {
        return T * T + S * S + E * E;
    }

    [[nodiscard]] double Norm() const noexcept {
        return std::sqrt(NormSquared());
    }

    [[nodiscard]] double L1Norm() const noexcept {
        return std::abs(T) + std::abs(S) + std::abs(E);
    }
};

struct GrammarCoordinate {
    std::string register_label;
    double tension_factor{0.0};
    int active_slot{0};
    bool locked{false};
};

#endif // TYPES_HPP
