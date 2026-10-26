#pragma once
#include "../head.hpp"

// ok
template <int P> 
struct ModInt {
    u32 val;
    constexpr ModInt(ll x = 0) : val((x %= P) < 0 ? x + P : x) {}
    // 注意i128等类型需要先转成ll
    using M = ModInt;

    // 希望编译期计算的话以下运算符全部补上constexpr
    M &operator+=(const M &p) {
        if ((val += p.val) >= P) {
            val -= P;
        }
        return *this;
    }
    M &operator-=(const M &p) {
        if ((val += P - p.val) >= P) {
            val -= P;
        }
        return *this;
    }
    M &operator*=(const M &p) {
        val = u64(val) * p.val % P;
        return *this;
    }
    M &operator/=(const M &p) { return (*this) *= p.inv(); }
    M operator-() const { return M(val ? P - val : u32(0)); }
    M operator+(const M &p) const { return M(*this) += p; }
    M operator-(const M &p) const { return M(*this) -= p; }
    M operator*(const M &p) const { return M(*this) *= p; }
    M operator/(const M &p) const { return M(*this) /= p; }
    bool operator==(const M &p) const { return val == p.val; }
    bool operator!=(const M &p) const { return val != p.val; }
    M pow(ll n) const {
        if (n < 0) return inv().pow(-n);
        M ret(1), mul(val);
        while (n > 0) {
            if (n & 1) ret *= mul;
            mul *= mul;
            n >>= 1;
        }
        return ret;
    }
    M inv() const {
        int a = val, b = P, u = 1, v = 0, t;
        while (b > 0) {
            t = a / b;
            a -= t * b;
            u -= t * v;
            swap(a, b);
            swap(u, v);
        }
        return M(u);
    }
    constexpr static M raw(u32 x) {
        M ret;
        ret.val = x;
        return ret;
    }
    constexpr static int mod() {
        return P;
    }
};
using mint = ModInt<MOD>;