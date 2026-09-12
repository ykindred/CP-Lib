// 卷积, 求逆...
template <typename Z>
vector<Z> mulT(vector<Z> a, vector<Z> b, int need) {
    vector<Z> ret(need);
    if (a.empty() || b.empty() || need == 0) {
        return ret;
    }
    int n = b.size();
    reverse(b.begin(), b.end());
    auto c = convolution(a, b);
    for (int i = 0; i < need; i++) {
        int p = i + n - 1;
        if (p < (int)c.size()) {
            ret[i] = c[p];
        }
    }
    return ret;
}

template <typename Z>
vector<Z> multipoint_eval(vector<Z> f, vector<Z> x) {
    int m = x.size();
    vector<Z> ans(m);
    if (m == 0) {
        return ans;
    }
    if (f.empty()) {
        return ans;
    }
    int n = max((int)f.size(), m);
    x.resize(n);
    vector<vector<Z>> q(4 * n);
    auto build = [&](auto self, int p, int l, int r) -> void {
        if (r - l == 1) {
            q[p] = {Z(1), -x[l]};
            return;
        }
        int mid = (l + r) / 2;
        self(self, p << 1, l, mid);
        self(self, p << 1 | 1, mid, r);
        q[p] = convolution(q[p << 1], q[p << 1 | 1]);
    };
    auto work = [&](auto self, int p, int l, int r, const vector<Z>& num) -> void {
        if (r - l == 1) {
            if (l < m) {
                ans[l] = num.empty() ? Z(0) : num[0];
            }
            return;
        }
        int mid = (l + r) / 2;
        auto vl = mulT(num, q[p << 1 | 1], mid - l);
        auto vr = mulT(num, q[p << 1], r - mid);
        self(self, p << 1, l, mid, vl);
        self(self, p << 1 | 1, mid, r, vr);
    };
    build(build, 1, 0, n);
    auto root = mulT(f, inv(q[1], n), n);
    work(work, 1, 0, n, root);
    return ans;
}
