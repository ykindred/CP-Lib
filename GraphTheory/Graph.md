#拓扑排序
#include <bits/stdc++.h>
using namespace std;

vector<int> topological_sort(int n, vector<vector<int>>& graph, vector<int>& indegree)
{
    queue<int> q;
    vector<int> order;

    for (int i = 1; i <= n; i++)
    {
        if (indegree[i] == 0)
            q.push(i);
    }

    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        order.push_back(u);

        for (int v : graph[u])
        {
            if (--indegree[v] == 0)
                q.push(v);
        }
    }
    if (order.size() != n)
        return {};

    return order;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> graph(n + 1);
    vector<int> indegree(n + 1, 0);

    for (int i = 0; i < m; i++)
    {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        indegree[v]++;
    }

    vector<int> ans = topological_sort(n, graph, indegree);

    if (ans.empty())
    {
        cout << "IMPOSSIBLE\n";
    }
    else
    {
        for (int x : ans)
            cout << x << " ";
        cout << "\n";
    }

    return 0;
}
