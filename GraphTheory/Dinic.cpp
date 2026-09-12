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
