// 平面圆相关（CG.md「平面圆相关（浮点数处理）」全节）
// 依赖 base.hpp：project / norm / dis / dis2 / crs / agl / intersection / PI
#include "base.hpp"
// 说明：圆用 base.hpp 里的 Circle{Pr o; Real r;} 或直接传 圆心 + 半径

// 根据圆心角获取圆上某点：把圆上最右侧的点绕圆心逆时针旋转 rad 弧度
// CG.md: getPoint(Point<ld> p, ld r, ld rad)
inline Pr pointOnCircle(const Pr& o, Real r, Real rad) {
    return {o.x + cosl(rad) * r, o.y + sinl(rad) * r};
}

// 点到圆的最近点：返回 {最近点, 最近距离}
// p 为圆心时最近点不唯一，视作输入错误，返回圆心（CG.md: pointToCircle）
inline pair<Pr, Real> pointToCircle(const Pr& p, const Pr& o, Real r) {
    if (sgn(dis(p, o)) == 0) {
        return {o, 0};
    }
    Pr u = o + norm(p - o) * r; // 近端：圆心指向 p 的方向
    Pr v = o - norm(p - o) * r; // 远端
    Real du = dis(u, p), dv = dis(v, p);
    if (du < dv) {
        return {u, du};
    }
    return {v, dv};
}

// 直线是否与圆相交及交点：0 不相交；1 相切；2 相交（CG.md: lineCircleCross）
// 需保证直线的方向向量非零
inline tuple<int, Pr, Pr> lineCircleCross(const Lr& l, const Pr& o, Real r) {
    Pr p = project(o, l);              // 圆心到直线的垂足
    Real tmp = r * r - dis2(p, o);     // 用平方距离，避免开根号
    if (sgn(tmp) < 0) {
        return {0, {}, {}};
    } else if (sgn(tmp) == 0) {
        return {1, p, {}};
    }
    Pr vec = norm(l.v) * sqrtl(tmp);
    return {2, p + vec, p - vec};
}

// 线段是否与圆相交及交点：0 不相交；1 相切于线段上；2 交于一个点；3 交于两个点
// CG.md: segmentCircleCross
inline tuple<int, Pr, Pr> segmentCircleCross(const Sr& l, const Pr& o, Real r) {
    auto [type, U, V] = lineCircleCross(Lr(l), o, r);
    bool f1 = rls(U, l) == ON, f2 = rls(V, l) == ON; // 交点是否落在线段上
    if (type == 1 && f1) {
        return {1, U, {}};
    } else if (type == 2 && f1 && f2) {
        return {3, U, V};
    } else if (type == 2 && f1) {
        return {2, U, {}};
    } else if (type == 2 && f2) {
        return {2, V, {}};
    }
    return {0, {}, {}};
}

// 两圆是否相交及交点：0 内含；1 相离；2 相切；3 相交（CG.md: circleIntersection）
// 需保证 r1 > 0（否则 p 可能为 0 而除零）
inline tuple<int, Pr, Pr> circleIntersection(const Pr& p1, Real r1, const Pr& p2, Real r2) {
    Real x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y, d = dis(p1, p2);
    if (sgn(fabsl(r1 - r2) - d) > 0) { // 内含
        return {0, {}, {}};
    } else if (sgn(r1 + r2 - d) < 0) { // 相离
        return {1, {}, {}};
    }
    // 求圆 1 上到圆 2 圆心距离为 r2 的点（原文档中的 p, q, r）
    Real a = r1 * (x1 - x2) * 2, b = r1 * (y1 - y2) * 2, c = r2 * r2 - r1 * r1 - d * d;
    Real p = a * a + b * b, q = -a * c * 2, r = c * c - b * b;
    Real cosa, sina, cosb, sinb;
    if (sgn(d - (r1 + r2)) == 0 || sgn(d - fabsl(r1 - r2)) == 0) { // 相切
        cosa = -q / p / 2;
        sina = sqrtl(1 - cosa * cosa);
        Pr p0 = {x1 + r1 * cosa, y1 + r1 * sina};
        if (sgn(dis(p0, p2) - r2)) {
            p0.y = y1 - r1 * sina;
        }
        return {2, p0, p0};
    }
    Real delta = sqrtl(q * q - p * r * 4);
    cosa = (delta - q) / p / 2;
    cosb = (-delta - q) / p / 2;
    sina = sqrtl(1 - cosa * cosa);
    sinb = sqrtl(1 - cosb * cosb);
    Pr ans1 = {x1 + r1 * cosa, y1 + r1 * sina};
    Pr ans2 = {x1 + r1 * cosb, y1 + r1 * sinb};
    // 两个根分别校验是否落在圆 2 上，不满足则取另一个正弦符号
    // （原文档 ans1 那行误写成 dis(ans1, p1)，这里统一改成 p2）
    if (sgn(dis(ans1, p2) - r2)) ans1.y = y1 - r1 * sina;
    if (sgn(dis(ans2, p2) - r2)) ans2.y = y1 - r1 * sinb;
    if (ans1 == ans2) ans1.y = y1 - r1 * sina;
    return {3, ans1, ans2};
}

// 两圆相交面积（内含、相离、相切、相交四种情况都能算）
// 公式为扇形面积减去扇形内部的那个三角形面积（CG.md: circleIntersectionArea）
inline Real circleIntersectionArea(const Pr& p1, Real r1, const Pr& p2, Real r2) {
    Real d = dis(p1, p2);
    if (sgn(fabsl(r1 - r2) - d) >= 0) { // 内含
        return PI * min(r1 * r1, r2 * r2);
    } else if (sgn(r1 + r2 - d) < 0) {  // 相离
        return 0;
    }
    Real theta1 = agl(r1, d, r2); // 余弦定理求圆心角
    Real area1 = r1 * r1 * (theta1 - sinl(theta1 * 2) / 2);
    Real theta2 = agl(r2, d, r1);
    Real area2 = r2 * r2 * (theta2 - sinl(theta2 * 2) / 2);
    return area1 + area2;
}

// 三点确定一圆：返回 {1, 圆心, 半径}；三点共线时返回 {0, {}, 0}
// CG.md: getCircle
inline tuple<int, Pr, Real> getCircle(const Pr& a, const Pr& b, const Pr& c) {
    if (sgn(crs(a, b, c)) == 0) { // 特判三点共线
        return {0, {}, 0};
    }
    Lr l1((a + b) / 2, perp(b - a)); // AB 的中垂线
    Lr l2((a + c) / 2, perp(c - a)); // AC 的中垂线
    Pr o = intersection(l1, l2);
    return {1, o, dis(a, o)};
}

// 点到圆的切线数量与切点：0 点在圆内；1 点在圆上（切点就是 p）；2 两条切线
// CG.md: tangent(Point<ld> p, Point<ld> A, ld r)
inline pair<int, vector<Pr>> tangent(const Pr& p, const Pr& A, Real r) {
    vector<Pr> ans;
    Real d = dis(p, A);
    if (sgn(r - d) > 0) {         // 点在圆内，无切线
        return {0, {}};
    } else if (sgn(d - r) == 0) { // 点在圆上，切点即 p
        ans.push_back(p);         // 原文档此处误写成 A - p
        return {1, ans};
    }
    Real base = atan2l(p.y - A.y, p.x - A.x); // 原文档漏掉了这个基准角
    // 直角三角形 A-T-p 中 AP 是斜边、AT 是角 A 的邻边，故角 A = acos(r / d)
    // （原文档此处误写成 asin(r / d)，那是角 p 而不是角 A）
    Real ang = acosl(r / d);
    ans.push_back(pointOnCircle(A, r, base - ang));
    ans.push_back(pointOnCircle(A, r, base + ang));
    return {2, ans};
}

// 两圆的内公、外公切线数量与切点：返回 {数量, A 上的切点, B 上的切点}
// 数量为 -1 表示两圆完全重合（无数条外公切线）；0 表示内含，无公切线
// CG.md: tangent(Point<ld> A, ld Ar, Point<ld> B, ld Br)
// 两个圆心按值传入，因为半径交换之后圆心也要跟着换
inline tuple<int, vector<Pr>, vector<Pr>> tangent(Pr A, Real Ar, Pr B, Real Br) {
    vector<Pr> a, b; // 储存切点
    if (Ar < Br) {   // 保证 A 的半径不小于 B
        swap(Ar, Br);
        swap(A, B);
        swap(a, b);
    }
    Real d = dis2(A, B), dif = Ar - Br, sum = Ar + Br; // 原文档用平方距离比较，免开根号
    if (sgn(d - dif * dif) < 0) { // 内含，无公切线
        return {0, {}, {}};
    }
    Real base = atan2l(B.y - A.y, B.x - A.x);
    if (sgn(d) == 0 && sgn(Ar - Br) == 0) { // 完全重合，无数条外公切线
        return {-1, {}, {}};
    }
    if (sgn(d - dif * dif) == 0) { // 内切，1 条外公切线
        a.push_back(pointOnCircle(A, Ar, base));
        b.push_back(pointOnCircle(B, Br, base));
        return {1, a, b};
    }
    Real ang = acosl(dif / sqrtl(d));
    a.push_back(pointOnCircle(A, Ar, base + ang)); // 保底 2 条外公切线
    a.push_back(pointOnCircle(A, Ar, base - ang));
    b.push_back(pointOnCircle(B, Br, base + ang));
    b.push_back(pointOnCircle(B, Br, base - ang));
    if (sgn(d - sum * sum) == 0) { // 外切，多 1 条内公切线
        a.push_back(pointOnCircle(A, Ar, base));
        b.push_back(pointOnCircle(B, Br, base + PI));
    } else if (sgn(d - sum * sum) > 0) { // 相离，多 2 条内公切线
        ang = acosl(sum / sqrtl(d));
        a.push_back(pointOnCircle(A, Ar, base + ang));
        a.push_back(pointOnCircle(A, Ar, base - ang));
        b.push_back(pointOnCircle(B, Br, base + ang + PI));
        b.push_back(pointOnCircle(B, Br, base - ang + PI));
    }
    return {(int)a.size(), a, b};
}
