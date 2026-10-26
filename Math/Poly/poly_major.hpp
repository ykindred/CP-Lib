#include "../head.hpp"
#include "NTT.hpp"

template<typename Z>
vector<Z> inv(const vector<Z>& a, int n) {
    if (n == 0) return {};
    // assert(a[0] != 0);
    // B = B'(2 - AB')
    int m = 1;
    vector<Z> b(1, a[0].inv());
    while (m < n) {
        int len = min(m * 2, n);
        vector<Z> A(min(len, (int)a.size()));
        for (int i = 0; i < (int)A.size(); i++) {
            A[i] = a[i];
        }
        vector<Z> B = b;
        auto C = convolution(A, B);
        C.resize(len);
        for (int i = 0; i < len; i++) {
            C[i] = -C[i];
        }
        C[0] += 2;
        b = convolution(B, C);
        b.resize(len);
        m *= 2;
    }
    b.resize(n);
    return b;
}

template <typename Z>
vector<Z> deriv(const vector<Z>& a) {
    int n = a.size();
    vector<Z> ret(max(0, n - 1));
    for (int i = 0; i < n - 1; i++) {
        ret[i] = a[i + 1] * Z(i + 1);
    }
    return ret;
}
template <typename Z>
vector<Z> intgr(const vector<Z>& a) {
    int n = a.size();
    assert(n < Z::mod());

    static vector<Z> invs{0, 1};
    int p = Z::mod();

    for (int i = invs.size(); i <= n; i++) {
        invs.push_back(-Z(p / i) * invs[p % i]);
    }

    vector<Z> ret(n + 1);
    for (int i = 1; i <= n; i++) {
        ret[i] = a[i - 1] * invs[i];
    }
    return ret;
}


template <typename Z>
pair<vector<Z>, vector<Z>> divmod(vector<Z> a, vector<Z> b) {
    assert(!b.empty() && b.back() != Z(0));
    int n = a.size();
    int m = b.size();
    if (n < m) {
        return { vector<Z>{ Z(0) }, a };
    }
    int len = n - m + 1;
    vector<Z> ra = a;
    vector<Z> rb = b;
    reverse(ra.begin(), ra.end());
    reverse(rb.begin(), rb.end());
    ra.resize(len);
    rb.resize(len);
    auto q = convolution(ra, inv(rb, len));
    q.resize(len);
    reverse(q.begin(), q.end());
    auto c = convolution(q, b, m - 1);
    a.resize(m - 1);
    for (int i = 0; i < (int)c.size(); i++) {
        a[i] -= c[i];
    }
    return {q, a};
}

template <typename Z>
vector<Z> ln(const vector<Z>& a, int n) {
    // B = ln(A) = intgr(deriv(A) * inv(A))
    // assert(a[0] == 1);
    vector<Z> A(n);
    for (int i = 0; i < n; i++) {
        if (i < a.size()) {
            A[i] = a[i];
        }
    }
    auto B = intgr(convolution(deriv(A), inv(A, n)));
    B.resize(n);
    return B;
}


template <typename Z>
vector<Z> exp(vector<Z> a, int n) {
    // assert(a[0] == 0);
    int m = 1;
    vector<Z> b(1);
    b[0] = 1;
    while (m < n) {
        // B = B' * (1 - ln(B') + A)
        int len = min(m * 2, n);
        vector<Z> A(len);
        for (int i = 0; i < min((int)a.size(), len); i++) {
            A[i] = a[i];
        }
        vector<Z> B(b);
        B.resize(len);
        auto C = ln(B, len);
        for (int i = 0; i < len; i++) {
            C[i] = A[i] - C[i];
        }
        C[0] += 1;
        b = convolution(B, C);
        b.resize(len);
        m *= 2;
    }
    b.resize(n);
    return b;
}


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


template <typename Z>
vector<Z> pow(vector<Z> a, ll b, int n) {
    if (b < 0) {
        return (inv(a, n), -b, n);
    }
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

template <typename Z>
vector<Z> sqrt(const vector<Z>& a, int n) {
    // assert(a[0] == 1);
    int m = 1;
    vector<Z> b(1);
    b[0] = 1;

    Z inv2 = Z(2).inv();
    while (m < n) {
        // B = (B' + A / B') / 2
        int len = min(m * 2, n);
        vector<Z> A(len);
        for (int i = 0; i < min((int)a.size(), len); i++) {
            A[i] = a[i];
        }
        b.resize(len);
        vector<Z> B(b);
        b = convolution(A, inv(B, len));
        b.resize(len);
        for (int i = 0; i < len; i++) {
            b[i] += B[i];
        }
        for (int i = 0; i < len; i++) {
            b[i] *= inv2;
        }
        m *= 2;
    }
    b.resize(n);
    return b;
}
