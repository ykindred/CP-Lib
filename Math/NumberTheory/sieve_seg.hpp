#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr ll R = 1'000'000'300;
constexpr int SQR = 32'000;
constexpr int PIS = 4'000;
// sqrt(10) < 3.17
bitset<SQR + 5> not_prime;
int primes[PIS + 5], pcnt = 0;
void init_sieve() {
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

vector<ll> get_primes(ll l, ll r) {
    vector<bool> notp(r - l + 1);
    if (l == 0) {
        notp[0] = notp[1] = 1;
    } else if (l == 1) {
        notp[0] = 1;
    }

    for (int i = 0; i < pcnt; i++) {
        ll p = primes[i];
        if (p * p > r) {
            break;
        }
        for (ll j = max(2LL, (l + p - 1) / p); j * p <= r; j++) {
            notp[j * p - l] = 1;
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