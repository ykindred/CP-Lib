#pragma once
#include "../head.hpp"
#include "exgcd.hpp"
// ok

// 依赖norm128, exgcd
// 要求模数大于0, lcm 在i128范围内. 返回 { 最小非负解, lcm }, 无解返回{ -1, -1 }
// O(nlog(max(m)))
inline array<i128, 2> excrt(const vector<ll>& b, const vector<ll>& m) {
    int n = b.size();
    i128 nowm = m[0], nowb = norm128(b[0], nowm);

    for (int i = 1; i < n; i++) {
        ll A = nowm % m[i], B = m[i];
        i128 C = (i128)b[i] - nowb % B;

        auto [y1, _, g] = exgcd(A, B);
        if (C % g != 0) {
            return {-1, -1};
        }

        i128 M = B / g;
        i128 y = norm128(y1, M) * norm128(C / g, M) % M;

        nowb += y * nowm;
        nowm *= M;
    }
    return {nowb, nowm};
}
