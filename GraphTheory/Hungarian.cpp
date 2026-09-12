#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 匈牙利算法求二分图最大匹配
 * 适用场景：二分图最大匹配
 * 相关结论（König 定理）：
 *   - 最小点覆盖 = 最大匹配
 *   - 最大独立集 = 总点数 - 最小点覆盖
 *   - 最小路径覆盖（DAG）= 总点数 - 拆点后的最大匹配
 * 复杂度：O(nm)
 * 注意：左右部点都从 1 开始编号
 */
struct Hungarian {
    int n, m;               // 左部 n 个点，右部 m 个点
    vector<vector<int>> g;  // 左部 -> 右部 的边
    vector<int> match;      // match[v]：右部点 v 匹配的左部点（0 表示未匹配）
    vector<bool> vis;

    Hungarian(int n, int m) : n(n), m(m), g(n + 1), match(m + 1, 0) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
    }

    bool dfs(int u) {
        for (int v : g[u]) {
            if (vis[v]) continue;
            vis[v] = true;
            if (match[v] == 0 || dfs(match[v])) {
                match[v] = u;
                return true;
            }
        }
        return false;
    }

    int solve() {
        int res = 0;
        for (int i = 1; i <= n; i++) {
            vis.assign(m + 1, false);
            if (dfs(i)) res++;
        }
        return res;
    }
};

// ================= 使用示例 =================
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int nL, nR, m;  // 左部 nL 个点，右部 nR 个点，m 条边
    cin >> nL >> nR >> m;
    Hungarian hg(nL, nR);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        hg.add_edge(u, v);
    }
    cout << hg.solve() << "\n";
    return 0;
}
