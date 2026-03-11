#include <bits/stdc++.h>
using namespace std;
using ll = long long;
inline int lowbit(int x) {
    return x & -x;
}
template <class S>
struct FwkTree {
    int n;
    vector<S> d;
    FwkTree(int n = 0, S def = S()) : n(n), d(n + 1, def) {}
    FwkTree(const vector<S>& arr) {
        build(arr);
    }
    void build(const vector<S>& arr) {
        n = arr.size();
        d.assign(n + 1, S());
        for (int i = 1; i <= n; i++) {
            d[i] = arr[i - 1];
        }
        for (int i = 1; i <= n; i++) {
            int pa = i + lowbit(i);
            if (pa <= n) {
                d[pa] = d[pa] + d[i];
            }
        }
    }

    void add(int pos, S val) {
        for (pos++; pos <= n; pos += lowbit(pos)) {
            d[pos] = d[pos] + val;
        }
    }
    S sum(int pos) {
        S res = S();
        for (pos++; pos > 0; pos -= lowbit(pos)) {
            res = res + d[pos];
        }
        return res;
    }
    S query(int l, int r) {
        return sum(r - 1) - sum(l - 1);
    }
};