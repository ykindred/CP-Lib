#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;

/**
 * @brief Floyd 全源最短路
 * 适用场景：n 较小（约 n <= 500）时的全源最短路
 * 复杂度：O(n^3)
 */
struct Floyd {
    int n;
    vector<vector<ll>> dis;

    Floyd(int n) : n(n), dis(n + 1, vector<ll>(n + 1, INF)) {
        for (int i = 1; i <= n; i++) dis[i][i] = 0;
    }

    void add_edge(int u, int v, ll w) {
        dis[u][v] = min(dis[u][v], w);  // 自动处理重边
    }

    void solve() {
        for (int k = 1; k <= n; k++)
            for (int i = 1; i <= n; i++)
                for (int j = 1; j <= n; j++)
                    if (dis[i][k] < INF && dis[k][j] < INF)
                        dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j]);
    }
};
