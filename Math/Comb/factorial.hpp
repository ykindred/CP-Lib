#pragma once
#include "../head.hpp"
#include "../NT/modint.hpp"

// ok
// 依赖 modint
// 要求N < MOD
template <int N, int MOD = 998'244'353>
struct Factorial {
    using mint = ModInt<MOD>;
    array<mint, N + 5> fact, factinv;
    Factorial() {
        fact[0] = 1;
        for (int i = 1; i <= N; i++) {
            fact[i] = fact[i - 1] * i;
        }
        factinv[N] = fact[N].inv();
        for (int i = N - 1; i >= 0; i--) {
            factinv[i] = factinv[i + 1] * (i + 1);
        }
    }
    mint nCr(ll n, ll r) {
        if (!(0 <= r && r <= n)) {
            return 0;
        }
        return fact[n] * factinv[r] * factinv[n - r];
    }
    mint nPr(ll n, ll r) {
        if (!(0 <= r && r <= n)) {
            return 0;
        }
        return fact[n] * factinv[n - r];
    }
};

// inline Factorial<N, MOD> f;

// O(min(r, n - r)), 要求r < MOD
template <int MOD = 998'244'353>
inline ModInt<MOD> nCr_large(ll n, ll r) {
    if (!(0 <= r && r <= n)) {
        return 0;
    }
    r = min(r, n - r);

    ModInt<MOD> ret = 1, denom = 1;
    for (ll i = 1; i <= r; i++) {
        ret *= n - i + 1;
        denom *= i;
    }
    return ret / denom;
};

// O(log_MOD n * C(n, r)) 其中C(n, r)为单次求组合数复杂度
template <int MOD = 998'244'353>
inline ModInt<MOD> lucas(ll n, ll r) {
    if (!(0 <= r && r <= n)) return 0;
    if (r == 0) {
        return 1;
    }

    return lucas<MOD>(n / MOD, r / MOD) * 
    nCr_large<MOD>(n % MOD, r % MOD); // 或改用其他求组合数方式
}

