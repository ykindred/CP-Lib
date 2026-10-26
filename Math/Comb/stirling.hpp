#pragma once
#include "../head.hpp"
#include "../NT/modint.hpp"
#include "../Poly/NTT.hpp"

// ok

// 依赖modint
template <int N, int MOD = 998'244'353>
struct Stirling {
    using mint = ModInt<MOD>;
    array<array<mint, N + 5>, N + 5> _c, _S;

    Stirling() {
        _c[0][0] = 1;
        _S[0][0] = 1;
        for (int n = 1; n <= N; n++) {
            for (int k = 1; k <= n; k++) {
                // c(n,k) = c(n-1,k-1) + (n-1)c(n-1,k)
                _c[n][k] = _c[n - 1][k - 1] + _c[n - 1][k] * (n - 1);

                // s(n,k) = s(n-1,k-1) - (n-1)s(n-1,k)

                // S(n,k) = S(n-1,k-1) + kS(n-1,k)
                _S[n][k] = _S[n - 1][k - 1] + _S[n - 1][k] * k;
            }
        }
    }
    
    mint c(ll n, ll k) {
        // 无符号第一类斯特林数
        if (k < 0 || k > n) {
            return 0;
        }
        return _c[n][k];
    }
    mint s(ll n, ll k) {
        // 有符号第一类斯特林数
        if (k < 0 || k > n) return 0;
        return (n - k) % 2 ? -_c[n][k] : _c[n][k];
    }
    mint S(ll n, ll k) {
        // 第二类斯特林数
        if (k < 0 || k > n) {
            return 0;
        }
        return _S[n][k];
    }
    
};

// inline Stirling<1'005> stir;



// 依赖NTT
// O(nlog n) 卷积求S(n), 要求n >= 0, 模数为NTT友好质数, need <= MOD
template <int MOD = 998'244'353>
vector<ModInt<MOD>> stirling2_row(ll n, ll need = -1) {
    // S(n, k) = 1/k! \sum_{i = 0}^k (-1)^{k - i} \binom{k}{i} i^n
    // 即(-1)^j / j! 与 i^n / i! 卷积
    using mint = ModInt<MOD>;

    if (need == -1 || need > n) {
        need = n + 1;
    }
    if (need == 0) return {};

    vector<mint> A(need), B(need);

    // A[i] = 1 / i!
    mint fac = 1;
    for (ll i = 1; i < need; i++) {
        fac *= i;
    }
    A[need - 1] = fac.inv();
    for (ll i = need - 1; i >= 1; i--) {
        A[i - 1] = A[i] * i;
    }

    // A[i] = (-1)^i / i!
    // B[i] = i^n / i!
    for (ll i = 0; i < need; i++) {
        B[i] = mint(i).pow(n) * A[i];
        if (i & 1) A[i] = -A[i];
    }

    return convolution(A, B, need);
}