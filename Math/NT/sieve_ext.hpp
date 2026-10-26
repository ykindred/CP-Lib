#pragma once
#include "../head.hpp"
// ok
constexpr int PIN = 78'498;
inline bitset<N + 5> not_prime;
inline int primes[PIN + 5], pcnt = 0;
// 1e8 : 5'761'455
// 1e7 : 664'579
// 1e6 : 78'498
// 1e5 : 9'592
// 1e4 : 1'229
// 1e3 : 168
// N > 61000时不超过 N / (log(N) - 1.1)

inline int minp[N + 5]; // 最小质因子
// int phi[N + 5]; // 欧拉函数
// int mu[N + 5]; // 莫比乌斯函数
// int d[N + 5], times[N + 5]; // 约数计数函数
// ll sigma[N + 5]; int g[N + 5]; // 约数和函数

// O(N)
inline void sieve() {
    not_prime[0] = not_prime[1] = 1;
    // phi[1] = 1;
    // mu[1] = 1;
    // d[1] = 1;
    // sigma[1] = 1;
    for (ll i = 2; i <= N; i++) {
        if (!not_prime[i]) {
            primes[pcnt] = i, pcnt++;
            minp[i] = i;
            // phi[i] = i - 1;
            // mu[i] = -1;
            // d[i] = 2, times[i] = 1;
            // sigma[i] = i + 1, g[i] = i + 1;
        }
        for (int j = 0; j < pcnt && i * primes[j] <= N; j++) {
            int pj = primes[j];
            not_prime[i * pj] = 1;
            minp[i * pj] = pj;
            if (i % primes[j] == 0) {
                // phi[i * pj] = phi[i] * pj;
                // mu[i * pj] = 0;
                // d[i * pj] = d[i] / (times[i] + 1) * (times[i] + 2), times[i * pj] = times[i] + 1;
                // g[i * pj] = g[i] * pj + 1, sigma[i * pj] = sigma[i] / g[i] * g[i * pj];
                break;
            }
            // phi[i * pj] = phi[i] * (pj - 1);
            // mu[i * pj] = -mu[i];
            // d[i * pj] = d[i] * 2, times[i * pj] = 1;
            // sigma[i * pj] = sigma[i] * sigma[pj], g[i * pj] = pj + 1;
        }
    }
}

// O(log x), requires 1 <= x <= N
inline vector<pair<ll, int>> prime_factor(ll x) {
    vector<pair<ll, int>> ret;
    while (x > 1) {
        if (ret.empty() || ret.back().first != minp[x]) {
            ret.push_back({ minp[x], 1 });
        } else {
            ret.back().second++;
        }
        x /= minp[x];
    }
    return ret;
}