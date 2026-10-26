#pragma once
#include "../head.hpp"
#include "tools.hpp"
#include "exgcd.hpp"
#include "../../DataStructure/hashtable.hpp"

// ok

// 依赖norm, mul, qkp, exgcd, inv_exgcd, HashMap
// O(sqrt(m)), require gcd(a, m) = 1, 注意空间开销
inline ll bsgs(ll a, ll b, ll m) {
    a = norm(a, m);
    b = norm(b, m);

    if (m == 1) return 0;
    if (b == 1) return 0;

    ll n = sqrtl(m) + 1;
    HashMap<ll, ll> mp;

    // b * a^q
    ll cur = b;
    for (ll q = 0; q < n; q++) {
        mp[cur] = q;
        cur = mul(cur, a, m);
    }

    ll an = qkp(a, n, m);
    cur = 1;
    for (ll p = 1; p <= n + 1; p++) {
        cur = mul(cur, an, m);
        auto it = mp.find(cur);
        if (it != mp.end()) {
            ll x = p * n - it->second;
            if (x >= 0) return x;
        }
    }
    return -1;
}

// O(sqrt(m) + log(m))
inline ll exbsgs(ll a, ll b, ll m) {
    a = norm(a, m);
    b = norm(b, m);

    if (m == 1) return 0;
    if (b == 1) return 0;

    ll cnt = 0;
    ll cur = 1;
    while (true) {
        ll g = gcd(a, m);
        if (g == 1) break;
        if (b % g != 0) return -1;

        b /= g;
        m /= g;
        cur = mul(cur, a / g, m);
        cnt++;
        if (cur == b) return cnt;
    }

    ll iv = inv_exgcd(cur, m);
    ll rhs = mul(b, iv, m);
    ll t = bsgs(a, rhs, m);
    if (t == -1) return -1;
    return cnt + t;
}
