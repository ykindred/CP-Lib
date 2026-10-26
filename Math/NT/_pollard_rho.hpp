#pragma once
#include "../head.hpp"
#include "primetest.hpp"
#include "_divisors.hpp"

namespace rho {
inline mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

// 内部函数：传入大于一的合数 n，返回它的一个非平凡因子 d，满足 $1<d<n$。
// 使用 Brent 迭代和分批最大公约数计算，期望时间约为 $O(n^{1/4})$。
inline ll rho(ll n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;

    constexpr int B = 128;

    while (true) {
        ll c = uniform_int_distribution<ll>(1, n - 1)(rng);
        ll y = uniform_int_distribution<ll>(0, n - 1)(rng);

        // 传入当前迭代值 x，返回 $(x^2+c)\bmod n$，使用减法避免加法溢出。
        auto f = [&](ll x) -> ll {
            ll t = mul(x, x, n);
            return t >= n - c ? t - (n - c) : t + c;
        };

        ll x = 0, ys = 0;
        ll d = 1, r = 1;

        while (d == 1) {
            x = y;
            for (ll i = 0; i < r; i++) {
                y = f(y);
            }

            for (ll k = 0; k < r && d == 1; k += B) {
                ys = y;
                ll q = 1;
                ll len = min<ll>(B, r - k);

                for (ll i = 0; i < len; i++) {
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

// 传入正整数 x，返回按质数递增排列的质因数分解，每项为 {质数, 指数}。
// x 须在有符号 ll 范围内；一返回空数组。外部调用名为 rho::prime_factor。
inline vector<pair<ll, int>> prime_factor(ll x) {
    assert(x >= 1);
    vector<ll> fac;

    // 递归分解传入的正整数 n，把所有质因子追加到 fac；self 用于递归，不直接返回结果。
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

    vector<pair<ll, int>> ret;
    for (ll p : fac) {
        if (ret.empty() || ret.back().first != p) {
            ret.emplace_back(p, 1);
        } else {
            ret.back().second++;
        }
    }
    return ret;
}

// 传入正整数 x，返回递增排列的所有正约数；先调用 Pollard-Rho 分解，一返回 {1}。
inline vector<ll> factor(ll x) {
    return divisors(prime_factor(x));
}
} 
