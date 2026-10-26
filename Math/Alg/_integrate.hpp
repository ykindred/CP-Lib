#pragma once
#include "../head.hpp"

// 传入函数 f、积分端点 l 和 r，返回定积分的 long double 估计值。
// eps 是目标绝对误差，depth 是递归深度上限；f 应在闭区间内有限且足够光滑。
// 不连续点需分段，端点奇点需换元；到达深度上限时返回当前估计，不保证已满足 eps。
template <typename F>
long double integrate(F f, long double l, long double r,
                      long double eps = 1e-9L, int depth = 25) {
    using Real = long double;
    assert(eps > 0 && depth >= 0);
    if (l == r) return 0;
    if (l > r) return -integrate(f, r, l, eps, depth);
    // 传入区间端点 l、r，以及左端点、中点、右端点的函数值 a、b、c，返回 Simpson 估计。
    auto simpson = [](Real l, Real r, Real a, Real b, Real c) {
        return (r - l) * (a + 4 * b + c) / 6;
    };
    // 接收区间、已缓存的函数值 a、b、c、旧估计 val、误差额度 eps 和剩余深度 depth。
    // 返回细分后的积分估计；self 用于递归，两侧区间分别使用一半误差额度。
    auto work = [&](auto self, Real l, Real r, Real a, Real b, Real c,
                    Real val, Real eps, int depth) -> Real {
        Real mid = l + (r - l) / 2;
        Real lm = l + (mid - l) / 2, rm = mid + (r - mid) / 2;
        if (lm == l || rm == r) return val;
        Real d = f(lm), e = f(rm);
        Real lt = simpson(l, mid, a, d, b), rt = simpson(mid, r, b, e, c);
        Real delta = lt + rt - val;
        if (depth == 0 || abs(delta) <= 15 * eps) return lt + rt + delta / 15;
        return self(self, l, mid, a, d, b, lt, eps / 2, depth - 1)
             + self(self, mid, r, b, e, c, rt, eps / 2, depth - 1);
    };
    Real mid = l + (r - l) / 2, a = f(l), b = f(mid), c = f(r);
    return work(work, l, r, a, b, c, simpson(l, r, a, b, c), eps, depth);
}
