// 平面直线方程转换（CG.md「平面直线方程转换」全节）
// 依赖 base.hpp
#include "base.hpp"

// 浮点斜率：注意平行于 y 轴时结果为 inf（CG.md: slope）
inline Real slope(const Pr& p1, const Pr& p2) {
    return (p1.y - p2.y) / (p1.x - p2.x);
}
inline Real slope(const Lr& l) {
    return slope(l.p, l.p + l.v);
}

// 分数精确斜率：返回最简分数 {分子, 分母}，分母恒正；平行于 y 轴时返回 {1, 0}
// CG.md 用 Frac<T> 实现（本仓库没有分数类，这里直接返回约分后的分子分母）
// 仅适用于整数类型
template <class T>
inline pair<T, T> slopeEx(const Pnt<T>& p1, const Pnt<T>& p2) {
    T u = p1.y - p2.y, v = p1.x - p2.x;
    if (v == 0) {
        return {1, 0}; // inf
    }
    if (u == 0) {
        return {0, 1};
    }
    T g = gcd(abs(u), abs(v));
    u /= g, v /= g;
    if (v < 0) { // 符号调整，恒保证分母为正
        u = -u, v = -v;
    }
    return {u, v};
}

// 两点式转一般式：返回 {A, B, C}，表示 Ax + By = C
// 可以处理平行于 x, y 轴、两点重合的情况（CG.md: getfun(Lt p)）
// 仅适用于整数类型
template <class T>
inline tuple<T, T, T> getfun(const Pnt<T>& p1, const Pnt<T>& p2) {
    T A = p1.y - p2.y, B = p2.x - p1.x, C = p1.x * A + p1.y * B;
    if (A < 0) { // 符号调整
        A = -A, B = -B, C = -C;
    } else if (A == 0) {
        if (B < 0) {
            B = -B, C = -C;
        } else if (B == 0 && C < 0) {
            C = -C;
        }
    }
    if (A == 0) { // 数值约分
        if (B == 0) {
            C = 0; // 两点重合
        } else {
            T g = gcd(abs(B), abs(C));
            B /= g, C /= g;
        }
    } else if (B == 0) {
        T g = gcd(abs(A), abs(C));
        A /= g, C /= g;
    } else {
        T g = gcd(gcd(abs(A), abs(B)), abs(C));
        A /= g, B /= g, C /= g;
    }
    return {A, B, C}; // Ax + By = C
}

// 一般式转两点式：返回直线上的两个点（用线段类型Seg装），A = B = 0 时不合法
// 整数点可能很大或不存在，故直接采用浮点数；与 x, y 轴有交点时取交点
// （CG.md: getfun(int A, int B, int C)）
inline Sr getfun(ll A, ll B, ll C) {
    Real x1 = 0, y1 = 0, x2 = 0, y2 = 0;
    if (A && B) { // 正常
        if (C) {
            x1 = 0, y1 = (Real)C / B;
            y2 = 0, x2 = (Real)C / A;
        } else { // 过原点
            x1 = 1, y1 = -(Real)A / B;
            x2 = 0, y2 = 0;
        }
    } else if (A && !B) { // 垂直
        if (C) {
            y1 = 0, x1 = (Real)C / A;
            y2 = 1, x2 = x1;
        } else {
            x1 = 0, y1 = 1;
            x2 = 0, y2 = 0;
        }
    } else if (!A && B) { // 水平
        if (C) {
            x1 = 0, y1 = (Real)C / B;
            x2 = 1, y2 = y1;
        } else {
            x1 = 1, y1 = 0;
            x2 = 0, y2 = 0;
        }
    } else { // 不合法，请特判
        assert(false);
    }
    return Sr({x1, y1}, {x2, y2});
}

// 抛物线与 x 轴是否相交及交点：0 没有交点；1 相切；2 有两个交点
// CG.md: getAns(ld a, ld b, ld c)，即 ax^2 + bx + c = 0
inline tuple<int, Real, Real> parabolaRoots(Real a, Real b, Real c) {
    Real delta = b * b - a * c * 4;
    if (delta < 0.) {
        return {0, 0, 0};
    }
    delta = sqrtl(delta);
    Real ans1 = -(delta + b) / 2 / a;
    Real ans2 = (delta - b) / 2 / a;
    if (ans1 > ans2) {
        swap(ans1, ans2);
    }
    if (sgn(delta) == 0) { // 相切
        return {1, ans2, 0};
    }
    return {2, ans1, ans2};
}
