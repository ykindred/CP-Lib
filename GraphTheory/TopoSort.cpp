#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 拓扑排序封装
 * 适用场景：有向无环图（DAG）的先后顺序处理，检测环。
 */
struct TopoSort {
    int n;
    vector<int> in;          // 入度数组
    vector<vector<int>> g;   // 邻接表

    TopoSort(int n) : n(n), in(n + 1, 0), g(n + 1) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        in[v]++;
    }

    vector<int> solve() {
        queue<int> q;
        vector<int> res;
        for (int i = 1; i <= n; i++)
            if (in[i] == 0) q.push(i);

        while (!q.empty()) {
            int u = q.front(); q.pop();
            res.push_back(u);
            for (int v : g[u]) {
                if (--in[v] == 0) q.push(v);
            }
        }
        // 如果结果集大小不足 n，说明图中存在环
        return (res.size() == n) ? res : vector<int>();
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    TopoSort ts(n);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        ts.add_edge(u, v);
    }
    vector<int> ans = ts.solve();
    if (ans.empty()) cout << "IMPOSSIBLE\n";
    else {
        for (int i = 0; i < ans.size(); i++) 
            cout << ans[i] << (i == ans.size() - 1 ? "" : " ");
        cout << "\n";
    }
    return 0;
}
