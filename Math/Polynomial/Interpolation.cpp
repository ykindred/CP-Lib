template <typename Z>
vector<Z> interpolate(vector<Z> x, vector<Z> y) {
    int n = x.size();
    assert((int)y.size() == n);
    if (n == 0) {
        return {};
    }
    vector<vector<Z>> q(4 * n);
    auto build = [&](auto self, int p, int l, int r) -> void {
        if (r - l == 1) {
            // x - x_l
            q[p] = {-x[l], Z(1)};
            return;
        }

        int mid = (l + r) / 2;

        self(self, p << 1, l, mid);
        self(self, p << 1 | 1, mid, r);

        q[p] = convolution(q[p << 1], q[p << 1 | 1]);
    };

    build(build, 1, 0, n);
    auto d = multipoint_eval(deriv(q[1]), x);
    vector<Z> w(n);
    for (int i = 0; i < n; i++) {
        w[i] = y[i] / d[i];
    }

    auto work = [&](auto self, int p, int l, int r) -> vector<Z> {
        if (r - l == 1) {
            return vector<Z>{w[l]};
        }

        int mid = (l + r) / 2;

        auto L = self(self, p << 1, l, mid);
        auto R = self(self, p << 1 | 1, mid, r);

        auto A = convolution(L, q[p << 1 | 1]);
        auto B = convolution(R, q[p << 1]);

        int len = max(A.size(), B.size());
        A.resize(len);
        B.resize(len);

        for (int i = 0; i < len; i++) {
            A[i] += B[i];
        }

        return A;
    };
    auto ans = work(work, 1, 0, n);
    ans.resize(n);
    return ans;
}
