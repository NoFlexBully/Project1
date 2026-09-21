#ifndef SIEVE_HPP
#define SIEVE_HPP

#include "Types.hpp"
#include <cmath>
#include <algorithm>

class DissipationSieve {
public:
    static constexpr double DISSIPATION_RATE = 0.08;

    [[nodiscard]] static Triad Filter(const Triad& current, const Triad& /*previous*/) noexcept {
        Triad filtered = current;
        double decay = DISSIPATION_RATE * std::tanh(current.E + 0.05);
        filtered.E = std::max(0.0, filtered.E - decay);
        return filtered;
    }
};

#endif // SIEVE_HPP
