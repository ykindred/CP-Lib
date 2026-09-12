template <typename Z>
void fwt_or(vector<Z>& a, bool invert = 0) {
    int n = a.size();
    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += len << 1) {
            for (int j = 0; j < len; j++) {
                if (!invert) {
                    a[i + j + len] += a[i + j];
                } else {
                    a[i + j + len] -= a[i + j];
                }
            }
        }
    }
}
template <typename Z>
void fwt_and(vector<Z>& a, bool invert = 0) {
    int n = a.size();
    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += len << 1) {
            for (int j = 0; j < len; j++) {
                if (!invert) {
                    a[i + j] += a[i + j + len];
                } else {
                    a[i + j] -= a[i + j + len];
                }
            }
        }
    }
}

template <typename Z>
void fwt_xor(vector<Z>& a, bool invert = 0) {
    int n = a.size();
    for (int len = 1; len < n; len <<= 1) {
        for (int i = 0; i < n; i += len << 1) {
            for (int j = 0; j < len; j++) {
                Z x = a[i + j];
                Z y = a[i + j + len];
                a[i + j] = x + y;
                a[i + j + len] = x - y;
            }
        }
    }

    if (invert) {
        Z invn = Z(n).inv();
        for (int i = 0; i < n; i++) {
            a[i] *= invn;
        }
    }
}
template <typename Z>
vector<Z> convolution_or(vector<Z> a, vector<Z> b) {
    int n = a.size();
    assert((int)b.size() == n);

    fwt_or(a);
    fwt_or(b);

    for (int i = 0; i < n; i++) {
        a[i] *= b[i];
    }

    fwt_or(a, 1);
    return a;
}

template <typename Z>
vector<Z> convolution_and(vector<Z> a, vector<Z> b) {
    int n = a.size();
    assert((int)b.size() == n);

    fwt_and(a);
    fwt_and(b);

    for (int i = 0; i < n; i++) {
        a[i] *= b[i];
    }

    fwt_and(a, 1);
    return a;
}

template <typename Z>
vector<Z> convolution_xor(vector<Z> a, vector<Z> b) {
    int n = a.size();
    assert((int)b.size() == n);

    fwt_xor(a);
    fwt_xor(b);

    for (int i = 0; i < n; i++) {
        a[i] *= b[i];
    }

    fwt_xor(a, 1);
    return a;
}
