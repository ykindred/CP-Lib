#pragma once
#include "../head.hpp"
// ok
constexpr int PIN = 78'498;
// 1e8 : 5'761'455
// 1e7 : 664'579
// 1e6 : 78'498
// 1e5 : 9'592
// 1e4 : 1'229
// 1e3 : 168
// N>61000时 不超过N / (log(N) - 1.1)
inline bitset<N + 5> not_prime;
inline int primes[PIN + 5], pcnt = 0;
// O(N)
inline void sieve() {
    not_prime[0] = not_prime[1] = 1;
    for (ll i = 2; i <= N; i++) {
        if (!not_prime[i]) {
            primes[pcnt] = i;
            pcnt++;
        }
        for (int j = 0; j < pcnt && i * primes[j] <= N; j++) {
            not_prime[i * primes[j]] = 1;
            if (i % primes[j] == 0) {
                break;
            }
        }
    }
}
