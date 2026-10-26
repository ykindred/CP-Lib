#pragma once
#include "../head.hpp"

// ok
// O(log m)
inline i128 floor_sum(ll n, ll m, ll a, ll b) {
    if (n == 0) return 0;

    i128 ret = 0;
    if (a >= m) {
        ret += (i128)n * (n - 1) / 2 * (a / m);
        a %= m;
    }
    if (b >= m) {
        ret += (i128)(b / m) * n;
        b %= m;
    }
    if (a == 0) return ret;

    ll Y = ((i128)a * (n - 1) + b) / m;
    if (Y == 0) return ret;

    ret += (i128)n * Y - floor_sum(Y, a, m, m + a - b - 1);
    return ret;
}
