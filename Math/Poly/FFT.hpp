#pragma once
#include "../head.hpp"

// ok
namespace FFT {

using Real = double;
using cd = complex<Real>;

inline constexpr Real PI = 3.14159265358979323846;
inline int base = 1;
inline vector<int> rev = {0, 1};
inline vector<cd> roots = {cd(0, 0), cd(1, 0)};

// O(2^nbase), 单位根及位逆序缓存
inline void ensure_base(int nbase) {
    if (nbase <= base) return;

    rev.resize(1 << nbase);
    roots.resize(1 << nbase);

    for (int i = 0; i < (1 << nbase); i++) {
        rev[i] = (rev[i >> 1] >> 1)
               | ((i & 1) << (nbase - 1));
    }

    while (base < nbase) {
        Real angle = 2 * PI / (1 << (base + 1));
        for (int i = 1 << (base - 1); i < (1 << base); i++) {
            roots[i << 1] = roots[i];
            Real ang = angle * (2 * i + 1 - (1 << base));
            roots[i << 1 | 1] = cd(cos(ang), sin(ang));
        }
        base++;
    }
}

// O(n log n), n 必须为 2 的幂
inline void fft(vector<cd>& a, int n) {
    int zeros = __builtin_ctz((unsigned)n);
    ensure_base(zeros);
    int shift = base - zeros;

    for (int i = 0; i < n; i++) {
        int j = rev[i] >> shift;
        if (i < j) swap(a[i], a[j]);
    }

    for (int k = 1; k < n; k <<= 1) {
        for (int i = 0; i < n; i += k << 1) {
            for (int j = 0; j < k; j++) {
                cd z = a[i + j + k] * roots[j + k];
                a[i + j + k] = a[i + j] - z;
                a[i + j] += z;
            }
        }
    }
}

inline void ifft(vector<cd>& a, int n) {
    assert(n > 0 && (n & (n - 1)) == 0 && (int)a.size() >= n);

    reverse(a.begin() + 1, a.begin() + n);
    fft(a, n);
    for (int i = 0; i < n; i++) a[i] /= n;
}

// O(L log L), L = ceil_pow2(n + m - 1)
// 支持 vector<double> 和 vector<ll>
template <class T>
vector<T> convolution( const vector<T>& a, const vector<T>& b, int need = -1) {
    static_assert(is_same_v<T, Real> || is_same_v<T, ll>);

    if (a.empty() || b.empty()) return {};

    int full = (int)a.size() + (int)b.size() - 1;
    if (need < 0 || need > full) need = full;
    if (need == 0) return {};

    int na = min<int>(a.size(), need);
    int nb = min<int>(b.size(), need);

    vector<T> c(need);
    if (1LL * na * nb <= 30000) {
        for (int i = 0; i < na; i++) {
            for (int j = 0; j < nb && i + j < need; j++) {
                c[i + j] += a[i] * b[j];
            }
        }
        return c;
    }

    int n = 1;
    while (n < na + nb - 1) n <<= 1;

    vector<cd> fa(n), fb(n);
    for (int i = 0; i < na; i++) fa[i] = (Real)a[i];
    for (int i = 0; i < nb; i++) fb[i] = (Real)b[i];

    fft(fa, n);
    fft(fb, n);
    for (int i = 0; i < n; i++) fa[i] *= fb[i];
    ifft(fa, n);

    for (int i = 0; i < need; i++) {
        if constexpr (is_same_v<T, ll>) {
            c[i] = llround(fa[i].real());
        } else {
            c[i] = fa[i].real();
        }
    }
    return c;
}

} // namespace FFT
