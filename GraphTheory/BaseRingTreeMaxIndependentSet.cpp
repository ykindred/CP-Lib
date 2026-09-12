#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = 0x3f3f3f3f3f3f3f3fLL;

/**
 * @brief 基环树最大点权独立集
 * 适用场景：n 个点 n 条边的连通图，选点使相邻点不能同时选，求最大权值和
 * 思路：
 *   1. 拓扑找环
 *   2. 对环上每个点挂的树做树形 DP：dp[u][0/1] 为 u 不选/选时子树最大权
 *   3. 把每个环上点看成「不选它 dp[u][0] / 选它 dp[u][1]」，在环上做环形 DP：
 *      枚举环上第一个点选/不选，做两次线性 DP 取最大值
 * 注意：若为基环树森林，对每个连通块分别求解并累加
 */
struct BaseRingTreeIS {
    int n;
    vector<ll> a;               // 点权
    vector<vector<int>> g;
    vector<int> in;             // 度数
    vector<int> ring;           // 环上点（按顺序）
    vector<bool> on_ring;
    vector<array<ll, 2>> dp;    // dp[u][0/1]：不选 / 选 u 时，u 子树的最大权

    BaseRingTreeIS(int n) : n(n), a(n + 1), g(n + 1), in(n + 1, 0),
                           on_ring(n + 1, false), dp(n + 1) {}

    void add_edge(int u, int v) {  // 无向边
        g[u].push_back(v); in[v]++;
        g[v].push_back(u); in[u]++;
    }

    // 拓扑找环
    void find_ring() {
        vector<int> deg = in;
        queue<int> q;
        for (int i = 1; i <= n; i++)
            if (deg[i] == 1) q.push(i);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u])
                if (--deg[v] == 1) q.push(v);
        }
        for (int i = 1; i <= n; i++)
            if (deg[i] > 1) on_ring[i] = true;

        int start = -1;
        for (int i = 1; i <= n; i++)
            if (on_ring[i]) { start = i; break; }

        vector<bool> vis(n + 1, false);
        int cur = start, pre = 0;
        while (!vis[cur]) {
            vis[cur] = true;
            ring.push_back(cur);
            int nxt = -1;
            for (int v : g[cur]) {
                if (on_ring[v] && v != pre) {  // 环上每个点恰有两个环邻居
                    nxt = v;
                    break;
                }
            }
            if (nxt == -1) break;  // 环已遍历完（如重边构成的二环）
            pre = cur;
            cur = nxt;
        }
    }

    // 对环上点挂的树做树形 DP
    void tree_dfs(int u, int fa) {
        dp[u][0] = 0;
        dp[u][1] = a[u];
        for (int v : g[u]) {
            if (v == fa || on_ring[v]) continue;  // 不进入环
            tree_dfs(v, u);
            dp[u][0] += max(dp[v][0], dp[v][1]);
            dp[u][1] += dp[v][0];
        }
    }

    ll solve() {
        find_ring();
        ll ans = 0;
        for (int u : ring) tree_dfs(u, 0);

        int sz = ring.size();
        // 情况 1：强制不选 ring[0]
        vector<array<ll, 2>> f(sz);           // f[i][0/1]：环上前 i 个点，ring[i] 不选/选
        f[0] = {dp[ring[0]][0], dp[ring[0]][0]};
        for (int i = 1; i < sz; i++) {
            f[i][0] = max(f[i - 1][0], f[i - 1][1]) + dp[ring[i]][0];
            f[i][1] = f[i - 1][0] + dp[ring[i]][1];
        }
        ans = max(ans, max(f[sz - 1][0], f[sz - 1][1]));

        // 情况 2：强制选 ring[0]（此时 ring[sz-1] 不能选）
        vector<array<ll, 2>> h(sz);
        h[0] = {-INF / 2, dp[ring[0]][1]};
        for (int i = 1; i < sz; i++) {
            h[i][0] = max(h[i - 1][0], h[i - 1][1]) + dp[ring[i]][0];
            h[i][1] = h[i - 1][0] + dp[ring[i]][1];
        }
        ans = max(ans, h[sz - 1][0]);
        return ans;
    }
};

// ================= 使用示例 =================
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    BaseRingTreeIS brt(n);
    for (int i = 1; i <= n; i++) cin >> brt.a[i];
    for (int i = 0; i < n; i++) {
        int u, v; cin >> u >> v;
        brt.add_edge(u, v);
    }
    cout << brt.solve() << "\n";
    return 0;
}
