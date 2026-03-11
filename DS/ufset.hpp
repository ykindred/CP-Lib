#include <bits/stdc++.h>
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
