//前面存储一个vector<vector<int>>g(n+1) ，代表邻接表
//一般默认 1 是树根，且没有0节点
vector<int>s(n+1) ,d(n+1) ,p(n+1) , t(n+1);
auto dfs1 = [&](auto&&self ,int u,int par ,int dep) -> void {
    d[u] = dep;
    p[u] = par;
    s[u] += 1; // s[u]+1 放在这里很重要
    for(auto &nxt:g[u]){
        if(nxt==par) continue;
        self(self,nxt,u,dep+1);
        if(s[nxt]>s[g[u][0]]||g[u][0] == par) swap(nxt , g[u][0]);
        s[u]+=s[nxt];
    }
} ;
vector<int>dsu , plz(n+1);
auto dfs2 = [&](auto&&self ,int u,int par ,int top)-> void {
    dsu.push_back(u);
    plz[u] = dsu.size() - 1;
    t[u] = top;
    for(auto nxt:g[u]){
        if(nxt == par) continue;
        self(self ,nxt , u,(nxt == g[u][0]?top:nxt));
    }
};
dfs1(dfs1,1,-1,1);
dfs2(dfs2,1,-1,1);
//其中，s代表子树大小，d代表节点深度，p代表父节点，t代表在重链中的头节点‘
//dsu代表欧拉序，plz代表节点在欧拉序中的位置
//保证重链上的节点在欧拉序中连续
//jump(演示)
/*
int u ,v;
cin>>u>>v
*/
while(t[u]!=t[v]){
    //总是挪较深的那一个，这里假设是u
    if(d[t[u]] < d[t[v]]) swap(u,v);
    /*
    这里一般合并u~t[u];
    query(t[u],u);
    */
    u = p[t[u]];
}
if(d[u]>d[v]) swap(u,v);
/*
query(u,v);
*/

