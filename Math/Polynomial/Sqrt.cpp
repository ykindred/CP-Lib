// 卷积, 求逆...
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
