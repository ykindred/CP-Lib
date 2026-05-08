//并查集（路径压缩）
struct dsu{
    vector<int>fa;
    dsu(int n);
    int find(int x);//查找x所在分量的代表元
    void unite(int x,int y); //x连接到y上
};
dsu::dsu(int x){
    this->fa.resize(x);
    for(int i = 0;i<x;i++) fa[i] = i;
};
int dsu::find(int x){return x==fa[x]?x:fa[x] = find(fa[x]);}
void dsu::unite(int x,int y){fa[find(x)] = find(y);}
//Written by Pilocode
/*#include <bits/stdc++.h>
using namespace std;
struct UfSet {
    int n;
    vector<int> pa, siz;
    void build(int s) {
        n = s;
        pa.assign(s, 0);
        siz.assign(s, 1);
        iota(pa.begin(), pa.end(), 0);
    }
    UfSet(int n = 0) { 
        build(n); 
    }
    int find(int x) {
        while (x != pa[x]) {
            x = pa[x] = pa[pa[x]];
        }
        return x;
    }
    bool uni(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) {
            return false;
        }
        // 按秩合并
        if (siz[x] < siz[y]) {
            swap(x, y);
        }
        pa[y] = x;
        siz[x] += siz[y];
        return true;
    }
    bool same(int x, int y) {
        return find(x) == find(y);
    }
};
using DSU = UfSet;
*/
