#pragma once
#include "../head.hpp"
#include "../NT/modint.hpp"

// 传入连续点函数值 y 和求值位置 x，返回多项式在 x 处的值；y[i] 是在横坐标 i 处的值。
// 若 y 的长度为 $k+1$，要求 $\deg f\le k<P$，Z 为 ModInt<P> 且 P 为质数；空 y 返回零，时间为 $O(k)$。
template <typename Z>
Z lagrange(const vector<Z>& y, ll x) {
    int n = y.size();
    if (n == 0) return 0;
    assert(n <= Z::mod());
    Z t = x;
    if (t.val < (u32)n) return y[t.val];
    vector<Z> pre(n + 1, 1), suf(n + 1, 1), factinv(n);
    Z fact = 1;
    for (int i = 1; i < n; i++) fact *= i;
    factinv[n - 1] = fact.inv();
    for (int i = n - 1; i > 0; i--) factinv[i - 1] = factinv[i] * i;
    for (int i = 0; i < n; i++) pre[i + 1] = pre[i] * (t - Z(i));
    for (int i = n - 1; i >= 0; i--) suf[i] = suf[i + 1] * (t - Z(i));
    Z ret = 0;
    for (int i = 0; i < n; i++) {
        Z val = y[i] * pre[i] * suf[i + 1] * factinv[i] * factinv[n - 1 - i];
        ret += (n - 1 - i) & 1 ? -val : val;
    }
    return ret;
}

// 传入非负整数 n、k，返回从一到 n 的 k 次幂之和，结果按 Z 的模数计算。
// Z 为质数模 ModInt<P>，要求 $k+1<P$；时间为 $O(k\log(k+1))$。
template <typename Z = mint>
Z power_sum(ll n, int k) {
    assert(n >= 0 && k >= 0 && (ll)k + 1 < Z::mod());
    vector<Z> y(k + 2);
    for (int i = 1; i <= k + 1; i++) y[i] = y[i - 1] + Z(i).pow(k);
    return lagrange(y, n);
}
