#pragma once
#include <bits/stdc++.h>
using ll = long long;
using i128 = __int128;
inline ll mul(ll a, ll b, ll p) {
    return (i128)a * b % p;
}
inline ll pow(ll a, ll b, ll p) {
    ll ret = 1 % p;
    while (b > 0) {
        if (b & 1) {
            ret = mul(ret, a, p);
        }
        a = mul(a, a, p);
        b >>= 1;
    }
    return b;
}
bool primetest(ll n) {
    if (n < 2 || n % 6 % 4 != 1) {
        return (n | 1) == 3;
    }
    ll s = __builtin_ctzll(n - 1), d = n >> s;
    for (ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        ll p = pow(a % n, d, n), i = s;
        while (p != 1 && p != n - 1 && a % n && i--) {
            p = mul(p, p, n);
        }
        if (p != n - 1 && i != s) {
            return false;
        }
    }
    return true;
}
