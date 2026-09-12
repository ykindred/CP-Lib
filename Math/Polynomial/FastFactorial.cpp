struct FastFactorial {
    using ld = long double;
    static constexpr int MAXD = 18;
    static constexpr int LIM = (1 << MAXD) + 5;
    static constexpr int BASE = 1 << 16;
    static constexpr int SF = 16;
    static constexpr int MASK = BASE - 1;
    struct Complex {
        ld r, i;
        friend Complex operator+(Complex a, Complex b) {
            return {a.r + b.r, a.i + b.i};
        }
        friend Complex operator-(Complex a, Complex b) {
            return {a.r - b.r, a.i - b.i};
        }
        friend Complex operator*(Complex a, Complex b) {
            return {a.r * b.r - a.i * b.i, a.r * b.i + a.i * b.r};
        }
        Complex& operator/=(int x) {
            r /= x;
            i /= x;
            return *this;
        }
    };
    ll mod, base2;
    vector<vector<int>> rev;
    vector<Complex> rt[2][MAXD + 1];
    vector<Complex> tr, tr1, tr2, tr3, tr4, tr5, tr6;
    vector<ll> m13, m14, m23, m24;
    vector<ll> f, g, h, ifac;
    vector<ll> val, tmp1, tmp2;
    FastFactorial() {
        rev.resize(MAXD + 1);
        tr.resize(LIM);
        tr1.resize(LIM);
        tr2.resize(LIM);
        tr3.resize(LIM);
        tr4.resize(LIM);
        tr5.resize(LIM);
        tr6.resize(LIM);
        m13.resize(LIM);
        m14.resize(LIM);
        m23.resize(LIM);
        m24.resize(LIM);
        f.resize(LIM);
        g.resize(LIM);
        h.resize(LIM);
        ifac.resize(LIM);
        val.resize(LIM);
        tmp1.resize(LIM);
        tmp2.resize(LIM);
        pre_fft();
    }
    ll norm(ll x) {
        x %= mod;
        if (x < 0) x += mod;
        return x;
    }
    ll qpow(ll a, ll b) {
        ll ret = 1 % mod;
        a = norm(a);
        while (b > 0) {
            if (b & 1) ret = (i128)ret * a % mod;
            a = (i128)a * a % mod;
            b >>= 1;
        }
        return ret;
    }
    void pre_fft() {
        const ld PI = acosl(-1.0L);
        for (int d = 1; d <= MAXD; d++) {
            int len = 1 << d;
            rev[d].assign(len, 0);
            for (int i = 1; i < len; i++) {
                rev[d][i] = (rev[d][i >> 1] >> 1) | ((i & 1) << (d - 1));
            }
        }
        for (int d = 1; d <= MAXD; d++) {
            int m = 1 << (d - 1);
            rt[0][d].resize(m);
            rt[1][d].resize(m);
            for (int i = 0; i < m; i++) {
                ld ang = PI * i / m;
                rt[0][d][i] = {cosl(ang), sinl(ang)};
                rt[1][d][i] = {cosl(ang), -sinl(ang)};
            }
        }
    }
    void fft(Complex* a, int len, int d, int inv) {
        for (int i = 1; i < len; i++) {
            if (i < rev[d][i]) {
                swap(a[i], a[rev[d][i]]);
            }
        }
        for (int k = 1, dep = 1; k < len; k <<= 1, dep++) {
            for (int s = 0; s < len; s += k << 1) {
                auto* w = rt[inv][dep].data();
                for (int i = s; i < s + k; i++, w++) {
                    Complex x = a[i];
                    Complex y = a[i + k] * (*w);
                    a[i] = x + y;
                    a[i + k] = x - y;
                }
            }
        }
        if (inv) {
            for (int i = 0; i < len; i++) {
                a[i] /= len;
            }
        }
    }
    void dbdft(ll* a, int len, int d, Complex* op1, Complex* op2) {
        for (int i = 0; i < len; i++) {
            tr[i] = {(ld)(a[i] >> SF), (ld)(a[i] & MASK)};
        }
        fft(tr.data(), len, d, 0);
        tr[len] = tr[0];
        for (int i = 0; i < len; i++) {
            Complex p = tr[i];
            Complex q = tr[len - i];

            op1[i] = Complex{p.r + q.r, p.i - q.i} * Complex{0.5, 0};
            op2[i] = Complex{p.r - q.r, p.i + q.i} * Complex{0, -0.5};
        }
    }
    ll round_mod(ld x) {
        ll v = x < 0 ? (ll)(x - 0.5) : (ll)(x + 0.5);
        v %= mod;
        if (v < 0) v += mod;
        return v;
    }
    void dbidft(Complex* a, int len, int d, ll* op1, ll* op2) {
        fft(a, len, d, 1);
        for (int i = 0; i < len; i++) {
            op1[i] = round_mod(a[i].r);
            op2[i] = round_mod(a[i].i);
        }
    }
    void poly_mul(ll* a, ll* b, ll* c, int len, int d) {
        dbdft(a, len, d, tr1.data(), tr2.data());
        dbdft(b, len, d, tr3.data(), tr4.data());
        for (int i = 0; i < len; i++) {
            tr5[i] = tr1[i] * tr3[i] + Complex{0, 1} * (tr2[i] * tr4[i]);
            tr6[i] = tr2[i] * tr3[i] + Complex{0, 1} * (tr1[i] * tr4[i]);
        }
        dbidft(tr5.data(), len, d, m13.data(), m24.data());
        dbidft(tr6.data(), len, d, m23.data(), m14.data());
        for (int i = 0; i < len; i++) {
            c[i] = (i128)m13[i] * base2 % mod;
            c[i] = (c[i] + (i128)(m23[i] + m14[i]) % mod * BASE + m24[i]) % mod;
        }
    }
    void init_ifac(int lim) {
        lim = min(lim, LIM - 1);
        ifac[0] = 1;
        if (lim >= 1) ifac[1] = 1;
        for (int i = 2; i <= lim; i++) {
            ifac[i] = (mod - mod / i) * ifac[mod % i] % mod;
        }
        for (int i = 1; i <= lim; i++) {
            ifac[i] = ifac[i] * ifac[i - 1] % mod;
        }
    }
    void shift_eval(ll del, int cur, ll* ip, ll* op) {
        int len = 1;
        int d = 0;
        while (len <= cur + cur + cur) {
            len <<= 1;
            d++;
        }
        for (int i = 0; i <= cur; i++) {
            f[i] = ip[i] * ifac[i] % mod * ifac[cur - i] % mod;
        }
        for (int i = cur - 1; i >= 0; i -= 2) {
            if (f[i]) f[i] = mod - f[i];
        }
        int total = cur + cur + 1;
        ll prod = 1;
        for (int i = 0; i < total; i++) {
            g[i] = norm(del - cur + i);
            prod = (i <= cur ? (i128)prod * g[i] % mod : prod);
        }
        h[0] = 1;
        for (int i = 0; i < total; i++) {
            h[i + 1] = (i128)h[i] * g[i] % mod;
        }
        ll inv_all = qpow(h[total], mod - 2);
        for (int i = total - 1; i >= 0; i--) {
            ll x = g[i];
            g[i] = (i128)inv_all * h[i] % mod;
            inv_all = (i128)inv_all * x % mod;
        }
        for (int i = cur + 1; i < len; i++) {
            f[i] = 0;
        }
        for (int i = total; i < len; i++) {
            g[i] = 0;
        }
        poly_mul(f.data(), g.data(), h.data(), len, d);
        ll cur_prod = prod;
        for (int i = 0; i <= cur; i++) {
            op[i] = h[i + cur] * cur_prod % mod;
            cur_prod = (i128)cur_prod * g[i] % mod;
            cur_prod = (i128)cur_prod * norm(del + i + 1) % mod;
        }
    }
    void build_block_values(int B) {
        int hb = 0;
        for (int x = B; x; x >>= 1) {
            hb++;
        }
        val[0] = 1;
        int cur = 0;
        ll invB = qpow(B, mod - 2);
        for (int z = hb; z >= 0; z--) {
            if (cur != 0) {
                shift_eval(cur + 1, cur, val.data(), tmp1.data());
                for (int i = 0; i <= cur; i++) {
                    val[cur + i + 1] = tmp1[i];
                }
                val[cur * 2 + 1] = 0;
                shift_eval((ll)cur * invB % mod, cur << 1, val.data(), tmp2.data());
                cur <<= 1;
                for (int i = 0; i <= cur; i++) {
                    val[i] = val[i] * tmp2[i] % mod;
                }
            }
            if ((B >> z) & 1) {
                for (int i = 0; i <= cur; i++) {
                    val[i] = val[i] * ((ll)B * i + cur + 1) % mod;
                }
                cur |= 1;
                val[cur] = 1;
                for (int i = 1; i <= cur; i++) {
                    val[cur] = val[cur] * ((ll)cur * B + i) % mod;
                }
            }
        }
    }
    ll brute_fact(ll n) {
        ll ans = 1 % mod;
        for (ll i = 1; i <= n; i++) {
            ans = (i128)ans * i % mod;
        }
        return ans;
    }
    ll fact_raw(ll n) {
        if (n == 0) return 1 % mod;
        // 这个阈值可以调整, 不要太小
        if (n <= 500000) {
            return brute_fact(n);
        }
        int B = sqrt((long double)n);
        while ((ll)B * B < n) B++;
        while (B > 1 && (ll)(B - 1) * (B - 1) >= n) B--;
        init_ifac(2 * B + 10);
        build_block_values(B);
        ll ans = 1 % mod;
        ll i = 0;
        int id = 0;
        while (i + B <= n) {
            ans = ans * val[id] % mod;
            i += B;
            id++;
        }
        for (ll j = i + 1; j <= n; j++) {
            ans = (i128)ans * j % mod;
        }
        return ans;
    }

    ll operator()(ll n, ll p) {
        mod = p;
        base2 = (ll)BASE * BASE % mod;
        if (n >= mod) return 0;
        if (n > mod - 1 - n) {
            ll r = mod - 1 - n;
            ll ans = fact_raw(r);
            ans = qpow(ans, mod - 2);
            if ((r + 1) & 1) {
                ans = (mod - ans) % mod;
            }
            return ans;
        }
        return fact_raw(n);
    }
};
FastFactorial f;
