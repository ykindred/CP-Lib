using cd = complex<double>;
using numbers::pi;
const cd I(0, 1);
vector<int> rev;
vector<cd> Wn;

void fft(vector<cd>& a, bool invert = 0) {
    int n = a.size();
    if ((int)rev.size() != n) {
        rev.assign(n, 0);
        for (int i = 1; i < n; i++) {
            rev[i] = (rev[i >> 1] >> 1) | ((i & 1) * (n >> 1));
        }
    }
    if ((int)Wn.size() != n) {
        Wn.assign(n, cd(0, 0));
        for (int i = 0; i < n; i++) {
            Wn[i] = cd(cos(pi / n * i), sin(pi / n * i));
        }
    }
    if (invert) {
        for (int i = 1; i < n; i++) {
            if (i < n - i) {
                swap(a[i], a[n - i]);
            }
        }
    }
    for (int i = 0; i < n; i++) {
        if (i < rev[i]) {
            swap(a[i], a[rev[i]]);
        }
    }
    for (int m = 1; m < n; m <<= 1) {
        for (int i = 0; i < n; i += m << 1) {
            for (int j = 0; j < m; j++) {
                cd w = Wn[1LL * j * n / m];
                cd x = a[i + j];
                cd y = a[i + j + m] * w;

                a[i + j] = x + y;
                a[i + j + m] = x - y;
            }
        }
    }
    if (invert) {
        for (int i = 0; i < n; i++) {
            a[i] /= n;
        }
    }
}

vector<ll> convolution(const vector<ll>& a, const vector<ll>& b) {
    if (a.empty() || b.empty()) {
        return {};
    }
    int sz = (int)a.size() + b.size() - 1;
    int n = 1;
    while (n < sz) {
        n <<= 1;
    }
    vector<cd> A(n), B(n);
    for (int i = 0; i < (int)a.size(); i++) {
        A[i] = a[i];
    }
    for (int i = 0; i < (int)b.size(); i++) {
        B[i] = b[i];
    }
    for (int i = 0; i < n; i++) {
        A[i] += I * B[i];
    }
    fft(A);
    for (int i = 0; i < n; i++) {
        B[i] = conj(A[i ? n - i : 0]);
    }
    for (int i = 0; i < n; i++) {
        cd p = A[i];
        cd q = B[i];
        A[i] = (p + q) * 0.5;
        B[i] = (q - p) * 0.5 * I;
    }
    for (int i = 0; i < n; i++) {
        A[i] *= B[i];
    }
    fft(A, 1);
    vector<ll> c(sz);
    for (int i = 0; i < sz; i++) {
        double x = A[i].real();
        c[i] = x < 0 ? (ll)(x - 0.5) : (ll)(x + 0.5);
    }
    return c;
}
