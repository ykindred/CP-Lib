#include <bits/stdc++.h>
using namespace std;
template <typename S>
struct SpTable {
    int n;
    int log;
    vector<vector<S>> st;

    SpTable(const vector<S>& a) {
        build(a);
    }
    void build(const vector<S>& a) {
        n = a.size();
        log = 32 - __builtin_clz(n);
        st.assign(log, vector<S>(n));
        for (int i = 0; i < n; i++) {
            st[0][i] = a[i];
        }
        for (int j = 1; j < log; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                st[j][i] = op(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
            }
        }
    }
    S query(int l, int r) {
        int j = 31 - __builtin_clz(r - l);
        return op(st[j][l], st[j][r - (1 << j)]);
    }
};