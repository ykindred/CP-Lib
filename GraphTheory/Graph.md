# 拓扑排序
```cpp
#include <bits/stdc++.h>
using namespace std;

vector<int> topo(int n, vector<vector<int>>& g, vector<int>& in)
{
    queue<int> q;
    vector<int> res;

    for(int i=1;i<=n;i++)
        if(!in[i]) q.push(i);

    while(q.size())
    {
        int u=q.front(); q.pop();
        res.push_back(u);

        for(int v:g[u])
            if(--in[v]==0) q.push(v);
    }

    return res.size()==n ? res : vector<int>();
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> g(n + 1);
    vector<int> in(n + 1);

    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        in[v]++;
    }

    vector<int> ans = topo(n, g, in);

    if (ans.empty())
        cout << "IMPOSSIBLE\n";
    else
    {
        for (int x : ans) cout << x << " ";
        cout << "\n";
    }
}
```
# 树
## 树的直径
```cpp
#include<bits/stdc++.h>
using namespace std;

void dfs(int u,int fa,int &c,vector<int>&d,const vector<vector<int>>&g)
{
    for(int v:g[u])
    {
        if(v==fa) continue;
        d[v]=d[u]+1;
        if(d[v]>d[c]) c=v;
        dfs(v,u,c,d,g);
    }
}

int main()
{
    int n;
    cin>>n;

    vector<vector<int>> g(n+1);
    vector<int> d(n+1,0);

    for(int i=1;i<n;i++)
    {
        int x,y;
        cin>>x>>y;
        g[x].push_back(y);
        g[y].push_back(x);
    }

    int c=1;

    dfs(1,0,c,d,g);

    d[c]=0;
    dfs(c,0,c,d,g);

    cout<<d[c]<<endl;
}
```
## 倍增LCA
```cpp
#include<bits/stdc++.h>
using namespace std;
int n,q;
vector<vector<int>>anc,g;
vector<int>d;
void dfs(int u,int fa)
{
    for(int v:g[u])
    {
        if(fa==v)continue;
        d[v]=d[u]+1;
        dfs(v,u);
    }
}
int lca(int a,int b)
{
    if(d[a]<d[b])swap(a,b);
    int dif=d[a]-d[b];
    for(int i=0;i<31;i++)
    {
        if(dif&(1<<i))a=anc[a][i];
    }
    if(a==b)return a;
    for(int i=30;i>=0;i--)
    {
         if(anc[a][i]!=anc[b][i])
        {
           a=anc[a][i];
           b=anc[b][i];
        }
    }
    return anc[a][0];
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin>>n>>q;
    anc.resize(n+1,vector(32,0));
    g.resize(n+1);
    d.resize(n+1,0);
    for(int i=2;i<=n;i++)
    {
        cin>>anc[i][0];
        g[i].push_back(anc[i][0]);
        g[anc[i][0]].push_back(i);
    }
    dfs(1,0);
    for(int i=1;i<31;i++)
    {
        for(int k=1;k<=n;k++)
        {
            if(!anc[k][i-1])anc[k][i]=0;
            else anc[k][i]=anc[anc[k][i-1]][i-1];
        }
    }
    while(q--)
    {
        int a,b;
        cin>>a>>b;
        cout<<lca(a,b)<<endl;
    }
}
```
# Tarjan
## SCC
```cpp
#include<bits/stdc++.h>
using namespace std;
vector<vector<int>>g,scc;
vector<int>dfn,low,sccid;
stack<int>s;
vector<bool>instk;
int n,m,Time=0,sccnum=0;
void tarjan(int u)
{
    dfn[u]=low[u]=++Time;
    s.push(u);
    instk[u]=1;
    for(int v:g[u])
    {
        if(!dfn[v])
        {
            tarjan(v);
            low[u]=min(low[u],low[v]);
        }
        else if(instk[v])
        low[u]=min(low[u],dfn[v]);
    }
    if(low[u]==dfn[u])
    {
        vector<int>num;
        sccnum++;
        while(!s.empty())
        {
            int x=s.top();
            s.pop();
            instk[x]=0;
            sccid[x]=sccnum;
            num.push_back(x);
            if(x==u)break;
        }
        scc.push_back(num);
    }

}
int main()
{
    cin>>n>>m;
    g.resize(n+1);
    dfn.resize(n+1,0);
    low.resize(n+1,0);
    instk.resize(n+1,0);
    sccid.resize(n+1, 0);
    for(int i=1;i<=m;i++)
    {
        int x,y;
        cin>>x>>y;
        g[x].push_back(y);
    }
    for(int i=1;i=<n;i++)
    {
        if(!dfn[i])tarjan(i);
    }
    reverse(scc.begin(), scc.end());
    cout<<sccnum<<'\n';
    for(auto&num:scc)
    {
        cout<<num.size()<<' ';
        for(int v:num)cout<<v<<' ';
        cout<<'\n';
    }
}
```
## 割边
```cpp
#include<bits/stdc++.h>
using namespace std;
int n,m;
vector<vector<pair<int,int>>>g;
vector<vector<int>>ans;
vector<int>dfn,low,instk,edg,vis;
int Time=0,ansnum=0;
stack<int>s;
void tarjan(int u,int fedg)
{
    dfn[u]=low[u]=++Time;
    
    for(auto [v,id]:g[u])
    {
        if(!dfn[v])
        {
            tarjan(v,id);
            low[u]=min(low[v],low[u]);
            if (low[v] > dfn[u])
            edg[id]=1;
        }
        else if(id!=fedg)
        low[u]=min(dfn[v],low[u]);
    }
}
void dfs(int u)
{
    vis[u]=1;
    
    ans[ansnum].push_back(u);
    for(auto [v,id]:g[u])
    {
        if(edg[id])continue;
        if(!vis[v])dfs(v);
    }
}
int main()
{
    cin>>n>>m;
    g.resize(n+1);
    dfn.resize(n+1,0);
    low.resize(n+1,0);
    instk.resize(n+1,0);
    edg.resize(m+1,0);
    ans.resize(n+1);
    vis.resize(n+1,0);
    for(int i=1;i<=m;i++)
    {
        int a,b;
        cin>>a>>b;
        g[a].push_back({b,i});
        g[b].push_back({a,i});
    }
    for(int i=1;i<=n;i++)
    if(!dfn[i])tarjan(i,-1);
    for(int i=1;i<=n;i++)
    {
        if(!vis[i])
        {
            ansnum++;
            dfs(i);
        }
    }
    cout<<ansnum<<'\n';
    for(int i=1;i<=ansnum;i++)
    {
        cout<<ans[i].size()<<' ';
        for(int v:ans[i])cout<<v<<' ';
        cout<<'\n';
    }
}
```
## 割点
```cpp
#include<bits/stdc++.h>
using namespace std;

int n,m;
vector<vector<pair<int,int>>> g;
vector<int> dfn, low;
vector<int> iscut;   
int Time = 0;

void tarjan(int u, int fa)
{
    dfn[u] = low[u] = ++Time;
    int child = 0;   

    for(auto [v,id] : g[u])
    {
        if(!dfn[v])
        {
            child++;
            tarjan(v, u);
            low[u] = min(low[u], low[v]);
            if(fa!= -1 && low[v] >= dfn[u])
                iscut[u] = 1;
        }
        else if(v !=fa)
            low[u] = min(low[u], dfn[v]);
        
    }

   
    if(fa == -1 && child >= 2)
        iscut[u] = 1;
}

int main()
{
    cin >> n >> m;

    g.resize(n+1);
    dfn.resize(n+1,0);
    low.resize(n+1,0);
    iscut.resize(n+1,0);

    for(int i=1;i<=m;i++)
    {
        int a,b;
        cin>>a>>b;
        g[a].push_back({b,i});
        g[b].push_back({a,i});
    }

    for(int i=1;i<=n;i++)
    if(!dfn[i])tarjan(i, -1);

    int ans = 0;
    for(int i=1;i<=n;i++)
        if(iscut[i]) ans++;

    cout << ans << '\n';

    return 0;
}
```
# 网络流
## 最大流/最小割
```cpp
#include <bits/stdc++.h>
#define ll  long long
using namespace std;
struct Edge {
    int to;
    ll cap;
    int rev; 
};
vector<vector<Edge>> G;
vector<int>level,cur;
void add_edge(int u, int v, ll c) 
{
    G[u].push_back({v, c, (int)G[v].size()});
    G[v].push_back({u, 0, (int)G[u].size() - 1});
}
bool bfs(int s, int t) 
{
    fill(level.begin(), level.end(), -1);
    queue<int> q;
    level[s] = 0;
    q.push(s);

    while (!q.empty()) 
    {
        int u = q.front(); q.pop();
        for (auto &e : G[u]) 
        {
            if (e.cap > 0 && level[e.to] == -1) 
            {
                level[e.to] = level[u] + 1;
                q.push(e.to);
            }
        }
    }
    return level[t] != -1;
}

ll dfs(int u, int t, ll flow) 
{
    if (u == t) return flow;
    for (int &i = cur[u]; i < G[u].size(); i++) 
    {
        auto &e = G[u][i];
        if (e.cap > 0 && level[e.to] == level[u] + 1) {
            ll f = dfs(e.to, t, min(flow, e.cap));
            if (f > 0) {
                e.cap -= f;
                G[e.to][e.rev].cap += f;
                return f;
            }
        }
    }
    return 0;
}

ll maxflow(int s, int t) 
{
    ll flow = 0;
    while (bfs(s, t)) 
    {
        fill(cur.begin(), cur.end(), 0);
        while (ll f = dfs(s, t, LLONG_MAX)) 
            flow += f;
        
    }
    return flow;
}

int main() 
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    level.resize(n+1,0);
    cur.resize(n+1,0);
    G.resize(n+1);
    for (int i = 1; i <= m; i++) 
    {
        int a, b;
        ll c;
        cin >> a >> b >> c;
        add_edge(a, b, c);
    }
    cout << maxflow(1, n) << '\n';
    return 0;
}
```
### 可加入记录最小割边
```cpp
struct InputEdge {
    int u, v;
};
vector<InputEdge> edges;
vector<int> vis;

void bfs_s(int s) {
    fill(vis.begin(), vis.end(), 0);
    queue<int> q;
    q.push(s);
    vis[s] = 1;

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (auto &e : G[u]) {
            if (e.cap > 0 && !vis[e.to]) {
                vis[e.to] = 1;
                q.push(e.to);
            }
        }
    }
}
int main() 
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    level.resize(n+1,0);
    cur.resize(n+1,0);
    G.resize(n+1);
    vis.resize(n+1,0);
    for (int i = 1; i <= m; i++) 
    {
        int a, b;
        ll c;
        cin >> a >> b ;
        add_edge(a, b, 1);
        edges.push_back({a, b}); 
    }
    cout << maxflow(1, n) << '\n';
    bfs_s(1);
    for (auto &e : edges) 
    {
        if (vis[e.u] && !vis[e.v]) 
            cout << e.u << " " << e.v << '\n';
        else if (vis[e.v] && !vis[e.u])
            cout << e.v << " " << e.u << '\n';
    }
    return 0;
}
```



