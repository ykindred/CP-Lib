// 依赖：
#include "../NumberTheory/modint.hpp"

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
