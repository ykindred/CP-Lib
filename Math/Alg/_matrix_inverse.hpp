#pragma once
#include "../head.hpp"
#include "../NT/modint.hpp"

// 传入方阵 a，返回 {是否可逆, 逆矩阵}；不可逆时返回 {false, {}}。
// 空矩阵返回 {true, {}}。Z 须为域元素，通常使用质数模 mint；时间为 $O(n^3)$。
template <typename Z>
pair<bool, vector<vector<Z>>> matrix_inv(vector<vector<Z>> a) {
    int n = a.size();
    for (int i = 0; i < n; i++) {
        assert((int)a[i].size() == n);
        a[i].resize(2 * n);
        a[i][n + i] = 1;
    }
    for (int col = 0; col < n; col++) {
        int pivot = col;
        while (pivot < n && a[pivot][col] == Z(0)) pivot++;
        if (pivot == n) return {false, {}};
        swap(a[pivot], a[col]);
        Z iv = Z(1) / a[col][col];
        for (int j = col; j < 2 * n; j++) a[col][j] *= iv;
        for (int i = 0; i < n; i++) {
            if (i == col || a[i][col] == Z(0)) continue;
            Z t = a[i][col];
            for (int j = col; j < 2 * n; j++) a[i][j] -= t * a[col][j];
        }
    }
    for (auto& row : a) row.erase(row.begin(), row.begin() + n);
    return {true, a};
}
