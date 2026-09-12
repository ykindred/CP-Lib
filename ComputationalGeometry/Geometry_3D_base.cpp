// 三维几何：点线面封装、初始化与基础判定
//（CG.md「三维几何必要初始化」+「三维点线面相关」中不含交点/距离的部分）
// base.hpp 里没有三维内容，所以三维部分自成一套：本文件自带三维点/线/面结构，
// 只复用 base.hpp 的标量层 Real / EPS / PI / sgn / cmp。
// 另外两个三维文件（Geometry_3D_intersect.cpp、Geometry_3D_distance.cpp）
// 通过 include 本文件来复用这些类型与函数。
#include "base.hpp"

// 三维点（CG.md: Point3）
struct Pnt3 {
    Real x, y, z;
    Pnt3(Real x_ = 0, Real y_ = 0, Real z_ = 0) : x(x_), y(y_), z(z_) {}
    Pnt3& operator+=(const Pnt3& p) & {
        return x += p.x, y += p.y, z += p.z, *this;
    }
    Pnt3& operator-=(const Pnt3& p) & {
        return x -= p.x, y -= p.y, z -= p.z, *this;
    }
    Pnt3& operator*=(const Pnt3& p) & { // 逐维相乘
        return x *= p.x, y *= p.y, z *= p.z, *this;
    }
    Pnt3& operator*=(Real t) & {
        return x *= t, y *= t, z *= t, *this;
    }
    Pnt3& operator/=(Real t) & {
        return x /= t, y /= t, z /= t, *this;
    }
    Pnt3 operator-() const {
        return Pnt3(-x, -y, -z);
    }
    friend Pnt3 operator+(Pnt3 a, const Pnt3& b) {
        return a += b;
    }
    friend Pnt3 operator-(Pnt3 a, const Pnt3& b) {
        return a -= b;
    }
    friend Pnt3 operator*(Pnt3 a, const Pnt3& b) {
        return a *= b;
    }
    friend Pnt3 operator*(Pnt3 a, Real b) {
        return a *= b;
    }
    friend Pnt3 operator*(Real a, Pnt3 b) {
        return b *= a;
    }
    friend Pnt3 operator/(Pnt3 a, Real b) {
        return a /= b;
    }
    friend istream& operator>>(istream& is, Pnt3& p) {
        return is >> p.x >> p.y >> p.z;
    }
    friend ostream& operator<<(ostream& os, const Pnt3& p) {
        return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    }
};
using P3 = Pnt3; // CG.md 中的 P3

// 三维直线（两点式），CG.md: Line3
struct Lin3 {
    P3 a, b;
};
using L3 = Lin3; // CG.md 中的 L3

// 平面：由不共线的三点确定，CG.md: Plane
struct Plane {
    P3 u, v, w;
};

// 原点到当前点的距离（CG.md: len）
inline Real len(const P3& p) {
    return sqrtl(p.x * p.x + p.y * p.y + p.z * p.z);
}
// 叉乘（结果仍是向量）（CG.md: crossEx）
inline P3 crossEx(const P3& a, const P3& b) {
    return P3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
// 叉乘的模长（CG.md: cross）
inline Real cross(const P3& a, const P3& b) {
    return len(crossEx(a, b));
}
// 点乘（CG.md: dot）
inline Real dot(const P3& a, const P3& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
// 平面的法向量（CG.md: getVec）
inline P3 getVec(const Plane& s) {
    return crossEx(s.u - s.v, s.v - s.w);
}
// 三维欧几里得距离（CG.md: dis）
inline Real dis(const P3& a, const P3& b) {
    return sqrtl((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}
// 将三维向量转换为单位向量（CG.md: standardize）
inline P3 standardize(P3 vec) {
    return vec / len(vec);
}

// 空间三点是否共线；第二个重载用于判断给定三点能否构成平面（CG.md: onLine）
inline bool onLine(const P3& p1, const P3& p2, const P3& p3) {
    return sgn(cross(p1 - p2, p3 - p2)) == 0;
}
inline bool onLine(const Plane& s) {
    return onLine(s.u, s.v, s.w);
}
// 四点是否共面（CG.md: onPlane）
inline bool onPlane(const P3& p1, const P3& p2, const P3& p3, const P3& p4) {
    return sgn(dot(getVec({p1, p2, p3}), p4 - p1)) == 0;
}

// 空间点是否在线段上（端点也算作在直线上）（CG.md: pointOnSegment）
inline bool pointOnSegment(const P3& p, const L3& l) {
    return sgn(cross(p - l.a, p - l.b)) == 0 && min(l.a.x, l.b.x) <= p.x && p.x <= max(l.a.x, l.b.x) &&
           min(l.a.y, l.b.y) <= p.y && p.y <= max(l.a.y, l.b.y) && min(l.a.z, l.b.z) <= p.z &&
           p.z <= max(l.a.z, l.b.z);
}
// 空间点是否在线段上（端点不算）（CG.md: pointOnSegmentEx）
inline bool pointOnSegmentEx(const P3& p, const L3& l) {
    return sgn(cross(p - l.a, p - l.b)) == 0 && min(l.a.x, l.b.x) < p.x && p.x < max(l.a.x, l.b.x) &&
           min(l.a.y, l.b.y) < p.y && p.y < max(l.a.y, l.b.y) && min(l.a.z, l.b.z) < p.z &&
           p.z < max(l.a.z, l.b.z);
}

// 空间两点是否在线段同侧；两点与线段不共面、有点在线段上时返回 false
// CG.md: pointOnSegmentSide
inline bool pointOnSegmentSide(const P3& p1, const P3& p2, const L3& l) {
    if (!onPlane(p1, p2, l.a, l.b)) { // 特判不共面
        return false;
    }
    return sgn(dot(crossEx(l.a - l.b, p1 - l.b), crossEx(l.a - l.b, p2 - l.b))) == 1;
}
// 两点是否在平面同侧；有点在平面上时返回 false（CG.md: pointOnPlaneSide）
inline bool pointOnPlaneSide(const P3& p1, const P3& p2, const Plane& s) {
    return sgn(dot(getVec(s), p1 - s.u) * dot(getVec(s), p2 - s.u)) == 1;
}

// 空间两直线是否平行 / 垂直（CG.md: lineParallel / lineVertical）
inline bool lineParallel(const L3& l1, const L3& l2) {
    return sgn(cross(l1.a - l1.b, l2.a - l2.b)) == 0;
}
inline bool lineVertical(const L3& l1, const L3& l2) {
    return sgn(dot(l1.a - l1.b, l2.a - l2.b)) == 0;
}
// 两平面是否平行 / 垂直（CG.md: planeParallel / planeVertical）
inline bool planeParallel(const Plane& s1, const Plane& s2) {
    return sgn(cross(getVec(s1), getVec(s2))) == 0;
}
inline bool planeVertical(const Plane& s1, const Plane& s2) {
    return sgn(dot(getVec(s1), getVec(s2))) == 0;
}
// 空间两直线是否是同一条（CG.md: same(L3, L3)）
inline bool same(const L3& l1, const L3& l2) {
    return lineParallel(l1, l2) && lineParallel({l1.a, l2.b}, {l1.b, l2.a});
}
// 两平面是否是同一个（CG.md: same(Plane, Plane)）
inline bool same(const Plane& s1, const Plane& s2) {
    return onPlane(s1.u, s2.u, s2.v, s2.w) && onPlane(s1.v, s2.u, s2.v, s2.w) &&
           onPlane(s1.w, s2.u, s2.v, s2.w);
}
// 直线是否与平面平行（CG.md: linePlaneParallel）
inline bool linePlaneParallel(const L3& l, const Plane& s) {
    return sgn(dot(l.a - l.b, getVec(s))) == 0;
}

// 点是否在空间三角形上（边界上返回 false）（CG.md: pointOnTriangle）
inline bool pointOnTriangle(const P3& p, const P3& p1, const P3& p2, const P3& p3) {
    return pointOnSegmentSide(p, p1, {p2, p3}) && pointOnSegmentSide(p, p2, {p1, p3}) &&
           pointOnSegmentSide(p, p3, {p1, p2});
}
