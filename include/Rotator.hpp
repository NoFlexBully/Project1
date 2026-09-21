#ifndef ROTATOR_HPP
#define ROTATOR_HPP

#include "Types.hpp"
#include <cmath>

class TriadicRotator {
public:
    TriadicRotator() = default;

    [[nodiscard]] Triad Rotate(const Triad& v) const noexcept {
        // Kinematic SO(3) conservative rotation without norm distortion
        const double theta = 0.05;
        const double cos_t = std::cos(theta);
        const double sin_t = std::sin(theta);

        // Rotation around spatial axis preserving total energetic metric
        double new_T = v.T * cos_t - v.E * sin_t;
        double new_S = v.S;
        double new_E = v.T * sin_t + v.E * cos_t;

        return Triad{.T = new_T, .S = new_S, .E = new_E};
    }
};

#endif // ROTATOR_HPP
