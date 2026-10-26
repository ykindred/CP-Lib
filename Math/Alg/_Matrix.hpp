#pragma once
#include "../head.hpp"
#include "../NT/modint.hpp"

// 存储矩阵并提供加减、乘法和非负整数次幂；Z 是元素类型。
// 可使用 mint、ll 或浮点类型；整数运算须保证结果不溢出。
template <typename Z>
struct Matrix {
    int n, m;
    vector<vector<Z>> a;
    // 传入非负行数 n、列数 m，构造一个全零矩阵。
    Matrix(int n, int m) : n(n), m(m), a(n, vector<Z>(m)) {}
    // 传入各行等长的二维数组 a，以其内容构造矩阵；空数组表示空矩阵。
    Matrix(vector<vector<Z>> a) : n(a.size()), m(n ? a[0].size() : 0), a(move(a)) {}
    // 传入行下标 i，返回该行的引用，可直接读写元素。
    vector<Z>& operator[](int i) { return a[i]; }
    // 传入行下标 i，返回该行的只读引用。
    const vector<Z>& operator[](int i) const { return a[i]; }

    // 传入阶数 n，返回 n 阶单位矩阵。
    static Matrix identity(int n) {
        Matrix ret(n, n);
        for (int i = 0; i < n; i++) ret[i][i] = 1;
        return ret;
    }
    // 传入同型矩阵 b，返回逐项相加的结果，不修改原矩阵。
    Matrix operator+(const Matrix& b) const {
        assert(n == b.n && m == b.m);
        Matrix ret = *this;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) ret[i][j] += b[i][j];
        }
        return ret;
    }
    // 传入同型矩阵 b，返回逐项相减的结果，不修改原矩阵。
    Matrix operator-(const Matrix& b) const {
        assert(n == b.n && m == b.m);
        Matrix ret = *this;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) ret[i][j] -= b[i][j];
        }
        return ret;
    }
    
    // 传入行数等于当前列数的矩阵 b，返回矩阵乘积。
    // 按行访问并跳过零系数；时间为 $O(nmq)$，q 是 b 的列数。
    Matrix operator*(const Matrix& b) const {
        assert(m == b.n);
        Matrix ret(n, b.m);
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < m; k++) {
                Z t = a[i][k];
                if (t == Z(0)) continue;
                for (int j = 0; j < b.m; j++) ret[i][j] += t * b[k][j];
            }
        }
        return ret;
    }
    // 传入长度为 m 的列向量 b，返回矩阵与列向量的乘积。
    vector<Z> operator*(const vector<Z>& b) const {
        assert(m == (int)b.size());
        vector<Z> ret(n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) ret[i] += a[i][j] * b[j];
        }
        return ret;
    }
    
    // 传入非负整数 k，返回当前方阵的 k 次幂；零次幂为单位矩阵。
    // 要求当前矩阵为方阵，时间为 $O(n^3\log k)$。
    Matrix pow(ll k) const {
        assert(n == m && k >= 0);
        Matrix ret = identity(n), mul = *this;
        for (; k; k >>= 1, mul = mul * mul) {
            if (k & 1) ret = ret * mul;
        }
        return ret;
    }
};
