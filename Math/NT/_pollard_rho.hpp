#pragma once
#include "../head.hpp"
#include "primetest.hpp"

// 依赖 mul, primetest(miller-rabin)
namespace rho {
inline mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

inline ll rho(ll n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;

    constexpr int B = 128;

    while (true) {
        ll c = uniform_int_distribution<ll>(1, n - 1)(rng);
        ll y = uniform_int_distribution<ll>(0, n - 1)(rng);

        auto f = [&](ll x) -> ll {
            ll t = mul(x, x, n);
            return t >= n - c ? t - (n - c) : t + c;
        };

        ll x = 0, ys = 0;
        ll d = 1, r = 1;

        while (d == 1) {
            x = y;
            for (ll i = 0; i < r; ++i) {
                y = f(y);
            }

            for (ll k = 0; k < r && d == 1; k += B) {
                ys = y;
                ll q = 1;
                ll len = min<ll>(B, r - k);

                for (ll i = 0; i < len; ++i) {
                    y = f(y);
                    q = mul(q, abs(x - y), n);
                }

                d = gcd(q, n);
            }

            r *= 2;
        }

        if (d == n) {
            do {
                ys = f(ys);
                d = gcd(abs(x - ys), n);
            } while (d == 1);
        }

        if (d != n) return d;
    }
}

inline vector<pair<ll, int>> prime_factor(ll x) {
    assert(x >= 1);
    vector<ll> fac;

    auto dfs = [&](auto self, ll n) -> void {
        if (n == 1) return;
        if (primetest(n)) {
            fac.push_back(n);
            return;
        }

        ll d = rho(n);
        self(self, d);
        self(self, n / d);
    };

    dfs(dfs, x);
    sort(fac.begin(), fac.end());

    vector<pair<ll, int>> res;
    for (ll p : fac) {
        if (res.empty() || res.back().first != p) {
            res.emplace_back(p, 1);
        } else {
            ++res.back().second;
        }
    }
    return res;
}
}