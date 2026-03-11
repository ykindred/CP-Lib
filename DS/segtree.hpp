#include <bits/stdc++.h>
using namespace std;
using ll = long long;
template <class S>
struct SegTree {
    int n;
    vector<S> d;
    SegTree(int n = 0, S def = S()) : n(n), d(4 * n, def) {}
    SegTree(const vector<S>& arr) {
        build(arr);
    }
    void build(const vector<S>& arr) {
        n = arr.size();
        d.assign(4 * arr.size(), S());
        build(0, 0, n, arr);
    }
    void set(int p, const S& val) {
        set(0, 0, n, p, val);
    }
    S query(int l, int r) {
        return query(0, 0, n, l, r);
    }
private:
    #define ls p * 2 + 1
    #define rs p * 2 + 2
    #define mid l + (r - l) / 2
    void pull(int p) {
        d[p] = d[ls] + d[rs];
    }
    void build(int p, int l, int r, const vector<S>& arr) {
        if (r == l + 1) {
            d[p] = arr[l];
            return;
        }
        build(ls, l, mid, arr);
        build(rs, mid, r, arr);
        pull(p);
    }
    
    void set(int p, int l, int r, int x, const S& val) {
        if (r == l + 1) {
            d[p] = val;
            return;
        }
        if (x < mid) {
            set(ls, l, mid, x, val);
        } else {
            set(rs, mid, r, x, val);
        }
        pull(p);
    }
    
    S query(int p, int l, int r, int x, int y) {
        if (y <= l || x >= r) {
            return S();
        }
        if (x <= l && r <= y) {
            return d[p];
        }
        return query(ls, l, mid, x, y) + query(rs, mid, r, x, y);
    }
    #undef ls
    #undef rs
    #undef mid
};

struct S {
    ll val = 0;
    friend S operator+(const S& a, const S& b) {
        return { a.val + b.val };
    }
};