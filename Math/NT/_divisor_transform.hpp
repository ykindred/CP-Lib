#pragma once
#include "../head.hpp"

// 传入按正整数下标存放的数组 a，原地把每项替换为其所有约数位置的值之和。
// invert 为真时做逆变换；a[0] 不参与，不返回新数组，时间为 $O(n\log n)$。
template <typename Z>
void divisor_zeta(vector<Z>& a, bool invert = false) {
    int n = (int)a.size() - 1;
    if (!invert) {
        for (int i = n; i >= 1; i--) {
            for (int j = i + i; j <= n; j += i) a[j] += a[i];
        }
    } else {
        for (int i = 1; i <= n; i++) {
            for (int j = i + i; j <= n; j += i) a[j] -= a[i];
        }
    }
}

// 传入按正整数下标存放的数组 a，原地把每项替换为其所有倍数位置的值之和。
// invert 为真时做逆变换；a[0] 不参与，不返回新数组，时间为 $O(n\log n)$。
template <typename Z>
void multiple_zeta(vector<Z>& a, bool invert = false) {
    int n = (int)a.size() - 1;
    if (!invert) {
        for (int i = 1; i <= n; i++) {
            for (int j = i + i; j <= n; j += i) a[i] += a[j];
        }
    } else {
        for (int i = n; i >= 1; i--) {
            for (int j = i + i; j <= n; j += i) a[i] -= a[j];
        }
    }
}

// 传入等长、非空的数组 a、b，返回按最大公约数归类的卷积。
// 返回项为 $c_k=\sum_{\gcd(i,j)=k}a_i b_j$；输入下标从一开始，返回的零号项为零。
template <typename Z>
vector<Z> convolution_gcd(vector<Z> a, vector<Z> b) {
    assert(!a.empty() && a.size() == b.size());
    multiple_zeta(a);
    multiple_zeta(b);
    for (int i = 1; i < (int)a.size(); i++) a[i] *= b[i];
    multiple_zeta(a, true);
    a[0] = 0;
    return a;
}

// 传入等长、非空的数组 a、b，返回按最小公倍数归类的卷积。
// 返回项为 $c_k=\sum_{\operatorname{lcm}(i,j)=k}a_i b_j$，仅保留原数组范围内的下标；零号项为零。
template <typename Z>
vector<Z> convolution_lcm(vector<Z> a, vector<Z> b) {
    assert(!a.empty() && a.size() == b.size());
    divisor_zeta(a);
    divisor_zeta(b);
    for (int i = 1; i < (int)a.size(); i++) a[i] *= b[i];
    divisor_zeta(a, true);
    a[0] = 0;
    return a;
}
