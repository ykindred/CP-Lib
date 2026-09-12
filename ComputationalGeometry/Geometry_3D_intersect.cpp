// 三维几何：相交判定与交点
//（CG.md「三维点线面相关」中的相交部分 +「空间多边形」中的三角形相交）
// 三维的点/线/面类型与基础运算都在 Geometry_3D_base.cpp 里（其中已包含 base.hpp）
#include "Geometry_3D_base.cpp"

// 空间两线段是否相交（重叠、相交于端点均视为相交）（CG.md: segmentIntersection(L3, L3)）
inline bool segmentIntersection(const L3& l1, const L3& l2) {
    if (!onPlane(l1.a, l1.b, l2.a, l2.b)) { // 特判不共面
        return false;
    }
    if (!onLine(l1.a, l1.b, l2.a) || !onLine(l1.a, l1.b, l2.b)) {
        return !pointOnSegmentSide(l1.a, l1.b, l2) && !pointOnSegmentSide(l2.a, l2.b, l1);
    }
    // 四点共线，判断是否有重叠
    //（原文档最后一项误写成 pointOnSegment(l2.b, l2)，恒为真，这里改为 pointOnSegment(l2.b, l1)）
    return pointOnSegment(l1.a, l2) || pointOnSegment(l1.b, l2) || pointOnSegment(l2.a, l1) ||
           pointOnSegment(l2.b, l1);
}
// 空间两点是否严格在线段所在直线的异侧；不共面、有点落在直线上时返回 false
inline bool pointOnSegmentSideEx(const P3& p1, const P3& p2, const L3& l) {
    if (!onPlane(p1, p2, l.a, l.b)) { // 特判不共面
        return false;
    }
    return sgn(dot(crossEx(l.a - l.b, p1 - l.b), crossEx(l.a - l.b, p2 - l.b))) == -1;
}
// 空间两线段是否相交（重叠、相交于端点不视为相交，即只有真正的“交叉”才算）
// CG.md: segmentIntersection1。注意原文档的写法用了 !pointOnSegmentSide，
// 而 pointOnSegmentSide 在端点落在另一条线段所在直线上时返回 false（取反即 true），
// 所以端点相交、共线重叠都会被判成相交，与它自己注释里写的语义相反；
// 这里按注释的本意改成「严格异侧」判定，只有两线段都在内部相交才返回 true
inline bool segmentIntersectionStrict(const L3& l1, const L3& l2) {
    return onPlane(l1.a, l1.b, l2.a, l2.b) && pointOnSegmentSideEx(l1.a, l1.b, l2) &&
           pointOnSegmentSideEx(l2.a, l2.b, l1);
}

// 空间两直线是否相交及交点；不共面、平行时返回 {false, {}}
// CG.md: lineIntersection(L3, L3)
inline pair<bool, P3> lineIntersection(const L3& l1, const L3& l2) {
    if (!onPlane(l1.a, l1.b, l2.a, l2.b) || lineParallel(l1, l2)) {
        return {false, {}};
    }
    const P3& s1 = l1.a;
    const P3& e1 = l1.b;
    const P3& s2 = l2.a;
    const P3& e2 = l2.b;
    // 找一个投影后两直线不平行的坐标平面，投影后求二维交点参数
    // 注意：原文档挑选平面的条件 onPlane(l1.a, l1.b, {0,0,0}, {0,0,1}) 在直线过原点、
    // 四点共面等情况下会退化，从而选到投影后平行的平面、除以 0 得到 nan；
    // 这里改成直接判投影方向的行列式是否为 0，三个平面里必有至少一个可用
    auto det2 = [](Real ax, Real ay, Real bx, Real by) { return ax * by - ay * bx; };
    Real val = 0, d = det2(s1.x - e1.x, s1.y - e1.y, s2.x - e2.x, s2.y - e2.y);
    if (sgn(d) != 0) { // 投影到 xOy 面
        val = det2(s1.x - s2.x, s1.y - s2.y, s2.x - e2.x, s2.y - e2.y) / d;
    } else if (d = det2(s1.x - e1.x, s1.z - e1.z, s2.x - e2.x, s2.z - e2.z), sgn(d) != 0) { // 投影到 xOz 面
        val = det2(s1.x - s2.x, s1.z - s2.z, s2.x - e2.x, s2.z - e2.z) / d;
    } else { // 投影到 yOz 面
        d = det2(s1.y - e1.y, s1.z - e1.z, s2.y - e2.y, s2.z - e2.z);
        val = det2(s1.y - s2.y, s1.z - s2.z, s2.y - e2.y, s2.z - e2.z) / d;
    }
    return {true, s1 + (e1 - s1) * val};
}

// 直线与平面是否相交及交点；平行、给定点构不成平面时返回 {false, {}}
// CG.md: linePlaneCross
inline pair<bool, P3> linePlaneCross(const L3& l, const Plane& s) {
    if (linePlaneParallel(l, s)) {
        return {false, {}};
    }
    P3 vec = getVec(s);
    // 原文档先逐维相乘再把三维加起来，等价于点乘
    Real val = dot(vec, s.u - l.a) / dot(vec, l.b - l.a);
    return {true, l.a + (l.b - l.a) * val};
}

// 两平面是否相交及交线；平行、同一平面时返回 {false, {}}
// CG.md: planeIntersection
inline pair<bool, L3> planeIntersection(const Plane& s1, const Plane& s2) {
    if (planeParallel(s1, s2) || same(s1, s2)) {
        return {false, {}};
    }
    P3 U = linePlaneParallel({s2.u, s2.v}, s1) ? linePlaneCross({s2.v, s2.w}, s1).second
                                               : linePlaneCross({s2.u, s2.v}, s1).second;
    P3 V = linePlaneParallel({s2.w, s2.u}, s1) ? linePlaneCross({s2.v, s2.w}, s1).second
                                               : linePlaneCross({s2.w, s2.u}, s1).second;
    return {true, {U, V}};
}

// 线段是否与空间三角形相交及交点；只有交点在三角形内部（边界不算）才视作相交
// CG.md: segmentOnTriangle（l, r 为线段两端点）
inline pair<bool, P3> segmentOnTriangle(const P3& l, const P3& r, const P3& p1, const P3& p2,
                                        const P3& p3) {
    P3 x = crossEx(p2 - p1, p3 - p1); // 三角形所在平面的法向量
    if (sgn(dot(x, r - l)) == 0) {    // 线段与平面平行
        return {false, {}};
    }
    Real t = dot(x, p1 - l) / dot(x, r - l);
    if (t < 0 || t - 1 > 0) { // 交点不在线段上
        return {false, {}};
    }
    if (pointOnTriangle(l + (r - l) * t, p1, p2, p3)) {
        return {true, l + (r - l) * t};
    }
    return {false, {}};
}

// 空间两三角形是否相交；相交线段在三角形内部（边界不算）才视作相交
// CG.md: triangleIntersection
inline bool triangleIntersection(const vector<P3>& a, const vector<P3>& b) {
    for (int i = 0; i < 3; i++) {
        if (segmentOnTriangle(b[i], b[(i + 1) % 3], a[0], a[1], a[2]).first) {
            return true;
        }
        if (segmentOnTriangle(a[i], a[(i + 1) % 3], b[0], b[1], b[2]).first) {
            return true;
        }
    }
    return false;
}
