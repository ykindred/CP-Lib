#pragma once
#include "NTT.hpp"

// 传入多项式的升幂系数 a 和常数 c，返回 $f(x+c)$ 的升幂系数，长度不变；空输入返回空数组。
// Z 为质数模类型，要求数组长度不超过模数，并满足现有 NTT 的条件；时间为 $O(n\log n)$。
template <typename Z>
vector<Z> taylor_shift(const vector<Z>& a, Z c) {
    int n = a.size();
    if (n == 0) return {};
    assert(n <= Z::mod());
    vector<Z> fact(n, 1), factinv(n, 1), b(n), d(n);
    for (int i = 1; i < n; i++) fact[i] = fact[i - 1] * i;
    factinv[n - 1] = fact[n - 1].inv();
    for (int i = n - 1; i > 0; i--) factinv[i - 1] = factinv[i] * i;
    Z mul = 1;
    for (int i = 0; i < n; i++) {
        b[n - 1 - i] = a[i] * fact[i];
        d[i] = mul * factinv[i];
        mul *= c;
    }
    auto ret = convolution(b, d, n);
    reverse(ret.begin(), ret.end());
    for (int i = 0; i < n; i++) ret[i] *= factinv[i];
    return ret;
}
