#pragma once
#include "../head.hpp"

// 传入树的邻接表 g，返回每次删除最小叶子得到的 Prüfer 序列。
// 点从零编号，点数 n 至少为二，返回长度为 $n-2$ 的数组；时间为 $O(n\log n)$。
inline vector<int> prufer_encode(const vector<vector<int>>& g) {
    int n = g.size();
    assert(n >= 2);
    vector<int> deg(n), ret;
    priority_queue<int, vector<int>, greater<int>> q;
    for (int i = 0; i < n; i++) {
        deg[i] = g[i].size();
        if (deg[i] == 1) q.push(i);
    }
    for (int i = 0; i < n - 2; i++) {
        int u = q.top();
        q.pop();
        deg[u] = 0;
        for (int v : g[u]) {
            if (deg[v] == 0) continue;
            ret.push_back(v);
            if (--deg[v] == 1) q.push(v);
            break;
        }
    }
    return ret;
}

// 传入 Prüfer 序列 a，返回对应树的无向边列表，共 $n-1$ 条边，其中 $n=|a|+2$。
// 序列元素须在 $[0,n)$ 内；空序列对应两个点的树，时间为 $O(n\log n)$。
inline vector<pair<int, int>> prufer_decode(const vector<int>& a) {
    int n = a.size() + 2;
    vector<int> deg(n, 1);
    for (int x : a) {
        assert(0 <= x && x < n);
        deg[x]++;
    }
    priority_queue<int, vector<int>, greater<int>> q;
    for (int i = 0; i < n; i++) if (deg[i] == 1) q.push(i);
    vector<pair<int, int>> ret;
    for (int x : a) {
        int u = q.top();
        q.pop();
        ret.emplace_back(u, x);
        if (--deg[x] == 1) q.push(x);
    }
    int u = q.top();
    q.pop();
    ret.emplace_back(u, q.top());
    return ret;
}
