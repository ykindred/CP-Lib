#pragma once
#include "../head.hpp"
#include "../NT/modint.hpp"
#include "../NT/factor.hpp"
#include "../NT/_divisors.hpp"

// 传入完整置换群 group 和颜色数 colors，返回把群作用视为相同后的不同染色数量。
// 每个置换 p 用 p[i] 表示 i 的像，位置从零编号；各位置可独立染色，群大小须在 Z 中可逆。
// 设位置数为 n，时间为 $O(|G|(n+\log n))$。
template <typename Z>
Z burnside(const vector<vector<int>>& group, Z colors) {
    assert(!group.empty());
    Z ret = 0;
    for (const auto& p : group) {
        int n = p.size(), cnt = 0;
        vector<bool> vis(n);
        for (int i = 0; i < n; i++) {
            if (vis[i]) continue;
            cnt++;
            for (int j = i; !vis[j]; j = p[j]) vis[j] = true;
        }
        ret += colors.pow(cnt);
    }
    return ret / Z(group.size());
}

// 传入正位置数 n 和颜色数 colors，返回所有旋转的不动染色数之和，尚未除以 n。
// 试除分解 n 的时间为 $O(\sqrt n)$，其余时间为 $O(\tau(n)(\omega(n)+\log n))$。
template <typename Z>
Z rotation_sum(ll n, Z colors) {
    assert(n >= 1);
    auto fac = prime_factor(n);
    Z ret = 0;
    for (ll d : divisors(fac)) {
        ll phi = d;
        for (auto [p, e] : fac) if (d % p == 0) phi = phi / p * (p - 1);
        ret += Z(phi) * colors.pow(n / d);
    }
    return ret;
}

// 传入正位置数 n 和颜色数 colors，返回只将旋转视为相同的项链数量。
// 要求 n 在 Z 中可逆；各位置可独立染色，颜色有标号。
template <typename Z>
Z necklaces(ll n, Z colors) {
    return rotation_sum(n, colors) / Z(n);
}
// 传入正位置数 n 和颜色数 colors，返回将旋转、翻转均视为相同的手链数量。
// 要求 $2n$ 在 Z 中可逆；各位置可独立染色，颜色有标号。
template <typename Z>
Z bracelets(ll n, Z colors) {
    Z ret = rotation_sum(n, colors);
    if (n & 1) ret += Z(n) * colors.pow(n / 2 + 1);
    else ret += Z(n / 2) * (colors.pow(n / 2 + 1) + colors.pow(n / 2));
    return ret / (Z(n) * 2);
}
