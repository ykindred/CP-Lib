// 依赖：
using cd = complex<double>;

vector<ll> mtt(vector<ll> a, vector<ll> b, ll mod) {
    if (a.empty() || b.empty()) {
        return {};
    }
    int sz = (int)a.size() + b.size() - 1;
    if ((ll)a.size() * b.size() <= 30000) {
        vector<ll> c(sz);
        for (int i = 0; i < (int)a.size(); i++) {
            a[i] %= mod;
            if (a[i] < 0) {
                a[i] += mod;
            }
        }
        for (int i = 0; i < (int)b.size(); i++) {
            b[i] %= mod;
            if (b[i] < 0) {
                b[i] += mod;
            }
        }
        for (int i = 0; i < (int)a.size(); i++) {
            for (int j = 0; j < (int)b.size(); j++) {
                c[i + j] = (c[i + j] + (i128)a[i] * b[j]) % mod;
            }
        }
        return c;
    }
    int n = 1;
    while (n < sz) {
        n <<= 1;
    }
    ll M = sqrt((long double)mod) + 1;
    vector<cd> a0(n), a1(n), b0(n), b1(n);
    for (int i = 0; i < (int)a.size(); i++) {
        a[i] %= mod;
        if (a[i] < 0) {
            a[i] += mod;
        }
        a0[i] = a[i] / M;
        a1[i] = a[i] % M;
    }
    for (int i = 0; i < (int)b.size(); i++) {
        b[i] %= mod;
        if (b[i] < 0) {
            b[i] += mod;
        }
        b0[i] = b[i] / M;
        b1[i] = b[i] % M;
    }
    auto fft2 = [&](vector<cd>& x, vector<cd>& y) {
        for (int i = 0; i < n; i++) {
            x[i] += I * y[i];
        }
        fft(x);
        for (int i = 0; i < n; i++) {
            y[i] = conj(x[i ? n - i : 0]);
        }
        for (int i = 0; i < n; i++) {
            cd p = x[i];
            cd q = y[i];
            x[i] = (p + q) * 0.5;
            y[i] = (q - p) * 0.5 * I;
        }
    };
    fft2(a0, a1);
    fft2(b0, b1);
    vector<cd> p(n), q(n);
    for (int i = 0; i < n; i++) {
        p[i] = a0[i] * b0[i] + I * a1[i] * b0[i];
        q[i] = a0[i] * b1[i] + I * a1[i] * b1[i];
    }
    fft(p, 1);
    fft(q, 1);
    auto num = [&](double x) -> ll {
        ll v = x < 0 ? (ll)(x - 0.5) : (ll)(x + 0.5);
        v %= mod;
        if (v < 0) {
            v += mod;
        }
        return v;
    };
    vector<ll> c(sz);
    ll M1 = M % mod;
    ll M2 = (i128)M1 * M1 % mod;
    for (int i = 0; i < sz; i++) {
        ll c00 = num(p[i].real());
        ll c10 = num(p[i].imag());
        ll c01 = num(q[i].real());
        ll c11 = num(q[i].imag());
        c[i] = (
            (i128)M2 * c00
            + (i128)M1 * ((c10 + c01) % mod)
            + c11
        ) % mod;
    }

    return c;
}
