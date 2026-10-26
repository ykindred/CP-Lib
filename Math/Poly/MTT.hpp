#pragma once
#include "../head.hpp"
#include "../NT/tools.hpp"
#include "../NT/modint.hpp"
#include "NTT.hpp"
// ok

// 三模 NTT + CRT
// O(L log L), L = ceil_pow2(n + m - 1)
// Require: mod > 0, NTT length <= 2^24
// 对 mod <= 1e9+7，在长度限制内保证正确
// 常用原根为3NTT友好模数:
// 167772161   = 5   * 2^25 + 1
// 469762049   = 7   * 2^26 + 1
// 998244353   = 119 * 2^23 + 1
// 1224736769  = 73  * 2^24 + 1
// 2130706433  = 127 * 2^24 + 1
// 1004535809  = 479 * 2^21 + 1
inline vector<ll> mtt(const vector<ll>& a, const vector<ll>& b, ll mod, int need = -1) {
    if (a.empty() || b.empty()) return {};

    int full = (int)a.size() + b.size() - 1;
    if (need == -1 || need > full) need = full;
    if (need <= 0) return {};

    int n = min<int>(a.size(), need);
    int m = min<int>(b.size(), need);

    if (1LL * n * m <= 30000) {
        vector<ll> c(need);
        for (int i = 0; i < n; i++) {
            ll x = norm(a[i], mod);
            for (int j = 0; j < m && i + j < need; j++) {
                ll y = norm(b[j], mod);
                c[i + j] = (c[i + j] + (i128)x * y) % mod;
            }
        }
        return c;
    }

    constexpr ll M1 = 167772161;
    constexpr ll M2 = 469762049;
    constexpr ll M3 = 1224736769;

    int len = 1;
    while (len < n + m - 1) len <<= 1;
    assert(len <= (1 << 24));

    using Z1 = ModInt<M1>;
    using Z2 = ModInt<M2>;
    using Z3 = ModInt<M3>;

    vector<Z1> a1(n), b1(m);
    vector<Z2> a2(n), b2(m);
    vector<Z3> a3(n), b3(m);

    for (int i = 0; i < n; i++) {
        ll x = norm(a[i], mod);
        a1[i] = x;
        a2[i] = x;
        a3[i] = x;
    }
    for (int i = 0; i < m; i++) {
        ll x = norm(b[i], mod);
        b1[i] = x;
        b2[i] = x;
        b3[i] = x;
    }

    auto c1 = convolution(move(a1), move(b1), need);
    auto c2 = convolution(move(a2), move(b2), need);
    auto c3 = convolution(move(a3), move(b3), need);

    static const ll inv12 = qkp(M1, M2 - 2, M2);
    static const ll inv123 = qkp(M1 * M2 % M3, M3 - 2, M3);

    ll m12 = M1 * M2 % mod;
    vector<ll> ans(need);

    for (int i = 0; i < need; i++) {
        ll x1 = c1[i].val;
        ll x2 = c2[i].val;
        ll x3 = c3[i].val;

        ll t1 = (x2 - x1 % M2 + M2) % M2 * inv12 % M2;
        ll x12 = x1 + t1 * M1;
        ll t2 = (x3 - x12 % M3 + M3) % M3 * inv123 % M3;

        ans[i] = (x12 % mod + (i128)t2 * m12) % mod;
    }
    return ans;
}

// 精确整数卷积, 结果在ll范围内
// Require: NTT length <= 2^24
template <int P>
vector<ModInt<P>> conv_ll_mod(const vector<ll>& a, const vector<ll>& b, int need) {
    using Z = ModInt<P>;
    vector<Z> x(a.begin(), a.end());
    vector<Z> y(b.begin(), b.end());
    return convolution(move(x), move(y), need);
}

inline vector<ll> convolution_ll(const vector<ll>& a, const vector<ll>& b, int need = -1) {
    if (a.empty() || b.empty()) return {};

    int full = (int)a.size() + (int)b.size() - 1;
    if (need == -1 || need > full) need = full;
    if (need <= 0) return {};

    constexpr ll P1 = 167772161;
    constexpr ll P2 = 469762049;
    constexpr ll P3 = 1224736769;
    constexpr i128 P = (i128)P1 * P2 * P3;

    auto c1 = conv_ll_mod<P1>(a, b, need);
    auto c2 = conv_ll_mod<P2>(a, b, need);
    auto c3 = conv_ll_mod<P3>(a, b, need);

    static const ll inv12 = qkp(P1, P2 - 2, P2);
    static const ll inv123 = qkp(P1 * P2 % P3, P3 - 2, P3);

    vector<ll> ans(need);
    for (int i = 0; i < need; i++) {
        ll x1 = c1[i].val;
        ll x2 = c2[i].val;
        ll x3 = c3[i].val;

        ll t1 = (x2 - x1 + P2) % P2 * inv12 % P2;
        ll x12 = x1 + P1 * t1;
        ll t2 = (x3 - x12 % P3 + P3) % P3 * inv123 % P3;

        i128 x = x12 + (i128)P1 * P2 * t2;
        if (x > P / 2) x -= P;

        ans[i] = (ll)x;
    }
    return ans;
}
