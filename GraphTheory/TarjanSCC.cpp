#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Tarjan 算法求强连通分量 (SCC)
 * 适用于有向图。
 */
struct TarjanSCC {
    int n, timer, scc_cnt;
    vector<vector<int>> g, sccs;
    vector<int> dfn, low, scc_id;
    vector<bool> instk;
    stack<int> stk;

    TarjanSCC(int n) : n(n), timer(0), scc_cnt(0), g(n + 1), 
                       dfn(n + 1, 0), low(n + 1, 0), scc_id(n + 1, 0), instk(n + 1, false) {}

    void add_edge(int u, int v) { g[u].push_back(v); }

    void tarjan(int u) {
        dfn[u] = low[u] = ++timer;
        stk.push(u);
        instk[u] = true;

        for (int v : g[u]) {
            if (!dfn[v]) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (instk[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (low[u] == dfn[u]) {
            scc_cnt++;
            vector<int> current_scc;
            while (true) {
                int v = stk.top(); stk.pop();
                instk[v] = false;
                scc_id[v] = scc_cnt;
                current_scc.push_back(v);
                if (u == v) break;
            }
            sccs.push_back(current_scc);
        }
    }

    void solve() {
        for (int i = 1; i <= n; i++) if (!dfn[i]) tarjan(i);
        reverse(sccs.begin(), sccs.end()); // 拓扑序反转
    }
};

int main() {
    int n, m; cin >> n >> m;
    TarjanSCC ts(n);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        ts.add_edge(u, v);
    }
    ts.solve();
    cout << ts.scc_cnt << endl;
    for (auto &scc : ts.sccs) {
        cout << scc.size() << " ";
        for (int v : scc) cout << v << " ";
        cout << "\n";
    }
    return 0;
}
