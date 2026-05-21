//这是并查集的一个极简实现
struct Disjoint_set_unoin{
    vector<int>fa;
    Disjoint_set_unoin(int n){fa.resize(n);iota(fa.begin(),fa.end(),0);};
    int find(int n){return n==fa[n]?n:fa[n] = find(fa[n]);}
    void unite(int x,int y){fa[find(x)] = find(y);}
};