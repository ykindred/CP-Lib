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
