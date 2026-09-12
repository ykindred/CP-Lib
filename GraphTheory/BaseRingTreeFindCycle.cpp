#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 基环树找环
 * 适用场景：n 个点 n 条边的连通图（基环树）中找出环上的点
 * 思路：类似拓扑排序，不断删掉度数为 1 的叶子，
 *       删完后度数 > 1 的点就是环上的点
 * 注意：若为基环树森林（n 个点 n 条边但不连通），需对每个连通块分别处理
 */
struct BaseRingTree {
    int n;
    vector<vector<int>> g;
    vector<int> in;        // 度数
    vector<bool> on_ring;  // 是否在环上

    BaseRingTree(int n) : n(n), g(n + 1), in(n + 1, 0), on_ring(n + 1, false) {}

    void add_edge(int u, int v) {  // 无向边
        g[u].push_back(v); in[v]++;
        g[v].push_back(u); in[u]++;
    }

    // 返回环上的点（按环上顺序排列）
    vector<int> find_ring() {
        vector<int> deg = in;
        queue<int> q;
        for (int i = 1; i <= n; i++)
            if (deg[i] == 1) q.push(i);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u])
                if (--deg[v] == 1) q.push(v);
        }
        // 删完叶子后，deg[i] > 1 的点就是环上的点
        for (int i = 1; i <= n; i++)
            if (deg[i] > 1) on_ring[i] = true;

        // 从环上一个点出发，沿环遍历得到环的顺序
        int start = -1;
        for (int i = 1; i <= n; i++)
            if (on_ring[i]) { start = i; break; }

        vector<int> ring;
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
        return ring;
    }
};

// ================= 使用示例 =================
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n; cin >> n;
    BaseRingTree brt(n);
    for (int i = 0; i < n; i++) {
        int u, v; cin >> u >> v;
        brt.add_edge(u, v);
    }
    vector<int> ring = brt.find_ring();
    for (int x : ring) cout << x << " ";
    cout << "\n";
    return 0;
}
