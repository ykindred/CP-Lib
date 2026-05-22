# 组合数学
## 基础
依赖ModInt, 否则需要手动取模.
```cpp
template <int N, int MOD = 998244353>
struct Factorial {
    using mint = ModInt<MOD>;
    array<mint, N + 5> fact, factinv;
    Factorial() {
        fact[0] = 1;
        for (int i = 1; i <= N; i++) {
            fact[i] = fact[i - 1] * i;
        }
        factinv[N] = fact[N].inv();
        for (int i = N - 1; i >= 0; i--) {
            factinv[i] = factinv[i + 1] * (i + 1);
        }
    }
    mint nCr(int n, int r) {
        if (!(0 <= r && r <= n)) {
            return 0;
        }
        return fact[n] * factinv[r] * factinv[n - r];
    }
    mint nPr(int n, int r) {
        if (!(0 <= r && r <= n)) {
            return 0;
        }
        return fact[n] * factinv[n - r];
    }
};
```

# 模运算
## 模运算类
```cpp
constexpr int MOD = 998'244'353;

template <int P> 
struct ModInt {
    u32 val;
    constexpr ModInt(ll x = 0) : val((x %= P) < 0 ? x + P : x) {}
    // 注意i128等类型需要先转成ll
    using M = ModInt;
    M &operator+=(const M &p) {
        if ((val += p.val) >= P) {
            val -= P;
        }
        return *this;
    }
    M &operator-=(const M &p) {
        if ((val += P - p.val) >= P) {
            val -= P;
        }
        return *this;
    }
    M &operator*=(const M &p) {
        val = u64(val) * p.val % P;
        return *this;
    }
    M &operator/=(const M &p) { return (*this) *= p.inv(); }
    constexpr M operator-() const { return M(val ? P - val : u32(0)); }
    constexpr M operator+(const M &p) const { return M(*this) += p; }
    constexpr M operator-(const M &p) const { return M(*this) -= p; }
    constexpr M operator*(const M &p) const { return M(*this) *= p; }
    constexpr M operator/(const M &p) const { return M(*this) /= p; }
    constexpr bool operator==(const M &p) const { return val == p.val; }
    constexpr bool operator!=(const M &p) const { return val != p.val; }
    M pow(ll n) const {
        if (n < 0) return inv().pow(-n);
        M ret(1), mul(val);
        while (n > 0) {
            if (n & 1) ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }
    M inv() const {
        int a = val, b = P, u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            a -= t * b;
            u -= t * v;
            swap(a, b);
            swap(u, v);
        }
        return M(u);
    }
    constexpr static M raw(u32 x) {
        M ret;
        ret.val = x;
        return ret;
    }
    constexpr static int mod() {
        return P;
    }
};
using mint = ModInt<MOD>;
```

# 数论

## exCRT
```cpp
array<i128, 2> excrt(const vector<ll>& b, const vector<ll>& m) {
    int n = b.size();
    i128 nowb = b[0], nowm = m[0];
    for (int i = 1; i < n; i++) {
        ll A = nowm % m[i], B = m[i];
        i128 C = b[i] - nowb;
        auto [y1, _, g] = exgcd(A, B);
        if (C % g != 0) {
            return { -1, -1 };
        }
        
        i128 M = m[i] / g;
        i128 y = (i128)y1 * (C / g) % M;
        y = (y < 0) ? y + M : y;
        nowb += y * nowm;
        nowm *= M;
        nowb %= nowm;
    }
    return { nowb, nowm };
}
```
## exGCD
```cpp
i128 abs128(i128 x) {
    return x < 0 ? -x : x;
}
array<i128, 3> exgcd(i128 a, i128 b) {
    if (b == 0) {
        if (a >= 0) return {1, 0, a};
        else return {-1, 0, -a};
    }
    auto [x, y, g] = exgcd(b, a % b);
    return {y, x - a / b * y, g};
}
i128 norm_mod(i128 x, i128 mod) {
    mod = abs128(mod);
    x %= mod;
    if (x < 0) x += mod;
    return x;
}
tuple<bool, i128, i128> solveLDE(i128 a, i128 b, i128 c) {
    // solve ax + by = c
    if (a == 0 && b == 0) {
        if (c == 0) return {true, 0, 0};
        else return {false, 0, 0};
    }
    auto [x, y, g] = exgcd(a, b);
    if (c % g != 0) {
        return {false, 0, 0};
    }
    i128 k = c / g;
    x *= k;
    y *= k;
    // 把 x 规范到 [0, |b / g| - 1]
    if (b != 0) {
        i128 step_x = b / g;
        i128 nx = norm_mod(x, step_x);
        // x = x0 + (b / g) * t
        // y = y0 - (a / g) * t
        i128 t = (nx - x) / step_x;
        x = nx;
        y = y - (a / g) * t;
    }
    return {true, x, y};
}
```

## 线性筛
```cpp
constexpr int N = 1'000'000;
constexpr int PIN = 78'498;
bitset<N + 5> not_prime;
int primes[PIN + 5], pcnt = 0;
// 1e9 : 50'847'534
// 1e8 : 5'761'455
// 1e7 : 664'579
// 1e6 : 78'498
// 1e5 : 9'592
// 1e4 : 1'229
// 1e3 : 168
// 1e2 : 25
// 1e1 : 4
// 下界N / log(N), 上界(>61000时) N / (log(N) - 1.1)",

// int minp[N + 5]; // 最小质因子
// int phi[N + 5]; // 欧拉函数
// int mu[N + 5]; // 莫比乌斯函数
// int d[N + 5], times[N + 5]; // 约数计数函数
// ll sigma[N + 5]; int g[N + 5]; // 约数和函数
void sieve() {
    // phi[1] = 1;
    // mu[1] = 1;
    // d[1] = 1;
    // sigma[1] = 1;
    for (int i = 2; i <= N; i++) {
        if (!not_prime[i]) {
            primes[pcnt] = i, pcnt++;
            // minp[i] = i;
            // phi[i] = i - 1;
            // mu[i] = -1;
            // d[i] = 2, times[i] = 1;
            // sigma[i] = i + 1, g[i] = i + 1;
        }
        for (int j = 0; j < pcnt && i * primes[j] <= N; j++) {
            int pj = primes[j];
            not_prime[i * pj] = 1;
            // minp[i * pj] = pj;
            if (i % primes[j] == 0) {
                // phi[i * pj] = phi[i] * pj;
                // mu[i * pj] = 0;
                // d[i * pj] = d[i] / (times[i] + 1) * (times[i] + 2), times[i * pj] = times[i] + 1;
                // g[i * pj] = g[i] * pj + 1, sigma[i * pj] = sigma[i] / g[i] * g[i * pj];
                break;
            }
            // phi[i * pj] = phi[i] * (pj - 1);
            // mu[i * pj] = -mu[i];
            // d[i * pj] = d[i] * 2, times[i * pj] = 1;
            // sigma[i * pj] = sigma[i] * sigma[pj], g[i * pj] = pj + 1;
        }
    }
}
```

## 分段筛
```cpp
constexpr ll R = 1'000'000'300;
constexpr int SQR = 32'000;
constexpr int PIS = 4'000;
// sqrt(10) < 3.17
bitset<SQR + 5> not_prime;
int primes[PIS + 5], pcnt = 0;
void init_sieve() {
    not_prime[0] = not_prime[1] = 1;
    for (ll i = 2; i * i <= R; i++) {
        if (!not_prime[i]) {
            primes[pcnt] = i;
            pcnt++;
            for (ll j = i * i; j <= SQR; j += i) {
                not_prime[j] = 1;
            }
        }
    }
}

vector<ll> get_primes(ll l, ll r) {
    vector<bool> notp(r - l + 1);
    if (l == 0) {
        notp[0] = notp[1] = 1;
    } else if (l == 1) {
        notp[0] = 1;
    }

    for (int i = 0; i < pcnt; i++) {
        ll p = primes[i];
        if (p * p > r) {
            break;
        }
        for (ll j = max(2LL, (l + p - 1) / p); j * p <= r; j++) {
            notp[j * p - l] = 1;
        }
    }
    vector<ll> ret;
    for (ll i = l; i <= r; i++) {
        if (!notp[i - l]) {
            ret.emplace_back(i);
        }
    }
    return ret;
}

```

## Miller-Rabin
```cpp
bool primetest(ll n) {
    if (n < 2 || n % 6 % 4 != 1) {
        return (n | 1) == 3;
    }
    ll s = __builtin_ctzll(n - 1), d = n >> s;
    for (ll a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        ll p = pow(a % n, d, n), i = s;
        while (p != 1 && p != n - 1 && a % n && i--) {
            p = mul(p, p, n);
        }
        if (p != n - 1 && i != s) {
            return false;
        }
    }
    return true;
}
```

## Pollard-rho
```cpp
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

ll rho(ll n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;

    while (true) {
        ll c = uniform_int_distribution<ll>(1, n - 1)(rng);
        ll x = uniform_int_distribution<ll>(0, n - 1)(rng);
        ll y = x, d = 1;

        auto f = [&](ll x) -> ll {
            return (mul(x, x, n) + c) % n;
        };

        while (d == 1) {
            x = f(x);
            y = f(f(y));
            d = gcd(abs(x - y), n);
        }

        if (d != n) return d;
    }
}
vector<ll> factorize(ll x) {
    vector<ll> res;

    auto dfs = [&](auto self, ll x) -> void {
        if (x == 1) return;
        if (primetest(x)) {
            res.push_back(x);
            return;
        }

        ll d = rho(x);
        self(self, d);
        self(self, x / d);
    };

    dfs(dfs, x);
    sort(res.begin(), res.end());
    return res;
}
```

## BSGS
```cpp
namespace BSGS {
ll mul(ll a, ll b, ll mod) {
    return (i128)a * b % mod;
}

ll power(ll a, ll b, ll mod) {
    ll ret = 1 % mod;
    while (b) {
        if (b & 1) ret = mul(ret, a, mod);
        a = mul(a, a, mod);
        b >>= 1;
    }
    return ret;
}

ll exgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1, y = 0;
        return a;
    }
    ll x1, y1;
    ll g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - a / b * y1;
    return g;
}

ll inv(ll a, ll mod) {
    ll x, y;
    ll g = exgcd(a, mod, x, y);
    if (g != 1) return -1;
    x %= mod;
    if (x < 0) x += mod;
    return x;
}
ll bsgs(ll a, ll b, ll m) {
    a %= m;
    b %= m;

    if (m == 1) return 0;
    if (b == 1) return 0;

    ll n = sqrtl(m) + 1;
    HashMap<ll, ll> mp;
    // b * a^q
    ll cur = b;
    for (ll q = 0; q < n; q++) {
        mp[cur] = q;
        cur = mul(cur, a, m);
    }
    ll an = power(a, n, m);
    cur = 1;
    for (ll p = 1; p <= n + 1; p++) {
        cur = mul(cur, an, m);
        auto it = mp.find(cur);
        if (it != mp.end()) {
            ll x = p * n - it->second;
            if (x >= 0) return x;
        }
    }
    return -1;
}

ll exbsgs(ll a, ll b, ll m) {
    a %= m;
    b %= m;
    if (m == 1) return 0;
    if (b == 1) return 0;
    ll cnt = 0;
    ll cur = 1;
    while (true) {
        ll g = gcd(a, m);
        if (g == 1) break;
        if (b % g != 0) return -1;
        b /= g;
        m /= g;
        cur = mul(cur, a / g, m);
        cnt++;
        if (cur == b) return cnt;
    }
    ll iv = inv(cur, m);
    ll rhs = mul(b, iv, m);
    ll t = bsgs(a, rhs, m);
    if (t == -1) return -1;
    return cnt + t;
}
};
```

## 杜教筛
```cpp
constexpr int N = 1'000'000;
constexpr int PIN = 78'498;
bitset<N + 5> not_prime;
int primes[PIN + 5], pcnt = 0;
int phi[N + 5];
int mu[N + 5];
ll smu[N + 5];
ll sphi[N + 5];
unordered_map<ll, ll> mp_mu;
unordered_map<ll, i128> mp_phi;
void sieve() {
    phi[1] = 1;
    mu[1] = 1;
    for (int i = 2; i <= N; i++) {
        if (!not_prime[i]) {
            primes[pcnt] = i, pcnt++;
            phi[i] = i - 1;
            mu[i] = -1;
        }
        for (int j = 0; j < pcnt && 1LL * i * primes[j] <= N; j++) {
            int pj = primes[j];
            not_prime[i * pj] = 1;

            if (i % pj == 0) {
                phi[i * pj] = phi[i] * pj;
                mu[i * pj] = 0;
                break;
            }

            phi[i * pj] = phi[i] * (pj - 1);
            mu[i * pj] = -mu[i];
        }
    }

    for (int i = 1; i <= N; i++) {
        smu[i] = smu[i - 1] + mu[i];
        sphi[i] = sphi[i - 1] + phi[i];
    }
}

ll getsmu(ll n) {
    // 莫比乌斯函数的前缀和
    if (n <= N) {
        return smu[n];
    }
    if (mp_mu.count(n)) {
        return mp_mu[n];
    }

    ll ans = 1;

    for (ll l = 2, r; l <= n; l = r + 1) {
        ll q = n / l;
        r = n / q;
        ans -= (r - l + 1) * getsmu(q);
    }
    return mp_mu[n] = ans;
}

i128 getsphi(ll n) {
    // 欧拉函数的前缀和
    if (n <= N) {
        return sphi[n];
    }
    if (mp_phi.count(n)) {
        return mp_phi[n];
    }

    i128 ans = (i128)n * (n + 1) / 2;

    for (ll l = 2, r; l <= n; l = r + 1) {
        ll q = n / l;
        r = n / q;
        ans -= (i128)(r - l + 1) * getsphi(q);
    }

    return mp_phi[n] = ans;
}
```

## 杂项结论
### 麦乐鸡定理
对于互质整数$n, m$来说, $an + bm(a \ge 0, b \ge 0)$不能表示出的最大整数为$nm - n - m$.

### 抽屉原理
$n$个物体放到$m$个抽屉中, 抽屉中物品的最大值大于等于$ceil(n / m)$

### Dilworth定理
偏序集中, 最大链长度等于最多反链划分数, 对偶结论亦成立.

### 取模与整数除法
`x % i = x - i * (x / i)`

### 欧拉函数性质
1. $[1, n]$中与$n$互质的数之和为$\frac{n\varphi(n)}{2}$
2. 若$a, b$互质, 则$\varphi(ab) = \varphi(a)\varphi(b)$
3. 若$p$是$n$的质因子: 若$p$的次数大于等于2, 则$\varphi(n) = \varphi(\frac{n}{p})p$ ; 否则$\varphi(n) = \varphi(\frac{n}{p}) (p - 1)$
4. $\sum_{d|n}\varphi(d) = n$
5. $\sum_{i = 1}^n\gcd(i, n) = \sum_{d|n}\varphi(d)\frac{n}{d}$. (欧拉反演)


# 代数
## 多项式
### FFT
```cpp
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
```

### NTT
```cpp
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
```

### 三模NTT
```cpp
// NTT...
ll mul(ll a, ll b, ll p) {
    return (i128)a * b % p;
}
ll qkp(ll a, ll b, ll p) {
    ll ret = 1 % p;
    while (b > 0) {
        if (b & 1) {
            ret = mul(ret, a, p);
        }
        b >>= 1;
        a = mul(a, a, p);
    }
    return ret;
}
ll inv(ll x, ll p) {
    return qkp(x, p - 2, p);
}
vector<ll> mtt(const vector<ll>& a, const vector<ll>& b, ll mod) {
    if (a.empty() || b.empty()) {
        return {};
    }
    constexpr int M1 = 998244353;
    constexpr int M2 = 1004535809;
    constexpr int M3 = 469762049;
    using Z1 = ModInt<M1>;
    using Z2 = ModInt<M2>;
    using Z3 = ModInt<M3>;
    vector<Z1> a1(a.begin(), a.end()), b1(b.begin(), b.end());
    vector<Z2> a2(a.begin(), a.end()), b2(b.begin(), b.end());
    vector<Z3> a3(a.begin(), a.end()), b3(b.begin(), b.end());
    
    auto c1 = convolution(a1, b1);
    auto c2 = convolution(a2, b2);
    auto c3 = convolution(a3, b3);

    ll inv12 = inv(M1, M2);
    ll modm3 = mul(M1, M2, M3);
    ll inv123 = inv(modm3, M3);
    
    int sz = c1.size();
    vector<ll> ans(sz);
    for (int i = 0; i < sz; i++) {
        ll v1 = c1[i].val, v2 = c2[i].val, v3 = c3[i].val;
        
        ll k1 = (v2 - v1 % M2 + M2) % M2 * inv12 % M2;
        ll x12 = v1 + k1 * M1;

        ll k2 = (v3 - x12 % M3 + M3) % M3 * inv123 % M3;
        
        ll m12 = mul(M1, M2, mod);
        ll val = (x12 % mod + k2 % mod * m12 % mod) % mod;
        
        ans[i] = (val + mod) % mod;
    }
    return ans;
}
```

### MTT
```cpp
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
```

### FWT/FMT
```cpp
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
```

### 求导积分
```cpp
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
```

### 求逆
```cpp
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
```

### 除法
```cpp
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
```

### 开根
```cpp
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
```

### 指数/对数
```cpp
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
```

### 幂
```cpp
template <typename Z>
vector<Z> pow(vector<Z> a, ll b, int n) {
    if (n == 0) return {};
    if (b == 0) {
        vector<Z> ret(n);
        ret[0] = 1;
        return ret;
    }
    a.resize(n);
    int t = n;
    for (int i = 0; i < n; i++) {
        if (a[i] != Z(0)) {
            t = i;
            break;
        }
    }
    vector<Z> ans(n);
    if (t == n) {
        return ans;
    }
    if ((__int128)t * b >= n) {
        return ans;
    }
    int s = t * b;
    int m = n - s;
    vector<Z> c(m);
    for (int i = 0; i < m && i + t < n; i++) {
        c[i] = a[i + t] / a[t];
    }
    c = ln(c, m);
    for (int i = 0; i < m; i++) {
        c[i] *= Z(b);
    }
    c = exp(c, m);
    Z l = a[t].pow(b);
    for (int i = 0; i < m; i++) {
        ans[i + s] = c[i] * l;
    }
    return ans;
}
```

### 多点求值
```cpp
// 卷积, 求逆...
template <typename Z>
vector<Z> mulT(vector<Z> a, vector<Z> b, int need) {
    vector<Z> ret(need);
    if (a.empty() || b.empty() || need == 0) {
        return ret;
    }
    int n = b.size();
    reverse(b.begin(), b.end());
    auto c = convolution(a, b);
    for (int i = 0; i < need; i++) {
        int p = i + n - 1;
        if (p < (int)c.size()) {
            ret[i] = c[p];
        }
    }
    return ret;
}

template <typename Z>
vector<Z> multipoint_eval(vector<Z> f, vector<Z> x) {
    int m = x.size();
    vector<Z> ans(m);
    if (m == 0) {
        return ans;
    }
    if (f.empty()) {
        return ans;
    }
    int n = max((int)f.size(), m);
    x.resize(n);
    vector<vector<Z>> q(4 * n);
    auto build = [&](auto self, int p, int l, int r) -> void {
        if (r - l == 1) {
            q[p] = {Z(1), -x[l]};
            return;
        }
        int mid = (l + r) / 2;
        self(self, p << 1, l, mid);
        self(self, p << 1 | 1, mid, r);
        q[p] = convolution(q[p << 1], q[p << 1 | 1]);
    };
    auto work = [&](auto self, int p, int l, int r, const vector<Z>& num) -> void {
        if (r - l == 1) {
            if (l < m) {
                ans[l] = num.empty() ? Z(0) : num[0];
            }
            return;
        }
        int mid = (l + r) / 2;
        auto vl = mulT(num, q[p << 1 | 1], mid - l);
        auto vr = mulT(num, q[p << 1], r - mid);
        self(self, p << 1, l, mid, vl);
        self(self, p << 1 | 1, mid, r, vr);
    };
    build(build, 1, 0, n);
    auto root = mulT(f, inv(q[1], n), n);
    work(work, 1, 0, n, root);
    return ans;
}
```

### 快速插值
```cpp
template <typename Z>
vector<Z> interpolate(vector<Z> x, vector<Z> y) {
    int n = x.size();
    assert((int)y.size() == n);
    if (n == 0) {
        return {};
    }
    vector<vector<Z>> q(4 * n);
    auto build = [&](auto self, int p, int l, int r) -> void {
        if (r - l == 1) {
            // x - x_l
            q[p] = {-x[l], Z(1)};
            return;
        }

        int mid = (l + r) / 2;

        self(self, p << 1, l, mid);
        self(self, p << 1 | 1, mid, r);

        q[p] = convolution(q[p << 1], q[p << 1 | 1]);
    };

    build(build, 1, 0, n);
    auto d = multipoint_eval(deriv(q[1]), x);
    vector<Z> w(n);
    for (int i = 0; i < n; i++) {
        w[i] = y[i] / d[i];
    }

    auto work = [&](auto self, int p, int l, int r) -> vector<Z> {
        if (r - l == 1) {
            return vector<Z>{w[l]};
        }

        int mid = (l + r) / 2;

        auto L = self(self, p << 1, l, mid);
        auto R = self(self, p << 1 | 1, mid, r);

        auto A = convolution(L, q[p << 1 | 1]);
        auto B = convolution(R, q[p << 1]);

        int len = max(A.size(), B.size());
        A.resize(len);
        B.resize(len);

        for (int i = 0; i < len; i++) {
            A[i] += B[i];
        }

        return A;
    };
    auto ans = work(work, 1, 0, n);
    ans.resize(n);
    return ans;
}
```

### 快速阶乘
```cpp
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
```

### 多点快速阶乘
```cpp
template <typename Z>
vector<Z> batch_inv(const vector<Z>& a) {
    int n = a.size();
    vector<Z> pre(n + 1), suf(n + 1), ret(n);
    pre[0] = 1;
    for (int i = 0; i < n; i++) {
        pre[i + 1] = pre[i] * a[i];
    }
    Z inv_all = pre[n].inv();
    for (int i = n - 1; i >= 0; i--) {
        ret[i] = inv_all * pre[i];
        inv_all *= a[i];
    }
    return ret;
}

template <typename Z>
vector<Z> many_factorials(const vector<int>& q) {
    constexpr int LIM = 100000;
    int n = q.size();
    int mod = Z::mod();
    vector<Z> ans(n, Z(1));
    vector<int> need_inv(n);
    vector<pair<int, int>> reg;
    vector<pair<int, int>> odd;
    reg.reserve(n);
    odd.reserve(n * 8);
    for (int id = 0; id < n; id++) {
        int t = q[id];
        if (t >= mod) {
            ans[id] = 0;
            continue;
        }
        if (t >= mod / 2) {
            int r = mod - 1 - t;
            if ((r & 1) == 0) {
                ans[id] = -ans[id];
            }
            need_inv[id] = 1;
            t = r;
        }
        long long pow2 = 0;
        while (t > LIM) {
            int x = (t - 1) / 2;
            odd.emplace_back(x, id);
            pow2 += t / 2;
            t >>= 1;
        }
        ans[id] *= Z(2).pow(pow2 % (mod - 1));
        reg.emplace_back(t, id);
    }
    sort(reg.begin(), reg.end());
    int cur = 0;
    Z prod = 1;
    for (auto [t, id] : reg) {
        while (cur < t) {
            cur++;
            prod *= cur;
        }
        ans[id] *= prod;
    }
    sort(odd.begin(), odd.end());
    int ocur = -1;
    Z oprod = 1;
    for (auto [x, id] : odd) {
        while (ocur < x) {
            ocur++;
            oprod *= Z(2LL * ocur + 1);
        }
        ans[id] *= oprod;
    }
    vector<Z> need;
    vector<int> pos;
    for (int i = 0; i < n; i++) {
        if (need_inv[i]) {
            pos.emplace_back(i);
            need.emplace_back(ans[i]);
        }
    }
    auto invs = batch_inv(need);
    for (int i = 0; i < (int)pos.size(); i++) {
        ans[pos[i]] = invs[i];
    }
    return ans;
}
```

## 线性代数
### 矩阵和列向量
```cpp
template <class S, int N>
struct ColVector {
    array<S, N> a;
    ColVector(S def = S()) {
        fill(a.begin(), a.end(), def);
    }
    
    S& operator[](int i) {
        return a[i];
    }
    const S& operator[](int i) const {
        return a[i];
    }
};

template <typename S, int N>
struct Matrix {
    array<array<S, N>, N> a;
    Matrix(S def = S()) {
        for (int i = 0; i < N; i++) {
            fill(a[i].begin(), a[i].end(), def);
        }
    }
    using M = Matrix;

    S* operator[](int i) {
        return a[i].data();
    }
    const S* operator[](int i) const {
        return a[i].data();
    }
    
    // 单位矩阵
    constexpr static M e() {
        M ret;
        for (int i = 0; i < N; i++) {
            ret[i][i] = 1;
        }
        return ret;
    }
    
    // 加法
    friend M operator+(M lt, const M& rt) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                lt[i][j] += rt[i][j];
            }
        }
        return lt;
    }

    // 乘法
    friend M operator*(const M& lt, const M& rt) {
        M ret;
        for (int i = 0; i < N; i++) {
            for (int k = 0; k < N; k++) {
                if (lt[i][k] == S()) {
                    continue;
                }
                
                for (int j = 0; j < N; j++) {
                    ret[i][j] += lt[i][k] * rt[k][j];
                }
            }
        }
        return ret;
    }

    M pow(ll b) const {
        M ret = e();
        M t = *this;
        while (b > 0) {
            if (b & 1) {
                ret = ret * t;
            }
            
            t = t * t;
            b >>= 1;
        }
        return ret;
    }

    ColVector<S, N> operator*(const ColVector<S, N>& vec) {
        ColVector<S, N> ret;
        for (int i = 0; i < N; i++) {
            for (int k = 0; k < N; k++) {
                if (a[i][k] == S()) {
                    continue;
                }
                
                ret[i] = ret[i] + (a[i][k] * vec[k]);
            }
        }
        return ret;
    }
};

template <int N>
struct Matrix_mp {
    array<array<ll, N>, N> a;
    Matrix_mp() {
        for (int i = 0; i < N; i++) {
            fill(a[i].begin(), a[i].end(), INFLL);
        }
    }

    ll* operator[](int i) {
        return a[i].data();
    }
    const ll* operator[](int i) const {
        return a[i].data();
    }
    
    static Matrix_mp e() {
        Matrix_mp ret;
        for (int i = 0; i < N; i++) {
            ret[i][i] = 0;
        }
        return ret;
    }
    
    friend Matrix_mp operator+(Matrix_mp lt, const Matrix_mp& rt) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                lt[i][j] = min(lt[i][j], rt[i][j]);
            }
        }
        return lt;
    }
    friend Matrix_mp operator*(const Matrix_mp& lt, const Matrix_mp& rt) {
        Matrix_mp ret;
        for (int i = 0; i < N; i++) {
            for (int k = 0; k < N; k++) {
                if (lt[i][k] >= INFLL) {
                    continue;
                }
                
                for (int j = 0; j < N; j++) {
                    ll now;
                    if (rt[k][j] >= INFLL) {
                        now = INFLL;
                    } else {
                        now = lt[i][k] + rt[k][j];
                    }
                    ret[i][j] = min(ret[i][j], now);
                }
            }
        }
        return ret;
    }
    
    Matrix_mp pow(ll b) const {
        Matrix_mp ret = e();
        Matrix_mp t = *this;
        while (b > 0) {
            if (b & 1) {
                ret = ret * t;
            }
            
            t = t * t;
            b >>= 1;
        }
        return ret;
    }
    
    ColVector<ll, N> operator*(const ColVector<ll, N>& vec) {
        ColVector<ll, N> ret(INFLL);
        for (int i = 0; i < N; i++) {
            for (int k = 0; k < N; k++) {
                if (a[i][k] >= INFLL) {
                    continue;
                }
                ll now;
                if (a[i][k] >= INFLL || vec[k] >= INFLL) {
                    now = INFLL;
                } else {
                    now = a[i][k] + vec[k];
                }
                ret[i] = min(ret[i], now);
            }
        }
        return ret;
    }
};
```

### 高斯消元(整数)
```cpp
template <typename S, int N>
struct GaussJordan {
    Matrix<S, N> A;
    ColVector<S, N> x;
    ColVector<S, N> b;
    
    int solve(int n, int m) {
        int r = 0;
        for (int c = 0; r < n && c < m; c++) {
            int pivot = r;
            while (pivot < n && A[pivot][c] == S()) {
                pivot++;
            }
            if (pivot == n) {
                continue;
            }
            if (pivot != r) {
                swap(A.a[pivot], A.a[r]);
                swap(b.a[pivot], b.a[r]);
            }
            S inv = S(1) / A[r][c];
            for (int j = c; j < m; j++) {
                A[r][j] = A[r][j] * inv;
            }
            b[r] = b[r] * inv;
            
            for (int i = 0; i < n; i++) {
                if (i != r && A[i][c] != S()) {
                    S factor = A[i][c];
                    for (int j = c; j < m; j++) {
                        A[i][j] = A[i][j] - factor * A[r][j];
                    }
                    b[i] = b[i] - factor * b[r];
                }
            }
            r++;
        }
        
        for (int i = r; i < n; i++) {
            if (b[i] != S()) {
                return -1;
            }
        }
        
        for (int i = 0; i < n; i++) {
            x[i] = S();
        }
        
        for (int i = 0; i < r; i++) {
            int maj = -1;
            for (int j = 0; j < m; j++) {
                if (A[i][j] != S()) {
                    maj = j;
                    break;
                }
            }
            if (maj != -1) {
                x[maj] = b[i];
            }
        }
        if (r < n) {
            return n - r;
        } else {
            return 0;
        }
    }
};
```

### 高斯消元(实数)
```cpp
// for real
template <typename S, int N>
struct GaussJordan {
    Matrix<S, N> A;
    ColVector<S, N> x;
    ColVector<S, N> b;
    
    int solve(int n, int m) {
        int r = 0;
        for (int c = 0; r < n && c < m; c++) {
            int pivot = r;
            for (int i = r + 1; i < n; i++) {
                if (fabs(A[i][c]) > fabs(A[pivot][c])) {
                    pivot = i;
                }
            }
            if (fabs(A[pivot][c] < EPS)) {
                continue;
            }
            
            if (pivot != r) {
                swap(A.a[pivot], A.a[r]);
                swap(b.a[pivot], b.a[r]);
            }
            S inv = S(1) / A[r][c];
            for (int j = c; j < m; j++) {
                A[r][j] = A[r][j] * inv;
            }
            b[r] = b[r] * inv;
            
            for (int i = 0; i < n; i++) {
                if (i != r && A[i][c] != S()) {
                    S factor = A[i][c];
                    for (int j = c; j < m; j++) {
                        A[i][j] = A[i][j] - factor * A[r][j];
                    }
                    b[i] = b[i] - factor * b[r];
                }
            }
            r++;
        }
        
        for (int i = r; i < n; i++) {
            if (fabs(b[i]) > EPS) {
                return -1;
            }
        }
        
        for (int i = 0; i < n; i++) {
            x[i] = S();
        }
        
        for (int i = 0; i < r; i++) {
            int maj = -1;
            for (int j = 0; j < m; j++) {
                if (fabs(A[i][j]) > EPS) {
                    maj = j;
                    break;
                }
            }
            if (maj != -1) {
                x[maj] = b[i];
            }
        }
        if (r < n) {
            return n - r;
        } else {
            return 0;
        }
    }
};
```

### 高斯消元(异或)
```cpp
template <int DIM>
struct GaussXor {
    array<bitset<DIM>, DIM> mat; // 存增广矩阵, 行从 0 到 n - 1, 列从 0 到 m

    array<int, DIM> ans;
    // 解异或线性方程组, 无解返回-1, 唯一解返回0, 无穷解返回自由变元数
    int solve(int n, int m) {
        // n 个方程, m 个未知量(即系数矩阵为 n 行 m 列)
        int r = 0, c = 0;
        for (; r < n && c < m; c++) {
            int pivot = r;
            while (pivot < n && !mat[pivot][c]) {
                pivot++;
            }
            if (pivot == n) {
                continue;
            }
            if (pivot != r) {
                swap(mat[pivot], mat[r]);
            }
            for (int i = 0; i < n; i++) {
                if (i != r && mat[i][c]) {
                    mat[i] ^= mat[r];
                }
            }
            r++;
        }

        for (int i = r; i < n; i++) {
            if (mat[i][m]) {
                return -1;
            }
        }
        if (r < m) {
            return m - r;
        }
        for (int i = 0; i < m; i++) {
            if (i < n) {
                ans[i] = mat[i][m];
            }
        }
        return 0;
    }
};
```

### 异或线性基
```cpp
template <int DIM = 64>
struct LinearBasis {
    array<ull, DIM> p = {};
    int cnt = 0;
    bool zero = 0;
    
    bool insert(ull x) {
        for (int i = DIM - 1; i >= 0; i--) {
            if ((x & (1ULL << i)) == 0) {
                continue;
            }
            if (!p[i]) {
                p[i] = x;
                cnt++;
                return true;
            }
            x ^= p[i];
        }
        zero = true;
        return false;
    }
    
    bool check(ull x) {
        for (int i = DIM - 1; i >= 0; i--) {
            if ((x & (1ULL << i)) == 0) {
                continue;
            }
            if (!p[i]) {
                return false;
            }
            x ^= p[i];
        }
        return true;
    }
    
    ull max() {
        ull ret = 0;
        for (int i = DIM - 1; i >= 0; i--) {
            if ((ret ^ p[i]) > ret) {
                ret ^= p[i];
            }
        }
        return ret;
    }
    
    ull min() {
        if (zero) {
            return 0;
        }
        for (int i = 0; i < DIM; i++) {
            if (p[i]) {
                return p[i];
            }
        }
    }
    
    // 合并
    friend bool operator+(const LinearBasis& a, const LinearBasis& b) {
        LinearBasis ret = a;
        for (int i = 0; i < DIM; i++) {
            if (b.p[i]) {
                ret.insert(b.p[i]);
            }
        }
        return ret;
    }
};

```