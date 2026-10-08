//预先定义邻接表数组 vector<vector<int>>g ,所有节点默认1-based
vector<bool> R(n+1) ;
vector<int> siz(n+1);
vector<vector<int>> Cg(n+1); //图的质心，子图的质心
int h = - 1;

auto get_siz = [&](auto&& self ,int u ,int p = -1)-> int {
    siz[u] = 1;
    for(auto nxt:g[u]){
        if(nxt == p || R[nxt]) continue;
        else siz[u] += self(self ,nxt ,u);
    }
    return siz[u];
};

auto get_ctd = [&](auto&& self ,int u ,int t , int p = -1) -> int {
    //int mxsiz = 0;
    for(auto nxt : g[u]) {
        if(R[nxt] || nxt == p) continue;
        if(siz[nxt] > t / 2)
            return self(self ,nxt , t, u);
    }
    return u;
};

auto build = [&](auto&& self ,int u) -> int {
    int h = get_ctd(get_ctd,u,get_siz(get_siz,u));
    R[h] = true;
    for(auto nxt : g[h]){
        if(R[nxt]) continue;
        else Cg[h].push_back(self(self ,nxt));
    }
    return h;
};
h = build(build ,1);

//h为整个树重心，Cg是一个有根树的邻接表，代表树与子树重心之间的边