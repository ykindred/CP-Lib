#pragma once
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using i128 = __int128;



constexpr int N = 1'000'000;
constexpr int PIN = 78'498;
bitset<N + 5> not_prime;
int primes[PIN + 5], pcnt = 0;
int phi[N + 5];
int mu[N + 5];
ll smu[N + 5];   // mu 前缀和
ll sphi[N + 5];  // phi 前缀和，小范围内 ll 足够
unordered_map<ll, ll> mp_mu;
unordered_map<ll, i128> mp_phi;
void sieve() {
    phi[1] = 1;
    mu[1] = 1;
    for (int i = 2; i <= N; i++) {
        if (!not_prime[i]) {
            primes[pcnt] = i, pcnt++;
            phi[i] = i - 1;
            mu[i] = -1;
        }
        for (int j = 0; j < pcnt && 1LL * i * primes[j] <= N; j++) {
            int pj = primes[j];
            not_prime[i * pj] = 1;

            if (i % pj == 0) {
                phi[i * pj] = phi[i] * pj;
                mu[i * pj] = 0;
                break;
            }

            phi[i * pj] = phi[i] * (pj - 1);
            mu[i * pj] = -mu[i];
        }
    }

    for (int i = 1; i <= N; i++) {
        smu[i] = smu[i - 1] + mu[i];
        sphi[i] = sphi[i - 1] + phi[i];
    }
}

ll getsmu(ll n) {
    // 莫比乌斯函数的前缀和
    if (n <= N) {
        return smu[n];
    }
    if (mp_mu.count(n)) {
        return mp_mu[n];
    }

    ll ans = 1;

    for (ll l = 2, r; l <= n; l = r + 1) {
        ll q = n / l;
        r = n / q;
        ans -= (r - l + 1) * getsmu(q);
    }
    return mp_mu[n] = ans;
}

i128 getsphi(ll n) {
    // 欧拉函数的前缀和
    if (n <= N) {
        return sphi[n];
    }
    if (mp_phi.count(n)) {
        return mp_phi[n];
    }

    i128 ans = (i128)n * (n + 1) / 2;

    for (ll l = 2, r; l <= n; l = r + 1) {
        ll q = n / l;
        r = n / q;
        ans -= (i128)(r - l + 1) * getsphi(q);
    }

    return mp_phi[n] = ans;
}