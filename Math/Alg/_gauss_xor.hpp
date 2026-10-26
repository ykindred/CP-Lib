#pragma once
#include "../head.hpp"

// 求解异或方程组：a 是增广矩阵，m 是未知数数量，要求 $0\le m<M$。
// 前 m 位为系数，第 m 位为常数；返回 {rank, 一个特解}，自由变量取零，无解时 rank 为 -1。
// 每次整行消元使用 bitset；时间约为 $O(n\min(n,m)M/w)$，w 为机器字长。
template <size_t M>
pair<int, vector<int>> gauss_xor(vector<bitset<M>> a, int m) {
    assert(0 <= m && m < (int)M);
    int n = a.size(), rank = 0;
    vector<int> pos;
    for (int col = 0; col < m && rank < n; col++) {
        int pivot = rank;
        while (pivot < n && !a[pivot][col]) pivot++;
        if (pivot == n) continue;
        swap(a[pivot], a[rank]);
        for (int i = rank + 1; i < n; i++) {
            if (a[i][col]) a[i] ^= a[rank];
        }
        pos.push_back(col);
        rank++;
    }
    for (int i = rank; i < n; i++) {
        if (a[i][m]) return {-1, {}};
    }
    bitset<M> x;
    for (int i = rank - 1; i >= 0; i--) {
        x[pos[i]] = a[i][m] ^ ((a[i] & x).count() & 1);
    }
    vector<int> ret(m);
    for (int i = 0; i < m; i++) ret[i] = x[i];
    return {rank, ret};
}
