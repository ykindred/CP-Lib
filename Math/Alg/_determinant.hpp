#pragma once
#include "../head.hpp"
#include "../NT/modint.hpp"

// 传入方阵 a，返回其行列式；空矩阵返回一，不修改传入的原数组。
// Z 必须是域元素，通常使用质数模 mint；不适用于整数除法或一般合数模。时间为 $O(n^3)$。
template <typename Z>
Z determinant(vector<vector<Z>> a) {
    int n = a.size();
    for (const auto& row : a) assert((int)row.size() == n);
    Z ret = 1;
    for (int col = 0; col < n; col++) {
        int pivot = col;
        while (pivot < n && a[pivot][col] == Z(0)) pivot++;
        if (pivot == n) return 0;
        if (pivot != col) {
            swap(a[pivot], a[col]);
            ret = -ret;
        }
        ret *= a[col][col];
        Z iv = Z(1) / a[col][col];
        for (int i = col + 1; i < n; i++) {
            Z t = a[i][col] * iv;
            for (int j = col + 1; j < n; j++) a[i][j] -= t * a[col][j];
            a[i][col] = 0;
        }
    }
    return ret;
}
