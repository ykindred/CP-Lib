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
    vector<Z> ret(n + 1);
    for (int i = 1; i <= n; i++) {
        ret[i] = a[i - 1] / Z(i);
    }
    return ret;
}
