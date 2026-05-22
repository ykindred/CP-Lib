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

# 数据结构模板库

---

## 树状数组

```cpp
template <typename T>
class FenWick
{
private:
    vector<T> BIT;
    T lowbit(T x);
    int n;

public:
    FenWick(T n) : n(n)
    {
        this->BIT.resize(n, 0);
    }
    FenWick(vector<T> &ori) : FenWick(ori.size())
    {
        this->BIT = ori;
    }
    void set(int poi, T value);
    T query(int poi);
    T query(int l, int r);
};
template <typename T>
T FenWick<T>::lowbit(T x)
{
    return x & (-x);
}
template <typename T>
void FenWick<T>::set(int poi, T value)
{
    for (; poi < n; poi += (lowbit(poi)))
        (this->BIT[poi]) += value;
    return;
}
template <typename T>
T FenWick<T>::query(int poi)
{
    T ans = 0;
    for (; poi > 0; poi -= lowbit(poi))
    {
        ans += (this->BIT)[poi];
    }
    return ans;
}
template <typename T>
T FenWick<T>::query(int l, int r)
{
    return (this->query(r) - this->query(l - 1));
}
```

---

## 并查集

### 基础并查集

```cpp
//这是并查集的一个极简实现
struct Disjoint_set_unoin{
    vector<int>fa;
    Disjoint_set_unoin(int n){fa.resize(n);iota(fa.begin(),fa.end(),0);};
    int find(int n){return n==fa[n]?n:fa[n] = find(fa[n]);}
    void unite(int x,int y){fa[find(x)] = find(y);}
};
```

### 可回滚并查集

```cpp
struct Disjoint_set_union_with_rollback
{
    //由于要保留路径信息，因此不能再使用路径压缩
    vector<int>fa;
    vector<int>siz;
    stack<pair<int,int>> undost;
    Disjoint_set_union_with_rollback(int n){
        fa.resize(n);
        iota(fa.begin(),fa.end(),0);
        siz.resize(n,1);
    }
    int find(int x){
        return x==fa[x]?x:find(fa[x]);
    }
    void unite(int x,int y){
        int fx = find(x) ,fy = find(y);
        if(fx==fy){
            //虽然没有实际操作，但是为了确保timer一致性，push空操作
            undost.push({-1,-1});
            return;
        }
        if(siz[fx]<=siz[fy]){
            siz[fy] += siz[fx];
            fa[fx] = fy;
            undost.push({fx,fy});
        }else
        {
            siz[fx] += siz[fy];
            fa[fy] = fx;
            undost.push({fy,fx});
        }
        return;
    }
    void undo(){
        auto [x,y] = undost.top();
        undost.pop();
        if(x==-1) return;
        else{
            siz[y] -= siz[x];
            fa[x] = x;
            return;
        }
    }
};
```

### 带删除并查集

```cpp
struct Disjoint_set_union_with_delete
{
    vector<int> fa;
    vector<int> siz;
    vector<int> id;
    int tot;       

    // n 为初始逻辑节点数，q 为预计的最大删除次数（用于预留物理节点空间）
    Disjoint_set_union_with_delete(int n, int q){
        int max_nodes = n + q + 10; 
        fa.resize(max_nodes);
        iota(fa.begin(), fa.end(), 0);
        siz.resize(max_nodes, 1);
        
        id.resize(n);
        iota(id.begin(), id.end(), 0); 
        tot = n - 1; 
    }

    int find(int x){
        return x == fa[x] ? x : fa[x] = find(fa[x]);
    }

    void unite(int x, int y){
        int fx = find(id[x]), fy = find(id[y]);
        if(fx == fy){
            return;
        }
        if(siz[fx] <= siz[fy]){
            siz[fy] += siz[fx];
            fa[fx] = fy;
        }else
        {
            siz[fx] += siz[fy];
            fa[fy] = fx;
        }
        return;
    }

    void del(int x){
        int fx = find(id[x]);
        // 从原集合的 size 中扣除自己 (视具体题目需求，通常删除节点意味着原集合大小-1)
        siz[fx]--;
        
        // 为逻辑节点 x 分配一个新的独立物理节点
        tot++;
        id[x] = tot;
        fa[tot] = tot;
        siz[tot] = 1;
        return;
    }
    
    // 获取逻辑节点 x 所在集合的大小
    int get_size(int x){
        return siz[find(id[x])];
    }
};
```

---

## 线段树

### 基础线段树

```cpp
struct D{
    int val = 0;//按需配置
};
D operator+(const D& left, const D& right){return {left.val+right.val};} //按需重载
struct segment_tree{
    vector<D>data;
    D e; //单位元
    segment_tree(int n){
        data.resize(4*n+10);
    }
    void modify(int nl,int nr ,int tp ,D val ,int p){
        if(nl==nr){
            data[p] = val;
            return;
        }
        else{
            int mid = (nl+nr)>>1;
            if(tp<=mid) modify(nl,mid,tp,val,2*p);
            if(tp>mid) modify(mid+1,nr,tp,val,2*p+1);
            //push up
            data[p] = data[2*p] + data[2*p+1];
            return;
        }
    }
    D query(int nl,int nr,int tl,int tr,int p) const{
        if(nl>=tl&&nr<=tr) return data[p];
        else{
            int mid = (nl+nr)>>1;
            D lt = e ,rt = e;
            if(tl<=mid) lt = query(nl,mid,tl,tr,2*p);
            if(tr>mid) rt = query(mid+1,nr,tl,tr,2*p+1);
            return lt+rt;
        }
    }
};
```

### 懒标记线段树

```cpp
//懒标记线段树的实现较为简单，但本身由于TAG的存在，非常灵活
//特别是涉及多种操作的时候，要仔细想清楚TAG的与DATA的合并逻辑与优先级
//基础逻辑就一句话，在递归之前，必须下传TAG，在结束之后，必须根据子区间信息push up

//这里给出一个区间赋值/加和/查询 操作的线段树实现，实际实现时对照修改即可
#include<bits/stdc++.h>
using namespace std;
using ll = long long;
struct D{
    ll val = 0;
};
D operator+(const D&l_op,const D& r_op){return {l_op.val+r_op.val};};
struct TAG{
    ll add = 0;
    ll set = -1;
};

TAG operator+(const TAG&l_op,const TAG&r_op){ //r_op是优先级更高的tag
    if(r_op.set!=-1){
        return {r_op.add,r_op.set};
    }
    else{
        return {l_op.add+ r_op.add,l_op.set};
    }
}
TAG& operator+=(TAG& l_op,const TAG& r_op){
    l_op = l_op + r_op;
    return l_op;
}
struct Segment_tree_with_lazy_tag{
    vector<D> data;
    vector<TAG> lz_tag;
    D e; //单位元
    Segment_tree_with_lazy_tag(int n){
        data.resize(4*n);
        lz_tag.resize(4*n);
    }
    //计算区间信息与tag合并的函数
    D merge(const D& data,const TAG& tag,ll length){ 
        const auto[add,set] = tag;
        if(set!=-1){
            return {(set+add)*length};
        }
        else{
            return {data.val + add*length};
        }
    }
    //区间编辑
    void modify(int nl,int nr,int tl,int tr,TAG val,int p){
        if(nl>=tl&&nr<=tr){
            lz_tag[p] += val;
            return;
        }
        else{
            //递归前push down
            lz_tag[2*p] += lz_tag[p];
            lz_tag[2*p+1] += lz_tag[p];
            lz_tag[p] = TAG();

            int mid = nl +((nr-nl)>>1);
            if(tl<=mid) modify (nl,mid,tl,tr,val,2*p);
            if(tr>mid) modify (mid+1,nr,tl,tr,val,2*p+1);

            //push up
            data[p] = merge(data[2*p],lz_tag[2*p],(mid - nl+1)) + merge(data[2*p+1],lz_tag[2*p+1],(nr-mid));
            return ;
        }
    }
    //区间查询
    D query(int nl,int nr ,int tl,int tr,int p){
        if(nl>=tl&&nr<=tr){
            return merge(data[p],lz_tag[p],nr-nl+1);
        }
        else{
            //递归前push down
            lz_tag[2*p] += lz_tag[p];
            lz_tag[2*p+1] += lz_tag[p];
            lz_tag[p] = TAG();

            D lt = e;
            D rt = e;
            int mid = nl +((nr-nl)>>1);
            if(tl<=mid) lt = query(nl,mid,tl,tr,2*p);
            if(tr>mid) rt = query(mid+1,nr,tl,tr,2*p+1);

            //push up
            data[p] = merge(data[2*p],lz_tag[2*p],(mid - nl+1)) + merge(data[2*p+1],lz_tag[2*p+1],(nr-mid));
            return lt+rt;
        }
    }
};
```

### 动态开点线段树

```cpp
struct D{
    int val = 0; //按需配置
};
D operator+(const D& left, const D& right){return {left.val+right.val};} //按需重载
struct segment_tree_with_dynamic_points{
    vector<D> data;
    vector<pair<int,int>> nxt;
    D e; //单位元
    segment_tree_with_dynamic_points(){
        data.resize(2);
        nxt.resize(2);
    }
    int newp(){
        data.emplace_back();
        nxt.emplace_back();
        return data.size() -1;
    }
    void modify(int nl,int nr ,int tp,D val,int p){
        if(nl==nr) data[p] = val;
        else{
            auto [lp,rp] = nxt[p]; //注意此处不能引用，避免扩容后悬垂
            int mid = (nl+nr)>>1;
            if(tp<=mid){
                if(lp==0) lp = newp();
                modify(nl,mid,tp,val,lp);
            }
            if(tp>mid){
                if(rp==0) rp =newp();
                modify(mid+1,nr,tp,val,rp);
            }
            nxt[p].first = lp;
            nxt[p].second = rp;
            //push up;
            D lt =e ,rt = e;
            if(lp!=0) lt = data[lp];
            if(rp!=0) rt = data[rp];
            data[p] = lt + rt;
            
            return;
        }
    }
    D query(int nl,int nr ,int tl ,int tr ,int p) const{
        if(nl>=tl&&nr<=tr) return data[p];
        else{
            const auto&[lp,rp] = nxt[p];
            int mid = (nl+nr)>>1;
            D lt =e ,rt = e;
            if(lp!=0&&tl<=mid) lt = query(nl,mid,tl,tr,lp);
            if(rp!=0&&tr>mid) rt = query(mid+1,nr,tl,tr,rp);
            return lt+rt;
        }
    }
};
```

### 可持久化线段树（主席树）

```cpp
struct D{
    int val = 0;
};
D operator+ (const D& left ,const D& right){return {left.val+right.val};}
struct persistent_segment_tree{
    int cnt_v;
    vector<int> h; 
    vector<D> data;
    D e;    //单位元
    vector<pair<int,int>>nxt;
    persistent_segment_tree(D e) : e(e){
        data.resize(2,e);
        nxt.resize(2);
        h.resize(1);
        h[0] = 1;
    }
    int newp(){
        data.push_back(e);
        nxt.push_back({});
        return data.size() - 1;
    }
    //每次修改都创建新节点，不修改的部分复用原来的节点，因此递归新旧同时进行
    void modify(int nl,int nr ,int tp ,D val,int p ,int old_p){
        if(nl==nr){
            data[p] = val;
            return;
        }else{
            const auto [o_lp,o_rp] = nxt[old_p]; //不允许使用引用，以免悬垂指针
            ll mid  = (nl+nr)>>1;
            if(tp<=mid){
                nxt[p].second = o_rp;
                nxt[p].first = newp();
                modify(nl,mid,tp,val,nxt[p].first,o_lp);
            }
            else{
                nxt[p].first = o_lp;
                nxt[p].second = newp();
                modify(mid+1,nr,tp,val,nxt[p].second,o_rp);
            }
            //push up;
            const auto[lp,rp] = nxt[p];
            data[p] = data[lp] + data[rp];
        }
    }
    //查询和正常线段树完全一致
    D query(int nl,int nr ,int tl ,int tr ,int p) const{
        if(nl>=tl&&nr<=tr){
            return data[p];
        }
        else{
            const auto [lp,rp] = nxt[p];
            ll mid = (nl+nr)>>1;
            D lt = e ,rt = e;
            if(tl<=mid&&lp)
                lt = query(nl,mid,tl,tr,lp);
            if(tr>mid&&rp)
                rt = query(mid+1,nr,tl,tr,rp);
            return lt+rt;
        }
    }
    void update(int tk,int nl,int nr ,int tp ,D val){
        int old_head = h[tk];
        int new_head = newp();
        modify(nl ,nr ,tp,val,new_head,old_head);
        h[tk] = new_head;
    }
    int backup(int tk){
        h.push_back(h[tk]);
        return h.size()-1;
    }
};
```

### 标记永久化线段树

```cpp
//在线段树区间查询的时候，除了打懒标记并下传，还有一种写法
//那就是我们将标记"保留"在区间中，查询的时候累计路径上的所有标记并计算结果
//坏处很明显，这要求标记满足结合律和交换律，这显然是阿贝尔群的性质。
//好处是什么？由于这样实现不要求下传标记，因此可以很方便的进行 "持久化"

//下给出一个普通线段树的标记永久化区间和实现
//支持区间增加及查询
struct D
{
    int val = 0;
};
D operator+(const D&l_op,const D&r_op) {return {l_op.val+r_op.val};}
struct TAG
{
    int add = 0;
};
TAG operator+(const TAG&l_op,const TAG&r_op){ //r_op是优先级更高的tag
   return {l_op.add+r_op.add};
}
TAG& operator+=(TAG& l_op,const TAG& r_op){
    l_op = l_op + r_op;
    return l_op;
}
struct Segment_tree_with_persistent_marks
{
    vector<D>data;
    vector<TAG>tags;
    Segment_tree_with_persistent_marks(int n){
        data.resize(4*n+10);
        tags.resize(4*n+10);
    }
    D e;    //单位元
    D merge(const D&data, const TAG&tag,int length){
        return {data.val+tag.add*length};
    }
    void modify(int nl,int nr,int tl,int tr,TAG add,int p){
        //每次途经节点，累加实际受影响的交集长度贡献
        int overlap = min(nr, tr) - max(nl, tl) + 1;
        data[p] = merge(data[p],add,overlap); 
        if(nl>=tl&&nr<=tr){
            
            tags[p] += add; //标记留在此处，不再递归
            return;
        }
        else{
            int mid = (nl+((nr-nl)>>1));
            if(tl<=mid) modify(nl,mid,tl,tr,add,2*p);
            if(tr>mid) modify(mid+1,nr,tl,tr,add,2*p+1);
            return;
        }
    }

    //查询时，只计算父节点的标记和！
    D query(int nl,int nr,int tl,int tr,int p){
        if(nl>=tl&&nr<=tr){
            return data[p];
        }
        else{
            int mid = (nl + ((nr-nl)>>1));
            D lt = e;
            D rt = e;
            if(tl<=mid) lt = query(nl,mid,tl,tr,2*p);
            if(tr>mid) rt = query(mid+1,nr,tl,tr,2*p+1);

            int overlap = min(nr, tr) - max(nl, tl) + 1;
            return merge(lt + rt, tags[p], overlap);
        }
    }
};
```

---

## 字典树

### 基础字典树

```cpp
struct D{
    int nxt[26] ={0};
    int cnt = 0;
};
struct Trie{
    vector<D>node;
    Trie(){node.resize(2);}
    int newp(){node.push_back({}); return node.size()-1;}
    //只给出insert的基本实现，简单来说，不能使用递归实现，查询时不断挪动指针向下
    //其余操作的查询操作均以此为基础，不再赘述
    void insert(const string& s){
        int n = s.size();
        int cur = 1;
        for(int i = 0;i<n;i++){
            auto val = s[i];
            if(!node[cur].nxt[val-'a']) {
                node[cur].nxt[val-'a'] = newp();
            }
            cur = node[cur].nxt[val-'a'] ;
        }
        node[cur].cnt++;
        return;
    }
};
```

### 01字典树

```cpp
//01字典树一般用来解决位运算相关的问题
//如果原题可以按位贪心，可以从01字典树的角度思考
struct D{
    int nxt[2] = {0};
};
struct Trie_01{
    vector<D> node;
    Trie_01(){
        node.resize(2);
    }
    int newp(){
        node.push_back({});
        return node.size() - 1;
    }
    //插入一个数
    void insert(int num){
        int cur = 1;
        for(int i = 30;i>=0;i--){
            int bit = (num>>i) &1;
            if(node[cur].nxt[bit]==0){
                node[cur].nxt[bit]  = newp();
            }
            cur = node[cur].nxt[bit];
        }
    }
    //返回异或值最大的结果
    int query(int num) const{
        int cur = 1;
        int ret = 0;
        for(int i = 30;i>=0;i--){
            int bit = (num>>i)&1;
            if(node[cur].nxt[!bit] ==0){
                cur = node[cur].nxt[bit];
            }
            else{
                cur = node[cur].nxt[!bit];
                ret += (1<<i);
            }
        }
        return ret;
    }
};
```

### 可持久化01字典树

```cpp
//为01字典树添加了持久化操作
//思路与主席树类似，只要操作，就修改附近所有节点，然后复用之前节点
#include<bits/stdc++.h>
using namespace std;
struct D{
    int nxt[2] = {0};
    int cnt = 0;
};
struct Trie_01{
    vector<D> node;
    vector<int> h;
    Trie_01(){
        node.resize(2);
        h.resize(1);
        h[0] = 1;
    }
    int newp(){
        node.push_back({});
        return node.size() - 1;
    }
    //给版本k,插入一个数
    void insert(int tk,int num){
        int cur = newp();
        int o_cur = h[tk];
        h[tk] = cur;    //cur 更换新的头节点
        for(int i = 30;i>=0;i--){
            int bit = (num>>i)&1;
            node[cur].nxt[bit] = newp();
            node[cur].cnt = node[o_cur].cnt + 1;

            node[cur].nxt[!bit] = node[o_cur].nxt[!bit];

            cur = node[cur].nxt[bit];
            o_cur = node[o_cur].nxt[bit];
        }
        node[cur].cnt = node[o_cur].cnt + 1;
    }
    //复制版本k，并返回新的版本编号
    int copy(int tk){
        h.push_back(h[tk]);
        return h.size()-1;
    }
    //查询版本k的最大值
    int query(int tk,int num) const{
        int cur = h[tk];
        int ret = 0;
        for(int i = 30;i>=0;i--){
            int bit = (num>>i)&1;
            if(node[cur].nxt[!bit] ==0){
                cur = node[cur].nxt[bit];
            }
            else{
                cur = node[cur].nxt[!bit];
                ret += (1<<i);
            }
        }
        return ret;
    }
    //查询版本l 与版本r之间的最大值  [l,r] 双闭区间
    //往往这个更有效果，因为可以处理区间
    int query(int tl,int tr,int num) const{
        if(tl==0) return query(tr,num);
        int curl = h[tl - 1];
        int curr = h[tr];
        int ret = 0;
        for(int i = 30;i>=0;i--){
            int bit = (num>>i)&1;
            int cnt = node[node[curr].nxt[!bit]].cnt - node[node[curl].nxt[!bit]].cnt;
            if(cnt>0){
                //共同进入 !bit 分支
                ret += (1<<i);
                curl = node[curl].nxt[!bit];
                curr = node[curr].nxt[!bit];
            }
            else{
                //共同进入 bit分支
                curl = node[curl].nxt[bit];
                curr = node[curr].nxt[bit];
            }
        }
        return ret;
    }
};
```

---

## 平衡树

### pbds实现

```cpp
//pbds自带平衡树，用这个非常方便，一下给出示例（Generated by Gemini 3.1 pro)
#include <iostream>
// 1. 引入 pb_ds 的核心头文件
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

using namespace std;
using namespace __gnu_pbds; // 必须引入此命名空间

// ==========================================
// 1. 普通平衡树（Set：元素不可重复）模板
// ==========================================
typedef tree<
    int,                                  // Key 类型
    null_type,                            // Value 类型（null_type 表示作为 set 使用）
    less<int>,                            // 比较函数（less 为升序，greater 为降序）
    rb_tree_tag,                          // 底层结构（红黑树）
    tree_order_statistics_node_update     // 开启名次树功能（必须带上这个）
> ordered_set;

// ==========================================
// 2. 多重集平衡树（Multiset：元素可重复）模板
// 竞赛避坑：直接用 less_equal 会导致 erase 行为异常
// 解决方案：通过插入 pair<值, 唯一ID> 强行变回单集
// ==========================================
typedef pair<int, int> pii;
typedef tree<
    pii, 
    null_type, 
    less<pii>, 
    rb_tree_tag, 
    tree_order_statistics_node_update
> ordered_multiset;

int main() {
    // --------------------------------------
    // 基础单集演示
    // --------------------------------------
    ordered_set tr;
    
    // 插入元素 (O(log N))
    tr.insert(10);
    tr.insert(30);
    tr.insert(20);
    tr.insert(50);
    tr.insert(40);

    // 【核心操作 1】order_of_key(x) -> 查询排名
    // 作用：返回严格小于 x 的元素个数（排名从 0 开始）
    int rank = tr.order_of_key(30); 
    cout << "严格小于 30 的元素个数: " << rank << "\n"; // 输出 2

    // 【核心操作 2】find_by_order(k) -> 按排名查询
    // 作用：返回第 k 小元素的迭代器（k 从 0 开始计算）
    auto it = tr.find_by_order(2); 
    cout << "第 2 小（0-indexed）的元素: " << *it << "\n\n"; // 输出 30

    // --------------------------------------
    // 多重集演示 (竞赛实战推荐)
    // --------------------------------------
    ordered_multiset mtr;
    int id_counter = 0; // 全局或局部的唯一时间戳/ID分配器

    // 插入多个相同元素
    mtr.insert({15, ++id_counter});
    mtr.insert({15, ++id_counter});
    mtr.insert({15, ++id_counter});
    mtr.insert({5, ++id_counter});

    // 1. 查询排名：直接查 {x, 0}
    // 因为 second=0 比所有真实插入的同值元素（ID>=1）都小
    cout << "严格小于 15 的元素个数: " << mtr.order_of_key({15, 0}) << "\n"; // 输出 1 (只有 5)

    // 2. 按排名查询：k_th_val = find_by_order(k)->first
    cout << "第 1 小的元素的值: " << mtr.find_by_order(1)->first << "\n"; // 输出 15

    // 3. 安全删除操作：需要先用 lower_bound 找到具体的 pair，然后再 erase
    auto del_it = mtr.lower_bound({15, 0});
    if (del_it != mtr.end() && del_it->first == 15) {
        mtr.erase(del_it); // 只删除了其中一个 15
    }

    // 验证删除后 15 还剩下几个 (原本3个，删了1个，应该剩2个，加上前置的5，总size为3)
    cout << "删除一个 15 后，树的大小: " << mtr.size() << "\n"; // 输出 3

    return 0;
}
```

### 隐式Treap（无旋Treap / FHQ-Treap）

```cpp
struct Implicit_treap {
    vector<int> val;
    vector<int> siz;
    vector<unsigned int> pri;
    vector<pair<int, int>> nxt;
    vector<bool> rev;   // 区间翻转懒标记
    int root;
    mt19937 rng;

    Implicit_treap() : root(0), rng(random_device{}()) {
        val.push_back(0);
        siz.push_back(0);
        pri.push_back(0);
        nxt.push_back({0, 0});
        rev.push_back(false);
    }

    int newp(int v) {
        val.push_back(v);
        siz.push_back(1);
        pri.push_back(rng());
        nxt.push_back({0, 0});
        rev.push_back(false);
        return siz.size() - 1;
    }

    void push_up(int p) {
        if (p) {
            const auto [lp, rp] = nxt[p];
            siz[p] = siz[lp] + siz[rp] + 1;
        }
    }

    void push_down(int p) {
        if (p && rev[p]) {
            // 1. 实际执行翻转：交换左右儿子
            swap(nxt[p].first, nxt[p].second);
            // 2. 交换后，再取出当前真正的左右儿子去下传标记
            const auto [lp, rp] = nxt[p];
            if (lp) rev[lp] = !rev[lp];
            if (rp) rev[rp] = !rev[rp];
            // 3. 清除当前节点的标记
            rev[p] = false;
        }
    }

    pair<int, int> split(int p, int k) {
        if (!p) return {0, 0};
        
        push_down(p); 

        const auto [lp, rp] = nxt[p];
        if (siz[lp] + 1 <= k) {
            auto [rx, ry] = split(rp, k - siz[lp] - 1);
            nxt[p].second = rx;
            push_up(p);
            return {p, ry};
        } else {
            auto [lx, ly] = split(lp, k);
            nxt[p].first = ly;
            push_up(p);
            return {lx, p};
        }
    }

    int merge(int x, int y) {
        if (!x || !y) return x | y;
        push_down(x); 
        push_down(y);

        if (pri[x] > pri[y]) {
            nxt[x].second = merge(nxt[x].second, y);
            push_up(x);
            return x;
        } else {
            nxt[y].first = merge(x, nxt[y].first);
            push_up(y);
            return y;
        }
    }

    // --- 0-based 对外接口 ---

    // 在下标 pos 处插入值 v (插入后它将成为新的 array[pos])
    // 允许 pos 取 0 到 siz[root] (即支持在首尾插入)
    void insert(int pos, int v) {
        auto [x, y] = split(root, pos);
        root = merge(merge(x, newp(v)), y);
    }

    // 删除下标为 pos 的节点 (0 <= pos < siz[root])
    void erase(int pos) {
        auto [x, z] = split(root, pos + 1); 
        auto [lx, del] = split(x, pos);    
        root = merge(lx, z); 
    }

    // 反转区间 [l, r] (0 <= l <= r < siz[root])
    void reverse_range(int l, int r) {
        if(l >= r) return;
        auto [x, z] = split(root, r + 1); 
        auto [lx, y] = split(x, l);       
        rev[y] = !rev[y]; 
        root = merge(merge(lx, y), z);
    }
};
```

---

## 其他数据结构与算法

### 稀疏表

```cpp
#include <bits/stdc++.h>
using namespace std;
template <typename S>
struct SpTable {
    int n;
    int log;
    vector<vector<S>> st;

    SpTable(const vector<S>& a) {
        build(a);
    }
    void build(const vector<S>& a) {
        n = a.size();
        log = 32 - __builtin_clz(n);
        st.assign(log, vector<S>(n));
        for (int i = 0; i < n; i++) {
            st[0][i] = a[i];
        }
        for (int j = 1; j < log; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                st[j][i] = op(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
            }
        }
    }
    S query(int l, int r) {
        int j = 31 - __builtin_clz(r - l);
        return op(st[j][l], st[j][r - (1 << j)]);
    }
};
```

### 离散化

```cpp
// 离散化
// 许多题目在处理数据的时候，并不关心值本身，只关心值之间的大小关系
// 这个时候进行离散化是比较正确的决定
template <typename T> 
vector<T> Discretization(vector<T> o_array)
{
    vector<T> knum = o_array;
    sort(knum.begin(), knum.end());
    knum.erase(unique(knum.begin(), knum.end()),knum.end());
    for(int i = 0;i<o_array.size();i++){
        o_array[i] = lower_bound(knum.begin(),knum.end(),o_array[i]) - knum.begin();
    }
    return o_array;
}
//传入一个数组，传出离散化之后的数组 如 num = Normalization(num);
```

### 哈希表

```cpp
#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
struct chash {
    static ull splitmix64(ull x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(ull x) const {
        static const ull F = 
            chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + F);
    }
};
template<typename K, typename V>
using HashMap = __gnu_pbds::gp_hash_table<K, V, chash>;
```

### 欧拉序

```cpp
//许多处理树上问题，特别是对 子树 进行操作的时候 ，往往利用欧拉序将树拍平后，再利用线段树维护
//实现了一个结构体，初始化n为节点数，并传入边（需人为确保是一棵树）
//调用成员函数.init后，会生成两个个长度为n的数组
//第一个为树的dfn序，注意默认视1为根节点
//第二个为子树索引，tp[p] 代表 p节点所在子树在dfn上对应的连续区间 ，请使用auto[lp,rp] = pt[p] 调用
struct GenerateEulerTourOrder
{
    vector<int>dfn;
    vector<pair<int,int>>tp;
    vector<vector<int>> g;
    GenerateEulerTourOrder(int n){
        dfn.reserve(n);
        tp.resize(n+1);
        g.resize(n+1);
    }
    void add(int u,int v){
        g[u].push_back(v);
        g[v].push_back(u);
    }
    void init(int head = 1){
        auto dfs = [this](auto&&self,int n,int p) ->void {
            auto & [lp,rp] = tp[n];
            dfn.emplace_back(n);
            lp = dfn.size()-1;
            for(auto nextp:g[n]){
                if(nextp== p) continue;
                else{
                    self(self,nextp,n);
                }
            }
            rp = dfn.size() - 1;
        };
        dfs(dfs,head,-1);
        return;
    }
};
```

### 莫队
```cpp
struct Mo {
    // 存储左闭右开区间[L, R)
    vector<pair<int, int>> queries;

    // 增加一个左闭右开的查询
    void add_query(int lt, int rt) {
        queries.emplace_back(lt, rt);
    }

    // 计算最优执行顺序
    vector<int> get_mo_order() {
        int qsiz = queries.size();
        if (qsiz == 0) return {};

        int tmp = 0;
        for (auto&& [lt, rt] : queries) {
            if (lt > tmp) tmp = lt;
            if (rt > tmp) tmp = rt;
        }

        // 理论最优块大小为 sqrt(3) * N / sqrt(2 * Q)
        int blocksiz = max(1, (int)(1.732 * tmp / sqrt(max(1.0, 2.0 * qsiz))));
        
        vector<int> ret(qsiz);
        iota(ret.begin(), ret.end(), 0);
        
        sort(ret.begin(), ret.end(), [&](int a, int b) {
            int ab = queries[a].first / blocksiz, bb = queries[b].first / blocksiz;
            if (ab != bb) return ab < bb;
            return (ab & 1) ? (queries[a].second > queries[b].second) : (queries[a].second < queries[b].second);
        });
        // 如果卡常, 则启用下面这段注释

        // auto get_cost = [&](int a, int b) -> int {
        //     return abs(queries[ret[a]].first - queries[ret[b]].first) + abs(queries[ret[a]].second - queries[ret[b]].second);
        // };
        
        // for (int k = 0; k + 4 < qsiz; ++k) {
        //     if (get_cost(k, k + 2) + get_cost(k + 1, k + 3) < get_cost(k, k + 1) + get_cost(k + 2, k + 3)) {
        //         swap(ret[k + 1], ret[k + 2]);
        //     }
        //     if (get_cost(k, k + 3) + get_cost(k + 1, k + 4) < get_cost(k, k + 1) + get_cost(k + 3, k + 4)) {
        //         swap(ret[k + 1], ret[k + 3]);
        //     }
        // }
        return ret;
    }

    // 非对称增删
    template <class F1, class F2, class F3, class F4, class Q>
    void solve(F1 add_l, F2 add_r, F3 rm_l, F4 rm_r, Q query) {
        auto I = get_mo_order();
        int l = 0, r = 0;
        for (int idx : I) {
            while (l > queries[idx].first) add_l(--l);
            while (r < queries[idx].second) add_r(r++);
            while (l < queries[idx].first) rm_l(l++);
            while (r > queries[idx].second) rm_r(--r);
            query(idx);
        }
    }

    // 对称增删
    template <class F1, class F2, class Q>
    void solve(F1 add, F2 rm, Q query) {
        solve(add, add, rm, rm, query);
    }
};
```

### 回滚莫队

> 此模板仅包含思路注释，无具体实现代码。

```
//常规莫队在挪动指针时，总是双指针交替挪动，O(1)更新答案
//回滚莫队的思想是：
//对于一些可离线的查询，我们进行add操作比较简单，del操作比较复杂
//我们在查询的时候，先确保右指针单调挪动，每次右指针挪动前，把左指针回溯到最初始的状态（区块右边界）
//再将指针左移以匹配区间
//注意到这样就只有add操作，我们只需要维护一个操作栈即可正确回溯，注意到左指针最多挪动sqrt(n)次
```


# 几何

### 库实数类实现（双精度）

```c++
using Real = int;
using Point = complex<Real>;
 
Real cross(const Point &a, const Point &b) {
    return (conj(a) * b).imag();
} 
Real dot(const Point &a, const Point &b) {
    return (conj(a) * b).real();
}
```

### 平面几何必要初始化

#### 字符串读入浮点数

```c++
const int Knum = 4;
int read(int k = Knum) {
    string s;
    cin >> s;
    
    int num = 0;
    int it = s.find('.');
    if (it != -1) { // 存在小数点
        num = s.size() - it - 1; // 计算小数位数
        s.erase(s.begin() + it); // 删除小数点
    }
    for (int i = 1; i <= k - num; i++) { // 补全小数位数
        s += '0';
    }
    return stoi(s); 
}
```

#### 预置函数

```c++
using ld = long double;
const ld PI = acos(-1);
const ld EPS = 1e-7;
const ld INF = numeric_limits<ld>::max();
#define cc(x) cout << fixed << setprecision(x);

ld fgcd(ld x, ld y) { // 实数域gcd
    return abs(y) < EPS ? abs(x) : fgcd(y, fmod(x, y));
}
template<class T, class S> bool equal(T x, S y) {
    return -EPS < x - y && x - y < EPS;
}
template<class T> int sign(T x) {
    if (-EPS < x && x < EPS) return 0;
    return x < 0 ? -1 : 1;
}
```

#### 点线封装

```c++
template<class T> struct Point { // 在C++17下使用 emplace_back 绑定可能会导致CE！
    T x, y;
    Point(T x_ = 0, T y_ = 0) : x(x_), y(y_) {} // 初始化
    template<class U> operator Point<U>() { // 自动类型匹配
        return Point<U>(U(x), U(y));
    }
    Point &operator+=(Point p) & { return x += p.x, y += p.y, *this; }
    Point &operator+=(T t) & { return x += t, y += t, *this; }
    Point &operator-=(Point p) & { return x -= p.x, y -= p.y, *this; }
    Point &operator-=(T t) & { return x -= t, y -= t, *this; }
    Point &operator*=(T t) & { return x *= t, y *= t, *this; }
    Point &operator/=(T t) & { return x /= t, y /= t, *this; }
    Point operator-() const { return Point(-x, -y); }
    friend Point operator+(Point a, Point b) { return a += b; }
    friend Point operator+(Point a, T b) { return a += b; }
    friend Point operator-(Point a, Point b) { return a -= b; }
    friend Point operator-(Point a, T b) { return a -= b; }
    friend Point operator*(Point a, T b) { return a *= b; }
    friend Point operator*(T a, Point b) { return b *= a; }
    friend Point operator/(Point a, T b) { return a /= b; }
    friend bool operator<(Point a, Point b) {
        return equal(a.x, b.x) ? a.y < b.y - EPS : a.x < b.x - EPS;
    }
    friend bool operator>(Point a, Point b) { return b < a; }
    friend bool operator==(Point a, Point b) { return !(a < b) && !(b < a); }
    friend bool operator!=(Point a, Point b) { return a < b || b < a; }
    friend auto &operator>>(istream &is, Point &p) {
        return is >> p.x >> p.y;
    }
    friend auto &operator<<(ostream &os, Point p) {
        return os << "(" << p.x << ", " << p.y << ")";
    }
};
template<class T> struct Line {
    Point<T> a, b;
    Line(Point<T> a_ = Point<T>(), Point<T> b_ = Point<T>()) : a(a_), b(b_) {}
    template<class U> operator Line<U>() { // 自动类型匹配
        return Line<U>(Point<U>(a), Point<U>(b));
    }
    friend auto &operator<<(ostream &os, Line l) {
        return os << "<" << l.a << ", " << l.b << ">";
    }
};
```

#### 叉乘

定义公式 $a\times b=|a||b|\sin \theta$。

```c++
template<class T> T cross(Point<T> a, Point<T> b) { // 叉乘
    return a.x * b.y - a.y * b.x;
}
template<class T> T cross(Point<T> p1, Point<T> p2, Point<T> p0) { // 叉乘 (p1 - p0) x (p2 - p0);
    return cross(p1 - p0, p2 - p0);
}
```

#### 点乘

定义公式 $a\times b=|a||b|\cos \theta$。

```c++
template<class T> T dot(Point<T> a, Point<T> b) { // 点乘
    return a.x * b.x + a.y * b.y;
}
template<class T> T dot(Point<T> p1, Point<T> p2, Point<T> p0) { // 点乘 (p1 - p0) * (p2 - p0);
    return dot(p1 - p0, p2 - p0);
}
```

#### 欧几里得距离公式

最常用的距离公式。**需要注意**，开根号会丢失精度，如无强制要求，先不要开根号，留到最后一步一起开。

```c++
template <class T> ld dis(T x1, T y1, T x2, T y2) {
    ld val = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
    return sqrt(val);
}
template <class T> ld dis(Point<T> a, Point<T> b) {
    return dis(a.x, a.y, b.x, b.y);
}
```

#### 曼哈顿距离公式

```c++
template <class T> T dis1(Point<T> p1, Point<T> p2) { // 曼哈顿距离公式
    return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}
```

#### 将向量转换为单位向量

```c++
Point<ld> standardize(Point<ld> vec) { // 转换为单位向量
    return vec / sqrt(vec.x * vec.x + vec.y * vec.y);
}
```

#### 向量旋转

将当前向量移动至原点后顺时针旋转 $90^{\circ}$ ，即获取垂直于当前向量的、起点为原点的向量。在计算垂线时非常有用。例如，要想获取点 $a$ 绕点 $o$ 顺时针旋转 $90^{\circ}$ 后的点，可以这样书写代码：`auto ans = o + rotate(o, a);` ；如果是逆时针旋转，那么只需更改符号即可：`auto ans = o - rotate(o, a);` 。

```c++
template<class T> Point<T> rotate(Point<T> p1, Point<T> p2) { // 旋转
    Point<T> vec = p1 - p2;
    return {-vec.y, vec.x};
}
```

### 平面角度与弧度

#### 弧度角度相互转换

```c++
ld toDeg(ld x) { // 弧度转角度
    return x * 180 / PI;
}
ld toArc(ld x) { // 角度转弧度
    return PI / 180 * x;
}
```

#### 正弦定理

$\dfrac{a}{\sin A}=\dfrac{b}{\sin B}=\dfrac{c}{\sin C}=2R$ ，其中 $R$ 为三角形外接圆半径；

#### 余弦定理（已知三角形三边，求角）

$\cos C=\dfrac{a^2+b^2-c^2}{2ab},\cos B=\dfrac{a^2+c^2-b^2}{2ac},\cos A=\dfrac{b^2+c^2-a^2}{2bc}$。可以借此推导出三角形面积公式 $S_{\triangle ABC}=\dfrac{ab\cdot\sin C}{2}=\dfrac{bc\cdot\sin A}{2}=\dfrac{ac\cdot\sin B}{2}$。

注意，计算格式是：由 $b,c,a$ 三边求 $\angle A$；由 $a, c, b$ 三边求 $\angle B$；由 $a, b, c$ 三边求 $\angle C$。

```c++
ld angle(ld a, ld b, ld c) { // 余弦定理
    ld val = acos((a * a + b * b - c * c) / (2.0 * a * b)); // 计算弧度
    return val;
}
```

#### 求两向量的夹角

能够计算 $[0^{\circ},180^{\circ}]$ 区间的角度。

```c++
ld angle(Point<ld> a, Point<ld> b) {
    ld val = abs(cross(a, b));
    return abs(atan2(val, a.x * b.x + a.y * b.y));
}
```

#### 向量旋转任意角度

逆时针旋转，转换公式：$\left\{\begin{matrix}
x'=x\cos \theta-y\sin \theta \\ 
y'=x\sin \theta+y\cos \theta
\end{matrix}\right.$

```c++
Point<ld> rotate(Point<ld> p, ld rad) {
    return {p.x * cos(rad) - p.y * sin(rad), p.x * sin(rad) + p.y * cos(rad)};
}
```

#### 点绕点旋转任意角度

逆时针旋转，转换公式：$\left\{\begin{matrix}
x'=(x_0-x_1)\cos\theta+(y_0-y_1)\sin\theta+x_1 \\ 
y'=(x_1-x_0)\sin\theta+(y_0-y_1)\cos\theta+y_1
\end{matrix}\right.$

```c++
Point<ld> rotate(Point<ld> a, Point<ld> b, ld rad) {
    ld x = (a.x - b.x) * cos(rad) + (a.y - b.y) * sin(rad) + b.x;
    ld y = (b.x - a.x) * sin(rad) + (a.y - b.y) * cos(rad) + b.y;
    return {x, y};
}
```

### 平面点线相关

#### 点是否在直线上（三点是否共线）

```c++
template<class T> bool onLine(Point<T> a, Point<T> b, Point<T> c) {
    return sign(cross(b, a, c)) == 0;
}
template<class T> bool onLine(Point<T> p, Line<T> l) {
    return onLine(p, l.a, l.b);
}
```

#### 点是否在向量（直线）左侧

**需要注意**，向量的方向会影响答案；点在向量上时不视为在左侧。

```c++
template<class T> bool pointOnLineLeft(Pt p, Lt l) {
    return cross(l.b, p, l.a) > 0;
}
```

#### 两点是否在直线同侧/异侧

```c++
template<class T> bool pointOnLineSide(Pt p1, Pt p2, Lt vec) {
    T val = cross(p1, vec.a, vec.b) * cross(p2, vec.a, vec.b);
    return sign(val) == 1;
}
template<class T> bool pointNotOnLineSide(Pt p1, Pt p2, Lt vec) {
    T val = cross(p1, vec.a, vec.b) * cross(p2, vec.a, vec.b);
    return sign(val) == -1;
}
```

#### 两直线相交交点

在使用前需要先判断直线是否平行。

```c++
Pd lineIntersection(Ld l1, Ld l2) {
    ld val = cross(l2.b - l2.a, l1.a - l2.a) / cross(l2.b - l2.a, l1.a - l1.b);
    return l1.a + (l1.b - l1.a) * val;
}
```

#### 两直线是否平行/垂直/相同

```c++
template<class T> bool lineParallel(Lt p1, Lt p2) {
    return sign(cross(p1.a - p1.b, p2.a - p2.b)) == 0;
}
template<class T> bool lineVertical(Lt p1, Lt p2) {
    return sign(dot(p1.a - p1.b, p2.a - p2.b)) == 0;
}
template<class T> bool same(Line<T> l1, Line<T> l2) {
    return lineParallel(Line{l1.a, l2.b}, {l1.b, l2.a}) &&
           lineParallel(Line{l1.a, l2.a}, {l1.b, l2.b}) && lineParallel(l1, l2);
}
```

#### 点到直线的最近距离与最近点

```c++
pair<Pd, ld> pointToLine(Pd p, Ld l) {
    Pd ans = lineIntersection({p, p + rotate(l.a, l.b)}, l);
    return {ans, dis(p, ans)};
}
```

如果只需要计算最近距离，下方的写法可以减少书写的代码量，效果一致。

```c++
template<class T> ld disPointToLine(Pt p, Lt l) {
    ld ans = cross(p, l.a, l.b);
    return abs(ans) / dis(l.a, l.b); // 面积除以底边长
}
```

#### 点是否在线段上

```c++
template<class T> bool pointOnSegment(Pt p, Lt l) { // 端点也算作在直线上
    return sign(cross(p, l.a, l.b)) == 0 && min(l.a.x, l.b.x) <= p.x && p.x <= max(l.a.x, l.b.x) &&
           min(l.a.y, l.b.y) <= p.y && p.y <= max(l.a.y, l.b.y);
}
template<class T> bool pointOnSegment(Pt p, Lt l) { // 端点不算
    return pointOnSegment(p, l) && min(l.a.x, l.b.x) < p.x && p.x < max(l.a.x, l.b.x) &&
           min(l.a.y, l.b.y) < p.y && p.y < max(l.a.y, l.b.y);
}
```

#### 点到线段的最近距离与最近点

```c++
pair<Pd, ld> pointToSegment(Pd p, Ld l) {
    if (sign(dot(p, l.b, l.a)) == -1) { // 特判到两端点的距离
        return {l.a, dis(p, l.a)};
    } else if (sign(dot(p, l.a, l.b)) == -1) {
        return {l.b, dis(p, l.b)};
    }
    return pointToLine(p, l);
}
```

#### 点在直线上的投影点（垂足）

```c++
Pd project(Pd p, Ld l) { // 投影
    Pd vec = l.b - l.a;
    ld r = dot(vec, p - l.a) / (vec.x * vec.x + vec.y * vec.y);
    return l.a + vec * r;
}
```

#### 线段的中垂线

```c++
template<class T> Lt midSegment(Lt l) {
    Pt mid = (l.a + l.b) / 2; // 线段中点
    return {mid, mid + rotate(l.a, l.b)};
}
```

#### 两线段是否相交及交点

该扩展版可以同时返回相交状态和交点，分为四种情况：$0$ 代表不相交；$1$ 代表普通相交；$2$ 代表重叠（交于两个点）；$3$ 代表相交于端点。**需要注意**，部分运算可能会使用到直线求交点，此时务必保证变量类型为浮点数！

```c++
template<class T> tuple<int, Pt, Pt> segmentIntersection(Lt l1, Lt l2) {
    auto [s1, e1] = l1;
    auto [s2, e2] = l2;
    auto A = max(s1.x, e1.x), AA = min(s1.x, e1.x);
    auto B = max(s1.y, e1.y), BB = min(s1.y, e1.y);
    auto C = max(s2.x, e2.x), CC = min(s2.x, e2.x);
    auto D = max(s2.y, e2.y), DD = min(s2.y, e2.y);
    if (A < CC || C < AA || B < DD || D < BB) {
        return {0, {}, {}};
    }
    if (sign(cross(e1 - s1, e2 - s2)) == 0) {
        if (sign(cross(s2, e1, s1)) != 0) {
            return {0, {}, {}};
        }
        Pt p1(max(AA, CC), max(BB, DD));
        Pt p2(min(A, C), min(B, D));
        if (!pointOnSegment(p1, l1)) {
            swap(p1.y, p2.y);
        }
        if (p1 == p2) {
            return {3, p1, p2};
        } else {
            return {2, p1, p2};
        }
    }
    auto cp1 = cross(s2 - s1, e2 - s1);
    auto cp2 = cross(s2 - e1, e2 - e1);
    auto cp3 = cross(s1 - s2, e1 - s2);
    auto cp4 = cross(s1 - e2, e1 - e2);
    if (sign(cp1 * cp2) == 1 || sign(cp3 * cp4) == 1) {
        return {0, {}, {}};
    }
    // 使用下方函数时请使用浮点数
    Pd p = lineIntersection(l1, l2);
    if (sign(cp1) != 0 && sign(cp2) != 0 && sign(cp3) != 0 && sign(cp4) != 0) {
        return {1, p, p};
    } else {
        return {3, p, p};
    }
}
```

如果不需要求交点，那么使用快速排斥+跨立实验即可，其中重叠、相交于端点均视为相交。

```c++
template<class T> bool segmentIntersection(Lt l1, Lt l2) {
    auto [s1, e1] = l1;
    auto [s2, e2] = l2;
    auto A = max(s1.x, e1.x), AA = min(s1.x, e1.x);
    auto B = max(s1.y, e1.y), BB = min(s1.y, e1.y);
    auto C = max(s2.x, e2.x), CC = min(s2.x, e2.x);
    auto D = max(s2.y, e2.y), DD = min(s2.y, e2.y);
    return A >= CC && B >= DD && C >= AA && D >= BB &&
           sign(cross(s1, s2, e1) * cross(s1, e1, e2)) == 1 &&
           sign(cross(s2, s1, e2) * cross(s2, e2, e1)) == 1;
}
```

#### 极角排序
从$x$轴正半轴开始.
```cpp
template <class S> 
int halfPlane(Point<S> a) {
    if (cmp(a.y, 0) == 0) {
        return cmp(a.x, 0);
    }
    return cmp(a.y, 0); // 如果需要从负半轴开始则把这里改成负的
}
template <class S>
bool polarCmp(Point<S> a, Point<S> b) {
    if (halfPlane(a) != halfPlane(b)) {
        return halfPlane(a) > halfPlane(b);
    }
    if (sgn(crs(a, b)) == 0) {
        return len2(a) < len2(b);
    }
    return sgn(crs(a, b)) > 0;
}
```

### 平面圆相关（浮点数处理）

#### 点到圆的最近点

同时返回最近点与最近距离。**需要注意**，当点为圆心时，这样的点有无数个，此时我们视作输入错误，直接返回圆心。

```c++
pair<Pd, ld> pointToCircle(Pd p, Pd o, ld r) {
    Pd U = o, V = o;
    ld d = dis(p, o);
    if (sign(d) == 0) { // p 为圆心时返回圆心本身
        return {o, 0};
    }
    ld val1 = r * abs(o.x - p.x) / d;
    ld val2 = r * abs(o.y - p.y) / d * ((o.x - p.x) * (o.y - p.y) < 0 ? -1 : 1);
    U.x += val1, U.y += val2;
    V.x -= val1, V.y -= val2;
    if (dis(U, p) < dis(V, p)) {
        return {U, dis(U, p)};
    } else {
        return {V, dis(V, p)};
    }
}
```

#### 根据圆心角获取圆上某点

将圆上最右侧的点以圆心为旋转中心，逆时针旋转 `rad` 度。

```c++
Point<ld> getPoint(Point<ld> p, ld r, ld rad) {
    return {p.x + cos(rad) * r, p.y + sin(rad) * r};
}
```

#### 直线是否与圆相交及交点

$0$ 代表不相交；$1$ 代表相切；$2$ 代表相交。

```c++
tuple<int, Pd, Pd> lineCircleCross(Ld l, Pd o, ld r) {
    Pd P = project(o, l);
    ld d = dis(P, o), tmp = r * r - d * d;
    if (sign(tmp) == -1) {
        return {0, {}, {}};
    } else if (sign(tmp) == 0) {
        return {1, P, {}};
    }
    Pd vec = standardize(l.b - l.a) * sqrt(tmp);
    return {2, P + vec, P - vec};
}
```

#### 线段是否与圆相交及交点

$0$ 代表不相交；$1$ 代表相切；$2$ 代表相交于一个点；$3$ 代表相交于两个点。

```c++
tuple<int, Pd, Pd> segmentCircleCross(Ld l, Pd o, ld r) {
    auto [type, U, V] = lineCircleCross(l, o, r);
    bool f1 = pointOnSegment(U, l), f2 = pointOnSegment(V, l);
    if (type == 1 && f1) {
        return {1, U, {}};
    } else if (type == 2 && f1 && f2) {
        return {3, U, V};
    } else if (type == 2 && f1) {
        return {2, U, {}};
    } else if (type == 2 && f2) {
        return {2, V, {}};
    } else {
        return {0, {}, {}};
    }
}
```

#### 两圆是否相交及交点

$0$ 代表内含；$1$ 代表相离；$2$ 代表相切；$3$ 代表相交。

```c++
tuple<int, Pd, Pd> circleIntersection(Pd p1, ld r1, Pd p2, ld r2) {
    ld x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y, d = dis(p1, p2);
    if (sign(abs(r1 - r2) - d) == 1) {
        return {0, {}, {}};
    } else if (sign(r1 + r2 - d) == -1) {
        return {1, {}, {}};
    }
    ld a = r1 * (x1 - x2) * 2, b = r1 * (y1 - y2) * 2, c = r2 * r2 - r1 * r1 - d * d;
    ld p = a * a + b * b, q = -a * c * 2, r = c * c - b * b;
    ld cosa, sina, cosb, sinb;
    if (sign(d - (r1 + r2)) == 0 || sign(d - abs(r1 - r2)) == 0) {
        cosa = -q / p / 2;
        sina = sqrt(1 - cosa * cosa);
        Point<ld> p0 = {x1 + r1 * cosa, y1 + r1 * sina};
        if (sign(dis(p0, p2) - r2)) {
            p0.y = y1 - r1 * sina;
        }
        return {2, p0, p0};
    } else {
        ld delta = sqrt(q * q - p * r * 4);
        cosa = (delta - q) / p / 2;
        cosb = (-delta - q) / p / 2;
        sina = sqrt(1 - cosa * cosa);
        sinb = sqrt(1 - cosb * cosb);
        Pd ans1 = {x1 + r1 * cosa, y1 + r1 * sina};
        Pd ans2 = {x1 + r1 * cosb, y1 + r1 * sinb};
        if (sign(dis(ans1, p1) - r2)) ans1.y = y1 - r1 * sina;
        if (sign(dis(ans2, p2) - r2)) ans2.y = y1 - r1 * sinb;
        if (ans1 == ans2) ans1.y = y1 - r1 * sina;
        return {3, ans1, ans2};
    }
}
```

#### 两圆相交面积

上述所言四种相交情况均可计算，之所以不使用三角形面积计算公式是因为在计算过程中会出现“负数”面积（扇形面积与三角形面积的符号关系会随圆的位置关系发生变化），故公式全部重新推导，这里采用的是扇形面积减去扇形内部的那个三角形的面积。

```c++
ld circleIntersectionArea(Pd p1, ld r1, Pd p2, ld r2) {
    ld x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y, d = dis(p1, p2);
    if (sign(abs(r1 - r2) - d) >= 0) {
        return PI * min(r1 * r1, r2 * r2);
    } else if (sign(r1 + r2 - d) == -1) {
        return 0;
    }
    ld theta1 = angle(r1, dis(p1, p2), r2);
    ld area1 = r1 * r1 * (theta1 - sin(theta1 * 2) / 2);
    ld theta2 = angle(r2, dis(p1, p2), r1);
    ld area2 = r2 * r2 * (theta2 - sin(theta2 * 2) / 2);
    return area1 + area2;
}
```

#### 三点确定一圆

```c++
tuple<int, Pd, ld> getCircle(Pd A, Pd B, Pd C) {
    if (onLine(A, B, C)) { // 特判三点共线
        return {0, {}, 0};
    }
    Ld l1 = midSegment(Line{A, B});
    Ld l2 = midSegment(Line{A, C});
    Pd O = lineIntersection(l1, l2);
    return {1, O, dis(A, O)};
}
```

#### 求解点到圆的切线数量与切点

```c++
pair<int, vector<Point<ld>>> tangent(Point<ld> p, Point<ld> A, ld r) {
    vector<Point<ld>> ans; // 储存切点
    Point<ld> u = A - p;
    ld d = sqrt(dot(u, u));
    if (d < r) {
        return {0, {}};
    } else if (sign(d - r) == 0) { // 点在圆上
        ans.push_back(u);
        return {1, ans};
    } else {
        ld ang = asin(r / d);
        ans.push_back(getPoint(A, r, -ang));
        ans.push_back(getPoint(A, r, ang));
        return {2, ans};
    }
}
```

#### 求解两圆的内公、外公切线数量与切点

同时返回公切线数量以及每个圆的切点。

```c++
tuple<int, vector<Point<ld>>, vector<Point<ld>>> tangent(Point<ld> A, ld Ar, Point<ld> B, ld Br) {
    vector<Point<ld>> a, b; // 储存切点
    if (Ar < Br) {
        swap(Ar, Br);
        swap(A, B);
        swap(a, b);
    }
    int d = disEx(A, B), dif = Ar - Br, sum = Ar + Br;
    if (d < dif * dif) { // 内含，无
        return {0, {}, {}};
    }
    ld base = atan2(B.y - A.y, B.x - A.x);
    if (d == 0 && Ar == Br) { // 完全重合，无数条外公切线
        return {-1, {}, {}};
    }
    if (d == dif * dif) { // 内切，1条外公切线
        a.push_back(getPoint(A, Ar, base));
        b.push_back(getPoint(B, Br, base));
        return {1, a, b};
    }
    ld ang = acos(dif / sqrt(d));
    a.push_back(getPoint(A, Ar, base + ang)); // 保底2条外公切线
    a.push_back(getPoint(A, Ar, base - ang));
    b.push_back(getPoint(B, Br, base + ang));
    b.push_back(getPoint(B, Br, base - ang));
    if (d == sum * sum) { // 外切，多1条内公切线
        a.push_back(getPoint(A, Ar, base));
        b.push_back(getPoint(B, Br, base + PI));
    } else if (d > sum * sum) { // 相离，多2条内公切线
        ang = acos(sum / sqrt(d));
        a.push_back(getPoint(A, Ar, base + ang));
        a.push_back(getPoint(A, Ar, base - ang));
        b.push_back(getPoint(B, Br, base + ang + PI));
        b.push_back(getPoint(B, Br, base - ang + PI));
    }
    return {a.size(), a, b};
}
```

### 平面三角形相关（浮点数处理）

#### 三角形面积

```c++
ld area(Point<ld> a, Point<ld> b, Point<ld> c) {
    return abs(cross(b, c, a)) / 2;
}
```

#### 三角形外心

三角形外接圆的圆心，即三角形三边垂直平分线的交点。

```c++
template<class T> Pt center1(Pt p1, Pt p2, Pt p3) { // 外心
    return lineIntersection(midSegment({p1, p2}), midSegment({p2, p3}));
}
```

#### 三角形内心

三角形内切圆的圆心，也是三角形三个内角的角平分线的交点。其到三角形三边的距离相等。

```c++
Pd center2(Pd p1, Pd p2, Pd p3) { // 内心
    #define atan2(p) atan2(p.y, p.x) // 注意先后顺序
    Line<ld> U = {p1, {}}, V = {p2, {}};
    ld m, n, alpha;
    m = atan2((p2 - p1));
    n = atan2((p3 - p1));
    alpha = (m + n) / 2;
    U.b = {p1.x + cos(alpha), p1.y + sin(alpha)};
    m = atan2((p1 - p2));
    n = atan2((p3 - p2));
    alpha = (m + n) / 2;
    V.b = {p2.x + cos(alpha), p2.y + sin(alpha)};
    return lineIntersection(U, V);
}
```

#### 三角形垂心

三角形的三条高线所在直线的交点。锐角三角形的垂心在三角形内；直角三角形的垂心在直角顶点上；钝角三角形的垂心在三角形外。

```c++
Pd center3(Pd p1, Pd p2, Pd p3) { // 垂心
    Ld U = {p1, p1 + rotate(p2, p3)}; // 垂线
    Ld V = {p2, p2 + rotate(p1, p3)};
    return lineIntersection(U, V);
}
```

### 平面直线方程转换

#### 浮点数计算直线的斜率

一般很少使用到这个函数，因为斜率的取值不可控（例如接近平行于 $x,y$ 轴时）。**需要注意**，当直线平行于 $y$ 轴时斜率为 `inf` 。

```c++
template <class T> ld slope(Pt p1, Pt p2) { // 斜率，注意 inf 的情况
    return (p1.y - p2.y) / (p1.x - p2.x);
}
template <class T> ld slope(Lt l) {
    return slope(l.a, l.b);
}
```

#### 分数精确计算直线的斜率

调用分数四则运算精确计算斜率，返回最简分数，只适用于整数计算。

```c++
template<class T> Frac<T> slopeEx(Pt p1, Pt p2) {
    Frac<T> U = p1.y - p2.y;
    Frac<T> V = p1.x - p2.x;
    return U / V; // 调用分数精确计算
}
```

#### 两点式转一般式

返回由三个整数构成的方程，在输入较大时可能找不到较小的满足题意的一组整数解。可以处理平行于 $x,y$ 轴、两点共点的情况。

```c++
template<class T> tuple<T, T, T> getfun(Lt p) {
    T A = p.a.y - p.b.y, B = p.b.x - p.a.x, C = p.a.x * A + p.a.y * B;
    if (A < 0) { // 符号调整
        A = -A, B = -B, C = -C;
    } else if (A == 0) {
        if (B < 0) {
            B = -B, C = -C;
        } else if (B == 0 && C < 0) {
            C = -C;
        }
    }
    if (A == 0) { // 数值计算
        if (B == 0) {
            C = 0; // 共点特判
        } else {
            T g = fgcd(abs(B), abs(C));
            B /= g, C /= g;
        }
    } else if (B == 0) {
        T g = fgcd(abs(A), abs(C));
        A /= g, C /= g;
    } else {
        T g = fgcd(fgcd(abs(A), abs(B)), abs(C));
        A /= g, B /= g, C /= g;
    }
    return tuple{A, B, C}; // Ax + By = C
}
```

#### 一般式转两点式

由于整数点可能很大或者不存在，故直接采用浮点数；如果与 $x,y$ 轴有交点则取交点。可以处理平行于 $x,y$ 轴的情况。

```c++
Line<ld> getfun(int A, int B, int C) { // Ax + By = C
    ld x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    if (A && B) { // 正常
        if (C) {
            x1 = 0, y1 = 1. * C / B;
            y2 = 0, x2 = 1. * C / A;
        } else { // 过原点
            x1 = 1, y1 = 1. * -A / B;
            x2 = 0, y2 = 0;
        }
    } else if (A && !B) { // 垂直
        if (C) {
            y1 = 0, x1 = 1. * C / A;
            y2 = 1, x2 = x1;
        } else {
            x1 = 0, y1 = 1;
            x2 = 0, y2 = 0;
        }
    } else if (!A && B) { // 水平
        if (C) {
            x1 = 0, y1 = 1. * C / B;
            x2 = 1, y2 = y1;
        } else {
            x1 = 1, y1 = 0;
            x2 = 0, y2 = 0;
        }
    } else { // 不合法，请特判
        assert(false);
    }
    return {{x1, y1}, {x2, y2}};
}
```

#### 抛物线与 x 轴是否相交及交点

$0$ 代表没有交点；$1$ 代表相切；$2$ 代表有两个交点。

```c++
tuple<int, ld, ld> getAns(ld a, ld b, ld c) {
    ld delta = b * b - a * c * 4;
    if (delta < 0.) {
        return {0, 0, 0};
    }
    delta = sqrt(delta);
    ld ans1 = -(delta + b) / 2 / a;
    ld ans2 = (delta - b) / 2 / a;
    if (ans1 > ans2) {
        swap(ans1, ans2);
    }
    if (sign(delta) == 0) {
        return {1, ans2, 0};
    }
    return {2, ans1, ans2};
}
```

### 平面多边形

#### 两向量构成的平面四边形有向面积

```c++
template<class T> T areaEx(Point<T> p1, Point<T> p2, Point<T> p3) {
    return cross(b, c, a);
}
```

#### 判断四个点能否组成矩形/正方形

可以处理浮点数、共点的情况。返回分为三种情况：$2$ 代表构成正方形；$1$ 代表构成矩形；$0$ 代表其他情况。

```c++
template<class T> int isSquare(vector<Pt> x) {
    sort(x.begin(), x.end());
    if (equal(dis(x[0], x[1]), dis(x[2], x[3])) && sign(dis(x[0], x[1])) &&
        equal(dis(x[0], x[2]), dis(x[1], x[3])) && sign(dis(x[0], x[2])) &&
        lineParallel(Lt{x[0], x[1]}, Lt{x[2], x[3]}) &&
        lineParallel(Lt{x[0], x[2]}, Lt{x[1], x[3]}) &&
        lineVertical(Lt{x[0], x[1]}, Lt{x[0], x[2]})) {
        return equal(dis(x[0], x[1]), dis(x[0], x[2])) ? 2 : 1;
    }
    return 0;
}
```

#### 点是否在任意多边形内

射线法判定，$t$ 为穿越次数，当其为奇数时即代表点在多边形内部；返回 $2$ 代表点在多边形边界上。

```c++
template<class T> int pointInPolygon(Point<T> a, vector<Point<T>> p) {
    int n = p.size();
    for (int i = 0; i < n; i++) {
        if (pointOnSegment(a, Line{p[i], p[(i + 1) % n]})) {
            return 2;
        }
    }
    int t = 0;
    for (int i = 0; i < n; i++) {
        auto u = p[i], v = p[(i + 1) % n];
        if (u.x < a.x && v.x >= a.x && pointOnLineLeft(a, Line{v, u})) {
            t ^= 1;
        }
        if (u.x >= a.x && v.x < a.x && pointOnLineLeft(a, Line{u, v})) {
            t ^= 1;
        }
    }
    return t == 1;
}
```

#### 线段是否在任意多边形内部

```c++
template<class T>
bool segmentInPolygon(Line<T> l, vector<Point<T>> p) {
// 线段与多边形边界不相交且两端点都在多边形内部
#define L(x, y) pointOnLineLeft(x, y)
    int n = p.size();
    if (!pointInPolygon(l.a, p)) return false;
    if (!pointInPolygon(l.b, p)) return false;
    for (int i = 0; i < n; i++) {
        auto u = p[i];
        auto v = p[(i + 1) % n];
        auto w = p[(i + 2) % n];
        auto [t, p1, p2] = segmentIntersection(l, Line(u, v));
        if (t == 1) return false;
        if (t == 0) continue;
        if (t == 2) {
            if (pointOnSegment(v, l) && v != l.a && v != l.b) {
                if (cross(v - u, w - v) > 0) {
                    return false;
                }
            }
        } else {
            if (p1 != u && p1 != v) {
                if (L(l.a, Line(v, u)) || L(l.b, Line(v, u))) {
                    return false;
                }
            } else if (p1 == v) {
                if (l.a == v) {
                    if (L(u, l)) {
                        if (L(w, l) && L(w, Line(u, v))) {
                            return false;
                        }
                    } else {
                        if (L(w, l) || L(w, Line(u, v))) {
                            return false;
                        }
                    }
                } else if (l.b == v) {
                    if (L(u, Line(l.b, l.a))) {
                        if (L(w, Line(l.b, l.a)) && L(w, Line(u, v))) {
                            return false;
                        }
                    } else {
                        if (L(w, Line(l.b, l.a)) || L(w, Line(u, v))) {
                            return false;
                        }
                    }
                } else {
                    if (L(u, l)) {
                        if (L(w, Line(l.b, l.a)) || L(w, Line(u, v))) {
                            return false;
                        }
                    } else {
                        if (L(w, l) || L(w, Line(u, v))) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}
```

#### 任意多边形的面积

```c++
template<class T> ld area(vector<Point<T>> P) {
    int n = P.size();
    ld ans = 0;
    for (int i = 0; i < n; i++) {
        ans += cross(P[i], P[(i + 1) % n]);
    }
    return ans / 2.0;
}
```

#### 皮克定理

绘制在方格纸上的多边形面积公式可以表示为 $S=n+\dfrac{s}{2}-1$ ，其中 $n$ 表示多边形内部的点数、$s$ 表示多边形边界上的点数。一条线段上的点数为 $\gcd(|x_1-x_2|,|y_1-y_2|)+1$。

#### 任意多边形上/内的网格点个数（仅能处理整数）

皮克定理用。

```c++
int onPolygonGrid(vector<Point<int>> p) { // 多边形上
    int n = p.size(), ans = 0;
    for (int i = 0; i < n; i++) {
        auto a = p[i], b = p[(i + 1) % n];
        ans += gcd(abs(a.x - b.x), abs(a.y - b.y));
    }
    return ans;
}
int inPolygonGrid(vector<Point<int>> p) { // 多边形内
    int n = p.size(), ans = 0;
    for (int i = 0; i < n; i++) {
        auto a = p[i], b = p[(i + 1) % n], c = p[(i + 2) % n];
        ans += b.y * (a.x - c.x);
    }
    ans = abs(ans);
    return (ans - onPolygonGrid(p)) / 2 + 1;
}
```

### 二维凸包

#### 获取二维静态凸包（Andrew算法）

`flag` 用于判定凸包边上的点、重复的顶点是否要加入到凸包中，为 $0$ 时代表加入凸包（不严格）；为 $1$ 时不加入凸包（严格）。时间复杂度为 $\mathcal O(N\log N)$ 。

```c++
template<class T> vector<Point<T>> staticConvexHull(vector<Point<T>> A, int flag = 1) {
    int n = A.size();
    if (n <= 2) { // 特判
        return A;
    }
    vector<Point<T>> ans(n * 2);
    sort(A.begin(), A.end());
    int now = -1;
    for (int i = 0; i < n; i++) { // 维护下凸包
        while (now > 0 && cross(A[i], ans[now], ans[now - 1]) <= 0) {
            now--;
        }
        ans[++now] = A[i];
    }
    int pre = now;
    for (int i = n - 2; i >= 0; i--) { // 维护上凸包
        while (now > pre && cross(A[i], ans[now], ans[now - 1]) <= 0) {
            now--;
        }
        ans[++now] = A[i];
    }
    ans.resize(now);
    return ans;
}
```

#### 二维动态凸包

固定为 `int` 型，需要重新书写 `Line` 函数，`cmp` 用于判定边界情况。可以处理如下两个要求：

- 动态插入点 $(x,y)$ 到当前凸包中；
- 判断点 $(x,y)$ 是否在凸包上或是在内部（包括边界）。

```c++
template<class T> bool turnRight(Pt a, Pt b) {
    return cross(a, b) < 0 || (cross(a, b) == 0 && dot(a, b) < 0);
}
struct Line {
    static int cmp;
    mutable Point<int> a, b;
    friend bool operator<(Line x, Line y) {
        return cmp ? x.a < y.a : turnRight(x.b, y.b);
    }
    friend auto &operator<<(ostream &os, Line l) {
        return os << "<" << l.a << ", " << l.b << ">";
    }
};

int Line::cmp = 1;
struct UpperConvexHull : set<Line> {
    bool contains(const Point<int> &p) const {
        auto it = lower_bound({p, 0});
        if (it != end() && it->a == p) return true;
        if (it != begin() && it != end() && cross(prev(it)->b, p - prev(it)->a) <= 0) {
            return true;
        }
        return false;
    }
    void add(const Point<int> &p) {
        if (contains(p)) return;
        auto it = lower_bound({p, 0});
        for (; it != end(); it = erase(it)) {
            if (turnRight(it->a - p, it->b)) {
                break;
            }
        }
        for (; it != begin() && prev(it) != begin(); erase(prev(it))) {
            if (turnRight(prev(prev(it))->b, p - prev(prev(it))->a)) {
                break;
            }
        }
        if (it != begin()) {
            prev(it)->b = p - prev(it)->a;
        }
        if (it == end()) {
            insert({p, {0, -1}});
        } else {
            insert({p, it->a - p});
        }
    }
};
struct ConvexHull {
    UpperConvexHull up, low;
    bool empty() const {
        return up.empty();
    }
    bool contains(const Point<int> &p) const {
        Line::cmp = 1;
        return up.contains(p) && low.contains(-p);
    }
    void add(const Point<int> &p) {
        Line::cmp = 1;
        up.add(p);
        low.add(-p);
    }
    bool isIntersect(int A, int B, int C) const {
        Line::cmp = 0;
        if (empty()) return false;
        Point<int> k = {-B, A};
        if (k.x < 0) k = -k;
        if (k.x == 0 && k.y < 0) k.y = -k.y;
        Point<int> P = up.upper_bound({{0, 0}, k})->a;
        Point<int> Q = -low.upper_bound({{0, 0}, k})->a;
        return sign(A * P.x + B * P.y - C) * sign(A * Q.x + B * Q.y - C) > 0;
    }
    friend ostream &operator<<(ostream &out, const ConvexHull &ch) {
        for (const auto &line : ch.up) out << "(" << line.a.x << "," << line.a.y << ")";
        cout << "/";
        for (const auto &line : ch.low) out << "(" << -line.a.x << "," << -line.a.y << ")";
        return out;
    }
};
```

#### 点与凸包的位置关系

$0$ 代表点在凸包外面；$1$ 代表在凸壳上；$2$ 代表在凸包内部。

```c++
template<class T> int contains(Point<T> p, vector<Point<T>> A) {
    int n = A.size();
    bool in = false;
    for (int i = 0; i < n; i++) {
        Point<T> a = A[i] - p, b = A[(i + 1) % n] - p;
        if (a.y > b.y) {
            swap(a, b);
        }
        if (a.y <= 0 && 0 < b.y && cross(a, b) < 0) {
            in = !in;
        }
        if (cross(a, b) == 0 && dot(a, b) <= 0) {
            return 1;
        }
    }
    return in ? 2 : 0;
}
```

#### 闵可夫斯基和

计算两个凸包合成的大凸包。

```c++
template<class T> vector<Point<T>> mincowski(vector<Point<T>> P1, vector<Point<T>> P2) {
    int n = P1.size(), m = P2.size();
    vector<Point<T>> V1(n), V2(m);
    for (int i = 0; i < n; i++) {
        V1[i] = P1[(i + 1) % n] - P1[i];
    }
    for (int i = 0; i < m; i++) {
        V2[i] = P2[(i + 1) % m] - P2[i];
    }
    vector<Point<T>> ans = {P1.front() + P2.front()};
    int t = 0, i = 0, j = 0;
    while (i < n && j < m) {
        Point<T> val = sign(cross(V1[i], V2[j])) > 0 ? V1[i++] : V2[j++];
        ans.push_back(ans.back() + val);
    }
    while (i < n) ans.push_back(ans.back() + V1[i++]);
    while (j < m) ans.push_back(ans.back() + V2[j++]);
    return ans;
}
```

#### 半平面交

计算多条直线左边平面部分的交集。

```c++
template<class T> vector<Point<T>> halfcut(vector<Line<T>> lines) {
    sort(lines.begin(), lines.end(), [&](auto l1, auto l2) {
        auto d1 = l1.b - l1.a;
        auto d2 = l2.b - l2.a;
        if (sign(d1) != sign(d2)) {
            return sign(d1) == 1;
        }
        return cross(d1, d2) > 0;
    });
    deque<Line<T>> ls;
    deque<Point<T>> ps;
    for (auto l : lines) {
        if (ls.empty()) {
            ls.push_back(l);
            continue;
        }
        while (!ps.empty() && !pointOnLineLeft(ps.back(), l)) {
            ps.pop_back();
            ls.pop_back();
        }
        while (!ps.empty() && !pointOnLineLeft(ps[0], l)) {
            ps.pop_front();
            ls.pop_front();
        }
        if (cross(l.b - l.a, ls.back().b - ls.back().a) == 0) {
            if (dot(l.b - l.a, ls.back().b - ls.back().a) > 0) {
                if (!pointOnLineLeft(ls.back().a, l)) {
                    assert(ls.size() == 1);
                    ls[0] = l;
                }
                continue;
            }
            return {};
        }
        ps.push_back(lineIntersection(ls.back(), l));
        ls.push_back(l);
    }
    while (!ps.empty() && !pointOnLineLeft(ps.back(), ls[0])) {
        ps.pop_back();
        ls.pop_back();
    }
    if (ls.size() <= 2) {
        return {};
    }
    ps.push_back(lineIntersection(ls[0], ls.back()));
    return vector(ps.begin(), ps.end());
}
```

### 三维几何必要初始化

#### 点线面封装

```c++
struct Point3 {
    ld x, y, z;
    Point3(ld x_ = 0, ld y_ = 0, ld z_ = 0) : x(x_), y(y_), z(z_) {}
    Point3 &operator+=(Point3 p) & {
        return x += p.x, y += p.y, z += p.z, *this;
    }
    Point3 &operator-=(Point3 p) & {
        return x -= p.x, y -= p.y, z -= p.z, *this;
    }
    Point3 &operator*=(Point3 p) & {
        return x *= p.x, y *= p.y, z *= p.z, *this;
    }
    Point3 &operator*=(ld t) & {
        return x *= t, y *= t, z *= t, *this;
    }
    Point3 &operator/=(ld t) & {
        return x /= t, y /= t, z /= t, *this;
    }
    friend Point3 operator+(Point3 a, Point3 b) { return a += b; }
    friend Point3 operator-(Point3 a, Point3 b) { return a -= b; }
    friend Point3 operator*(Point3 a, Point3 b) { return a *= b; }
    friend Point3 operator*(Point3 a, ld b) { return a *= b; }
    friend Point3 operator*(ld a, Point3 b) { return b *= a; }
    friend Point3 operator/(Point3 a, ld b) { return a /= b; }
    friend auto &operator>>(istream &is, Point3 &p) {
        return is >> p.x >> p.y >> p.z;
    }
    friend auto &operator<<(ostream &os, Point3 p) {
        return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    }
};
struct Line3 {
    Point3 a, b;
};
struct Plane {
    Point3 u, v, w;
};
```

#### 其他函数

```c++
ld len(P3 p) { // 原点到当前点的距离计算
    return sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
}
P3 crossEx(P3 a, P3 b) { // 叉乘
    P3 ans;
    ans.x = a.y * b.z - a.z * b.y;
    ans.y = a.z * b.x - a.x * b.z;
    ans.z = a.x * b.y - a.y * b.x;
    return ans;
}
ld cross(P3 a, P3 b) {
    return len(crossEx(a, b));
}
ld dot(P3 a, P3 b) { // 点乘
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
P3 getVec(Plane s) { // 获取平面法向量
    return crossEx(s.u - s.v, s.v - s.w);
}
ld dis(P3 a, P3 b) { // 三维欧几里得距离公式
    ld val = (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
    return sqrt(val);
}
P3 standardize(P3 vec) { // 将三维向量转换为单位向量
    return vec / len(vec);
}
```

### 三维点线面相关

#### 空间三点是否共线

其中第二个函数是专门用来判断给定的三个点能否构成平面的，因为不共线的三点才能构成平面。

```c++
bool onLine(P3 p1, P3 p2, P3 p3) { // 三点是否共线
    return sign(cross(p1 - p2, p3 - p2)) == 0;
}
bool onLine(Plane s) {
    return onLine(s.u, s.v, s.w);
}
```

#### 四点是否共面

```c++
bool onPlane(P3 p1, P3 p2, P3 p3, P3 p4) { // 四点是否共面
    ld val = dot(getVec({p1, p2, p3}), p4 - p1);
    return sign(val) == 0;
}
```

#### 空间点是否在线段上

```c++
bool pointOnSegment(P3 p, L3 l) {
    return sign(cross(p - l.a, p - l.b)) == 0 && min(l.a.x, l.b.x) <= p.x &&
           p.x <= max(l.a.x, l.b.x) && min(l.a.y, l.b.y) <= p.y && p.y <= max(l.a.y, l.b.y) &&
           min(l.a.z, l.b.z) <= p.z && p.z <= max(l.a.z, l.b.z);
}
bool pointOnSegmentEx(P3 p, L3 l) { // pointOnSegment去除端点版
    return sign(cross(p - l.a, p - l.b)) == 0 && min(l.a.x, l.b.x) < p.x &&
           p.x < max(l.a.x, l.b.x) && min(l.a.y, l.b.y) < p.y && p.y < max(l.a.y, l.b.y) &&
           min(l.a.z, l.b.z) < p.z && p.z < max(l.a.z, l.b.z);
}
```

#### 空间两点是否在线段同侧

当给定的两点与线段不共面、点在线段上时返回 $false$ 。

```c++
bool pointOnSegmentSide(P3 p1, P3 p2, L3 l) {
    if (!onPlane(p1, p2, l.a, l.b)) { // 特判不共面
        return 0;
    }
    ld val = dot(crossEx(l.a - l.b, p1 - l.b), crossEx(l.a - l.b, p2 - l.b));
    return sign(val) == 1;
}
```

#### 两点是否在平面同侧

点在平面上时返回 $false$ 。

```c++
bool pointOnPlaneSide(P3 p1, P3 p2, Plane s) {
    ld val = dot(getVec(s), p1 - s.u) * dot(getVec(s), p2 - s.u);
    return sign(val) == 1;
}
```

#### 空间两直线是否平行/垂直

```c++
bool lineParallel(L3 l1, L3 l2) {
    return sign(cross(l1.a - l1.b, l2.a - l2.b)) == 0;
}
bool lineVertical(L3 l1, L3 l2) {
    return sign(dot(l1.a - l1.b, l2.a - l2.b)) == 0;
}
```

#### 两平面是否平行/垂直

```c++
bool planeParallel(Plane s1, Plane s2) {
    ld val = cross(getVec(s1), getVec(s2));
    return sign(val) == 0;
}
bool planeVertical(Plane s1, Plane s2) {
    ld val = dot(getVec(s1), getVec(s2));
    return sign(val) == 0;
}
```

#### 空间两直线是否是同一条

```c++
bool same(L3 l1, L3 l2) {
    return lineParallel(l1, l2) && lineParallel({l1.a, l2.b}, {l1.b, l2.a});
}
```

#### 两平面是否是同一个

```c++
bool same(Plane s1, Plane s2) {
    return onPlane(s1.u, s2.u, s2.v, s2.w) && onPlane(s1.v, s2.u, s2.v, s2.w) &&
           onPlane(s1.w, s2.u, s2.v, s2.w);
}
```

#### 直线是否与平面平行

```c++
bool linePlaneParallel(L3 l, Plane s) {
    ld val = dot(l.a - l.b, getVec(s));
    return sign(val) == 0;
}
```

#### 空间两线段是否相交

```c++
bool segmentIntersection(L3 l1, L3 l2) { // 重叠、相交于端点均视为相交
    if (!onPlane(l1.a, l1.b, l2.a, l2.b)) { // 特判不共面
        return 0;
    }
    if (!onLine(l1.a, l1.b, l2.a) || !onLine(l1.a, l1.b, l2.b)) {
        return !pointOnSegmentSide(l1.a, l1.b, l2) && !pointOnSegmentSide(l2.a, l2.b, l1);
    }
    return pointOnSegment(l1.a, l2) || pointOnSegment(l1.b, l2) || pointOnSegment(l2.a, l1) ||
           pointOnSegment(l2.b, l2);
}
bool segmentIntersection1(L3 l1, L3 l2) { // 重叠、相交于端点不视为相交
    return onPlane(l1.a, l1.b, l2.a, l2.b) && !pointOnSegmentSide(l1.a, l1.b, l2) &&
           !pointOnSegmentSide(l2.a, l2.b, l1);
}
```

#### 空间两直线是否相交及交点

当两直线不共面、两直线平行时返回 $false$ 。

```c++
pair<bool, P3> lineIntersection(L3 l1, L3 l2) {
    if (!onPlane(l1.a, l1.b, l2.a, l2.b) || lineParallel(l1, l2)) {
        return {0, {}};
    }
    auto [s1, e1] = l1;
    auto [s2, e2] = l2;
    ld val = 0;
    if (!onPlane(l1.a, l1.b, {0, 0, 0}, {0, 0, 1})) {
        val = ((s1.x - s2.x) * (s2.y - e2.y) - (s1.y - s2.y) * (s2.x - e2.x)) /
              ((s1.x - e1.x) * (s2.y - e2.y) - (s1.y - e1.y) * (s2.x - e2.x));
    } else if (!onPlane(l1.a, l1.b, {0, 0, 0}, {0, 1, 0})) {
        val = ((s1.x - s2.x) * (s2.z - e2.z) - (s1.z - s2.z) * (s2.x - e2.x)) /
              ((s1.x - e1.x) * (s2.z - e2.z) - (s1.z - e1.z) * (s2.x - e2.x));
    } else {
        val = ((s1.y - s2.y) * (s2.z - e2.z) - (s1.z - s2.z) * (s2.y - e2.y)) /
              ((s1.y - e1.y) * (s2.z - e2.z) - (s1.z - e1.z) * (s2.y - e2.y));
    }
    return {1, s1 + (e1 - s1) * val};
}
```

#### 直线与平面是否相交及交点

当直线与平面平行、给定的点构不成平面时返回 $false$ 。

```c++
pair<bool, P3> linePlaneCross(L3 l, Plane s) {
    if (linePlaneParallel(l, s)) {
        return {0, {}};
    }
    P3 vec = getVec(s);
    P3 U = vec * (s.u - l.a), V = vec * (l.b - l.a);
    ld val = (U.x + U.y + U.z) / (V.x + V.y + V.z);
    return {1, l.a + (l.b - l.a) * val};
}
```

#### 两平面是否相交及交线

当两平面平行、两平面为同一个时返回 $false$ 。

```c++
pair<bool, L3> planeIntersection(Plane s1, Plane s2) {
    if (planeParallel(s1, s2) || same(s1, s2)) {
        return {0, {}};
    }
    P3 U = linePlaneParallel({s2.u, s2.v}, s1) ? linePlaneCross({s2.v, s2.w}, s1).second
                                               : linePlaneCross({s2.u, s2.v}, s1).second;
    P3 V = linePlaneParallel({s2.w, s2.u}, s1) ? linePlaneCross({s2.v, s2.w}, s1).second
                                               : linePlaneCross({s2.w, s2.u}, s1).second;
    return {1, {U, V}};
}
```

#### 点到直线的最近点与最近距离

```c++
pair<ld, P3> pointToLine(P3 p, L3 l) {
    ld val = cross(p - l.a, l.a - l.b) / dis(l.a, l.b); // 面积除以底边长
    ld val1 = dot(p - l.a, l.a - l.b) / dis(l.a, l.b);
    return {val, l.a + val1 * standardize(l.a - l.b)};
}
```

#### 点到平面的最近点与最近距离

```c++
pair<ld, P3> pointToPlane(P3 p, Plane s) {
    P3 vec = getVec(s);
    ld val = dot(vec, p - s.u);
    val = abs(val) / len(vec); // 面积除以底边长
    return {val, p - val * standardize(vec)};
}
```

#### 空间两直线的最近距离与最近点对

```c++
tuple<ld, P3, P3> lineToLine(L3 l1, L3 l2) {
    P3 vec = crossEx(l1.a - l1.b, l2.a - l2.b); // 计算同时垂直于两直线的向量
    ld val = abs(dot(l1.a - l2.a, vec)) / len(vec);
    P3 U = l1.b - l1.a, V = l2.b - l2.a;
    vec = crossEx(U, V);
    ld p = dot(vec, vec);
    ld t1 = dot(crossEx(l2.a - l1.a, V), vec) / p;
    ld t2 = dot(crossEx(l2.a - l1.a, U), vec) / p;
    return {val, l1.a + (l1.b - l1.a) * t1, l2.a + (l2.b - l2.a) * t2};
}
```

### 三维角度与弧度

#### 空间两直线夹角的 cos 值

任意位置的空间两直线。

```c++
ld lineCos(L3 l1, L3 l2) {
    return dot(l1.a - l1.b, l2.a - l2.b) / len(l1.a - l1.b) / len(l2.a - l2.b);
}
```

#### 空间两平面夹角的 cos 值

```c++
ld planeCos(Plane s1, Plane s2) {
    P3 U = getVec(s1), V = getVec(s2);
    return dot(U, V) / len(U) / len(V);
}
```

#### 直线与平面夹角的 sin 值

```c++
ld linePlaneSin(L3 l, Plane s) {
    P3 vec = getVec(s);
    return dot(l.a - l.b, vec) / len(l.a - l.b) / len(vec);
}
```

### 空间多边形

#### 正N棱锥体积公式

棱锥通用体积公式 $V=\dfrac{1}{3}Sh$ ，当其恰好是棱长为 $l$ 的正 $n$ 棱锥时，有公式 $\displaystyle V=\frac{l^3\cdot n}{12\tan \frac{\pi}{n}}\cdot\sqrt{1-\frac{1}{4\cdot \sin^2\frac{\pi}{n}}}$。

```c++
ld V(ld l, int n) { // 正n棱锥体积公式
    return l * l * l * n / (12 * tan(PI / n)) * sqrt(1 - 1 / (4 * sin(PI / n) * sin(PI / n)));
}
```

#### 四面体体积

```c++
ld V(P3 a, P3 b, P3 c, P3 d) {
    return abs(dot(d - a, crossEx(b - a, c - a))) / 6;
}
```

#### 点是否在空间三角形上

点位于边界上时返回 $false$ 。

```c++
bool pointOnTriangle(P3 p, P3 p1, P3 p2, P3 p3) {
    return pointOnSegmentSide(p, p1, {p2, p3}) && pointOnSegmentSide(p, p2, {p1, p3}) &&
           pointOnSegmentSide(p, p3, {p1, p2});
}
```

#### 线段是否与空间三角形相交及交点

只有交点在空间三角形内部时才视作相交。

```c++
pair<bool, P3> segmentOnTriangle(P3 l, P3 r, P3 p1, P3 p2, P3 p3) {
    P3 x = crossEx(p2 - p1, p3 - p1);
    if (sign(dot(x, r - l)) == 0) {
        return {0, {}};
    }
    ld t = dot(x, p1 - l) / dot(x, r - l);
    if (t < 0 || t - 1 > 0) { // 不在线段上
        return {0, {}};
    }
    bool type = pointOnTriangle(l + (r - l) * t, p1, p2, p3);
    if (type) {
        return {1, l + (r - l) * t};
    } else {
        return {0, {}};
    }
}
```

#### 空间三角形是否相交

相交线段在空间三角形内部时才视作相交。

```c++
bool triangleIntersection(vector<P3> a, vector<P3> b) {
    for (int i = 0; i < 3; i++) {
        if (segmentOnTriangle(b[i], b[(i + 1) % 3], a[0], a[1], a[2]).first) {
            return 1;
        }
        if (segmentOnTriangle(a[i], a[(i + 1) % 3], b[0], b[1], b[2]).first) {
            return 1;
        }
    }
    return 0;
}
```

### 常用结论

#### 平面几何结论归档

- `hypot` 函数可以直接计算直角三角形的斜边长；
- **边心距**是指正多边形的外接圆圆心到正多边形某一边的距离，边长为 $s$ 的正 $n$ 角形的边心距公式为 $\displaystyle a=\frac{t}{2\cdot\tan \frac{\pi}{n}}$ ，外接圆半径为 $R$ 的正 $n$ 角形的边心距公式为 $a=R\cdot \cos \dfrac{\pi}{n}$ ；
- **三角形外接圆半径**为 $\dfrac{a}{2\sin A}=\dfrac{abc}{4S}$ ，其中 $S$ 为三角形面积，内切圆半径为 $\dfrac{2S}{a+b+c}$；
- 由小正三角形拼成的大正三角形，耗费的小三角形数量即为构成一条边的小三角形数量的平方。如下图，总数量即为 $4^2$ [See](https://codeforces.com/problemset/problem/559/A)。
  
  <img src="https://s2.loli.net/2023/08/17/p7kRACD4cTf3YxK.png" alt="91044c3ef9c959aae5be2e7d53c13dd0.png" style="zoom:30%;" />
- 正 $n$ 边形圆心角为 $\dfrac{360^{\circ}}{n}$ ，圆周角为 $\dfrac{180^{\circ}}{n}$ 。定义正 $n$ 边形上的三个顶点 $A,B$ 和 $C$（可以不相邻），使得 $\angle ABC=\theta$ ，当 $n\le 360$ 时，$\theta$ 可以取 $1^{\circ}$ 到 $179^{\circ}$ 间的任何一个整数 [See](https://codeforces.com/problemset/problem/1096/C)。
- 某一点 $B$ 到直线 $AC$ 的距离公式为 $\dfrac{|\vec{BA}\times \vec{BC}|}{|AC|}$ ，等价于 $\dfrac{|aX+bY+c|}{\sqrt{a^2+b^2}}$。
- `atan(y / x)` 函数仅用于计算第一、四象限的值，而 `atan2(y, x)` 则允许计算所有四个象限的正反切，在使用这个函数时，需要尽量保证 $x$ 和 $y$ 的类型为整数型，如果使用浮点数，实测会慢十倍。
- 在平面上有奇数个点 $A_0,A_1,\dots,A_n$ 以及一个点 $X_0$ ，构造 $X_1$ 使得 $X_0,X_1$ 关于 $A_0$ 对称、构造 $X_2$ 使得 $X_1,X_2$ 关于 $A_1$ 对称、……、构造 $X_j$ 使得 $X_{j-1},X_j$ 关于 $A_{(j-1)\mod n}$ 对称。那么周期为 $2n$ ，即 $A_0$ 与 $A_{2n}$ 共点、$A_1$ 与 $A_{2n+1}$ 共点 [See](https://codeforces.com/contest/24/problem/C) 。
- 已知 $A\ (x_A, y_A)$ 和 $X\ (x_X,y_X)$ 两点及这两点的坐标，构造 $Y$ 使得 $X,Y$ 关于 $A$ 对称，那么 $Y$ 的坐标为 $(2\cdot x_A-x_X,2\cdot y_A-y_X)$ 。
- **海伦公式**：已知三角形三边长 $a,b$ 和 $c$ ，定义 $p=\dfrac{a+b+c}{2}$ ，则 $S_{\triangle}=\sqrt{p(p-a)(p-b)(p-c)}$ ，在使用时需要注意越界问题，本质是铅锤定理，一般多使用叉乘计算三角形面积而不使用该公式。
- 棱台体积 $V=\frac{1}{3}(S_1+S_2+\sqrt{S_1S_2})\cdot h$，其中 $S_1,S_2$ 为上下底面积。
- 正棱台侧面积 $\frac{1}{2}(C_1+C_2)\cdot L$，其中 $C_1,C_2$ 为上下底周长，$L$ 为斜高（上下底对应的平行边的距离）。
- 球面积 $4\pi r^2$，体积 $\frac{4}{3}\pi r^3$。
- 正三角形面积 $\dfrac{\sqrt 3 a^2}{4}$，正四面体面积 $\dfrac{\sqrt 2 a^3}{12}$。
- 设扇形对应的圆心角弧度为 $\theta$ ，则面积为 $S=\frac{\theta}{2}\cdot R^2$ 。

#### 立体几何结论归档

- 已知向量 $\vec{r}=\{x,y,z\}$ ，则该向量的三个方向余弦为 $\cos \alpha =\dfrac{x}{|\vec r|}=\dfrac{x}{\sqrt{x^2+y^2+z^2}}; \ \cos \beta = \dfrac{y}{|\vec r|};\ \cos \gamma =\dfrac{z}{|\vec r|}$ 。其中 $\alpha,\beta,\gamma\in [0,\pi]$ ，$\cos^2\alpha+\cos^2\beta+\cos^2\gamma=1$ 。

### 常用例题

#### 将平面某点旋转任意角度

题意：给定平面上一点 $(a,b)$ ，输出将其逆时针旋转 $d$ 度之后的坐标。

```c++
signed main() {
    int a, b, d;
    cin >> a >> b >> d;
    
    ld l = hypot(a, b); // 库函数，求直角三角形的斜边
    ld alpha = atan2(b, a) + toArc(d);
    
    cout << l * cos(alpha) << " " << l * sin(alpha) << endl;
}
```

#### 平面最近点对（set解）

借助 `set` ，在严格 $\mathcal O(N\log N)$ 复杂度内求解，比常见的分治法稍快。

```c++
template<class T> T sqr(T x) {
    return x * x;
}

using V = Point<int>;
signed main() {
    int n;
    cin >> n;

    vector<V> in(n);
    for (auto &it : in) {
        cin >> it;
    }

    int dis = disEx(in[0], in[1]); // 设定阈值
    sort(in.begin(), in.end());

    set<V> S;
    for (int i = 0, h = 0; i < n; i++) {
        V now = {in[i].y, in[i].x};
        while (dis && dis <= sqr(in[i].x - in[h].x)) { // 删除超过阈值的点
            S.erase({in[h].y, in[h].x});
            h++;
        }
        auto it = S.lower_bound(now);
        for (auto k = it; k != S.end() && sqr(k->x - now.x) < dis; k++) {
            dis = min(dis, disEx(*k, now));
        }
        if (it != S.begin()) {
            for (auto k = prev(it); sqr(k->x - now.x) < dis; k--) {
                dis = min(dis, disEx(*k, now));
                if (k == S.begin()) break;
            }
        }
        S.insert(now);
    }
    cout << sqrt(dis) << endl;
}
```

#### 平面若干点能构成的最大四边形的面积（简单版，暴力枚举）

题意：平面上存在若干个点，保证没有两点重合、没有三点共线，你需要从中选出四个点，使得它们构成的四边形面积是最大的，注意这里能组成的四边形可以不是凸四边形。

暴力枚举其中一条对角线后枚举剩余两个点，$\mathcal O(N^3)$ 。

```c++
signed main() {
    int n;
    cin >> n;
    vector<Pi> in(n);
    for (auto &it : in) {
        cin >> it;
    }
    ld ans = 0;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) { // 枚举对角线
            ld l = 0, r = 0;
            for (int k = 0; k < n; k++) { // 枚举第三点
                if (k == i || k == j) continue;
                if (pointOnLineLeft(in[k], {in[i], in[j]})) {
                    l = max(l, triangleS(in[k], in[j], in[i]));
                } else {
                    r = max(r, triangleS(in[k], in[j], in[i]));
                }
            }
            if (l * r != 0) { // 确保构成的是四边形
                ans = max(ans, l + r);
            }
        }
    }
    cout << ans << endl;
}
```

#### 平面若干点能构成的最大四边形的面积（困难版，分类讨论+旋转卡壳）

题意：平面上存在若干个点，可能存在多点重合、共线的情况，你需要从中选出四个点，使得它们构成的四边形面积是最大的，注意这里能组成的四边形可以不是凸四边形、可以是退化的四边形。

当凸包大小 $\le 2$ 时，说明是退化的四边形，答案直接为 $0$ ；大小恰好为 $3$ 时，说明是凹四边形，我们枚举不在凸包上的那一点，将两个三角形面积相减既可得到答案；大小恰好为 $4$ 时，说明是凸四边形，使用旋转卡壳求解。

```c++
using V = Point<int>;
signed main() {
    int Task = 1;
    for (cin >> Task; Task; Task--) {
        int n;
        cin >> n;
        
        vector<V> in_(n);
        for (auto &it : in_) {
            cin >> it;
        }
        auto in = staticConvexHull(in_, 0);
        n = in.size();
        
        int ans = 0;
        if (n > 3) {
            ans = rotatingCalipers(in);
        } else if (n == 3) {
            int area = triangleAreaEx(in[0], in[1], in[2]);
            for (auto it : in_) {
                if (it == in[0] || it == in[1] || it == in[2]) continue;
                int Min = min({triangleAreaEx(it, in[0], in[1]), triangleAreaEx(it, in[0], in[2]), triangleAreaEx(it, in[1], in[2])});
                ans = max(ans, area - Min);
            }
        }
        
        cout << ans / 2;
        if (ans % 2) {
            cout << ".5";
        }
        cout << endl;
    }
}
```

#### 线段将多边形切割为几个部分

题意：给定平面上一线段与一个任意多边形，求解线段将多边形切割为几个部分；保证线段的端点不在多边形内、多边形边上，多边形顶点不位于线段上，多边形的边不与线段重叠；多边形端点按逆时针顺序给出。下方的几个样例均合法，答案均为 $3$ 。

<img src="https://img2023.cnblogs.com/blog/2491503/202308/2491503-20230827211620035-1506522300.png" alt="截图" style="zoom:60%;" /><img src="https://img2023.cnblogs.com/blog/2491503/202308/2491503-20230827211624866-1953825694.png" alt="截图" style="zoom:60%;" />

当线段切割多边形时，本质是与多边形的边交于两个点、或者说是与多边形的两条边相交，设交点数目为 $x$ ，那么答案即为 $\frac{x}{2}+1$ 。于是，我们只需要计算交点数量即可，先判断某一条边是否与线段相交，再判断边的两个端点是否位于线段两侧。

```c++
signed main() {
    Pi s, e;
    cin >> s >> e; // 读入线段
    
    int n;
    cin >> n;
    vector<Pi> in(n);
    for (auto &it : in) {
        cin >> it; // 读入多边形端点
    }
    
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        Pi x = in[i], y = in[(i + 1) % n];
        cnt += (pointNotOnLineSide(x, y, {s, e}) && segmentIntersection(Line{x, y}, {s, e}));
    }
    cout << cnt / 2 + 1 << endl;
}
```

#### 平面若干点能否构成凸包（暴力枚举）

题意：给定平面上若干个点，判断其是否构成凸包 [See](https://atcoder.jp/contests/abc266/tasks/abc266_c) 。

可以直接使用凸包模板，但是代码较长；在这里我们使用暴力枚举试点，也能以 $\mathcal O(N)$ 的复杂度通过。当两个向量的叉乘 $\le0$ 时说明其夹角大于等于 $180\degree$ ，使用这一点即可判定。

```c++
signed main() {
    int n;
    cin >> n;
    
    vector<Point<ld>> in(n);
    for (auto &it : in) {
        cin >> it;
    }
    
    for (int i = 0; i < n; i++) {
        auto A = in[(i - 1 + n) % n];
        auto B = in[i];
        auto C = in[(i + 1) % n];
        if (cross(A - B, C - B) > 0) {
            cout << "No\n";
            return 0;
        }
    }
    cout << "Yes\n";
}
```

#### 凸包上的点能构成的最大三角形（暴力枚举）

可以直接使用凸包模板，但是代码较长；在这里我们使用暴力枚举试点，也能以 $\mathcal O(N)$ 的复杂度通过。

> 另外补充一点性质：所求三角形的反互补三角形一定包含了凸包上的所有点（可以在边界）。通俗的说，构成的三角形是这个反互补三角形的中点三角形。如下图所示，点 $A$ 不在 $\triangle BCE$ 的反互补三角形内部，故 $\triangle BCE$ 不是最大三角形；$\triangle ACE$ 才是。
> 
> <img src="https://img2023.cnblogs.com/blog/2491503/202308/2491503-20230827205516769-1055425260.png" alt="截图" style="zoom:80%;" />
> 
> ![](https://img2023.cnblogs.com/blog/2491503/202308/2491503-20230827205528116-1886683012.png)

```c++
signed main() {
    int n;
    cin >> n;
     
    vector<Point<int>> in(n);
    for (auto &it : in) {
        cin >> it;
    }
    
    #define S(x, y, z) triangleAreaEx(in[x], in[y], in[z])
     
    int i = 0, j = 1, k = 2;
    while (true) {
        int val = S(i, j, k);
        if (S((i + 1) % n, j, k) > val) {
            i = (i + 1) % n;
        } else if (S((i - 1 + n) % n, j, k) > val) {
            i = (i - 1 + n) % n;
        } else if (S(i, (j + 1) % n, k) > val) {
            j = (j + 1) % n;
        } else if (S(i, (j - 1 + n) % n, k) > val) {
            j = (j - 1 + n) % n;
        } else if (S(i, j, (k + 1) % n) > val) {
            k = (k + 1) % n;
        } else if (S(i, j, (k - 1 + n) % n) > val) {
            k = (k - 1 + n) % n;
        } else {
            break;
        }
    }
    cout << i + 1 << " " << j + 1 << " " << k + 1 << endl;
}
```

#### 凸包上的点能构成的最大四角形的面积（旋转卡壳）

由于是凸包上的点，所以保证了四边形一定是凸四边形，时间复杂度 $\mathcal O(N^2)$ 。

```c++
template<class T> T rotatingCalipers(vector<Point<T>> &p) {
    #define S(x, y, z) triangleAreaEx(p[x], p[y], p[z])
    int n = p.size();
    T ans = 0;
    auto nxt = [&](int i) -> int {
        return i == n - 1 ? 0 : i + 1;
    };
    for (int i = 0; i < n; i++) {
        int p1 = nxt(i), p2 = nxt(nxt(nxt(i)));
        for (int j = nxt(nxt(i)); nxt(j) != i; j = nxt(j)) {
            while (nxt(p1) != j && S(i, j, nxt(p1)) > S(i, j, p1)) {
                p1 = nxt(p1);
            }
            if (p2 == j) {
                p2 = nxt(p2);
            }
            while (nxt(p2) != i && S(i, j, nxt(p2)) > S(i, j, p2)) {
                p2 = nxt(p2);
            }
            ans = max(ans, S(i, j, p1) + S(i, j, p2));
        }
    }
    return ans;
    #undef S
}
```

#### 判断一个凸包是否完全在另一个凸包内

题意：给定一个凸多边形 $A$ 和一个凸多边形 $B$ ，询问 $B$ 是否被 $A$ 包含，分别判断严格/不严格包含。[例题](https://codeforces.com/contest/166/problem/B)。

考虑严格包含，使用 $A$ 点集计算出凸包 $T_1$ ，使用 $A,B$ 两个点集计算出不严格凸包 $T_2$ ，如果包含，那么 $T_1$ 应该与 $T_2$ 完全相等；考虑不严格包含，在计算凸包 $T_2$ 时严格即可。最终以 $\mathcal O(N)$ 复杂度求解，且代码不算很长。


# 组合数学
## 基础
依赖ModInt, 否则需要手动取模.
```cpp
template <int N, int MOD = 998244353>
struct Factorial {
    using mint = ModInt<MOD>;
    array<mint, N + 5> fact, factinv;
    Factorial() {
        fact[0] = 1;
        for (int i = 1; i <= N; i++) {
            fact[i] = fact[i - 1] * i;
        }
        factinv[N] = fact[N].inv();
        for (int i = N - 1; i >= 0; i--) {
            factinv[i] = factinv[i + 1] * (i + 1);
        }
    }
    mint nCr(int n, int r) {
        if (!(0 <= r && r <= n)) {
            return 0;
        }
        return fact[n] * factinv[r] * factinv[n - r];
    }
    mint nPr(int n, int r) {
        if (!(0 <= r && r <= n)) {
            return 0;
        }
        return fact[n] * factinv[n - r];
    }
};
```

# 模运算
## 模运算类
```cpp
constexpr int MOD = 998'244'353;

template <int P> 
struct ModInt {
    u32 val;
    constexpr ModInt(ll x = 0) : val((x %= P) < 0 ? x + P : x) {}
    // 注意i128等类型需要先转成ll
    using M = ModInt;
    M &operator+=(const M &p) {
        if ((val += p.val) >= P) {
            val -= P;
        }
        return *this;
    }
    M &operator-=(const M &p) {
        if ((val += P - p.val) >= P) {
            val -= P;
        }
        return *this;
    }
    M &operator*=(const M &p) {
        val = u64(val) * p.val % P;
        return *this;
    }
    M &operator/=(const M &p) { return (*this) *= p.inv(); }
    constexpr M operator-() const { return M(val ? P - val : u32(0)); }
    constexpr M operator+(const M &p) const { return M(*this) += p; }
    constexpr M operator-(const M &p) const { return M(*this) -= p; }
    constexpr M operator*(const M &p) const { return M(*this) *= p; }
    constexpr M operator/(const M &p) const { return M(*this) /= p; }
    constexpr bool operator==(const M &p) const { return val == p.val; }
    constexpr bool operator!=(const M &p) const { return val != p.val; }
    M pow(ll n) const {
        if (n < 0) return inv().pow(-n);
        M ret(1), mul(val);
        while (n > 0) {
            if (n & 1) ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }
    M inv() const {
        int a = val, b = P, u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            a -= t * b;
            u -= t * v;
            swap(a, b);
            swap(u, v);
        }
        return M(u);
    }
    constexpr static M raw(u32 x) {
        M ret;
        ret.val = x;
        return ret;
    }
    constexpr static int mod() {
        return P;
    }
};
using mint = ModInt<MOD>;
```

# 数论

## exCRT
```cpp
array<i128, 2> excrt(const vector<ll>& b, const vector<ll>& m) {
    int n = b.size();
    i128 nowb = b[0], nowm = m[0];
    for (int i = 1; i < n; i++) {
        ll A = nowm % m[i], B = m[i];
        i128 C = b[i] - nowb;
        auto [y1, _, g] = exgcd(A, B);
        if (C % g != 0) {
            return { -1, -1 };
        }
        
        i128 M = m[i] / g;
        i128 y = (i128)y1 * (C / g) % M;
        y = (y < 0) ? y + M : y;
        nowb += y * nowm;
        nowm *= M;
        nowb %= nowm;
    }
    return { nowb, nowm };
}
```
## exGCD
```cpp
i128 abs128(i128 x) {
    return x < 0 ? -x : x;
}
array<i128, 3> exgcd(i128 a, i128 b) {
    if (b == 0) {
        if (a >= 0) return {1, 0, a};
        else return {-1, 0, -a};
    }
    auto [x, y, g] = exgcd(b, a % b);
    return {y, x - a / b * y, g};
}
i128 norm_mod(i128 x, i128 mod) {
    mod = abs128(mod);
    x %= mod;
    if (x < 0) x += mod;
    return x;
}
tuple<bool, i128, i128> solveLDE(i128 a, i128 b, i128 c) {
    // solve ax + by = c
    if (a == 0 && b == 0) {
        if (c == 0) return {true, 0, 0};
        else return {false, 0, 0};
    }
    auto [x, y, g] = exgcd(a, b);
    if (c % g != 0) {
        return {false, 0, 0};
    }
    i128 k = c / g;
    x *= k;
    y *= k;
    // 把 x 规范到 [0, |b / g| - 1]
    if (b != 0) {
        i128 step_x = b / g;
        i128 nx = norm_mod(x, step_x);
        // x = x0 + (b / g) * t
        // y = y0 - (a / g) * t
        i128 t = (nx - x) / step_x;
        x = nx;
        y = y - (a / g) * t;
    }
    return {true, x, y};
}
```

## 线性筛
```cpp
constexpr int N = 1'000'000;
constexpr int PIN = 78'498;
bitset<N + 5> not_prime;
int primes[PIN + 5], pcnt = 0;
// 1e9 : 50'847'534
// 1e8 : 5'761'455
// 1e7 : 664'579
// 1e6 : 78'498
// 1e5 : 9'592
// 1e4 : 1'229
// 1e3 : 168
// 1e2 : 25
// 1e1 : 4
// 下界N / log(N), 上界(>61000时) N / (log(N) - 1.1)",

// int minp[N + 5]; // 最小质因子
// int phi[N + 5]; // 欧拉函数
// int mu[N + 5]; // 莫比乌斯函数
// int d[N + 5], times[N + 5]; // 约数计数函数
// ll sigma[N + 5]; int g[N + 5]; // 约数和函数
void sieve() {
    // phi[1] = 1;
    // mu[1] = 1;
    // d[1] = 1;
    // sigma[1] = 1;
    for (int i = 2; i <= N; i++) {
        if (!not_prime[i]) {
            primes[pcnt] = i, pcnt++;
            // minp[i] = i;
            // phi[i] = i - 1;
            // mu[i] = -1;
            // d[i] = 2, times[i] = 1;
            // sigma[i] = i + 1, g[i] = i + 1;
        }
        for (int j = 0; j < pcnt && i * primes[j] <= N; j++) {
            int pj = primes[j];
            not_prime[i * pj] = 1;
            // minp[i * pj] = pj;
            if (i % primes[j] == 0) {
                // phi[i * pj] = phi[i] * pj;
                // mu[i * pj] = 0;
                // d[i * pj] = d[i] / (times[i] + 1) * (times[i] + 2), times[i * pj] = times[i] + 1;
                // g[i * pj] = g[i] * pj + 1, sigma[i * pj] = sigma[i] / g[i] * g[i * pj];
                break;
            }
            // phi[i * pj] = phi[i] * (pj - 1);
            // mu[i * pj] = -mu[i];
            // d[i * pj] = d[i] * 2, times[i * pj] = 1;
            // sigma[i * pj] = sigma[i] * sigma[pj], g[i * pj] = pj + 1;
        }
    }
}
```

## 分段筛
```cpp
constexpr ll R = 1'000'000'300;
constexpr int SQR = 32'000;
constexpr int PIS = 4'000;
// sqrt(10) < 3.17
bitset<SQR + 5> not_prime;
int primes[PIS + 5], pcnt = 0;
void init_sieve() {
    not_prime[0] = not_prime[1] = 1;
    for (ll i = 2; i * i <= R; i++) {
        if (!not_prime[i]) {
            primes[pcnt] = i;
            pcnt++;
            for (ll j = i * i; j <= SQR; j += i) {
                not_prime[j] = 1;
            }
        }
    }
}

vector<ll> get_primes(ll l, ll r) {
    vector<bool> notp(r - l + 1);
    if (l == 0) {
        notp[0] = notp[1] = 1;
    } else if (l == 1) {
        notp[0] = 1;
    }

    for (int i = 0; i < pcnt; i++) {
        ll p = primes[i];
        if (p * p > r) {
            break;
        }
        for (ll j = max(2LL, (l + p - 1) / p); j * p <= r; j++) {
            notp[j * p - l] = 1;
        }
    }
    vector<ll> ret;
    for (ll i = l; i <= r; i++) {
        if (!notp[i - l]) {
            ret.emplace_back(i);
        }
    }
    return ret;
}

```

## Miller-Rabin
```cpp
bool primetest(ll n) {
    if (n < 2 || n % 6 % 4 != 1) {
        return (n | 1) == 3;
    }
    ll s = __builtin_ctzll(n - 1), d = n >> s;
    for (ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        ll p = pow(a % n, d, n), i = s;
        while (p != 1 && p != n - 1 && a % n && i--) {
            p = mul(p, p, n);
        }
        if (p != n - 1 && i != s) {
            return false;
        }
    }
    return true;
}
```

## Pollard-rho
```cpp
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

ll rho(ll n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;

    while (true) {
        ll c = uniform_int_distribution<ll>(1, n - 1)(rng);
        ll x = uniform_int_distribution<ll>(0, n - 1)(rng);
        ll y = x, d = 1;

        auto f = [&](ll x) -> ll {
            return (mul(x, x, n) + c) % n;
        };

        while (d == 1) {
            x = f(x);
            y = f(f(y));
            d = gcd(abs(x - y), n);
        }

        if (d != n) return d;
    }
}
vector<ll> factorize(ll x) {
    vector<ll> res;

    auto dfs = [&](auto self, ll x) -> void {
        if (x == 1) return;
        if (primetest(x)) {
            res.push_back(x);
            return;
        }

        ll d = rho(x);
        self(self, d);
        self(self, x / d);
    };

    dfs(dfs, x);
    sort(res.begin(), res.end());
    return res;
}
```

## BSGS
```cpp
namespace BSGS {
ll mul(ll a, ll b, ll mod) {
    return (i128)a * b % mod;
}

ll power(ll a, ll b, ll mod) {
    ll ret = 1 % mod;
    while (b) {
        if (b & 1) ret = mul(ret, a, mod);
        a = mul(a, a, mod);
        b >>= 1;
    }
    return ret;
}

ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    ll x1, y1;
    ll g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - a / b * y1;
    return g;
}

ll inv(ll a, ll mod) {
    ll x, y;
    ll g = exgcd(a, mod, x, y);
    if (g != 1) return -1;
    x %= mod;
    if (x < 0) x += mod;
    return x;
}
ll bsgs(ll a, ll b, ll m) {
    a %= m;
    b %= m;

    if (m == 1) return 0;
    if (b == 1) return 0;

    ll n = sqrtl(m) + 1;
    HashMap<ll, ll> mp;
    // b * a^q
    ll cur = b;
    for (ll q = 0; q < n; q++) {
        mp[cur] = q;
        cur = mul(cur, a, m);
    }
    ll an = power(a, n, m);
    cur = 1;
    for (ll p = 1; p <= n + 1; p++) {
        cur = mul(cur, an, m);
        auto it = mp.find(cur);
        if (it != mp.end()) {
            ll x = p * n - it->second;
            if (x >= 0) return x;
        }
    }
    return -1;
}

ll exbsgs(ll a, ll b, ll m) {
    a %= m;
    b %= m;
    if (m == 1) return 0;
    if (b == 1) return 0;
    ll cnt = 0;
    ll cur = 1;
    while (true) {
        ll g = gcd(a, m);
        if (g == 1) break;
        if (b % g != 0) return -1;
        b /= g;
        m /= g;
        cur = mul(cur, a / g, m);
        cnt++;
        if (cur == b) return cnt;
    }
    ll iv = inv(cur, m);
    ll rhs = mul(b, iv, m);
    ll t = bsgs(a, rhs, m);
    if (t == -1) return -1;
    return cnt + t;
}
};
```

## 杜教筛
```cpp
constexpr int N = 1'000'000;
constexpr int PIN = 78'498;
bitset<N + 5> not_prime;
int primes[PIN + 5], pcnt = 0;
int phi[N + 5];
int mu[N + 5];
ll smu[N + 5];
ll sphi[N + 5];
unordered_map<ll, ll> mp_mu;
unordered_map<ll, i128> mp_phi;
void sieve() {
    phi[1] = 1;
    mu[1] = 1;
    for (int i = 2; i <= N; i++) {
        if (!not_prime[i]) {
            primes[pcnt] = i, pcnt++;
            phi[i] = i - 1;
            mu[i] = -1;
        }
        for (int j = 0; j < pcnt && 1LL * i * primes[j] <= N; j++) {
            int pj = primes[j];
            not_prime[i * pj] = 1;

            if (i % pj == 0) {
                phi[i * pj] = phi[i] * pj;
                mu[i * pj] = 0;
                break;
            }

            phi[i * pj] = phi[i] * (pj - 1);
            mu[i * pj] = -mu[i];
        }
    }

    for (int i = 1; i <= N; i++) {
        smu[i] = smu[i - 1] + mu[i];
        sphi[i] = sphi[i - 1] + phi[i];
    }
}

ll getsmu(ll n) {
    // 莫比乌斯函数的前缀和
    if (n <= N) {
        return smu[n];
    }
    if (mp_mu.count(n)) {
        return mp_mu[n];
    }

    ll ans = 1;

    for (ll l = 2, r; l <= n; l = r + 1) {
        ll q = n / l;
        r = n / q;
        ans -= (r - l + 1) * getsmu(q);
    }
    return mp_mu[n] = ans;
}

i128 getsphi(ll n) {
    // 欧拉函数的前缀和
    if (n <= N) {
        return sphi[n];
    }
    if (mp_phi.count(n)) {
        return mp_phi[n];
    }

    i128 ans = (i128)n * (n + 1) / 2;

    for (ll l = 2, r; l <= n; l = r + 1) {
        ll q = n / l;
        r = n / q;
        ans -= (i128)(r - l + 1) * getsphi(q);
    }

    return mp_phi[n] = ans;
}
```

## 杂项结论
### 麦乐鸡定理
对于互质整数$n, m$来说, $an + bm(a \ge 0, b \ge 0)$不能表示出的最大整数为$nm - n - m$.

### 抽屉原理
$n$个物体放到$m$个抽屉中, 抽屉中物品的最大值大于等于$ceil(n / m)$

### Dilworth定理
偏序集中, 最大链长度等于最多反链划分数, 对偶结论亦成立.

### 取模与整数除法
`x % i = x - i * (x / i)`

### 欧拉函数性质
1. $[1, n]$中与$n$互质的数之和为$\frac{n\varphi(n)}{2}$
2. 若$a, b$互质, 则$\varphi(ab) = \varphi(a)\varphi(b)$
3. 若$p$是$n$的质因子: 若$p$的次数大于等于2, 则$\varphi(n) = \varphi(\frac{n}{p})p$ ; 否则$\varphi(n) = \varphi(\frac{n}{p}) (p - 1)$
4. $\sum_{d|n}\varphi(d) = n$
5. $\sum_{i = 1}^n\gcd(i, n) = \sum_{d|n}\varphi(d)\frac{n}{d}$. (欧拉反演)


# 代数
## 多项式
### FFT
```cpp
using cd = complex<double>;
using numbers::pi;
const cd I(0, 1);
vector<int> rev;
vector<cd> Wn;

void fft(vector<cd>& a, bool invert = 0) {
    int n = a.size();
    if ((int)rev.size() != n) {
        rev.assign(n, 0);
        for (int i = 1; i < n; i++) {
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) * (n >> 1));
        }
    }
    if ((int)Wn.size() != n) {
        Wn.assign(n, cd(0, 0));
        for (int i = 0; i < n; i++) {
            Wn[i] = cd(cos(pi / n * i), sin(pi / n * i));
        }
    }
    if (invert) {
        for (int i = 1; i < n; i++) {
            if (i < n - i) {
                swap(a[i], a[n - i]);
            }
        }
    }
    for (int i = 0; i < n; i++) {
        if (i < rev[i]) {
            swap(a[i], a[rev[i]]);
        }
    }
    for (int m = 1; m < n; m <<= 1) {
        for (int i = 0; i < n; i += m << 1) {
            for (int j = 0; j < m; j++) {
                cd w = Wn[1LL * j * n / m];
                cd x = a[i + j];
                cd y = a[i + j + m] * w;

                a[i + j] = x + y;
                a[i + j + m] = x - y;
            }
        }
    }
    if (invert) {
        for (int i = 0; i < n; i++) {
            a[i] /= n;
        }
    }
}

vector<ll> convolution(const vector<ll>& a, const vector<ll>& b) {
    if (a.empty() || b.empty()) {
        return {};
    }
    int sz = (int)a.size() + b.size() - 1;
    int n = 1;
    while (n < sz) {
        n <<= 1;
    }
    vector<cd> A(n), B(n);
    for (int i = 0; i < (int)a.size(); i++) {
        A[i] = a[i];
    }
    for (int i = 0; i < (int)b.size(); i++) {
        B[i] = b[i];
    }
    for (int i = 0; i < n; i++) {
        A[i] += I * B[i];
    }
    fft(A);
    for (int i = 0; i < n; i++) {
        B[i] = conj(A[i ? n - i : 0]);
    }
    for (int i = 0; i < n; i++) {
        cd p = A[i];
        cd q = B[i];
        A[i] = (p + q) * 0.5;
        B[i] = (q - p) * 0.5 * I;
    }
    for (int i = 0; i < n; i++) {
        A[i] *= B[i];
    }
    fft(A, 1);
    vector<ll> c(sz);
    for (int i = 0; i < sz; i++) {
        double x = A[i].real();
        c[i] = x < 0 ? (ll)(x - 0.5) : (ll)(x + 0.5);
    }
    return c;
}
```

### NTT
```cpp
template<class Z>
void ntt(vector<Z>& a, bool invert = 0) {
    int n = a.size();
    vector<int> rev(n);
    for (int i = 0; i < n; i++) {
        rev[i] = ((i & 1) * (n / 2)) | (rev[i / 2] / 2);
        if (i < rev[i]) {
            swap(a[i], a[rev[i]]);
        }
    }
    Z g = 3;

    for (int len = 2; len <= n; len *= 2) {
        Z wlen = g.pow((Z::mod() - 1) / len);
        if (invert) {
            wlen = wlen.inv();
        }
        
        for (int i = 0; i < n; i += len) {
            Z w = 1;
            for (int j = 0; j < len / 2; j++) {
                Z u = a[i + j];
                Z v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    
    if (invert) {
        Z ni = Z(n).inv();
        for (int i = 0; i < n; i++) {
            a[i] *= ni;
        }
    }
}

template<class Z>
vector<Z> convolution(vector<Z> a, vector<Z> b) {
    if (a.empty() || b.empty()) {
        return {};
    }
    int sz = (int)a.size() + b.size() - 1;
    int n = 1;
    while (n < sz) {
        n *= 2;
    }
    
    a.resize(n), b.resize(n);
    ntt(a), ntt(b);
    for (int i = 0; i < n; i++) {
        a[i] *= b[i];
    }
    ntt(a, 1);
    
    a.resize(sz);
    return a;
}
```

### 三模NTT
```cpp
// NTT...
ll mul(ll a, ll b, ll p) {
    return (i128)a * b % p;
}
ll qkp(ll a, ll b, ll p) {
    ll ret = 1 % p;
    while (b > 0) {
        if (b & 1) {
            ret = mul(ret, a, p);
        }
        b >>= 1;
        a = mul(a, a, p);
    }
    return ret;
}
ll inv(ll x, ll p) {
    return qkp(x, p - 2, p);
}
vector<ll> mtt(const vector<ll>& a, const vector<ll>& b, ll mod) {
    if (a.empty() || b.empty()) {
        return {};
    }
    constexpr int M1 = 998244353;
    constexpr int M2 = 1004535809;
    constexpr int M3 = 469762049;
    using Z1 = ModInt<M1>;
    using Z2 = ModInt<M2>;
    using Z3 = ModInt<M3>;
    vector<Z1> a1(a.begin(), a.end()), b1(b.begin(), b.end());
    vector<Z2> a2(a.begin(), a.end()), b2(b.begin(), b.end());
    vector<Z3> a3(a.begin(), a.end()), b3(b.begin(), b.end());
    
    auto c1 = convolution(a1, b1);
    auto c2 = convolution(a2, b2);
    auto c3 = convolution(a3, b3);

    ll inv12 = inv(M1, M2);
    ll modm3 = mul(M1, M2, M3);
    ll inv123 = inv(modm3, M3);
    
    int sz = c1.size();
    vector<ll> ans(sz);
    for (int i = 0; i < sz; i++) {
        ll v1 = c1[i].val, v2 = c2[i].val, v3 = c3[i].val;
        
        ll k1 = (v2 - v1 % M2 + M2) % M2 * inv12 % M2;
        ll x12 = v1 + k1 * M1;

        ll k2 = (v3 - x12 % M3 + M3) % M3 * inv123 % M3;
        
        ll m12 = mul(M1, M2, mod);
        ll val = (x12 % mod + k2 % mod * m12 % mod) % mod;
        
        ans[i] = (val + mod) % mod;
    }
    return ans;
}
```

### MTT
```cpp
vector<ll> mtt(vector<ll> a, vector<ll> b, ll mod) {
    if (a.empty() || b.empty()) {
        return {};
    }
    int sz = (int)a.size() + b.size() - 1;
    if ((ll)a.size() * b.size() <= 30000) {
        vector<ll> c(sz);
        for (int i = 0; i < (int)a.size(); i++) {
            a[i] %= mod;
            if (a[i] < 0) {
                a[i] += mod;
            }
        }
        for (int i = 0; i < (int)b.size(); i++) {
            b[i] %= mod;
            if (b[i] < 0) {
                b[i] += mod;
            }
        }
        for (int i = 0; i < (int)a.size(); i++) {
            for (int j = 0; j < (int)b.size(); j++) {
                c[i + j] = (c[i + j] + (i128)a[i] * b[j]) % mod;
            }
        }
        return c;
    }
    int n = 1;
    while (n < sz) {
        n <<= 1;
    }
    ll M = sqrt((long double)mod) + 1;
    vector<cd> a0(n), a1(n), b0(n), b1(n);
    for (int i = 0; i < (int)a.size(); i++) {
        a[i] %= mod;
        if (a[i] < 0) {
            a[i] += mod;
        }
        a0[i] = a[i] / M;
        a1[i] = a[i] % M;
    }
    for (int i = 0; i < (int)b.size(); i++) {
        b[i] %= mod;
        if (b[i] < 0) {
            b[i] += mod;
        }
        b0[i] = b[i] / M;
        b1[i] = b[i] % M;
    }
    auto fft2 = [&](vector<cd>& x, vector<cd>& y) {
        for (int i = 0; i < n; i++) {
            x[i] += I * y[i];
        }
        fft(x);
        for (int i = 0; i < n; i++) {
            y[i] = conj(x[i ? n - i : 0]);
        }
        for (int i = 0; i < n; i++) {
            cd p = x[i];
            cd q = y[i];
            x[i] = (p + q) * 0.5;
            y[i] = (q - p) * 0.5 * I;
        }
    };
    fft2(a0, a1);
    fft2(b0, b1);
    vector<cd> p(n), q(n);
    for (int i = 0; i < n; i++) {
        p[i] = a0[i] * b0[i] + I * a1[i] * b0[i];
        q[i] = a0[i] * b1[i] + I * a1[i] * b1[i];
    }
    fft(p, 1);
    fft(q, 1);
    auto num = [&](double x) -> ll {
        ll v = x < 0 ? (ll)(x - 0.5) : (ll)(x + 0.5);
        v %= mod;
        if (v < 0) {
            v += mod;
        }
        return v;
    };
    vector<ll> c(sz);
    ll M1 = M % mod;
    ll M2 = (i128)M1 * M1 % mod;
    for (int i = 0; i < sz; i++) {
        ll c00 = num(p[i].real());
        ll c10 = num(p[i].imag());
        ll c01 = num(q[i].real());
        ll c11 = num(q[i].imag());
        c[i] = (
            (i128)M2 * c00
            + (i128)M1 * ((c10 + c01) % mod)
            + c11
        ) % mod;
    }

    return c;
}
```

### FWT/FMT
```cpp
template <typename Z>
void fwt_or(vector<Z>& a, bool invert = 0) {
    int n = a.size();
    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += len << 1) {
            for (int j = 0; j < len; j++) {
                if (!invert) {
                    a[i + j + len] += a[i + j];
                } else {
                    a[i + j + len] -= a[i + j];
                }
            }
        }
    }
}
template <typename Z>
void fwt_and(vector<Z>& a, bool invert = 0) {
    int n = a.size();
    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += len << 1) {
            for (int j = 0; j < len; j++) {
                if (!invert) {
                    a[i + j] += a[i + j + len];
                } else {
                    a[i + j] -= a[i + j + len];
                }
            }
        }
    }
}

template <typename Z>
void fwt_xor(vector<Z>& a, bool invert = 0) {
    int n = a.size();
    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += len << 1) {
            for (int j = 0; j < len; j++) {
                Z x = a[i + j];
                Z y = a[i + j + len];
                a[i + j] = x + y;
                a[i + j + len] = x - y;
            }
        }
    }

    if (invert) {
        Z invn = Z(n).inv();
        for (int i = 0; i < n; i++) {
            a[i] *= invn;
        }
    }
}
template <typename Z>
vector<Z> convolution_or(vector<Z> a, vector<Z> b) {
    int n = a.size();
    assert((int)b.size() == n);

    fwt_or(a);
    fwt_or(b);

    for (int i = 0; i < n; i++) {
        a[i] *= b[i];
    }

    fwt_or(a, 1);
    return a;
}

template <typename Z>
vector<Z> convolution_and(vector<Z> a, vector<Z> b) {
    int n = a.size();
    assert((int)b.size() == n);

    fwt_and(a);
    fwt_and(b);

    for (int i = 0; i < n; i++) {
        a[i] *= b[i];
    }

    fwt_and(a, 1);
    return a;
}

template <typename Z>
vector<Z> convolution_xor(vector<Z> a, vector<Z> b) {
    int n = a.size();
    assert((int)b.size() == n);

    fwt_xor(a);
    fwt_xor(b);

    for (int i = 0; i < n; i++) {
        a[i] *= b[i];
    }

    fwt_xor(a, 1);
    return a;
}
```

### 求导积分
```cpp
template <typename Z>
vector<Z> deriv(const vector<Z>& a) {
    int n = a.size();
    vector<Z> ret(max(0, n - 1));
    for (int i = 0; i < n - 1; i++) {
        ret[i] = a[i + 1] * Z(i + 1);
    }
    return ret;
}
template <typename Z>
vector<Z> intgr(const vector<Z>& a) {
    int n = a.size();
    vector<Z> ret(n + 1);
    for (int i = 1; i <= n; i++) {
        ret[i] = a[i - 1] / Z(i);
    }
    return ret;
}
```

### 求逆
```cpp
// 卷积...
template<typename Z>
vector<Z> inv(vector<Z> a, int n) {
    // find B that AB === 1 (mod x^n)
    int m = 1;
    vector<Z> b(1);
    b[0] = a[0].inv();
    while (m < n) {
        // B = 2B' - A(B'^2)
        int len = min(m * 2, n);
        vector<Z> A(min(len, (int)a.size()));
        for (int i = 0; i < (int)A.size(); i++) {
            A[i] = a[i];
        }
        vector<Z> B(b);
        B.resize(len);
        auto C = convolution(A, convolution(B, B));
        C.resize(len);
        b.resize(len);
        for (int i = 0; i < len; i++) {
            b[i] = B[i] * 2 - C[i];
        }
        m *= 2;
    }
    b.resize(n);
    return b;
}
```

### 除法
```cpp
// 卷积, 求逆...
template <typename Z>
pair<vector<Z>, vector<Z>> divmod(vector<Z> a, vector<Z> b) {
    int n = a.size();
    int m = b.size();
    if (n < m) {
        return { vector<Z>{ Z(0) }, a };
    }
    int len = n - m + 1;
    vector<Z> ra = a;
    vector<Z> rb = b;
    reverse(ra.begin(), ra.end());
    reverse(rb.begin(), rb.end());
    ra.resize(len);
    rb.resize(len);
    auto q = convolution(ra, inv(rb, len));
    q.resize(len);
    reverse(q.begin(), q.end());
    auto c = convolution(q, b);
    for (int i = 0; i < min((int)a.size(), (int)c.size()); i++) {
        a[i] -= c[i];
    }
    a.resize(m - 1);
    return { q, a };
}
```

### 开根
```cpp
// 卷积, 求逆...
template <typename Z>
vector<Z> sqrt(const vector<Z>& a, int n) {
    // assert(a[0] == 1);
    int m = 1;
    vector<Z> b(1);
    b[0] = 1;

    Z inv2 = Z(2).inv();
    while (m < n) {
        // B = (B' + A / B') / 2
        int len = min(m * 2, n);
        vector<Z> A(len);
        for (int i = 0; i < min((int)a.size(), len); i++) {
            A[i] = a[i];
        }
        b.resize(len);
        vector<Z> B(b);
        b = convolution(A, inv(B, len));
        b.resize(len);
        for (int i = 0; i < len; i++) {
            b[i] += B[i];
        }
        for (int i = 0; i < len; i++) {
            b[i] *= inv2;
        }
        m *= 2;
    }
    b.resize(n);
    return b;
}
```

### 指数/对数
```cpp
// 卷积, 求导, 积分...
template <typename Z>
vector<Z> ln(const vector<Z>& a, int n) {
    // B = ln(A) = intgr(deriv(A) * inv(A))
    // assert(a[0] == 1);
    vector<Z> A(n);
    for (int i = 0; i < n; i++) {
        if (i < a.size()) {
            A[i] = a[i];
        }
    }
    auto B = intgr(convolution(deriv(A), inv(A, n)));
    B.resize(n);
    return B;
}

template <typename Z>
vector<Z> exp(vector<Z> a, int n) {
    // assert(a[0] == 0);
    int m = 1;
    vector<Z> b(1);
    b[0] = 1;
    while (m < n) {
        // B = B' * (1 - ln(B') + A)
        int len = min(m * 2, n);
        vector<Z> A(len);
        for (int i = 0; i < min((int)a.size(), len); i++) {
            A[i] = a[i];
        }
        vector<Z> B(b);
        B.resize(len);
        auto C = ln(B, len);
        for (int i = 0; i < len; i++) {
            C[i] = A[i] - C[i];
        }
        C[0] += 1;
        b = convolution(B, C);
        b.resize(len);
        m *= 2;
    }
    b.resize(n);
    return b;
}
```

### 幂
```cpp
template <typename Z>
vector<Z> pow(vector<Z> a, ll b, int n) {
    if (n == 0) return {};
    if (b == 0) {
        vector<Z> ret(n);
        ret[0] = 1;
        return ret;
    }
    a.resize(n);
    int t = n;
    for (int i = 0; i < n; i++) {
        if (a[i] != Z(0)) {
            t = i;
            break;
        }
    }
    vector<Z> ans(n);
    if (t == n) {
        return ans;
    }
    if ((__int128)t * b >= n) {
        return ans;
    }
    int s = t * b;
    int m = n - s;
    vector<Z> c(m);
    for (int i = 0; i < m && i + t < n; i++) {
        c[i] = a[i + t] / a[t];
    }
    c = ln(c, m);
    for (int i = 0; i < m; i++) {
        c[i] *= Z(b);
    }
    c = exp(c, m);
    Z l = a[t].pow(b);
    for (int i = 0; i < m; i++) {
        ans[i + s] = c[i] * l;
    }
    return ans;
}
```

### 多点求值
```cpp
// 卷积, 求逆...
template <typename Z>
vector<Z> mulT(vector<Z> a, vector<Z> b, int need) {
    vector<Z> ret(need);
    if (a.empty() || b.empty() || need == 0) {
        return ret;
    }
    int n = b.size();
    reverse(b.begin(), b.end());
    auto c = convolution(a, b);
    for (int i = 0; i < need; i++) {
        int p = i + n - 1;
        if (p < (int)c.size()) {
            ret[i] = c[p];
        }
    }
    return ret;
}

template <typename Z>
vector<Z> multipoint_eval(vector<Z> f, vector<Z> x) {
    int m = x.size();
    vector<Z> ans(m);
    if (m == 0) {
        return ans;
    }
    if (f.empty()) {
        return ans;
    }
    int n = max((int)f.size(), m);
    x.resize(n);
    vector<vector<Z>> q(4 * n);
    auto build = [&](auto self, int p, int l, int r) -> void {
        if (r - l == 1) {
            q[p] = {Z(1), -x[l]};
            return;
        }
        int mid = (l + r) / 2;
        self(self, p << 1, l, mid);
        self(self, p << 1 | 1, mid, r);
        q[p] = convolution(q[p << 1], q[p << 1 | 1]);
    };
    auto work = [&](auto self, int p, int l, int r, const vector<Z>& num) -> void {
        if (r - l == 1) {
            if (l < m) {
                ans[l] = num.empty() ? Z(0) : num[0];
            }
            return;
        }
        int mid = (l + r) / 2;
        auto vl = mulT(num, q[p << 1 | 1], mid - l);
        auto vr = mulT(num, q[p << 1], r - mid);
        self(self, p << 1, l, mid, vl);
        self(self, p << 1 | 1, mid, r, vr);
    };
    build(build, 1, 0, n);
    auto root = mulT(f, inv(q[1], n), n);
    work(work, 1, 0, n, root);
    return ans;
}
```

### 快速插值
```cpp
template <typename Z>
vector<Z> interpolate(vector<Z> x, vector<Z> y) {
    int n = x.size();
    assert((int)y.size() == n);
    if (n == 0) {
        return {};
    }
    vector<vector<Z>> q(4 * n);
    auto build = [&](auto self, int p, int l, int r) -> void {
        if (r - l == 1) {
            // x - x_l
            q[p] = {-x[l], Z(1)};
            return;
        }

        int mid = (l + r) / 2;

        self(self, p << 1, l, mid);
        self(self, p << 1 | 1, mid, r);

        q[p] = convolution(q[p << 1], q[p << 1 | 1]);
    };

    build(build, 1, 0, n);
    auto d = multipoint_eval(deriv(q[1]), x);
    vector<Z> w(n);
    for (int i = 0; i < n; i++) {
        w[i] = y[i] / d[i];
    }

    auto work = [&](auto self, int p, int l, int r) -> vector<Z> {
        if (r - l == 1) {
            return vector<Z>{w[l]};
        }

        int mid = (l + r) / 2;

        auto L = self(self, p << 1, l, mid);
        auto R = self(self, p << 1 | 1, mid, r);

        auto A = convolution(L, q[p << 1 | 1]);
        auto B = convolution(R, q[p << 1]);

        int len = max(A.size(), B.size());
        A.resize(len);
        B.resize(len);

        for (int i = 0; i < len; i++) {
            A[i] += B[i];
        }

        return A;
    };
    auto ans = work(work, 1, 0, n);
    ans.resize(n);
    return ans;
}
```

### 快速阶乘
```cpp
struct FastFactorial {
    using ld = long double;
    static constexpr int MAXD = 18;
    static constexpr int LIM = (1 << MAXD) + 5;
    static constexpr int BASE = 1 << 16;
    static constexpr int SF = 16;
    static constexpr int MASK = BASE - 1;
    struct Complex {
        ld r, i;
        friend Complex operator+(Complex a, Complex b) {
            return {a.r + b.r, a.i + b.i};
        }
        friend Complex operator-(Complex a, Complex b) {
            return {a.r - b.r, a.i - b.i};
        }
        friend Complex operator*(Complex a, Complex b) {
            return {a.r * b.r - a.i * b.i, a.r * b.i + a.i * b.r};
        }
        Complex& operator/=(int x) {
            r /= x;
            i /= x;
            return *this;
        }
    };
    ll mod, base2;
    vector<vector<int>> rev;
    vector<Complex> rt[2][MAXD + 1];
    vector<Complex> tr, tr1, tr2, tr3, tr4, tr5, tr6;
    vector<ll> m13, m14, m23, m24;
    vector<ll> f, g, h, ifac;
    vector<ll> val, tmp1, tmp2;
    FastFactorial() {
        rev.resize(MAXD + 1);
        tr.resize(LIM);
        tr1.resize(LIM);
        tr2.resize(LIM);
        tr3.resize(LIM);
        tr4.resize(LIM);
        tr5.resize(LIM);
        tr6.resize(LIM);
        m13.resize(LIM);
        m14.resize(LIM);
        m23.resize(LIM);
        m24.resize(LIM);
        f.resize(LIM);
        g.resize(LIM);
        h.resize(LIM);
        ifac.resize(LIM);
        val.resize(LIM);
        tmp1.resize(LIM);
        tmp2.resize(LIM);
        pre_fft();
    }
    ll norm(ll x) {
        x %= mod;
        if (x < 0) x += mod;
        return x;
    }
    ll qpow(ll a, ll b) {
        ll ret = 1 % mod;
        a = norm(a);
        while (b > 0) {
            if (b & 1) ret = (i128)ret * a % mod;
            a = (i128)a * a % mod;
            b >>= 1;
        }
        return ret;
    }
    void pre_fft() {
        const ld PI = acosl(-1.0L);
        for (int d = 1; d <= MAXD; d++) {
            int len = 1 << d;
            rev[d].assign(len, 0);
            for (int i = 1; i < len; i++) {
                rev[d][i] = (rev[d][i >> 1] >> 1) | ((i & 1) << (d - 1));
            }
        }
        for (int d = 1; d <= MAXD; d++) {
            int m = 1 << (d - 1);
            rt[0][d].resize(m);
            rt[1][d].resize(m);
            for (int i = 0; i < m; i++) {
                ld ang = PI * i / m;
                rt[0][d][i] = {cosl(ang), sinl(ang)};
                rt[1][d][i] = {cosl(ang), -sinl(ang)};
            }
        }
    }
    void fft(Complex* a, int len, int d, int inv) {
        for (int i = 1; i < len; i++) {
            if (i < rev[d][i]) {
                swap(a[i], a[rev[d][i]]);
            }
        }
        for (int k = 1, dep = 1; k < len; k <<= 1, dep++) {
            for (int s = 0; s < len; s += k << 1) {
                auto* w = rt[inv][dep].data();
                for (int i = s; i < s + k; i++, w++) {
                    Complex x = a[i];
                    Complex y = a[i + k] * (*w);
                    a[i] = x + y;
                    a[i + k] = x - y;
                }
            }
        }
        if (inv) {
            for (int i = 0; i < len; i++) {
                a[i] /= len;
            }
        }
    }
    void dbdft(ll* a, int len, int d, Complex* op1, Complex* op2) {
        for (int i = 0; i < len; i++) {
            tr[i] = {(ld)(a[i] >> SF), (ld)(a[i] & MASK)};
        }
        fft(tr.data(), len, d, 0);
        tr[len] = tr[0];
        for (int i = 0; i < len; i++) {
            Complex p = tr[i];
            Complex q = tr[len - i];

            op1[i] = Complex{p.r + q.r, p.i - q.i} * Complex{0.5, 0};
            op2[i] = Complex{p.r - q.r, p.i + q.i} * Complex{0, -0.5};
        }
    }
    ll round_mod(ld x) {
        ll v = x < 0 ? (ll)(x - 0.5) : (ll)(x + 0.5);
        v %= mod;
        if (v < 0) v += mod;
        return v;
    }
    void dbidft(Complex* a, int len, int d, ll* op1, ll* op2) {
        fft(a, len, d, 1);
        for (int i = 0; i < len; i++) {
            op1[i] = round_mod(a[i].r);
            op2[i] = round_mod(a[i].i);
        }
    }
    void poly_mul(ll* a, ll* b, ll* c, int len, int d) {
        dbdft(a, len, d, tr1.data(), tr2.data());
        dbdft(b, len, d, tr3.data(), tr4.data());
        for (int i = 0; i < len; i++) {
            tr5[i] = tr1[i] * tr3[i] + Complex{0, 1} * (tr2[i] * tr4[i]);
            tr6[i] = tr2[i] * tr3[i] + Complex{0, 1} * (tr1[i] * tr4[i]);
        }
        dbidft(tr5.data(), len, d, m13.data(), m24.data());
        dbidft(tr6.data(), len, d, m23.data(), m14.data());
        for (int i = 0; i < len; i++) {
            c[i] = (i128)m13[i] * base2 % mod;
            c[i] = (c[i] + (i128)(m23[i] + m14[i]) % mod * BASE + m24[i]) % mod;
        }
    }
    void init_ifac(int lim) {
        lim = min(lim, LIM - 1);
        ifac[0] = 1;
        if (lim >= 1) ifac[1] = 1;
        for (int i = 2; i <= lim; i++) {
            ifac[i] = (mod - mod / i) * ifac[mod % i] % mod;
        }
        for (int i = 1; i <= lim; i++) {
            ifac[i] = ifac[i] * ifac[i - 1] % mod;
        }
    }
    void shift_eval(ll del, int cur, ll* ip, ll* op) {
        int len = 1;
        int d = 0;
        while (len <= cur + cur + cur) {
            len <<= 1;
            d++;
        }
        for (int i = 0; i <= cur; i++) {
            f[i] = ip[i] * ifac[i] % mod * ifac[cur - i] % mod;
        }
        for (int i = cur - 1; i >= 0; i -= 2) {
            if (f[i]) f[i] = mod - f[i];
        }
        int total = cur + cur + 1;
        ll prod = 1;
        for (int i = 0; i < total; i++) {
            g[i] = norm(del - cur + i);
            prod = (i <= cur ? (i128)prod * g[i] % mod : prod);
        }
        h[0] = 1;
        for (int i = 0; i < total; i++) {
            h[i + 1] = (i128)h[i] * g[i] % mod;
        }
        ll inv_all = qpow(h[total], mod - 2);
        for (int i = total - 1; i >= 0; i--) {
            ll x = g[i];
            g[i] = (i128)inv_all * h[i] % mod;
            inv_all = (i128)inv_all * x % mod;
        }
        for (int i = cur + 1; i < len; i++) {
            f[i] = 0;
        }
        for (int i = total; i < len; i++) {
            g[i] = 0;
        }
        poly_mul(f.data(), g.data(), h.data(), len, d);
        ll cur_prod = prod;
        for (int i = 0; i <= cur; i++) {
            op[i] = h[i + cur] * cur_prod % mod;
            cur_prod = (i128)cur_prod * g[i] % mod;
            cur_prod = (i128)cur_prod * norm(del + i + 1) % mod;
        }
    }
    void build_block_values(int B) {
        int hb = 0;
        for (int x = B; x; x >>= 1) {
            hb++;
        }
        val[0] = 1;
        int cur = 0;
        ll invB = qpow(B, mod - 2);
        for (int z = hb; z >= 0; z--) {
            if (cur != 0) {
                shift_eval(cur + 1, cur, val.data(), tmp1.data());
                for (int i = 0; i <= cur; i++) {
                    val[cur + i + 1] = tmp1[i];
                }
                val[cur * 2 + 1] = 0;
                shift_eval((ll)cur * invB % mod, cur << 1, val.data(), tmp2.data());
                cur <<= 1;
                for (int i = 0; i <= cur; i++) {
                    val[i] = val[i] * tmp2[i] % mod;
                }
            }
            if ((B >> z) & 1) {
                for (int i = 0; i <= cur; i++) {
                    val[i] = val[i] * ((ll)B * i + cur + 1) % mod;
                }
                cur |= 1;
                val[cur] = 1;
                for (int i = 1; i <= cur; i++) {
                    val[cur] = val[cur] * ((ll)cur * B + i) % mod;
                }
            }
        }
    }
    ll brute_fact(ll n) {
        ll ans = 1 % mod;
        for (ll i = 1; i <= n; i++) {
            ans = (i128)ans * i % mod;
        }
        return ans;
    }
    ll fact_raw(ll n) {
        if (n == 0) return 1 % mod;
        // 这个阈值可以调整, 不要太小
        if (n <= 500000) {
            return brute_fact(n);
        }
        int B = sqrt((long double)n);
        while ((ll)B * B < n) B++;
        while (B > 1 && (ll)(B - 1) * (B - 1) >= n) B--;
        init_ifac(2 * B + 10);
        build_block_values(B);
        ll ans = 1 % mod;
        ll i = 0;
        int id = 0;
        while (i + B <= n) {
            ans = ans * val[id] % mod;
            i += B;
            id++;
        }
        for (ll j = i + 1; j <= n; j++) {
            ans = (i128)ans * j % mod;
        }
        return ans;
    }

    ll operator()(ll n, ll p) {
        mod = p;
        base2 = (ll)BASE * BASE % mod;
        if (n >= mod) return 0;
        if (n > mod - 1 - n) {
            ll r = mod - 1 - n;
            ll ans = fact_raw(r);
            ans = qpow(ans, mod - 2);
            if ((r + 1) & 1) {
                ans = (mod - ans) % mod;
            }
            return ans;
        }
        return fact_raw(n);
    }
};
FastFactorial f;
```

### 多点快速阶乘
```cpp
template <typename Z>
vector<Z> batch_inv(const vector<Z>& a) {
    int n = a.size();
    vector<Z> pre(n + 1), suf(n + 1), ret(n);
    pre[0] = 1;
    for (int i = 0; i < n; i++) {
        pre[i + 1] = pre[i] * a[i];
    }
    Z inv_all = pre[n].inv();
    for (int i = n - 1; i >= 0; i--) {
        ret[i] = inv_all * pre[i];
        inv_all *= a[i];
    }
    return ret;
}

template <typename Z>
vector<Z> many_factorials(const vector<int>& q) {
    constexpr int LIM = 100000;
    int n = q.size();
    int mod = Z::mod();
    vector<Z> ans(n, Z(1));
    vector<int> need_inv(n);
    vector<pair<int, int>> reg;
    vector<pair<int, int>> odd;
    reg.reserve(n);
    odd.reserve(n * 8);
    for (int id = 0; id < n; id++) {
        int t = q[id];
        if (t >= mod) {
            ans[id] = 0;
            continue;
        }
        if (t >= mod / 2) {
            int r = mod - 1 - t;
            if ((r & 1) == 0) {
                ans[id] = -ans[id];
            }
            need_inv[id] = 1;
            t = r;
        }
        long long pow2 = 0;
        while (t > LIM) {
            int x = (t - 1) / 2;
            odd.emplace_back(x, id);
            pow2 += t / 2;
            t >>= 1;
        }
        ans[id] *= Z(2).pow(pow2 % (mod - 1));
        reg.emplace_back(t, id);
    }
    sort(reg.begin(), reg.end());
    int cur = 0;
    Z prod = 1;
    for (auto [t, id] : reg) {
        while (cur < t) {
            cur++;
            prod *= cur;
        }
        ans[id] *= prod;
    }
    sort(odd.begin(), odd.end());
    int ocur = -1;
    Z oprod = 1;
    for (auto [x, id] : odd) {
        while (ocur < x) {
            ocur++;
            oprod *= Z(2LL * ocur + 1);
        }
        ans[id] *= oprod;
    }
    vector<Z> need;
    vector<int> pos;
    for (int i = 0; i < n; i++) {
        if (need_inv[i]) {
            pos.emplace_back(i);
            need.emplace_back(ans[i]);
        }
    }
    auto invs = batch_inv(need);
    for (int i = 0; i < (int)pos.size(); i++) {
        ans[pos[i]] = invs[i];
    }
    return ans;
}
```

## 线性代数
### 矩阵和列向量
```cpp
template <class S, int N>
struct ColVector {
    array<S, N> a;
    ColVector(S def = S()) {
        fill(a.begin(), a.end(), def);
    }
    
    S& operator[](int i) {
        return a[i];
    }
    const S& operator[](int i) const {
        return a[i];
    }
};

template <typename S, int N>
struct Matrix {
    array<array<S, N>, N> a;
    Matrix(S def = S()) {
        for (int i = 0; i < N; i++) {
            fill(a[i].begin(), a[i].end(), def);
        }
    }
    using M = Matrix;

    S* operator[](int i) {
        return a[i].data();
    }
    const S* operator[](int i) const {
        return a[i].data();
    }
    
    // 单位矩阵
    constexpr static M e() {
        M ret;
        for (int i = 0; i < N; i++) {
            ret[i][i] = 1;
        }
        return ret;
    }
    
    // 加法
    friend M operator+(M lt, const M& rt) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                lt[i][j] += rt[i][j];
            }
        }
        return lt;
    }

    // 乘法
    friend M operator*(const M& lt, const M& rt) {
        M ret;
        for (int i = 0; i < N; i++) {
            for (int k = 0; k < N; k++) {
                if (lt[i][k] == S()) {
                    continue;
                }
                
                for (int j = 0; j < N; j++) {
                    ret[i][j] += lt[i][k] * rt[k][j];
                }
            }
        }
        return ret;
    }

    M pow(ll b) const {
        M ret = e();
        M t = *this;
        while (b > 0) {
            if (b & 1) {
                ret = ret * t;
            }
            
            t = t * t;
            b >>= 1;
        }
        return ret;
    }

    ColVector<S, N> operator*(const ColVector<S, N>& vec) {
        ColVector<S, N> ret;
        for (int i = 0; i < N; i++) {
            for (int k = 0; k < N; k++) {
                if (a[i][k] == S()) {
                    continue;
                }
                
                ret[i] = ret[i] + (a[i][k] * vec[k]);
            }
        }
        return ret;
    }
};

template <int N>
struct Matrix_mp {
    array<array<ll, N>, N> a;
    Matrix_mp() {
        for (int i = 0; i < N; i++) {
            fill(a[i].begin(), a[i].end(), INFLL);
        }
    }

    ll* operator[](int i) {
        return a[i].data();
    }
    const ll* operator[](int i) const {
        return a[i].data();
    }
    
    static Matrix_mp e() {
        Matrix_mp ret;
        for (int i = 0; i < N; i++) {
            ret[i][i] = 0;
        }
        return ret;
    }
    
    friend Matrix_mp operator+(Matrix_mp lt, const Matrix_mp& rt) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                lt[i][j] = min(lt[i][j], rt[i][j]);
            }
        }
        return lt;
    }
    friend Matrix_mp operator*(const Matrix_mp& lt, const Matrix_mp& rt) {
        Matrix_mp ret;
        for (int i = 0; i < N; i++) {
            for (int k = 0; k < N; k++) {
                if (lt[i][k] >= INFLL) {
                    continue;
                }
                
                for (int j = 0; j < N; j++) {
                    ll now;
                    if (rt[k][j] >= INFLL) {
                        now = INFLL;
                    } else {
                        now = lt[i][k] + rt[k][j];
                    }
                    ret[i][j] = min(ret[i][j], now);
                }
            }
        }
        return ret;
    }
    
    Matrix_mp pow(ll b) const {
        Matrix_mp ret = e();
        Matrix_mp t = *this;
        while (b > 0) {
            if (b & 1) {
                ret = ret * t;
            }
            
            t = t * t;
            b >>= 1;
        }
        return ret;
    }
    
    ColVector<ll, N> operator*(const ColVector<ll, N>& vec) {
        ColVector<ll, N> ret(INFLL);
        for (int i = 0; i < N; i++) {
            for (int k = 0; k < N; k++) {
                if (a[i][k] >= INFLL) {
                    continue;
                }
                ll now;
                if (a[i][k] >= INFLL || vec[k] >= INFLL) {
                    now = INFLL;
                } else {
                    now = a[i][k] + vec[k];
                }
                ret[i] = min(ret[i], now);
            }
        }
        return ret;
    }
};
```

### 高斯消元(整数)
```cpp
template <typename S, int N>
struct GaussJordan {
    Matrix<S, N> A;
    ColVector<S, N> x;
    ColVector<S, N> b;
    
    int solve(int n, int m) {
        int r = 0;
        for (int c = 0; r < n && c < m; c++) {
            int pivot = r;
            while (pivot < n && A[pivot][c] == S()) {
                pivot++;
            }
            if (pivot == n) {
                continue;
            }
            if (pivot != r) {
                swap(A.a[pivot], A.a[r]);
                swap(b.a[pivot], b.a[r]);
            }
            S inv = S(1) / A[r][c];
            for (int j = c; j < m; j++) {
                A[r][j] = A[r][j] * inv;
            }
            b[r] = b[r] * inv;
            
            for (int i = 0; i < n; i++) {
                if (i != r && A[i][c] != S()) {
                    S factor = A[i][c];
                    for (int j = c; j < m; j++) {
                        A[i][j] = A[i][j] - factor * A[r][j];
                    }
                    b[i] = b[i] - factor * b[r];
                }
            }
            r++;
        }
        
        for (int i = r; i < n; i++) {
            if (b[i] != S()) {
                return -1;
            }
        }
        
        for (int i = 0; i < n; i++) {
            x[i] = S();
        }
        
        for (int i = 0; i < r; i++) {
            int maj = -1;
            for (int j = 0; j < m; j++) {
                if (A[i][j] != S()) {
                    maj = j;
                    break;
                }
            }
            if (maj != -1) {
                x[maj] = b[i];
            }
        }
        if (r < n) {
            return n - r;
        } else {
            return 0;
        }
    }
};
```

### 高斯消元(实数)
```cpp
// for real
template <typename S, int N>
struct GaussJordan {
    Matrix<S, N> A;
    ColVector<S, N> x;
    ColVector<S, N> b;
    
    int solve(int n, int m) {
        int r = 0;
        for (int c = 0; r < n && c < m; c++) {
            int pivot = r;
            for (int i = r + 1; i < n; i++) {
                if (fabs(A[i][c]) > fabs(A[pivot][c])) {
                    pivot = i;
                }
            }
            if (fabs(A[pivot][c] < EPS)) {
                continue;
            }
            
            if (pivot != r) {
                swap(A.a[pivot], A.a[r]);
                swap(b.a[pivot], b.a[r]);
            }
            S inv = S(1) / A[r][c];
            for (int j = c; j < m; j++) {
                A[r][j] = A[r][j] * inv;
            }
            b[r] = b[r] * inv;
            
            for (int i = 0; i < n; i++) {
                if (i != r && A[i][c] != S()) {
                    S factor = A[i][c];
                    for (int j = c; j < m; j++) {
                        A[i][j] = A[i][j] - factor * A[r][j];
                    }
                    b[i] = b[i] - factor * b[r];
                }
            }
            r++;
        }
        
        for (int i = r; i < n; i++) {
            if (fabs(b[i]) > EPS) {
                return -1;
            }
        }
        
        for (int i = 0; i < n; i++) {
            x[i] = S();
        }
        
        for (int i = 0; i < r; i++) {
            int maj = -1;
            for (int j = 0; j < m; j++) {
                if (fabs(A[i][j]) > EPS) {
                    maj = j;
                    break;
                }
            }
            if (maj != -1) {
                x[maj] = b[i];
            }
        }
        if (r < n) {
            return n - r;
        } else {
            return 0;
        }
    }
};
```

### 高斯消元(异或)
```cpp
template <int DIM>
struct GaussXor {
    array<bitset<DIM>, DIM> mat; // 存增广矩阵, 行从 0 到 n - 1, 列从 0 到 m

    array<int, DIM> ans;
    // 解异或线性方程组, 无解返回-1, 唯一解返回0, 无穷解返回自由变元数
    int solve(int n, int m) {
        // n 个方程, m 个未知量(即系数矩阵为 n 行 m 列)
        int r = 0, c = 0;
        for (; r < n && c < m; c++) {
            int pivot = r;
            while (pivot < n && !mat[pivot][c]) {
                pivot++;
            }
            if (pivot == n) {
                continue;
            }
            if (pivot != r) {
                swap(mat[pivot], mat[r]);
            }
            for (int i = 0; i < n; i++) {
                if (i != r && mat[i][c]) {
                    mat[i] ^= mat[r];
                }
            }
            r++;
        }

        for (int i = r; i < n; i++) {
            if (mat[i][m]) {
                return -1;
            }
        }
        if (r < m) {
            return m - r;
        }
        for (int i = 0; i < m; i++) {
            if (i < n) {
                ans[i] = mat[i][m];
            }
        }
        return 0;
    }
};
```

### 异或线性基
```cpp
template <int DIM = 64>
struct LinearBasis {
    array<ull, DIM> p = {};
    int cnt = 0;
    bool zero = 0;
    
    bool insert(ull x) {
        for (int i = DIM - 1; i >= 0; i--) {
            if ((x & (1ULL << i)) == 0) {
                continue;
            }
            if (!p[i]) {
                p[i] = x;
                cnt++;
                return true;
            }
            x ^= p[i];
        }
        zero = true;
        return false;
    }
    
    bool check(ull x) {
        for (int i = DIM - 1; i >= 0; i--) {
            if ((x & (1ULL << i)) == 0) {
                continue;
            }
            if (!p[i]) {
                return false;
            }
            x ^= p[i];
        }
        return true;
    }
    
    ull max() {
        ull ret = 0;
        for (int i = DIM - 1; i >= 0; i--) {
            if ((ret ^ p[i]) > ret) {
                ret ^= p[i];
            }
        }
        return ret;
    }
    
    ull min() {
        if (zero) {
            return 0;
        }
        for (int i = 0; i < DIM; i++) {
            if (p[i]) {
                return p[i];
            }
        }
    }
    
    // 合并
    friend bool operator+(const LinearBasis& a, const LinearBasis& b) {
        LinearBasis ret = a;
        for (int i = 0; i < DIM; i++) {
            if (b.p[i]) {
                ret.insert(b.p[i]);
            }
        }
        return ret;
    }
};

```

# 字符串
## 滚动哈希
```cpp
// 随机数生成器

// mod = (1ULL << 61) - 1, 便于rolling hash使用
struct modint61 {
    static constexpr u64 mod = (1ULL << 61) - 1;
    u64 val;

    constexpr modint61(ll x = 0) : val(((x %= static_cast<ll>(mod)) < 0) ? (x + static_cast<ll>(mod)) : x) {}

    modint61 &operator+=(const modint61 &a) {
        val = ((val += a.val) >= mod) ? (val - mod) : val;
        return *this;
    }
    modint61 &operator-=(const modint61 &a) {
        val = ((val -= a.val) >= mod) ? (val + mod) : val;
        return *this;
    }
    modint61 &operator*=(const modint61 &a) {
        const unsigned __int128 y = static_cast<unsigned __int128>(val) * a.val;
        val = (y >> 61) + (y & mod);
        val = (val >= mod) ? (val - mod) : val;
        return *this;
    }
    modint61 operator-() const { return modint61(val ? mod - val : u64(0)); }
    modint61 &operator/=(const modint61 &a) { return (*this *= a.inv()); }
    modint61 operator+(const modint61 &p) const { return modint61(*this) += p; }
    modint61 operator-(const modint61 &p) const { return modint61(*this) -= p; }
    modint61 operator*(const modint61 &p) const { return modint61(*this) *= p; }
    modint61 operator/(const modint61 &p) const { return modint61(*this) /= p; }
    bool operator==(const modint61 &p) const { return val == p.val; }
    bool operator!=(const modint61 &p) const { return val != p.val; }
    modint61 inv() const {
        ll a = val, b = mod, u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            a -= t * b;
            u -= t * v;
            swap(a, b);
            swap(u, v);
        }
        return modint61(u);
    }
    modint61 pow(ll n) const {
        assert(n >= 0);
        modint61 ret(1), mul(val);
        while (n > 0) {
            if (n & 1) ret *= mul;
            mul *= mul, n >>= 1;
        }
        return ret;
    }
};
struct StringHash {
    using m61 = modint61;
    static m61 base1, base2;
    static vector<m61> pow1, pow2;

    vector<m61> h1, h2;

    // 初始化base
    static void init() {
        if (base1.val != 0) return; 
        base1 = m61(RNG(1000, m61::mod - 1));
        base2 = m61(RNG(1000, m61::mod - 1));
        while (base2 == base1) base2 = m61(RNG(1000, modint61::mod - 1));
        pow1 = {1};
        pow2 = {1};
    }

    // 预处理幂次, 按需扩展
    static void expand(int n) {
        if (pow1.empty()) init();
        while ((int)pow1.size() <= n) {
            pow1.push_back(pow1.back() * base1);
            pow2.push_back(pow2.back() * base2);
        }
    }

    StringHash() {}

    // O(|s|)
    StringHash(const string& s) {
        int n = s.size();
        expand(n);
        h1.resize(n + 1);
        h2.resize(n + 1);
        for (int i = 0; i < n; i++) {
            h1[i + 1] = h1[i] * base1 + s[i];
            h2[i + 1] = h2[i] * base2 + s[i];
        }
    }

    // 查询子串 s[l...r-1] 的哈希值 (左闭右开)
    // 返回 pair<u64, u64>
    pair<u64, u64> query(int l, int r) const {
        // H[l...r-1] = H[r] - H[l] * B^(r-l)
        m61 res1 = h1[r] - h1[l] * pow1[r - l];
        m61 res2 = h2[r] - h2[l] * pow2[r - l];
        return {res1.val, res2.val};
    }
    
    // 合并两个 Hash 值 (对应 s1 + s2)
    // len_right: 右边子串的长度
    static pair<u64, u64> merge(pair<u64, u64> h_left, pair<u64, u64> h_right, int len_right) {
        expand(len_right);
        m61 res1 = m61(h_left.first) * pow1[len_right] + modint61(h_right.first);
        m61 res2 = m61(h_left.second) * pow2[len_right] + modint61(h_right.second);
        return {res1.val, res2.val};
    }
};

// 全局成员定义
inline modint61 StringHash::base1;
inline modint61 StringHash::base2;
inline vector<modint61> StringHash::pow1;
inline vector<modint61> StringHash::pow2;
```


# 杂
## 随机数生成
```cpp
// random number giver
inline u64 RNG_64() {
    static u64 x_ = u64(chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count()) * 10150724397891781847ULL;
    x_ ^= x_ << 7;
    return x_ ^= x_ >> 9;
}
// gives random number of [0, lim)
inline u64 RNG(u64 lim) { return RNG_64() % lim; }
// gives random number of [l, r)
inline ll RNG(ll l, ll r) { return l + RNG_64() % (r - l); }
```