#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;

/**
 * @brief SPFA 判负环
 * 适用场景：判断图中是否存在负权环（不连通图也能检测所有连通块）
 * 原理：cnt[v] 记录到 v 的最短路经过的边数，若 cnt[v] >= n 说明必有负环
 * 复杂度：最坏 O(nm)
 */
struct SPFA_Cycle {
    int n;
    vector<vector<pair<int, ll>>> g;
    vector<ll> dis;
    vector<int> cnt;
    vector<bool> inq;

    SPFA_Cycle(int n) : n(n), g(n + 1), dis(n + 1, 0), cnt(n + 1, 0), inq(n + 1, false) {}

    void add_edge(int u, int v, ll w) {
        g[u].push_back({v, w});
    }

    bool has_negative_cycle() {
        fill(dis.begin(), dis.end(), 0);    // 初始全 0，等价于加一个到所有点的超级源点
        fill(cnt.begin(), cnt.end(), 0);
        fill(inq.begin(), inq.end(), false);
        queue<int> q;
        for (int i = 1; i <= n; i++) q.push(i), inq[i] = true;  // 所有点入队
        while (!q.empty()) {
            int u = q.front(); q.pop(); inq[u] = false;
            for (auto e : g[u]) {
                int v = e.first;
                ll w = e.second;
                if (dis[u] + w < dis[v]) {
                    dis[v] = dis[u] + w;
                    cnt[v] = cnt[u] + 1;
                    if (cnt[v] >= n) return true;   // 存在负环
                    if (!inq[v]) q.push(v), inq[v] = true;
                }
            }
        }
        return false;
    }
};
// 注意：若只需要判断「从 s 出发能到达的负环」，改为只把 s 入队，dis[s] = 0，其余 INF

// ================= 使用示例 =================
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int T; cin >> T;
    while (T--) {
        int n, m;
        cin >> n >> m;
        SPFA_Cycle sc(n);
        for (int i = 0; i < m; i++) {
            int u, v; ll w; cin >> u >> v >> w;
            sc.add_edge(u, v, w);
        }
        cout << (sc.has_negative_cycle() ? "YES" : "NO") << "\n";
    }
    return 0;
}
