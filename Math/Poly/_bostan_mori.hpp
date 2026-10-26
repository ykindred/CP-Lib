#pragma once
#include "NTT.hpp"

// 传入按升幂存储的分子 p、分母 q 和非负下标 n，返回形式幂级数 $P(x)/Q(x)$ 的第 n 项系数。
// 要求 q 非空、q[0] 可逆，并满足现有 NTT 的条件；当 $\deg P<\deg Q=k$ 时，时间为 $O(k\log k\log n)$。
template <typename Z>
Z bostan_mori(vector<Z> p, vector<Z> q, ll n) {
    assert(n >= 0 && !q.empty() && q[0] != Z(0));
    while (n && !p.empty()) {
        auto b = q;
        for (int i = 1; i < (int)b.size(); i += 2) b[i] = -b[i];
        auto a = convolution(p, b), c = convolution(q, b);
        p.clear();
        q.clear();
        for (int i = n & 1; i < (int)a.size(); i += 2) p.push_back(a[i]);
        for (int i = 0; i < (int)c.size(); i += 2) q.push_back(c[i]);
        n >>= 1;
    }
    return p.empty() ? Z(0) : p[0] / q[0];
}

// 传入已知前缀 a、递推系数 c 和非负下标 n，返回第 n 项；c[0] 对应最近一项。
// a 至少包含前 k 项，其中 k 是 c 的长度；空递推的后续项为零。使用现有 NTT 加速。
template <typename Z>
Z linear_nth_ntt(const vector<Z>& a, const vector<Z>& c, ll n) {
    assert(n >= 0 && a.size() >= c.size());
    if (n < (ll)a.size()) return a[n];
    int k = c.size();
    if (k == 0) return 0;
    vector<Z> q(k + 1), b(a.begin(), a.begin() + k);
    q[0] = 1;
    for (int i = 0; i < k; i++) q[i + 1] = -c[i];
    auto p = convolution(b, q, k);
    return bostan_mori(move(p), move(q), n);
}
