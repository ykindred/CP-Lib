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




