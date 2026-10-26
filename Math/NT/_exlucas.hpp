#pragma once
#include "../head.hpp"
#include "factor.hpp"
#include "tools.hpp"
#include "exgcd.hpp"

// 预处理合数模组合数；构造时传入正 int 模数，使用 nCr 查询，结果返回为 ll。
// 预处理时间、空间为 $O(\sum p^e)$，只适合各素数幂不大的模数。
struct ExLucas {
    // 保存一个素数幂模数及其去除素因子的阶乘表，供 ExLucas 内部使用。
    // 构造时指定质数 p 和正指数 e，查询结果都按该素数幂取模。
    struct Part {
        ll p, mod;
        int e;
        vector<ll> fact;
        // 传入质数 p 和正指数 e，预处理模 $p^e$ 的阶乘表，要求该素数幂不超过外层模数。
        Part(ll p, int e) : p(p), mod(1), e(e) {
            for (int i = 0; i < e; i++) mod *= p;
            fact.resize(mod + 1);
            fact[0] = 1;
            for (ll i = 1; i <= mod; i++) {
                fact[i] = fact[i - 1];
                if (i % p) fact[i] = mul(fact[i], i, mod);
            }
        }
        // 传入非负整数 n，返回 n 的阶乘中素因子 p 的指数。
        ll count(ll n) const {
            ll ret = 0;
            for (; n; n /= p) ret += n / p;
            return ret;
        }
        
        // 传入非负整数 n，返回从 n 的阶乘中去掉所有 p 因子后，剩余部分模当前 mod 的值。
        // 一个整块中所有与 p 互质的数之积只能是 $1$ 或 $-1$，因此只需检查整块数的奇偶。
        ll unit_fact(ll n) const {
            ll ret = 1;
            for (; n; n /= p) {
                if (n / mod & 1) ret = mul(ret, fact[mod], mod);
                ret = mul(ret, fact[n % mod], mod);
            }
            return ret;
        }
        // 内部查询：传入合法的 n、r，返回组合数模当前素数幂的值，要求 $0\le r\le n$。
        ll nCr(ll n, ll r) const {
            ll k = count(n) - count(r) - count(n - r);
            if (k >= e) return 0;
            ll denom = mul(unit_fact(r), unit_fact(n - r), mod);
            ll ret = mul(unit_fact(n), inv_exgcd(denom, mod), mod);
            return mul(ret, qkp(p, k, mod), mod);
        }
    };
    int mod;
    vector<Part> parts;
    // 传入正 int 模数 mod，分解模数并建立各素数幂的表；模数为一时无需建立表。
    ExLucas(int mod) : mod(mod) {
        assert(mod > 0);
        for (auto [p, e] : prime_factor(mod)) parts.emplace_back(p, e);
    }
    // 传入非负整数 n 和选取数 r，返回组合数模构造时指定的模数的最小非负值。
    // 非法 r 返回零；每个素数幂的查询时间约为 $O(\log_p n+\log M)$，M 是该素数幂模数。
    ll nCr(ll n, ll r) const {
        if (!(0 <= r && r <= n) || mod == 1) return 0;
        ll ret = 0;
        for (const auto& part : parts) {
            ll m = mod / part.mod;
            ret = (ret + (i128)part.nCr(n, r) * m * inv_exgcd(m, part.mod)) % mod;
        }
        return ret;
    }
};
