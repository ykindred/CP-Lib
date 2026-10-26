#pragma once
#include "../head.hpp"
#include "../NT/modint.hpp"

// 传入数列前缀 a，返回与它一致的最短递推系数 c，c[0] 对应最近一项；全零前缀返回空数组。
// Z 必须是域元素；n 为前缀长度，时间为 $O(n^2)$。该结果是否对后续成立，仍需题目结构保证。
template <typename Z>
vector<Z> berlekamp_massey(const vector<Z>& a) {
    vector<Z> c{1}, b{1};
    int len = 0, step = 1;
    Z last = 1;
    for (int i = 0; i < (int)a.size(); i++) {
        Z d = a[i];
        for (int j = 1; j <= len; j++) d += c[j] * a[i - j];
        if (d == Z(0)) {
            step++;
            continue;
        }
        auto old = c;
        Z t = d / last;
        c.resize(max(c.size(), b.size() + step));
        for (int j = 0; j < (int)b.size(); j++) c[j + step] -= t * b[j];
        if (2 * len <= i) {
            len = i + 1 - len;
            b = move(old);
            last = d;
            step = 1;
        } else {
            step++;
        }
    }
    c.resize(len + 1);
    c.erase(c.begin());
    for (auto& x : c) x = -x;
    return c;
}

// 传入已知前缀 a、递推系数 c 和非负下标 n，返回第 n 项；c[0] 对应最近一项。
// a 至少含前 k 项，k 为 c 的长度；空递推的后续项为零。Kitamasa 时间为 $O(k^2\log n)$，支持合数模。
template <typename Z>
Z linear_nth(const vector<Z>& a, const vector<Z>& c, ll n) {
    assert(n >= 0 && a.size() >= c.size());
    if (n < (ll)a.size()) return a[n];
    int k = c.size();
    if (k == 0) return 0;
    // 传入两个次数小于 k 的多项式系数数组 a、b，返回乘积对递推特征多项式取模后的系数。
    auto combine = [&](const vector<Z>& a, const vector<Z>& b) {
        vector<Z> ret(2 * k - 1);
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < k; j++) ret[i + j] += a[i] * b[j];
        }
        for (int i = 2 * k - 2; i >= k; i--) {
            for (int j = 0; j < k; j++) ret[i - 1 - j] += ret[i] * c[j];
        }
        ret.resize(k);
        return ret;
    };
    vector<Z> ret(k), mul(k);
    ret[0] = 1;
    if (k == 1) mul[0] = c[0];
    else mul[1] = 1;
    for (; n; n >>= 1, mul = combine(mul, mul)) {
        if (n & 1) ret = combine(ret, mul);
    }
    Z ans = 0;
    for (int i = 0; i < k; i++) ans += ret[i] * a[i];
    return ans;
}

// 传入数列前缀 a 和非负下标 n，先用 BM 恢复递推，再返回据此推算的第 n 项。
// Z 必须是域元素；递推在前缀之外是否成立，须由调用者根据题目保证。
template <typename Z>
Z linear_guess(const vector<Z>& a, ll n) {
    return linear_nth(a, berlekamp_massey(a), n);
}
