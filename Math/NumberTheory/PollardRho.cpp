// 依赖：
#include "primetest.hpp"

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

ll rho(ll n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;

    while (true) {
        ll c = uniform_int_distribution<ll>(1, n - 1)(rng);
        ll x = uniform_int_distribution<ll>(0, n - 1)(rng);
        ll y = x, d = 1;

        auto f = [&](ll x) -> ll {
            return (mul(x, x, n) + c) % n;
        };

        while (d == 1) {
            x = f(x);
            y = f(f(y));
            d = gcd(abs(x - y), n);
        }

        if (d != n) return d;
    }
}
vector<ll> factorize(ll x) {
    vector<ll> res;

    auto dfs = [&](auto self, ll x) -> void {
        if (x == 1) return;
        if (primetest(x)) {
            res.push_back(x);
            return;
        }

        ll d = rho(x);
        self(self, d);
        self(self, x / d);
    };

    dfs(dfs, x);
    sort(res.begin(), res.end());
    return res;
}
