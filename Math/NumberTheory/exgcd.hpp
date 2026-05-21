#pragma once
#include <bits/stdc++.h>
using namespace std;
using i128 = __int128;
using ll = long long;
inline array<ll, 3> exgcd(ll a, ll b) {
    if (b == 0) {
        return { 1, 0, a };
    }
    auto [nx, ny, g] = exgcd(b, a % b);
    return { ny, nx - (a / b) * ny, g };
}
inline bool solveLDE(ll A, i128& x, ll B, i128& y, ll C) {
    // solve ax + by = c
    auto [nx, ny, g] = exgcd(A, B);
    if (C % g != 0) {
        return false;
    }
    x = nx * (C / g) % (B / g);
    y = (i128)C - x * A;
    return true;
}