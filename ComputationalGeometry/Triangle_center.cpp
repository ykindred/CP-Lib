// 平面三角形相关（CG.md「平面三角形相关（浮点数处理）」全节）
// 依赖 base.hpp：crs / perp / norm / intersection
#include "base.hpp"
// 说明：三角形面积也可以直接用 base.hpp 的 area(Polygon<Real>)，这里给出三点版本

// 三角形面积（CG.md: area(Point<ld> a, Point<ld> b, Point<ld> c)）
inline Real area(const Pr& a, const Pr& b, const Pr& c) {
    return fabsl(crs(b, c, a)) / 2;
}

// 外心：三边垂直平分线的交点（外接圆圆心）（CG.md: center1）
// 需保证三点不共线
inline Pr circumcenter(const Pr& p1, const Pr& p2, const Pr& p3) {
    Lr u((p1 + p2) / 2, perp(p2 - p1)); // p1p2 的中垂线
    Lr v((p2 + p3) / 2, perp(p3 - p2)); // p2p3 的中垂线
    return intersection(u, v);
}

// 内心：三条内角平分线的交点（内切圆圆心）（CG.md: center2）
// 需保证三点不共线
inline Pr incenter(const Pr& p1, const Pr& p2, const Pr& p3) {
    // 角平分线方向 = 两条边的单位向量之和
    // （原文档用 atan2 取两角度的平均值，跨越 ±π 时会算错方向）
    auto bisector = [](const Pr& o, const Pr& u, const Pr& v) {
        return norm(u - o) + norm(v - o);
    };
    Lr U(p1, bisector(p1, p2, p3));
    Lr V(p2, bisector(p2, p1, p3));
    return intersection(U, V);
}

// 垂心：三条高线所在直线的交点（CG.md: center3）
// 需保证三点不共线
inline Pr orthocenter(const Pr& p1, const Pr& p2, const Pr& p3) {
    Lr U(p1, perp(p2 - p3)); // 过 p1 的高线，垂直于 p2p3
    Lr V(p2, perp(p1 - p3)); // 过 p2 的高线，垂直于 p1p3
    return intersection(U, V);
}
