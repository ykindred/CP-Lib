#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 倍增法求最近公共祖先 (LCA)
 * 预处理 O(n log n)，单次查询 O(log n)。
 */
struct BinaryLCA {
    int n, LOG;
    vector<vector<int>> g, anc;
    vector<int> depth;

    BinaryLCA(int n) : n(n), LOG(20), g(n + 1), anc(n + 1, vector<int>(21, 0)), depth(n + 1, 0) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int fa, int d) {
        depth[u] = d;
        anc[u][0] = fa;
        for (int i = 1; i <= LOG; i++) {
            anc[u][i] = anc[anc[u][i - 1]][i - 1];
        }
        for (int v : g[u]) {
            if (v != fa) dfs(v, u, d + 1);
        }
    }

    int get_lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        for (int i = LOG; i >= 0; i--) {
            if (depth[a] - (1 << i) >= depth[b]) a = anc[a][i];
        }
        if (a == b) return a;
        for (int i = LOG; i >= 0; i--) {
            if (anc[a][i] != anc[b][i]) {
                a = anc[a][i];
                b = anc[b][i];
            }
        }
        return anc[a][0];
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    BinaryLCA lca(n);
    for (int i = 2; i <= n; i++) {
        int p; cin >> p; // 假设给出的是父节点
        lca.add_edge(p, i);
    }
    lca.dfs(1, 0, 1);
    while (q--) {
        int a, b; cin >> a >> b;
        cout << lca.get_lca(a, b) << "\n";
    }
    return 0;
}
