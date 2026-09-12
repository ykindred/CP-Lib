#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;

/**
 * @brief 分层图最短路
 * 适用场景：带「免费 / 优惠 / 特殊状态」次数限制的最短路（如最多免费走 k 条边）
 * 建图思路：把每个点拆成 k+1 层（0 ~ k 层，层数 = 已用掉的机会数）
 *           - 层内连边：边权 w（正常走）
 *           - 跨层连边：边权 0（消耗一次机会）
 * 节点编号：u + level * n
 * 复杂度：O((k+1)(n+m) log(kn))
 */
struct LayeredDijkstra {
    int n, k;
    vector<vector<pair<int, ll>>> g;
    vector<ll> dis;

    LayeredDijkstra(int n, int k) : n(n), k(k),
        g(n * (k + 1) + 1), dis(n * (k + 1) + 1) {}

    // 添加一条有向边 u -> v，权值 w（无向图需要正反各加一次）
    void add_edge(int u, int v, ll w) {
        for (int lv = 0; lv <= k; lv++) {
            g[u + lv * n].push_back({v + lv * n, w});            // 层内正常走
            if (lv < k)
                g[u + lv * n].push_back({v + (lv + 1) * n, 0});  // 跨层，消耗一次机会
        }
    }

    // s 到 t 的最短路（免费机会不强制用完，终点取各层最小值）
    ll solve(int s, int t) {
        fill(dis.begin(), dis.end(), INF);
        dis[s] = 0;
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
        pq.push({0, s});
        while (!pq.empty()) {
            pair<ll, int> cur = pq.top(); pq.pop();
            ll d = cur.first;
            int u = cur.second;
            if (d > dis[u]) continue;
            for (auto e : g[u]) {
                int v = e.first;
                ll w = e.second;
                if (dis[u] + w < dis[v]) {
                    dis[v] = dis[u] + w;
                    pq.push({dis[v], v});
                }
            }
        }
        ll ans = INF;
        for (int lv = 0; lv <= k; lv++) ans = min(ans, dis[t + lv * n]);
        return ans;
    }
};

// ================= 使用示例 =================
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m, k, s, t;
    cin >> n >> m >> k >> s >> t;
    LayeredDijkstra ld(n, k);
    for (int i = 0; i < m; i++) {
        int u, v; ll w;
        cin >> u >> v >> w;
        ld.add_edge(u, v, w);
        ld.add_edge(v, u, w);  // 无向边
    }
    cout << ld.solve(s, t) << "\n";
    return 0;
}
