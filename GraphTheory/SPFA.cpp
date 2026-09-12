#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;

/**
 * @brief SPFA
 * 适用场景：单源最短路，可处理负权边（但不能有负环）
 * 复杂度：最坏 O(nm)，随机图表现接近 O(m)
 */
struct SPFA {
    int n;
    vector<vector<pair<int, ll>>> g;
    vector<ll> dis;
    vector<bool> inq;

    SPFA(int n) : n(n), g(n + 1), dis(n + 1), inq(n + 1, false) {}

    void add_edge(int u, int v, ll w) {
        g[u].push_back({v, w});
    }

    vector<ll> solve(int s) {
        fill(dis.begin(), dis.end(), INF);
        dis[s] = 0;
        queue<int> q;
        q.push(s); inq[s] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop(); inq[u] = false;
            for (auto e : g[u]) {
                int v = e.first;
                ll w = e.second;
                if (dis[u] + w < dis[v]) {
                    dis[v] = dis[u] + w;
                    if (!inq[v]) q.push(v), inq[v] = true;
                }
            }
        }
        return dis;
    }
};
