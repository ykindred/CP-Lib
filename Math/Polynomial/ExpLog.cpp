// 卷积, 求导, 积分...
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
