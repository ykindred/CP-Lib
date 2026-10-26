#pragma once
#include "../head.hpp"

// 传入正整数的质因数分解 fac，每项为 {质数, 指数}，返回递增排列的所有正约数。
// 空分解表示一，返回 {1}；原数须在 ll 范围内，时间为 $O(\tau(n)\log\tau(n))$。
inline vector<ll> divisors(const vector<pair<ll, int>>& fac) {
    vector<ll> ret{1};
    for (auto [p, e] : fac) {
        int n = ret.size();
        ll mul = 1;
        for (int k = 1; k <= e; k++) {
            mul *= p;
            for (int i = 0; i < n; i++) ret.push_back(ret[i] * mul);
        }
    }
    sort(ret.begin(), ret.end());
    return ret;
}
