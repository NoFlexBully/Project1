#ifndef RATIONAL_HPP
#define RATIONAL_HPP

#include <cstdint>
#include <numeric>
#include <stdexcept>
#include <string>

class Rational {
public:
    int64_t num{0};
    int64_t den{1};

    constexpr Rational() noexcept : num(0), den(1) {}

    constexpr Rational(int64_t numerator, int64_t denominator = 1) 
        : num(numerator), den(denominator) 
    {
        if (den == 0) {
            den = 1;
            throw std::invalid_argument("Rational denominator cannot be zero.");
        }
        if (den < 0) {
            num = -num;
            den = -den;
        }
        const int64_t g = std::gcd(num, den);
        num /= g;
        den /= g;
    }

    [[nodiscard]] constexpr double ToDouble() const noexcept {
        return static_cast<double>(num) / static_cast<double>(den);
    }

    constexpr Rational operator+(const Rational& rhs) const noexcept {
        const int64_t common_den = std::lcm(den, rhs.den);
        const int64_t adjusted_num = (num * (common_den / den)) + (rhs.num * (common_den / rhs.den));
        return Rational(adjusted_num, common_den);
    }

    constexpr Rational operator-(const Rational& rhs) const noexcept {
        const int64_t common_den = std::lcm(den, rhs.den);
        const int64_t adjusted_num = (num * (common_den / den)) - (rhs.num * (common_den / rhs.den));
        return Rational(adjusted_num, common_den);
    }

    constexpr Rational operator*(const Rational& rhs) const noexcept {
        return Rational(num * rhs.num, den * rhs.den);
    }

    constexpr Rational operator/(const Rational& rhs) const {
        if (rhs.num == 0) {
            throw std::invalid_argument("Division by zero rational.");
        }
        return Rational(num * rhs.den, den * rhs.num);
    }

    constexpr bool operator==(const Rational& rhs) const noexcept {
        return num == rhs.num && den == rhs.den;
    }

    constexpr bool operator<(const Rational& rhs) const noexcept {
        return (num * rhs.den) < (rhs.num * den);
    }

    [[nodiscard]] std::string ToString() const {
        if (den == 1) return std::to_string(num);
        return std::to_string(num) + "/" + std::to_string(den);
    }
};

#endif // RATIONAL_HPP
