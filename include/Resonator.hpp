#ifndef RESONATOR_HPP
#define RESONATOR_HPP

#include "Types.hpp"
#include <cmath>
#include <numbers>

struct ResonanceResult {
    int best_slot{0};
    double phase_delta{0.0};
    bool locked{false};
};

class PhaseResonator {
public:
    static constexpr int NUM_SLOTS = 14;

    [[nodiscard]] static ResonanceResult Evaluate(const Triad& state) noexcept {
        double angle = std::atan2(state.E, state.T);
        if (angle < 0.0) angle += 2.0 * std::numbers::pi;

        double slot_arc = (2.0 * std::numbers::pi) / NUM_SLOTS;
        int slot = static_cast<int>(angle / slot_arc) % NUM_SLOTS;
        double slot_center = (slot + 0.5) * slot_arc;
        double delta = std::abs(angle - slot_center);

        bool lock = (delta < 0.05);
        return ResonanceResult{
            .best_slot = slot,
            .phase_delta = delta,
            .locked = lock
        };
    }
};

#endif // RESONATOR_HPP
