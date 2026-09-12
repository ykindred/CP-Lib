#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 树的直径 (两次 DFS 法)
 * 注意：仅适用于边权为正的树。
 */
struct TreeDiameter {
    int n, far_node;
    vector<vector<int>> g;
    vector<int> dist;

    TreeDiameter(int n) : n(n), g(n + 1), dist(n + 1), far_node(0) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int fa) {
        if (dist[u] > dist[far_node]) far_node = u;
        for (int v : g[u]) {
            if (v == fa) continue;
            dist[v] = dist[u] + 1;
            dfs(v, u);
        }
    }

    int solve() {
        far_node = 1;
        fill(dist.begin(), dist.end(), 0);
        dfs(1, 0); // 第一次 DFS 找到距离起点最远的点
        
        int start_node = far_node;
        fill(dist.begin(), dist.end(), 0);
        dfs(start_node, 0); // 第二次 DFS 从最远点出发找到直径
        return dist[far_node];
    }
};

int main() {
    int n; cin >> n;
    TreeDiameter td(n);
    for (int i = 1; i < n; i++) {
        int u, v; cin >> u >> v;
        td.add_edge(u, v);
    }
    cout << td.solve() << endl;
    return 0;
}
