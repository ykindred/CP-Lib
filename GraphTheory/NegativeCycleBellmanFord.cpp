#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;

/**
 * @brief Bellman-Ford 判负环（从源点 1 可达部分）
 * 原理：最短路最多经过 n-1 条边，若第 n 轮仍能松弛则存在负环
 * 复杂度：O(nm)
 */
struct BellmanFord {
    int n;
    struct Edge { int u, v; ll w; };
    vector<Edge> e;
    vector<ll> dis;

    BellmanFord(int n) : n(n), dis(n + 1) {}

    void add_edge(int u, int v, ll w) {
        e.push_back({u, v, w});
    }

    bool has_negative_cycle() {
        fill(dis.begin(), dis.end(), INF);
        dis[1] = 0;
        for (int i = 1; i <= n; i++) {
            bool upd = false;
            for (auto ed : e) {
                if (dis[ed.u] != INF && dis[ed.u] + ed.w < dis[ed.v]) {
                    dis[ed.v] = dis[ed.u] + ed.w;
                    upd = true;
                }
            }
            if (i == n && upd) return true;  // 第 n 轮仍能松弛
        }
        return false;
    }
};
