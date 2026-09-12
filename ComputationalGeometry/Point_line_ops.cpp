// 平面点线补充（CG.md「平面点线相关」中 base.hpp 未覆盖的部分）
// 依赖 base.hpp
#include "base.hpp"
// 注：点在直线/线段上、垂足、距离、平行垂直等 base.hpp 已提供，见 rls / project / dist / parallel / orthogonal

// 线段的中垂线（垂直平分线）（CG.md: midSegment）
inline Lr midSegment(const Sr& l) {
    Pr mid = (l.a + l.b) / 2; // 线段中点
    return Lr(mid, perp(l.b - l.a));
}

// 点到直线的最近点与最近距离，返回 {最近点, 最近距离}（CG.md: pointToLine）
inline pair<Pr, Real> pointToLine(const Pr& p, const Lr& l) {
    Pr ans = project(p, l);
    return {ans, dis(p, ans)};
}

// 点到线段的最近点与最近距离，返回 {最近点, 最近距离}（CG.md: pointToSegment）
inline pair<Pr, Real> pointToSegment(const Pr& p, const Sr& s) {
    if (sgn(dot(s.a, p, s.b)) < 0) {        // 垂足落在 a 端外侧，最近点是 a
        return {s.a, dis(p, s.a)};
    } else if (sgn(dot(s.b, p, s.a)) < 0) { // 垂足落在 b 端外侧，最近点是 b
        return {s.b, dis(p, s.b)};
    }
    return pointToLine(p, Lr(s));
}

// 点相对直线的方位：1 左侧(逆时针)，0 直线上，-1 右侧
template <class T>
inline int lineSide(const Pnt<T>& p, const Lin<T>& l) {
    return sgn(crs(l.v, p - l.p));
}

// 两点是否在直线同侧 / 异侧（有任一点落在直线上时都返回 false）
// CG.md: pointOnLineSide / pointNotOnLineSide
// 原文档用两个叉乘相乘再取符号，这里先取符号再相乘，避免整数溢出
template <class T>
inline bool pointOnLineSide(const Pnt<T>& p1, const Pnt<T>& p2, const Lin<T>& l) {
    return lineSide(p1, l) * lineSide(p2, l) == 1;
}
template <class T>
inline bool pointNotOnLineSide(const Pnt<T>& p1, const Pnt<T>& p2, const Lin<T>& l) {
    return lineSide(p1, l) * lineSide(p2, l) == -1;
}

// 两线段是否相交及交点（扩展版，可以处理共线重叠）
// 返回 {type, p1, p2}：0 不相交；1 普通相交；2 重叠（交于两点）；3 相交于端点
// CG.md: segmentIntersection（返回交点的那一版，务必用浮点）
inline tuple<int, Pr, Pr> segmentIntersection(const Sr& l1, const Sr& l2) {
    auto [s1, e1] = l1;
    auto [s2, e2] = l2;
    Real A = max(s1.x, e1.x), AA = min(s1.x, e1.x);
    Real B = max(s1.y, e1.y), BB = min(s1.y, e1.y);
    Real C = max(s2.x, e2.x), CC = min(s2.x, e2.x);
    Real D = max(s2.y, e2.y), DD = min(s2.y, e2.y);
    if (A < CC || C < AA || B < DD || D < BB) { // 快速排斥实验
        return {0, {}, {}};
    }
    if (sgn(crs(e1 - s1, e2 - s2)) == 0) { // 平行
        if (sgn(crs(s1, s2, e1)) != 0) {   // 不共线
            return {0, {}, {}};
        }
        Pr p1(max(AA, CC), max(BB, DD));
        Pr p2(min(A, C), min(B, D));
        if (rls(p1, l1) != ON) { // 修正端点顺序
            swap(p1.y, p2.y);
        }
        if (p1 == p2) {
            return {3, p1, p2};
        }
        return {2, p1, p2};
    }
    Real cp1 = crs(s2 - s1, e2 - s1);
    Real cp2 = crs(s2 - e1, e2 - e1);
    Real cp3 = crs(s1 - s2, e1 - s2);
    Real cp4 = crs(s1 - e2, e1 - e2);
    if (sgn(cp1 * cp2) == 1 || sgn(cp3 * cp4) == 1) { // 跨立实验不通过
        return {0, {}, {}};
    }
    Pr p = intersection(Lr(l1), Lr(l2)); // 需要浮点
    if (sgn(cp1) != 0 && sgn(cp2) != 0 && sgn(cp3) != 0 && sgn(cp4) != 0) {
        return {1, p, p};
    }
    return {3, p, p};
}
