#include <bits/stdc++.h>
#include "../NumberTheory/modint.hpp"
template <int N, int MOD = 998244353>
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
    mint nCr(int n, int r) {
        if (!(0 <= r && r <= n)) {
            return 0;
        }
        return fact[n] * factinv[r] * factinv[n - r];
    }
    mint nPr(int n, int r) {
        if (!(0 <= r && r <= n)) {
            return 0;
        }
        return fact[n] * factinv[n - r];
    }
};