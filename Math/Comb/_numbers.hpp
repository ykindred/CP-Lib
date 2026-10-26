#pragma once
#include "../head.hpp"
#include "factorial.hpp"

// 传入非负上限 n，返回从零到 n 的错排数，按元素类型 Z 运算。
// 空排列计为一个错排；时间和空间均为 $O(n)$。
template <typename Z = mint>
vector<Z> derangements(int n) {
    assert(n >= 0);
    vector<Z> ret(n + 1);
    ret[0] = 1;
    for (int i = 2; i <= n; i++) ret[i] = (ret[i - 1] + ret[i - 2]) * Z(i - 1);
    return ret;
}

// 传入非负上限 n，返回从零到 n 的无序正整数拆分数，零的拆分数为一。
// 使用五边形数递推，时间为 $O(n\sqrt n)$，空间为 $O(n)$。
template <typename Z = mint>
vector<Z> partition_numbers(int n) {
    assert(n >= 0);
    vector<Z> ret(n + 1);
    ret[0] = 1;
    for (int i = 1; i <= n; i++) {
        for (ll j = 1; j * (3 * j - 1) / 2 <= i; j++) {
            ll a = j * (3 * j - 1) / 2, b = a + j;
            Z val = ret[i - a];
            if (b <= i) val += ret[i - b];
            ret[i] += j & 1 ? val : -val;
        }
    }
    return ret;
}

// 传入非负上限 n，返回从零到 n 的 Bell 数，即不同元素划分为无标号非空集合的方案数。
// 空集有一种划分；时间为 $O(n^2)$，额外空间为 $O(n)$。
template <typename Z = mint>
vector<Z> bell_numbers(int n) {
    assert(n >= 0);
    vector<Z> row(n + 1), ret(n + 1);
    row[0] = ret[0] = 1;
    for (int i = 1; i <= n; i++) {
        Z prev = row[0];
        row[0] = row[i - 1];
        for (int j = 1; j <= i; j++) {
            Z cur = row[j];
            row[j] = row[j - 1] + prev;
            prev = cur;
        }
        ret[i] = row[0];
    }
    return ret;
}

// 传入非负下标 n 和现有阶乘表 f，返回第 n 个 Catalan 数，要求 $2n\le N<P$。
// P 须为质数，返回值类型为 ModInt<P>；零号 Catalan 数为一。
template <int N, int P>
ModInt<P> catalan(ll n, Factorial<N, P>& f) {
    assert(0 <= n && n <= N / 2);
    return f.nCr(2 * n, n) - f.nCr(2 * n, n - 1);
}

// 传入正步数 a、负步数 b 和阶乘表 f，返回所有前缀和非负的步序列数量。
// a、b 均非负，要求 $a+b\le N<P$ 且 P 为质数；当 $a<b$ 时返回零。
template <int N, int P>
ModInt<P> ballot(ll a, ll b, Factorial<N, P>& f) {
    assert(a >= 0 && b >= 0 && a <= N && b <= N - a);
    if (a < b) return 0;
    return f.nCr(a + b, b) - f.nCr(a + b, b - 1);
}
// 传入各类元素数量 cnt 和阶乘表 f，返回多重集排列数。
// 数量均非负，要求总数不超过 N 且 $N<P$，P 为质数；空多重集返回一。
template <int N, int P>
ModInt<P> multinomial(const vector<int>& cnt, Factorial<N, P>& f) {
    int n = 0;
    ModInt<P> ret = 1;
    for (int x : cnt) {
        assert(x >= 0 && x <= N - n);
        n += x;
        ret *= f.factinv[x];
    }
    return ret * f.fact[n];
}

// 传入正点数 n，返回 n 个标号点的无根树数量，按类型 Z 运算；单点树返回一。
template <typename Z = mint>
Z cayley(ll n) {
    assert(n >= 1);
    return n == 1 ? Z(1) : Z(n).pow(n - 2);
}

// 传入各点度数 deg 和阶乘表 f，返回满足这些度数的标号树数量；无此类树时返回零。
// 至少有一个点；n 为点数，要求 $n-2\le N<P$ 且 P 为质数，单点树的合法度数为零。
template <int N, int P>
ModInt<P> tree_degree_count(const vector<int>& deg, Factorial<N, P>& f) {
    int n = deg.size();
    assert(n >= 1 && n - 2 <= N);
    if (n == 1) return deg[0] == 0;
    ll sum = 0;
    ModInt<P> ret = f.fact[n - 2];
    for (int d : deg) {
        if (d <= 0 || d >= n) return 0;
        sum += d;
        ret *= f.factinv[d - 1];
    }
    return sum == 2LL * n - 2 ? ret : ModInt<P>(0);
}
