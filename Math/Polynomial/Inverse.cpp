// 卷积...
template<typename Z>
vector<Z> inv(vector<Z> a, int n) {
    // find B that AB === 1 (mod x^n)
    int m = 1;
    vector<Z> b(1);
    b[0] = a[0].inv();
    while (m < n) {
        // B = 2B' - A(B'^2)
        int len = min(m * 2, n);
        vector<Z> A(min(len, (int)a.size()));
        for (int i = 0; i < (int)A.size(); i++) {
            A[i] = a[i];
        }
        vector<Z> B(b);
        B.resize(len);
        auto C = convolution(A, convolution(B, B));
        C.resize(len);
        b.resize(len);
        for (int i = 0; i < len; i++) {
            b[i] = B[i] * 2 - C[i];
        }
        m *= 2;
    }
    b.resize(n);
    return b;
}
