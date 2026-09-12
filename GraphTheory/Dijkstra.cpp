#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;

/**
 * @brief Dijkstra 堆优化
 * 适用场景：单源最短路，边权非负（出现负权边会出错）
 * 复杂度：O((n + m) log n)
 */
struct Dijkstra {
    int n;
    vector<vector<pair<int, ll>>> g;  // 邻接表 (v, w)
    vector<ll> dis;

    Dijkstra(int n) : n(n), g(n + 1), dis(n + 1) {}

    void add_edge(int u, int v, ll w) {
        g[u].push_back({v, w});
    }

    // 以 s 为源点求最短路，返回 dis 数组（不可达为 INF）
    vector<ll> solve(int s) {
        fill(dis.begin(), dis.end(), INF);
        dis[s] = 0;
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
        pq.push({0, s});
        while (!pq.empty()) {
            pair<ll, int> cur = pq.top(); pq.pop();
            ll d = cur.first;
            int u = cur.second;
            if (d > dis[u]) continue;          // 跳过过期的松弛信息
            for (auto e : g[u]) {
                int v = e.first;
                ll w = e.second;
                if (dis[u] + w < dis[v]) {
                    dis[v] = dis[u] + w;
                    pq.push({dis[v], v});
                }
            }
        }
        return dis;
    }
};

// ================= 使用示例 =================
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m, s;
    cin >> n >> m >> s;
    Dijkstra dij(n);
    for (int i = 0; i < m; i++) {
        int u, v; ll w; cin >> u >> v >> w;
        dij.add_edge(u, v, w);
    }
    vector<ll> dis = dij.solve(s);
    for (int i = 1; i <= n; i++) cout << dis[i] << " ";
    return 0;
}
