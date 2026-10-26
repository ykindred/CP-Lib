#pragma once
#include "../head.hpp"

// 传入各堆石子数 a，判断普通 Nim 是否先手必胜；空局面返回 false。
inline bool nim_win(const vector<u64>& a) {
    u64 ret = 0;
    for (u64 x : a) ret ^= x;
    return ret != 0;
}
// 传入各堆石子数 a，判断拿走最后一颗者输的反常 Nim 是否先手必胜。
// 无石子局面按反常规则返回 true；不能将此规则直接用于一般 SG 游戏。
inline bool misere_nim_win(const vector<u64>& a) {
    u64 ret = 0;
    int ones = 0;
    bool big = false;
    for (u64 x : a) {
        ret ^= x;
        ones += x == 1;
        big |= x > 1;
    }
    return big ? ret != 0 : ones % 2 == 0;
}

// 传入无环局面图 g，g[u] 列出局面 u 一步可到达的局面；返回各点的 SG 值。
// 点从零编号，不能行动者输；若存在环则断言失败。时间为 $O(n+m)$。
inline vector<int> sg_dag(const vector<vector<int>>& g) {
    int n = g.size(), cnt = 0;
    vector<vector<int>> rev(n);
    vector<int> deg(n), sg(n), seen;
    queue<int> q;
    for (int u = 0; u < n; u++) {
        deg[u] = g[u].size();
        for (int v : g[u]) rev[v].push_back(u);
        if (deg[u] == 0) q.push(u);
    }
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        cnt++;
        seen.assign(g[u].size() + 1, 0);
        for (int v : g[u]) {
            if (sg[v] < (int)seen.size()) seen[sg[v]] = 1;
        }
        while (seen[sg[u]]) sg[u]++;
        for (int v : rev[u]) if (--deg[v] == 0) q.push(v);
    }
    assert(cnt == n);
    return sg;
}
