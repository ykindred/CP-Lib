// 依赖：
#include "../../Miscellaneous/hashtable.hpp"

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
