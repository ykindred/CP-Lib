#pragma once
#include "../head.hpp"
#include <cstdlib>

// ok
inline ll norm(ll x, ll mod) { 
    if (0 <= x && x < mod) {
        return x;
    }
    return ((x %= mod) < 0) ? (x + mod) : x; 
}
inline ll mul(ll a, ll b, ll mod) { 
    a = norm(a, mod), b = norm(b, mod);
    return (i128)a * b % mod;
}
inline ll add(ll a, ll b, ll mod) {
    a = norm(a, mod), b = norm(b, mod);
    return ((a += b) >= mod) ? a - mod : a;
}
inline ll sub(ll a, ll b, ll mod) {
    a = norm(a, mod), b = norm(b, mod);
    return ((a -= b) < 0) ? a + mod : a;
}
inline ll qkp(ll a, ll b, ll mod) {
    a = norm(a, mod);
    ll ret = 1 % mod;
    for (; b; b >>= 1, a = mul(a, a, mod)) if (b & 1) ret = mul(ret, a, mod); 
    return ret;
}

// 要求质数模, O(log a)
inline ll inv(ll a, ll mod_prime) { 
    return qkp(a, mod_prime - 2, mod_prime); 
}

template <typename T>
T div_floor(T a, T b) {
    T q = a / b, r = a % b;
    if (r != 0 && (r < 0) != (b < 0)) --q;
    return q;
}
template <typename T>
T div_ceil(T a, T b) {
    T q = a / b, r = a % b;
    if (r != 0 && (r < 0) == (b < 0)) ++q;
    return q;
}