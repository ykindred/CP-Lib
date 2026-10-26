#pragma once
#include "_determinant.hpp"

// 传入点数 n 和无向边 edges，返回所有生成树的边权乘积之和。
// 点从零编号，n 至少为一，边为 {u, v, w}；允许重边，自环忽略，普通计数令边权为一。
// Z 通常取质数模 mint；单点图返回一，时间为 $O(n^3)$。
template <typename Z>
Z spanning_trees(int n, const vector<tuple<int, int, Z>>& edges) {
    assert(n >= 1);
    vector<vector<Z>> a(n - 1, vector<Z>(n - 1));
    for (auto [u, v, w] : edges) {
        if (u == v) continue;
        if (u < n - 1) a[u][u] += w;
        if (v < n - 1) a[v][v] += w;
        if (u < n - 1 && v < n - 1) {
            a[u][v] -= w;
            a[v][u] -= w;
        }
    }
    return determinant(move(a));
}

// 传入点数 n、有向边 edges 和根 root，返回所有朝向根的生成树的权值和。
// 边 {u, v, w} 表示从 u 到 v；每个点须沿树边最终到达 root。点从零编号，Z 为域元素。
template <typename Z>
Z arborescences(int n, const vector<tuple<int, int, Z>>& edges, int root) {
    assert(0 <= root && root < n);
    vector<vector<Z>> a(n - 1, vector<Z>(n - 1));
    // 传入原点编号 x，返回删除 root 后对应的矩阵下标；调用时 x 不能是 root。
    auto id = [&](int x) { return x - (x > root); };
    for (auto [u, v, w] : edges) {
        if (u == v || u == root) continue;
        a[id(u)][id(u)] += w;
        if (v != root) a[id(u)][id(v)] -= w;
    }
    return determinant(move(a));
}
