#pragma once
#include "../head.hpp"
#include "../NT/modint.hpp"

// O(n * m * min(n, m))
// 求解 Ax = b，a 为 n 行 m+1 列增广矩阵
// 返回 {rank, x}，无解时 rank = -1
// rank = m 时唯一解，否则有自由变量（取 0）
template <typename Z>
pair<int, vector<Z>> gauss(vector<vector<Z>> a, double eps = 1e-9) {
    int n = a.size(), m = a[0].size() - 1;
    vector<int> pos;
    int rank = 0;

    auto zero = [&](Z x) {
        if constexpr (is_floating_point_v<Z>) {
            return abs(x) <= eps;
        } else {
            return x == Z(0);
        }
    };

    for (int col = 0; col < m && rank < n; col++) {
        int pivot = rank;
        if constexpr (is_floating_point_v<Z>) {
            for (int i = rank + 1; i < n; i++) {
                if (abs(a[i][col]) > abs(a[pivot][col])) {
                    pivot = i;
                }
            }
            if (zero(a[pivot][col])) continue;
        } else {
            while (pivot < n && zero(a[pivot][col])) {
                pivot++;
            }
            if (pivot == n) continue;
        }

        swap(a[pivot], a[rank]);
        Z iv = Z(1) / a[rank][col];
        for (int j = col; j <= m; j++) {
            a[rank][j] *= iv;
        }

        for (int i = rank + 1; i < n; i++) {
            Z t = a[i][col];
            if (t == Z(0)) continue;
            for (int j = col; j <= m; j++) {
                a[i][j] -= t * a[rank][j];
            }
        }
        pos.push_back(col);
        rank++;
    }

    for (int i = rank; i < n; i++) {
        if (!zero(a[i][m])) {
            return {-1, {}};
        }
    }

    vector<Z> x(m);
    for (int i = rank - 1; i >= 0; i--) {
        int col = pos[i];
        Z val = a[i][m];
        for (int j = col + 1; j < m; j++) {
            val -= a[i][j] * x[j];
        }
        x[col] = val;
    }
    
    // 基础解系
    // vector<bool> is_pivot(m);
    // for (int col : pos) {
    //     is_pivot[col] = true;
    // }

    // vector<vector<Z>> basis;
    // for (int j = 0; j < m; j++) {
    //     if (is_pivot[j]) continue;

    //     vector<Z> v(m);
    //     v[j] = 1;

    //     for (int i = rank - 1; i >= 0; i--) {
    //         int col = pos[i];
    //         for (int k = col + 1; k < m; k++) {
    //             v[col] -= a[i][k] * v[k];
    //         }
    //     }
    //     basis.push_back(v);
    // }
    // return { rank, x, basis };
    return { rank, x };
}
