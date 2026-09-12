// 三维几何：距离、夹角与体积
//（CG.md「三维点线面相关」的最近点/距离部分 +「三维角度与弧度」+「空间多边形」的体积与体积公式）
// 三维的点/线/面类型与基础运算都在 Geometry_3D_base.cpp 里（其中已包含 base.hpp）
#include "Geometry_3D_base.cpp"

// 点到直线的最近距离与最近点，返回 {距离, 最近点}
// 注意：CG.md 二维版本的返回顺序是 {最近点, 距离}，这里保持原文档三维版本的顺序
// CG.md: pointToLine(P3 p, L3 l)
inline pair<Real, P3> pointToLine(const P3& p, const L3& l) {
    Real val = cross(p - l.a, l.a - l.b) / dis(l.a, l.b); // 面积除以底边长
    Real val1 = dot(p - l.a, l.a - l.b) / dis(l.a, l.b);
    return {val, l.a + val1 * standardize(l.a - l.b)};
}

// 点到平面的最近距离与最近点，返回 {距离, 最近点}
// CG.md: pointToPlane(P3 p, Plane s)
inline pair<Real, P3> pointToPlane(const P3& p, const Plane& s) {
    P3 vec = getVec(s);
    Real val = dot(vec, p - s.u);
    val = fabsl(val) / len(vec); // 面积除以底边长
    return {val, p - val * standardize(vec)};
}

// 空间两直线的最近距离与最近点对，返回 {距离, 直线 1 上的点, 直线 2 上的点}
// 需保证两直线不平行（平行时 vec 为零向量，出现除零）
// CG.md: lineToLine(L3 l1, L3 l2)
inline tuple<Real, P3, P3> lineToLine(const L3& l1, const L3& l2) {
    P3 vec = crossEx(l1.a - l1.b, l2.a - l2.b); // 同时垂直于两直线的向量
    Real val = fabsl(dot(l1.a - l2.a, vec)) / len(vec);
    P3 U = l1.b - l1.a, V = l2.b - l2.a;
    vec = crossEx(U, V);
    Real p = dot(vec, vec);
    Real t1 = dot(crossEx(l2.a - l1.a, V), vec) / p;
    Real t2 = dot(crossEx(l2.a - l1.a, U), vec) / p;
    return {val, l1.a + (l1.b - l1.a) * t1, l2.a + (l2.b - l2.a) * t2};
}

// 空间两直线夹角的 cos 值（CG.md: lineCos）
inline Real lineCos(const L3& l1, const L3& l2) {
    return dot(l1.a - l1.b, l2.a - l2.b) / len(l1.a - l1.b) / len(l2.a - l2.b);
}
// 空间两平面夹角的 cos 值（CG.md: planeCos）
inline Real planeCos(const Plane& s1, const Plane& s2) {
    P3 U = getVec(s1), V = getVec(s2);
    return dot(U, V) / len(U) / len(V);
}
// 直线与平面夹角的 sin 值（CG.md: linePlaneSin）
inline Real linePlaneSin(const L3& l, const Plane& s) {
    P3 vec = getVec(s);
    return dot(l.a - l.b, vec) / len(l.a - l.b) / len(vec);
}

// 正 n 棱锥（所有棱长均为 l）的体积公式（CG.md: V(ld l, int n)）
// 棱锥通用体积 V = Sh / 3，正 n 棱锥代入底面积与高之后即下式
inline Real regularPyramidVolume(Real l, int n) {
    return l * l * l * n / (12 * tanl(PI / n)) * sqrtl(1 - 1 / (4 * sinl(PI / n) * sinl(PI / n)));
}

// 四面体体积（CG.md: V(P3 a, P3 b, P3 c, P3 d)）
inline Real tetrahedronVolume(const P3& a, const P3& b, const P3& c, const P3& d) {
    return fabsl(dot(d - a, crossEx(b - a, c - a))) / 6;
}
