#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Tarjan 算法求割点
 * 适用于无向图。
 */
struct TarjanCutPoint {
    int n, timer;
    vector<vector<int>> g;
    vector<int> dfn, low;
    vector<bool> is_cut;

    TarjanCutPoint(int n) : n(n), timer(0), g(n + 1), 
                            dfn(n + 1, 0), low(n + 1, 0), is_cut(n + 1, false) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void tarjan(int u, int fa) {
        dfn[u] = low[u] = ++timer;
        int child = 0;
        for (int v : g[u]) {
            if (!dfn[v]) {
                child++;
                tarjan(v, u);
                low[u] = min(low[u], low[v]);
                if (fa != -1 && low[v] >= dfn[u]) is_cut[u] = true;
            } else if (v != fa) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (fa == -1 && child >= 2) is_cut[u] = true;
    }

    int count() {
        int cnt = 0;
        for (int i = 1; i <= n; i++) if (is_cut[i]) cnt++;
        return cnt;
    }
};

int main() {
    int n, m; cin >> n >> m;
    TarjanCutPoint tcp(n);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        tcp.add_edge(u, v);
    }
    for (int i = 1; i <= n; i++) if (!tcp.dfn[i]) tcp.tarjan(i, -1);
    cout << tcp.count() << endl;
    return 0;
}
