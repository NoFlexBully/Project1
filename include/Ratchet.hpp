#ifndef RATCHET_HPP
#define RATCHET_HPP

#include "Types.hpp"

struct RatchetResult {
    bool committed{false};
};

class EpistemicRatchet {
public:
    static constexpr double COMMITMENT_ENERGY_THRESHOLD = 0.85;

    [[nodiscard]] static RatchetResult Evaluate(const Triad& state, bool harmonic_lock) noexcept {
        if (harmonic_lock && state.E >= COMMITMENT_ENERGY_THRESHOLD) {
            return RatchetResult{.committed = true};
        }
        return RatchetResult{.committed = false};
    }
};

#endif // RATCHET_HPP
