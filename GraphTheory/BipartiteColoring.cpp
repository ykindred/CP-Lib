#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 染色法判断二分图
 * 适用场景：判断无向图能否用两种颜色染色，使相邻点颜色不同（即无奇环）
 * 复杂度：O(n + m)
 */
struct BipartiteCheck {
    int n;
    vector<vector<int>> g;
    vector<int> col;  // 0 未染色，1 / 2 两种颜色

    BipartiteCheck(int n) : n(n), g(n + 1), col(n + 1, 0) {}

    void add_edge(int u, int v) {  // 无向边
        g[u].push_back(v);
        g[v].push_back(u);
    }

    bool check() {  // 支持不连通图
        for (int i = 1; i <= n; i++)
            if (col[i] == 0 && !dfs(i, 1))
                return false;
        return true;
    }

    bool dfs(int u, int c) {
        col[u] = c;
        for (int v : g[u]) {
            if (col[v] == 0) {
                if (!dfs(v, 3 - c)) return false;
            } else if (col[v] == c) {
                return false;  // 相邻同色，不是二分图
            }
        }
        return true;
    }
};

// ================= 使用示例 =================
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    BipartiteCheck bc(n);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        bc.add_edge(u, v);
    }
    cout << (bc.check() ? "Yes" : "No") << "\n";
    return 0;
}
