#pragma once
#include "FWHT.hpp"

// 传入按集合掩码存储、长度同为 $2^k$ 的非空数组 a、b，返回只合并不相交集合的卷积。
// 结果为 $c_S=\sum_{T\subseteq S}a_T b_{S\setminus T}$；时间为 $O(k^2 2^k)$。
// 主要数组存放约 $2(k+1)2^k$ 个 Z；当 $k=20$ 且 Z 为 mint 时，约占 $168\,\mathrm{MiB}$。
template <typename Z>
vector<Z> subset_convolution(const vector<Z>& a, const vector<Z>& b) {
    int n = a.size();
    assert(n > 0 && (n & (n - 1)) == 0 && a.size() == b.size());
    int k = __builtin_ctz((unsigned)n);
    vector<int> cnt(n);
    vector<vector<Z>> A(k + 1, vector<Z>(n)), B = A;
    for (int s = 0; s < n; s++) {
        cnt[s] = __builtin_popcount((unsigned)s);
        A[cnt[s]][s] = a[s];
        B[cnt[s]][s] = b[s];
    }
    for (int i = 0; i <= k; i++) {
        fwt_or(A[i]);
        fwt_or(B[i]);
    }
    vector<Z> ret(n);
    for (int i = 0; i <= k; i++) {
        vector<Z> c(n);
        for (int j = 0; j <= i; j++) {
            for (int s = 0; s < n; s++) c[s] += A[j][s] * B[i - j][s];
        }
        fwt_or(c, true);
        for (int s = 0; s < n; s++) if (cnt[s] == i) ret[s] = c[s];
    }
    return ret;
}
