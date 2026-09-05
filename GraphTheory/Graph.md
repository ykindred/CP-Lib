好的，我为你将这些常用的图论和树论算法模板进行了**封装（Struct/Class化）**，并添加了详细的中文注释。封装后的模板更易于在比赛中快速复用，减少全局变量污染。

---

# 拓扑排序

```cpp
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

```

# 树

## 树的直径

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 树的直径 (两次 DFS 法)
 * 注意：仅适用于边权为正的树。
 */
struct TreeDiameter {
    int n, far_node;
    vector<vector<int>> g;
    vector<int> dist;

    TreeDiameter(int n) : n(n), g(n + 1), dist(n + 1), far_node(0) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int fa) {
        if (dist[u] > dist[far_node]) far_node = u;
        for (int v : g[u]) {
            if (v == fa) continue;
            dist[v] = dist[u] + 1;
            dfs(v, u);
        }
    }

    int solve() {
        far_node = 1;
        fill(dist.begin(), dist.end(), 0);
        dfs(1, 0); // 第一次 DFS 找到距离起点最远的点
        
        int start_node = far_node;
        fill(dist.begin(), dist.end(), 0);
        dfs(start_node, 0); // 第二次 DFS 从最远点出发找到直径
        return dist[far_node];
    }
};

int main() {
    int n; cin >> n;
    TreeDiameter td(n);
    for (int i = 1; i < n; i++) {
        int u, v; cin >> u >> v;
        td.add_edge(u, v);
    }
    cout << td.solve() << endl;
    return 0;
}

```

## 树的重心

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 树的重心
 * 性质：删除重心后，最大连通分量的节点数最小，且该值 <= n/2。
 */
struct TreeCentroid {
    int n;
    vector<vector<int>> g;
    vector<int> siz, wei, centroids;

    TreeCentroid(int n) : n(n), g(n + 1), siz(n + 1, 0), wei(n + 1, 0) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int fa) {
        siz[u] = 1;
        wei[u] = 0;
        for (int v : g[u]) {
            if (v == fa) continue;
            dfs(v, u);
            siz[u] += siz[v];
            wei[u] = max(wei[u], siz[v]); // 最大的子树大小
        }
        wei[u] = max(wei[u], n - siz[u]); // 向上连接的部分
        if (wei[u] <= n / 2) centroids.push_back(u);
    }

    int get_one() {
        dfs(1, 0);
        return centroids.empty() ? -1 : centroids[0];
    }
};

int main() {
    int n; cin >> n;
    TreeCentroid tc(n);
    for (int i = 1; i < n; i++) {
        int u, v; cin >> u >> v;
        tc.add_edge(u, v);
    }
    cout << tc.get_one() << endl;
    return 0;
}

```

## 倍增LCA

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 倍增法求最近公共祖先 (LCA)
 * 预处理 O(n log n)，单次查询 O(log n)。
 */
struct BinaryLCA {
    int n, LOG;
    vector<vector<int>> g, anc;
    vector<int> depth;

    BinaryLCA(int n) : n(n), LOG(20), g(n + 1), anc(n + 1, vector<int>(21, 0)), depth(n + 1, 0) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs(int u, int fa, int d) {
        depth[u] = d;
        anc[u][0] = fa;
        for (int i = 1; i <= LOG; i++) {
            anc[u][i] = anc[anc[u][i - 1]][i - 1];
        }
        for (int v : g[u]) {
            if (v != fa) dfs(v, u, d + 1);
        }
    }

    int get_lca(int a, int b) {
        if (depth[a] < depth[b]) swap(a, b);
        for (int i = LOG; i >= 0; i--) {
            if (depth[a] - (1 << i) >= depth[b]) a = anc[a][i];
        }
        if (a == b) return a;
        for (int i = LOG; i >= 0; i--) {
            if (anc[a][i] != anc[b][i]) {
                a = anc[a][i];
                b = anc[b][i];
            }
        }
        return anc[a][0];
    }
};

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, q; cin >> n >> q;
    BinaryLCA lca(n);
    for (int i = 2; i <= n; i++) {
        int p; cin >> p; // 假设给出的是父节点
        lca.add_edge(p, i);
    }
    lca.dfs(1, 0, 1);
    while (q--) {
        int a, b; cin >> a >> b;
        cout << lca.get_lca(a, b) << "\n";
    }
    return 0;
}

```

## 树上启发式合并 (DSU on Tree)

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 树上启发式合并 (DSU on Tree)
 * 复杂度 O(n log n)。常用于统计子树内颜色数量等问题。
 */
struct DSUonTree {
    int n;
    vector<vector<int>> g;
    vector<int> color, siz, hson, ans, cnt;
    int dist; // 当前统计的颜色种类数

    DSUonTree(int n) : n(n), g(n + 1), color(n + 1), siz(n + 1, 0), 
                       hson(n + 1, 0), ans(n + 1), cnt(1000005, 0), dist(0) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void dfs_siz(int u, int fa) {
        siz[u] = 1;
        for (int v : g[u]) {
            if (v == fa) continue;
            dfs_siz(v, u);
            siz[u] += siz[v];
            if (siz[v] > siz[hson[u]]) hson[u] = v;
        }
    }

    void update(int u, int fa, int val) {
        if (val == 1) {
            if (cnt[color[u]] == 0) dist++;
            cnt[color[u]]++;
        } else {
            cnt[color[u]]--;
            if (cnt[color[u]] == 0) dist--;
        }
        for (int v : g[u]) {
            if (v != fa) update(v, u, val);
        }
    }

    // 核心逻辑：先处理轻儿子（不保留结果），再处理重儿子（保留结果），最后合并
    void dfs_solve(int u, int fa, bool keep, bool is_hson = false) {
        for (int v : g[u]) {
            if (v != fa && v != hson[u]) dfs_solve(v, u, false);
        }
        if (hson[u]) dfs_solve(hson[u], u, true, true);

        for (int v : g[u]) {
            if (v != fa && v != hson[u]) update(v, u, 1);
        }
        
        if (cnt[color[u]] == 0) dist++;
        cnt[color[u]]++;
        
        ans[u] = dist;

        if (!keep) update(u, fa, -1);
    }
};

int main() {
    int n; cin >> n;
    DSUonTree dsu(n);
    for (int i = 1; i <= n; i++) cin >> dsu.color[i];
    for (int i = 1; i < n; i++) {
        int u, v; cin >> u >> v;
        dsu.add_edge(u, v);
    }
    dsu.dfs_siz(1, 0);
    dsu.dfs_solve(1, 0, false);
    for (int i = 1; i <= n; i++) cout << dsu.ans[i] << " ";
    return 0;
}

```

# Tarjan

## SCC (强连通分量)

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Tarjan 算法求强连通分量 (SCC)
 * 适用于有向图。
 */
struct TarjanSCC {
    int n, timer, scc_cnt;
    vector<vector<int>> g, sccs;
    vector<int> dfn, low, scc_id;
    vector<bool> instk;
    stack<int> stk;

    TarjanSCC(int n) : n(n), timer(0), scc_cnt(0), g(n + 1), 
                       dfn(n + 1, 0), low(n + 1, 0), scc_id(n + 1, 0), instk(n + 1, false) {}

    void add_edge(int u, int v) { g[u].push_back(v); }

    void tarjan(int u) {
        dfn[u] = low[u] = ++timer;
        stk.push(u);
        instk[u] = true;

        for (int v : g[u]) {
            if (!dfn[v]) {
                tarjan(v);
                low[u] = min(low[u], low[v]);
            } else if (instk[v]) {
                low[u] = min(low[u], dfn[v]);
            }
        }

        if (low[u] == dfn[u]) {
            scc_cnt++;
            vector<int> current_scc;
            while (true) {
                int v = stk.top(); stk.pop();
                instk[v] = false;
                scc_id[v] = scc_cnt;
                current_scc.push_back(v);
                if (u == v) break;
            }
            sccs.push_back(current_scc);
        }
    }

    void solve() {
        for (int i = 1; i <= n; i++) if (!dfn[i]) tarjan(i);
        reverse(sccs.begin(), sccs.end()); // 拓扑序反转
    }
};

int main() {
    int n, m; cin >> n >> m;
    TarjanSCC ts(n);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        ts.add_edge(u, v);
    }
    ts.solve();
    cout << ts.scc_cnt << endl;
    for (auto &scc : ts.sccs) {
        cout << scc.size() << " ";
        for (int v : scc) cout << v << " ";
        cout << "\n";
    }
    return 0;
}

```

## 割边 (Bridges)

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Tarjan 算法求割边（桥）
 * 适用于无向图。
 */
struct TarjanBridge {
    int n, m, timer;
    struct Edge { int to, id; };
    vector<vector<Edge>> g;
    vector<int> dfn, low;
    vector<bool> is_bridge;

    TarjanBridge(int n, int m) : n(n), m(m), timer(0), g(n + 1), 
                                 dfn(n + 1, 0), low(n + 1, 0), is_bridge(m + 1, false) {}

    void add_edge(int u, int v, int id) {
        g[u].push_back({v, id});
        g[v].push_back({u, id});
    }

    void tarjan(int u, int from_edge) {
        dfn[u] = low[u] = ++timer;
        for (auto &e : g[u]) {
            if (!dfn[e.to]) {
                tarjan(e.to, e.id);
                low[u] = min(low[u], low[e.to]);
                if (low[e.to] > dfn[u]) is_bridge[e.id] = true;
            } else if (e.id != from_edge) {
                low[u] = min(low[u], dfn[e.to]);
            }
        }
    }
};

int main() {
    int n, m; cin >> n >> m;
    TarjanBridge tb(n, m);
    for (int i = 1; i <= m; i++) {
        int u, v; cin >> u >> v;
        tb.add_edge(u, v, i);
    }
    for (int i = 1; i <= n; i++) if (!tb.dfn[i]) tb.tarjan(i, -1);
    // 后续可根据 is_bridge 统计或 DFS 求双连通分量
    return 0;
}

```

## 割点 (Cut Points)

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * @brief Tarjan 算法求割点
 * 适用于无向图。
 */
struct TarjanCutPoint {
    int n, timer;
    vector<vector<int>> g;
    vector<int> dfn, low;
    vector<bool> is_cut;

    TarjanCutPoint(int n) : n(n), timer(0), g(n + 1), 
                            dfn(n + 1, 0), low(n + 1, 0), is_cut(n + 1, false) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
        g[v].push_back(u);
    }

    void tarjan(int u, int fa) {
        dfn[u] = low[u] = ++timer;
        int child = 0;
        for (int v : g[u]) {
            if (!dfn[v]) {
                child++;
                tarjan(v, u);
                low[u] = min(low[u], low[v]);
                if (fa != -1 && low[v] >= dfn[u]) is_cut[u] = true;
            } else if (v != fa) {
                low[u] = min(low[u], dfn[v]);
            }
        }
        if (fa == -1 && child >= 2) is_cut[u] = true;
    }

    int count() {
        int cnt = 0;
        for (int i = 1; i <= n; i++) if (is_cut[i]) cnt++;
        return cnt;
    }
};

int main() {
    int n, m; cin >> n >> m;
    TarjanCutPoint tcp(n);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        tcp.add_edge(u, v);
    }
    for (int i = 1; i <= n; i++) if (!tcp.dfn[i]) tcp.tarjan(i, -1);
    cout << tcp.count() << endl;
    return 0;
}

```

# 网络流

## 最大流/最小割 (Dinic)

```cpp
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/**
 * @brief Dinic 算法求最大流
 * 时间复杂度：一般图 O(V^2 E)，二分图 O(E sqrt(V))。
 */
struct Dinic {
    struct Edge {
        int to;
        ll cap;
        int rev;
    };
    int n;
    vector<vector<Edge>> g;
    vector<int> level, cur;

    Dinic(int n) : n(n), g(n + 1), level(n + 1), cur(n + 1) {}

    void add_edge(int u, int v, ll cap) {
        g[u].push_back({v, cap, (int)g[v].size()});
        g[v].push_back({u, 0, (int)g[u].size() - 1});
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        level[s] = 0;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (auto &e : g[u]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[u] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    ll dfs(int u, int t, ll f) {
        if (u == t || f == 0) return f;
        for (int &i = cur[u]; i < g[u].size(); i++) {
            Edge &e = g[u][i];
            if (e.cap > 0 && level[e.to] == level[u] + 1) {
                ll d = dfs(e.to, t, min(f, e.cap));
                if (d > 0) {
                    e.cap -= d;
                    g[e.to][e.rev].cap += d;
                    return d;
                }
            }
        }
        return 0;
    }

    ll max_flow(int s, int t) {
        ll flow = 0;
        while (bfs(s, t)) {
            fill(cur.begin(), cur.end(), 0);
            while (ll f = dfs(s, t, LLONG_MAX)) flow += f;
        }
        return flow;
    }

    // 求最小割边：BFS 找到从 S 可达的点，跨越可达与不可达点集的边即为割边
    vector<pair<int, int>> get_min_cut_edges(const vector<pair<int, int>>& original_edges) {
        vector<int> vis(n + 1, 0);
        queue<int> q; q.push(1); vis[1] = 1;
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto &e : g[u]) if(e.cap > 0 && !vis[e.to]) { vis[e.to] = 1; q.push(e.to); }
        }
        vector<pair<int, int>> res;
        for(auto &p : original_edges) if(vis[p.first] ^ vis[p.second]) res.push_back(p);
        return res;
    }
};

int main() {
    int n, m; cin >> n >> m;
    Dinic dinic(n);
    for (int i = 0; i < m; i++) {
        int u, v; ll c; cin >> u >> v >> c;
        dinic.add_edge(u, v, c);
    }
    cout << dinic.max_flow(1, n) << endl;
    return 0;
}

```
## 目录

- 一、最短路（Dijkstra / SPFA / Floyd）
- 二、分层图最短路
- 三、判负环（SPFA / Bellman-Ford）
- 四、二分图（染色判定 / 匈牙利 / KM）
- 五、基环树（找环 / 最大独立集）

---

## 一、最短路

### 1.1 Dijkstra（堆优化，非负权）

```cpp
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
```

使用示例（P3371 单源最短路径）：

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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
```

### 1.2 SPFA（可处理负权边）

```cpp
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
```

### 1.3 Floyd（多源最短路）

```cpp
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
```

---

## 二、分层图最短路

```cpp
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
```

使用示例（P4568 飞行路线：无向图，最多免费 k 条边）：

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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
```

---

## 三、判负环

### 3.1 SPFA 判负环（检测全图，P3385 风格）

```cpp
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
```

使用示例：

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;

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
```

### 3.2 Bellman-Ford 判负环

```cpp
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
```

---

## 四、二分图

### 4.1 染色法判定二分图

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 染色法判断二分图
 * 适用场景：判断无向图能否用两种颜色染色，使相邻点颜色不同（即无奇环）
 * 复杂度：O(n + m)
 */
struct BipartiteCheck {
    int n;
    vector<vector<int>> g;
    vector<int> col;  // 0 未染色，1 / 2 两种颜色

    BipartiteCheck(int n) : n(n), g(n + 1), col(n + 1, 0) {}

    void add_edge(int u, int v) {  // 无向边
        g[u].push_back(v);
        g[v].push_back(u);
    }

    bool check() {  // 支持不连通图
        for (int i = 1; i <= n; i++)
            if (col[i] == 0 && !dfs(i, 1))
                return false;
        return true;
    }

    bool dfs(int u, int c) {
        col[u] = c;
        for (int v : g[u]) {
            if (col[v] == 0) {
                if (!dfs(v, 3 - c)) return false;
            } else if (col[v] == c) {
                return false;  // 相邻同色，不是二分图
            }
        }
        return true;
    }
};
```

使用示例：

```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    BipartiteCheck bc(n);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        bc.add_edge(u, v);
    }
    cout << (bc.check() ? "Yes" : "No") << "\n";
    return 0;
}
```

### 4.2 匈牙利算法（最大匹配）

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 匈牙利算法求二分图最大匹配
 * 适用场景：二分图最大匹配
 * 相关结论（König 定理）：
 *   - 最小点覆盖 = 最大匹配
 *   - 最大独立集 = 总点数 - 最小点覆盖
 *   - 最小路径覆盖（DAG）= 总点数 - 拆点后的最大匹配
 * 复杂度：O(nm)
 * 注意：左右部点都从 1 开始编号
 */
struct Hungarian {
    int n, m;               // 左部 n 个点，右部 m 个点
    vector<vector<int>> g;  // 左部 -> 右部 的边
    vector<int> match;      // match[v]：右部点 v 匹配的左部点（0 表示未匹配）
    vector<bool> vis;

    Hungarian(int n, int m) : n(n), m(m), g(n + 1), match(m + 1, 0) {}

    void add_edge(int u, int v) {
        g[u].push_back(v);
    }

    bool dfs(int u) {
        for (int v : g[u]) {
            if (vis[v]) continue;
            vis[v] = true;
            if (match[v] == 0 || dfs(match[v])) {
                match[v] = u;
                return true;
            }
        }
        return false;
    }

    int solve() {
        int res = 0;
        for (int i = 1; i <= n; i++) {
            vis.assign(m + 1, false);
            if (dfs(i)) res++;
        }
        return res;
    }
};
```

使用示例：

```cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int nL, nR, m;  // 左部 nL 个点，右部 nR 个点，m 条边
    cin >> nL >> nR >> m;
    Hungarian hg(nL, nR);
    for (int i = 0; i < m; i++) {
        int u, v; cin >> u >> v;
        hg.add_edge(u, v);
    }
    cout << hg.solve() << "\n";
    return 0;
}
```

### 4.3 KM 算法（最大权完美匹配，O(n^3) BFS 版）

```cpp
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
```

---

## 五、基环树

### 5.1 找环（拓扑排序剪叶法）

```cpp
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
```

使用示例：

```cpp
#include <bits/stdc++.h>
using namespace std;

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
```

### 5.2 基环树最大独立集（点权版，经典题：骑士）

```cpp
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
```

使用示例（P2607 骑士）：

```cpp
#include <bits/stdc++.h>
using namespace std;

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
```
