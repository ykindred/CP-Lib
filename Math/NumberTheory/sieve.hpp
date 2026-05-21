#include <bits/stdc++.h>
using ll = long long;
using namespace std;
constexpr int N = 100'000'000;
constexpr int PIN = 5'761'455;
// 1e9 : 50'847'534
// 1e8 : 5'761'455
// 1e7 : 664'579
// 1e6 : 78'498
// 1e5 : 9'592
// 1e4 : 1'229
// 1e3 : 168
// 1e2 : 25
// 1e1 : 4
// 下界N / log(N), 上界(>61000时) N / (log(N) - 1.1)
bitset<N + 5> not_prime;
int primes[PIN + 5], pcnt = 0;
void sieve() {
    not_prime[1] = 1;
    for (int i = 2; i <= N; i++) {
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
