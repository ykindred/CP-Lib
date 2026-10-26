#pragma once
#include "../head.hpp"

// 传入非负整数 n 和回调 work，按整数商分块依次调用 work(l, r, q)。
// 回调收到闭区间 [l, r] 及其中相同的商 q；本函数无返回值，块数为 $O(\sqrt n)$。
template <typename F>
void quotient_blocks(ll n, F work) {
    assert(n >= 0);
    for (ll l = 1, r; l <= n; l = r + 1) {
        ll q = n / l;
        r = n / q;
        work(l, r, q);
        if (r == n) break;
    }
}

// 传入非负上限 n、m，以及权函数 f 的前缀和 s，返回带权的整数商乘积之和：
// $\sum_{d=1}^{\min(n,m)}f(d)\lfloor n/d\rfloor\lfloor m/d\rfloor$，要求 s[0] 为零且长度大于 $\min(n,m)$。
// 结果类型与 s 的元素类型相同；较大整数结果用 i128，模意义结果用 mint。
template <typename Z>
Z quotient_sum(ll n, ll m, const vector<Z>& s) {
    assert(n >= 0 && m >= 0 && (ll)s.size() > min(n, m));
    Z ret = 0;
    ll lim = min(n, m);
    for (ll l = 1, r; l <= lim; l = r + 1) {
        ll a = n / l, b = m / l;
        r = min(n / a, m / b);
        ret += (s[r] - s[l - 1]) * Z(a) * Z(b);
        if (r == lim) break;
    }
    return ret;
}

