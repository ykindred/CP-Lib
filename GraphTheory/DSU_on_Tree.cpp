#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 树上启发式合并 (DSU on Tree)
 * 复杂度 O(n log n)。常用于统计子树内颜色数量等问题。
 */
struct DSUonTree {
    int n;
    vector<vector<int>> g;
    vector<int> color, siz, hson, ans, cnt;
    int dist; // 当前统计的颜色种类数

    DSUonTree(int n) : n(n), g(n + 1), color(n + 1), siz(n + 1, 0), 
                       hson(n + 1, 0), ans(n + 1), cnt(1000005, 0), dist(0) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs_siz(int u, int fa) {
        siz[u] = 1;
        for (int v : g[u]) {
            if (v == fa) continue;
            dfs_siz(v, u);
            siz[u] += siz[v];
            if (siz[v] > siz[hson[u]]) hson[u] = v;
        }
    }

    void update(int u, int fa, int val) {
        if (val == 1) {
            if (cnt[color[u]] == 0) dist++;
            cnt[color[u]]++;
        } else {
            cnt[color[u]]--;
            if (cnt[color[u]] == 0) dist--;
        }
        for (int v : g[u]) {
            if (v != fa) update(v, u, val);
        }
    }

    // 核心逻辑：先处理轻儿子（不保留结果），再处理重儿子（保留结果），最后合并
    void dfs_solve(int u, int fa, bool keep, bool is_hson = false) {
        for (int v : g[u]) {
            if (v != fa && v != hson[u]) dfs_solve(v, u, false);
        }
        if (hson[u]) dfs_solve(hson[u], u, true, true);

        for (int v : g[u]) {
            if (v != fa && v != hson[u]) update(v, u, 1);
        }
        
        if (cnt[color[u]] == 0) dist++;
        cnt[color[u]]++;
        
        ans[u] = dist;

        if (!keep) update(u, fa, -1);
    }
};

int main() {
    int n; cin >> n;
    DSUonTree dsu(n);
    for (int i = 1; i <= n; i++) cin >> dsu.color[i];
    for (int i = 1; i < n; i++) {
        int u, v; cin >> u >> v;
        dsu.add_edge(u, v);
    }
    dsu.dfs_siz(1, 0);
    dsu.dfs_solve(1, 0, false);
    for (int i = 1; i <= n; i++) cout << dsu.ans[i] << " ";
    return 0;
}
