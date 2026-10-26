#pragma once
#include "_mobius.hpp"

// 预筛小范围并缓存大查询，用于求莫比乌斯函数、欧拉函数的前缀和。
// 构造时传入预筛上限，随后调用 getsmu 或 getsphi；预筛数组沿用 MobiusSieve。
struct DuJiao : MobiusSieve {
    unordered_map<ll, ll> mp_mu;
    unordered_map<ll, i128> mp_phi;
    // 传入正预筛上限 n，建立筛表和空查询缓存；预处理时间、空间均为 $O(n)$。
    DuJiao(int n = 1'000'000) : MobiusSieve(n) {}

    // 传入非负上限 x，返回从一到 x 的莫比乌斯函数之和，结果类型为 ll。
    ll getsmu(ll x) {
        assert(x >= 0);
        if (x <= n) return smu[x];
        auto it = mp_mu.find(x);
        if (it != mp_mu.end()) return it->second;
        i128 ret = 1;
        for (ll l = 2, r; l <= x; l = r + 1) {
            ll q = x / l;
            r = x / q;
            ret -= (i128)(r - l + 1) * getsmu(q);
            if (r == x) break;
        }
        return mp_mu[x] = (ll)ret;
    }
    // 传入非负上限 x，返回从一到 x 的欧拉函数之和，结果类型为 i128。
    i128 getsphi(ll x) {
        assert(x >= 0);
        if (x <= n) return sphi[x];
        auto it = mp_phi.find(x);
        if (it != mp_phi.end()) return it->second;
        i128 ret = (i128)x * ((i128)x + 1) / 2;
        for (ll l = 2, r; l <= x; l = r + 1) {
            ll q = x / l;
            r = x / q;
            ret -= (i128)(r - l + 1) * getsphi(q);
            if (r == x) break;
        }
        return mp_phi[x] = ret;
    }
};
