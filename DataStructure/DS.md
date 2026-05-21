# 并查集
```cpp
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
```

# ST表
```cpp
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
```

# 树状数组
```cpp
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
```

# 线段树
## 普通(点修改, 区间查询)
```cpp
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
        d.assign(4 * n, S());
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
```

## 懒标记线段树


# 哈希
## 哈希表
```cpp
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
struct chash {
    static ull splitmix64(ull x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(ull x) const {
        static const ull F = 
            chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + F);
    }
};
template<typename K, typename V>
using HashMap = __gnu_pbds::gp_hash_table<K, V, chash>;
```