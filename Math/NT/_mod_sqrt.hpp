#pragma once
#include "../head.hpp"
#include "tools.hpp"

// 传入整数 a 和质数 p，求模平方根，返回较小的非负根；无解返回 -1。
// p 至少为二且须在有符号 ll 范围内；Tonelli-Shanks 主循环为 $O(\log^2 p)$，另需寻找二次非剩余。
inline ll mod_sqrt(ll a, ll p) {
    assert(p >= 2);
    a = norm(a, p);
    if (a == 0 || p == 2) return a;
    if (qkp(a, (p - 1) / 2, p) != 1) return -1;
    if (p % 4 == 3) {
        ll x = qkp(a, p / 4 + 1, p);
        return min(x, p - x);
    }
    ll d = p - 1;
    int s = __builtin_ctzll(d);
    d >>= s;
    ll z = 2;
    while (qkp(z, (p - 1) / 2, p) == 1) z++;
    ll x = qkp(a, (d + 1) / 2, p), t = qkp(a, d, p), c = qkp(z, d, p);
    while (t != 1) {
        int i = 0;
        ll cur = t;
        while (cur != 1) {
            cur = mul(cur, cur, p);
            i++;
        }
        ll b = c;
        for (int j = 0; j < s - i - 1; j++) b = mul(b, b, p);
        x = mul(x, b, p);
        c = mul(b, b, p);
        t = mul(t, c, p);
        s = i;
    }
    return min(x, p - x);
}
