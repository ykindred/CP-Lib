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



