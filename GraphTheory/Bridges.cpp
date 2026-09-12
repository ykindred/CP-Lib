#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Tarjan 算法求割边（桥）
 * 适用于无向图。
 */
struct TarjanBridge {
    int n, m, timer;
    struct Edge { int to, id; };
    vector<vector<Edge>> g;
    vector<int> dfn, low;
    vector<bool> is_bridge;

    TarjanBridge(int n, int m) : n(n), m(m), timer(0), g(n + 1), 
                                 dfn(n + 1, 0), low(n + 1, 0), is_bridge(m + 1, false) {}

    void add_edge(int u, int v, int id) {
        g[u].push_back({v, id});
        g[v].push_back({u, id});
    }

    void tarjan(int u, int from_edge) {
        dfn[u] = low[u] = ++timer;
        for (auto &e : g[u]) {
            if (!dfn[e.to]) {
                tarjan(e.to, e.id);
                low[u] = min(low[u], low[e.to]);
                if (low[e.to] > dfn[u]) is_bridge[e.id] = true;
            } else if (e.id != from_edge) {
                low[u] = min(low[u], dfn[e.to]);
            }
        }
    }
};

int main() {
    int n, m; cin >> n >> m;
    TarjanBridge tb(n, m);
    for (int i = 1; i <= m; i++) {
        int u, v; cin >> u >> v;
        tb.add_edge(u, v, i);
    }
    for (int i = 1; i <= n; i++) if (!tb.dfn[i]) tb.tarjan(i, -1);
    // 后续可根据 is_bridge 统计或 DFS 求双连通分量
    return 0;
}
