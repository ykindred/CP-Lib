// 卷积, 求逆...
template <typename Z>
pair<vector<Z>, vector<Z>> divmod(vector<Z> a, vector<Z> b) {
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
    auto c = convolution(q, b);
    for (int i = 0; i < min((int)a.size(), (int)c.size()); i++) {
        a[i] -= c[i];
    }
    a.resize(m - 1);
    return { q, a };
}
