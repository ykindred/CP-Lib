#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 树的重心
 * 性质：删除重心后，最大连通分量的节点数最小，且该值 <= n/2。
 */
struct TreeCentroid {
    int n;
    vector<vector<int>> g;
    vector<int> siz, wei, centroids;

    TreeCentroid(int n) : n(n), g(n + 1), siz(n + 1, 0), wei(n + 1, 0) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int fa) {
        siz[u] = 1;
        wei[u] = 0;
        for (int v : g[u]) {
            if (v == fa) continue;
            dfs(v, u);
            siz[u] += siz[v];
            wei[u] = max(wei[u], siz[v]); // 最大的子树大小
        }
        wei[u] = max(wei[u], n - siz[u]); // 向上连接的部分
        if (wei[u] <= n / 2) centroids.push_back(u);
    }

    int get_one() {
        dfs(1, 0);
        return centroids.empty() ? -1 : centroids[0];
    }
};

int main() {
    int n; cin >> n;
    TreeCentroid tc(n);
    for (int i = 1; i < n; i++) {
        int u, v; cin >> u >> v;
        tc.add_edge(u, v);
    }
    cout << tc.get_one() << endl;
    return 0;
}
