template<class Z>
void ntt(vector<Z>& a, bool invert = 0) {
    int n = a.size();
    vector<int> rev(n);
    for (int i = 0; i < n; i++) {
        rev[i] = ((i & 1) * (n / 2)) | (rev[i / 2] / 2);
        if (i < rev[i]) {
            swap(a[i], a[rev[i]]);
        }
    }
    Z g = 3;

    for (int len = 2; len <= n; len *= 2) {
        Z wlen = g.pow((Z::mod() - 1) / len);
        if (invert) {
            wlen = wlen.inv();
        }
        
        for (int i = 0; i < n; i += len) {
            Z w = 1;
            for (int j = 0; j < len / 2; j++) {
                Z u = a[i + j];
                Z v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    
    if (invert) {
        Z ni = Z(n).inv();
        for (int i = 0; i < n; i++) {
            a[i] *= ni;
        }
    }
}

template<class Z>
vector<Z> convolution(vector<Z> a, vector<Z> b) {
    if (a.empty() || b.empty()) {
        return {};
    }
    int sz = (int)a.size() + b.size() - 1;
    int n = 1;
    while (n < sz) {
        n *= 2;
    }
    
    a.resize(n), b.resize(n);
    ntt(a), ntt(b);
    for (int i = 0; i < n; i++) {
        a[i] *= b[i];
    }
    ntt(a, 1);
    
    a.resize(sz);
    return a;
}
