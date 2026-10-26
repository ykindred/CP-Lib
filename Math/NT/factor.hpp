#pragma once
#include "../head.hpp"

// ok
inline vector<pair<ll, int>> prime_factor(ll x) {
    vector<pair<ll, int>> ret;
    for (int i = 2; i <= x / i; i++) {
        int e = 0;
        while (x % i == 0) {
            x /= i;
            e++;
        }
        if (e > 0) {
            ret.emplace_back(i, e);
        }
    }
    if (x > 1) {
        ret.emplace_back(x, 1);
    }
    return ret;
}

inline vector<ll> factor(ll x) {
    vector<ll> head, tail;
    for (ll i = 1; i * i <= x; i++) {
        if (x % i == 0) {
            head.emplace_back(i);
            if (i * i < x) {
                tail.emplace_back(x / i);
            }
        }
    }
    head.insert(head.end(), tail.rbegin(), tail.rend());
    return head;
}