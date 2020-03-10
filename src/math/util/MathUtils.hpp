//
// Created by MTesseracT on 29/02/2020.
//

#ifndef VALCANO_MATHUTILS_HPP
#define VALCANO_MATHUTILS_HPP

#include <cstdlib>

namespace mt::math {
constexpr double e = 2.71828182845904523536;
constexpr double pi = 3.14159265358979323846;
constexpr double deg_to_rad = pi / 180.0;
constexpr double rad_to_deg = 180.0 / pi;

template <typename T>
inline T lerp(T p_start, T p_end, T p_t)
{
    return p_start + (p_end - p_start) * p_t;
}

template <typename T>
inline T to_rad(T p_deg)
{
    return p_deg * deg_to_rad;
}

template <typename T>
inline T to_deg(T p_rad)
{
    return p_rad * rad_to_deg;
}

template <typename T>
inline int sign(const T& p_val)
{
    return (T(0) < p_val) - (p_val < T(0));
}

int round_to_nearest_multiple(int n, int m)
{
    if (m == 0)
        return n;

    int rem = std::abs(n) % m;
    if (rem == 0)
        return n;

    if (n < 0)
        return -(std::abs(n) - rem);
    else
        return n + m - rem;
}
} // namespace mt::math

#endif // VALCANO_MATHUTILS_HPP
