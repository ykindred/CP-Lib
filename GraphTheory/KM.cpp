#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;
const ll NEG = -1e18;  // 不存在边的权值（要求边权绝对值不超过 1e15）

/**
 * @brief KM 算法求二分图最大权完美匹配
 * 适用场景：左右点数都为 n 的带权二分图，求最大权完美匹配（不存在边的权设为 NEG）
 * 注意：要求存在完美匹配，否则结果会包含 NEG
 * 复杂度：O(n^3)
 */
struct KM {
    int n;
    vector<vector<ll>> w;  // w[i][j]：左 i 与右 j 的边权
    vector<ll> lx, ly;     // 顶标
    vector<int> match;     // match[j]：右 j 匹配的左点
    vector<ll> slack;
    vector<int> pre;       // 交错树记录
    vector<bool> vy;

    KM(int n) : n(n), w(n + 1, vector<ll>(n + 1, NEG)),
                lx(n + 1), ly(n + 1, 0), match(n + 1, 0),
                slack(n + 1), pre(n + 1), vy(n + 1, false) {}

    void set_edge(int i, int j, ll val) {
        w[i][j] = max(w[i][j], val);
    }

    void bfs(int u) {
        fill(slack.begin(), slack.end(), INF);
        fill(pre.begin(), pre.end(), 0);
        fill(vy.begin(), vy.end(), false);
        int y = 0, yy = 0;
        match[0] = u;  // 虚拟点 0
        do {
            int x = match[y];
            ll delta = INF;
            vy[y] = true;
            for (int v = 1; v <= n; v++) {
                if (vy[v]) continue;
                ll gap = lx[x] + ly[v] - w[x][v];
                if (gap < slack[v]) {
                    slack[v] = gap;
                    pre[v] = y;
                }
                if (slack[v] < delta) {
                    delta = slack[v];
                    yy = v;
                }
            }
            for (int v = 0; v <= n; v++) {
                if (vy[v]) {
                    lx[match[v]] -= delta;
                    ly[v] += delta;
                } else {
                    slack[v] -= delta;
                }
            }
            y = yy;
        } while (match[y] != 0);
        while (y) {
            match[y] = match[pre[y]];
            y = pre[y];
        }
    }

    ll solve() {
        for (int i = 1; i <= n; i++) {
            lx[i] = NEG;
            for (int j = 1; j <= n; j++) lx[i] = max(lx[i], w[i][j]);
        }
        for (int i = 1; i <= n; i++) bfs(i);
        ll res = 0;
        for (int j = 1; j <= n; j++) res += w[match[j]][j];
        return res;
    }
};
