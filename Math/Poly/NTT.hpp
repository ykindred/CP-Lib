#pragma once
#include "../head.hpp"
#include "../NT/factor.hpp"
#include "../NT/tools.hpp"
#include "../NT/modint.hpp"

// ok

// 依赖 modint, prime_factor, mul, qkp

// 返回 m 的最小原根，不存在则返回 -1
// 约O(n^0.25)
inline ll find_primitive_root(ll m) {
    if (m == 2) return 1;
    if (m <= 1) return -1;

    auto fac = prime_factor(m);

    // 原根存在当且仅当 m = 2, 4, p^k, 2p^k (p 为奇素数)
    if (fac[0].first == 2) {
        if (fac.size() == 1) {
            if (fac[0].second != 2) return -1;
        } else {
            if (fac.size() != 2 || fac[0].second != 1) return -1;
        }
    } else if (fac.size() != 1) {
        return -1;
    }

    ll phi = m;
    for (auto [p, cnt] : fac) {
        phi = phi / p * (p - 1);
    }

    auto pf = prime_factor(phi);
    for (ll g = 2; g < m; g++) {
        if (gcd(g, m) != 1) continue;
        bool ok = true;
        for (auto [p, cnt] : pf) {
            if (qkp(g, phi / p, m) == 1) {
                ok = false;
                break;
            }
        }
        if (ok) return g;
    }
    return -1;
}

// O(n log n)
template <class Z, int G = 3>
void ntt(vector<Z>& a, bool inv = false) {
    int n = a.size();

    static vector<int> rev;
    if ((int)rev.size() != n) {
        rev.assign(n, 0);
        for (int i = 1; i < n; i++) {
            rev[i] = (rev[i >> 1] >> 1)
                   | ((i & 1) ? n >> 1 : 0);
        }
    }

    // roots[len + j] = w_(2*len)^j
    static vector<Z> roots{0, 1};
    if ((int)roots.size() < n) {
        int k = __builtin_ctz((unsigned)roots.size());
        roots.resize(n);
        while ((1 << k) < n) {
            Z w = Z(G).pow(
                (Z::mod() - 1) / (1LL << (k + 1))
            );
            for (int i = 1 << (k - 1); i < (1 << k); i++) {
                roots[i << 1] = roots[i];
                roots[i << 1 | 1] = roots[i] * w;
            }
            k++;
        }
    }

    for (int i = 0; i < n; i++) {
        if (i < rev[i]) swap(a[i], a[rev[i]]);
    }

    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += len << 1) {
            for (int j = 0; j < len; j++) {
                Z x = a[i + j];
                Z y = a[i + j + len] * roots[len + j];
                a[i + j] = x + y;
                a[i + j + len] = x - y;
            }
        }
    }

    if (inv) {
        reverse(a.begin() + 1, a.end());
        Z in = Z(n).inv();
        for (auto& x : a) x *= in;
    }
}

// O(L log L), L = ceil_pow2(n + m - 1)
template <class Z, int G = 3>
vector<Z> convolution(vector<Z> a, vector<Z> b, int need = -1) {
    if (a.empty() || b.empty()) return {};

    int full = (int)a.size() + (int)b.size() - 1;
    if (need == -1 || need > full) need = full;
    if (need <= 0) return {};

    int n = min<int>(a.size(), need);
    int m = min<int>(b.size(), need);
    a.resize(n);
    b.resize(m);

    if (min(n, m) <= 60) {
        vector<Z> c(need);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m && i + j < need; j++) {
                c[i + j] += a[i] * b[j];
            }
        }
        return c;
    }

    int len = 1;
    while (len < n + m - 1) len <<= 1;
    assert((Z::mod() - 1) % len == 0);

    a.resize(len);
    b.resize(len);
    ntt<Z, G>(a);
    ntt<Z, G>(b);

    for (int i = 0; i < len; i++) a[i] *= b[i];

    ntt<Z, G>(a, true);
    a.resize(need);
    return a;
}