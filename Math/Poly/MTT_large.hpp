#pragma once
#include "../head.hpp"
#include "../NT/tools.hpp"
#include "../NT/modint.hpp"
#include "NTT.hpp"
// ok maybe

// 单个 NTT 模数配置
template <int P, int G = 3>
struct NTTPrime {
    static constexpr int mod = P, root = G;
};

// 单模卷积，返回原始余数
template <int P, int G>
vector<u32> mtt_residue(
    const vector<ll>& a, const vector<ll>& b, int need
) {
    using Z = ModInt<P>;
    vector<Z> x(a.size()), y(b.size());

    for (int i = 0; i < (int)a.size(); i++) x[i] = a[i];
    for (int i = 0; i < (int)b.size(); i++) y[i] = b[i];

    auto c = convolution<Z, G>(move(x), move(y), need);
    vector<u32> ret(need);
    for (int i = 0; i < need; i++) ret[i] = c[i].val;
    return ret;
}

// K 模 NTT + Garner
// Require: mod > 0, primes are distinct NTT primes
// Require: integer convolution coefficients < product(primes)
template <class... Cfg>
vector<ll> mtt_k(
    const vector<ll>& a, const vector<ll>& b,
    ll mod, int need = -1
) {
    constexpr int K = sizeof...(Cfg);
    static_assert(K > 0);
    constexpr array<ll, K> ps = {Cfg::mod...};

    assert(mod > 0);
    if (a.empty() || b.empty()) return {};

    int full = (int)a.size() + (int)b.size() - 1;
    if (need == -1 || need > full) need = full;
    if (need <= 0) return {};
    if (mod == 1) return vector<ll>(need, 0);

    int n = min<int>(a.size(), need);
    int m = min<int>(b.size(), need);

    vector<ll> aa(n), bb(m);
    for (int i = 0; i < n; i++) aa[i] = norm(a[i], mod);
    for (int i = 0; i < m; i++) bb[i] = norm(b[i], mod);

    if (1LL * n * m <= 30000) {
        vector<ll> c(need);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m && i + j < need; j++) {
                c[i + j] = (c[i + j] + (i128)aa[i] * bb[j]) % mod;
            }
        }
        return c;
    }

    int len = 1;
    while (len < n + m - 1) len <<= 1;
    assert(( ((Cfg::mod - 1) % len == 0) && ... ));

    // 分别在 K 个模数下进行卷积
    array<vector<u32>, K> rs = {
        mtt_residue<Cfg::mod, Cfg::root>(aa, bb, need)...
    };

    // invs[j][i] = inverse(ps[j]) mod ps[i]
    static const auto invs = [] {
        array<array<ll, K>, K> v{};
        constexpr array<ll, K> p = {Cfg::mod...};
        for (int i = 0; i < K; i++) {
            for (int j = 0; j < i; j++) {
                v[j][i] = qkp(p[j] % p[i], p[i] - 2, p[i]);
            }
        }
        return v;
    }();

    // pref[i] = product(ps[0...i-1]) % mod
    array<ll, K> pref{};
    pref[0] = 1 % mod;
    for (int i = 1; i < K; i++) {
        pref[i] = (i128)pref[i - 1] * ps[i - 1] % mod;
    }

    vector<ll> ans(need);

    for (int i = 0; i < need; i++) {
        array<ll, K> digit{};

        // Garner: 计算混合进制系数
        for (int j = 0; j < K; j++) {
            ll x = rs[j][i];
            for (int t = 0; t < j; t++) {
                x = (x - digit[t]) % ps[j];
                if (x < 0) x += ps[j];
                x = (i128)x * invs[t][j] % ps[j];
            }
            digit[j] = x;
        }

        // 直接在目标模数下还原
        ll x = 0;
        for (int j = 0; j < K; j++) {
            x = (x + (i128)digit[j] * pref[j]) % mod;
        }
        ans[i] = x;
    }
    return ans;
}

// 五模配置
using P0 = NTTPrime<167772161>;
using P1 = NTTPrime<469762049>;
using P2 = NTTPrime<1224736769>;
using P3 = NTTPrime<2130706433>;
using P4 = NTTPrime<2013265921, 31>;

inline vector<ll> mtt(
    const vector<ll>& a, const vector<ll>& b,
    ll mod, int need = -1
) {
    return mtt_k<P0, P1, P2, P3, P4>(a, b, mod, need);
}
