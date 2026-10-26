#pragma once
#include "../head.hpp"
// ok
inline i128 abs128(i128 x) {
    return x < 0 ? -x : x;
}
// O(log min(a, b))
inline array<i128, 3> exgcd(i128 a, i128 b) {
    if (b == 0) {
        if (a >= 0) return {1, 0, a};
        else return {-1, 0, -a};
    }
    auto [x, y, g] = exgcd(b, a % b);
    return {y, x - a / b * y, g};
}

inline i128 norm128(i128 x, i128 mod) {
    mod = abs128(mod);
    return ((x %= mod) < 0) ? x += mod : x;
}

// O(log min(a, b)), 要求a, b, c不超过ll范围
// solve ax + by = c, 其中x为被规范化后的值. 即0 <= x < b/g
// 步长为(b/g, -a/g)
inline tuple<bool, i128, i128> solveLDE(i128 a, i128 b, i128 c) {
    if (a == 0 && b == 0) {
        return {c == 0, 0, 0};
    }

    auto [x, y, g] = exgcd(a, b);
    if (c % g != 0) {
        return {false, 0, 0};
    }

    i128 k = c / g;
    if (b == 0) {
        return {true, x * k, 0};
    }

    x = norm128(x * k, b / g);
    y = (c - a * x) / b;
    return {true, x, y};
}

inline ll inv_exgcd(ll a, ll mod) {
    auto [x, y, g] = exgcd(a, mod);
    if (g != 1) return -1;
    return (ll)norm128(x, mod);
}