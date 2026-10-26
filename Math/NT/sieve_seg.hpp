#pragma once
#include "../head.hpp"
// ok
constexpr ll R = 1'000'000'300;
constexpr int SQR = 32'000;
constexpr int PIS = 4'000; // pi(SQR)
// sqrt(10) < 3.17
inline bitset<SQR + 5> not_prime;
inline int primes[PIS + 5], pcnt = 0;
// O(SQRloglogSQR)
inline void init_sieve() {
    not_prime[0] = not_prime[1] = 1;
    for (ll i = 2; i * i <= R; i++) {
        if (!not_prime[i]) {
            primes[pcnt] = i;
            pcnt++;
            for (ll j = i * i; j <= SQR; j += i) {
                not_prime[j] = 1;
            }
        }
    }
}

// O(PIS + (r-l)loglogr)
inline vector<ll> get_primes(ll l, ll r) {
    if (l >= r) {
        return {};
    }
    r--;
    vector<bool> notp(r - l + 1);
    if (l == 0) {
        notp[0] = 1;
        if (r >= 1) {
            notp[1] = 1;
        }
    } else if (l == 1) {
        notp[0] = 1;
    }

    for (int i = 0; i < pcnt; i++) {
        ll p = primes[i];
        if (p * p > r) {
            break;
        }
        ll st = max(p * p, (l + p - 1) / p * p);
        for (ll j = st; j <= r; j += p) {
            notp[j - l] = 1;
        }
    }
    vector<ll> ret;
    for (ll i = l; i <= r; i++) {
        if (!notp[i - l]) {
            ret.emplace_back(i);
        }
    }
    return ret;
}