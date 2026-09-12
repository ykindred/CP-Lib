template <typename Z>
vector<Z> pow(vector<Z> a, ll b, int n) {
    if (n == 0) return {};
    if (b == 0) {
        vector<Z> ret(n);
        ret[0] = 1;
        return ret;
    }
    a.resize(n);
    int t = n;
    for (int i = 0; i < n; i++) {
        if (a[i] != Z(0)) {
            t = i;
            break;
        }
    }
    vector<Z> ans(n);
    if (t == n) {
        return ans;
    }
    if ((__int128)t * b >= n) {
        return ans;
    }
    int s = t * b;
    int m = n - s;
    vector<Z> c(m);
    for (int i = 0; i < m && i + t < n; i++) {
        c[i] = a[i + t] / a[t];
    }
    c = ln(c, m);
    for (int i = 0; i < m; i++) {
        c[i] *= Z(b);
    }
    c = exp(c, m);
    Z l = a[t].pow(b);
    for (int i = 0; i < m; i++) {
        ans[i + s] = c[i] * l;
    }
    return ans;
}
