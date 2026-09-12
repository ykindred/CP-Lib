// 二维动态凸包（CG.md「二维凸包」的动态凸包）
// 支持：动态插入点；查询点是否在凸包内（含边界）；判断直线 Ax + By = C 是否与凸包相交
// 依赖 base.hpp
#include "base.hpp"
// 说明：CG.md 原版用 Point<int>，这里改用 ll（base.hpp 里 sgn / cmp 对 int 有重载歧义，
// 且 Pnt<int> 的 operator== 无法通过编译）；只维护上凸壳，下凸壳用 (-x, -y) 镜像得到

// a -> b 是否右转（含反向共线）（CG.md: turnRight）
template <class T>
inline bool turnRight(const Pnt<T>& a, const Pnt<T>& b) {
    T c = crs(a, b);
    return c < 0 || (c == 0 && dot(a, b) < 0);
}

// 凸包的一条边 <当前点, 指向下一个点的向量>
// cmp = 1：按点排序（查询用）；cmp = 0：按斜率排序（判断与直线相交用）
struct DynLine { // CG.md: Line
    inline static int cmp = 1;
    mutable Pnt<ll> a, b;
    friend bool operator<(const DynLine& x, const DynLine& y) {
        return cmp ? x.a < y.a : turnRight(x.b, y.b);
    }
    friend ostream& operator<<(ostream& os, const DynLine& l) {
        return os << "<" << l.a << ", " << l.b << ">";
    }
};

// 上凸壳（CG.md: UpperConvexHull）
struct UpperHull : set<DynLine> {
    // 点是否在上凸壳内（含边界）
    bool contains(const Pnt<ll>& p) const {
        auto it = lower_bound({p, 0});
        if (it != end() && it->a == p) {
            return true;
        }
        if (it != begin() && it != end() && crs(prev(it)->b, p - prev(it)->a) <= 0) {
            return true;
        }
        return false;
    }
    // 插入点并维护凸性
    void add(const Pnt<ll>& p) {
        if (contains(p)) {
            return;
        }
        auto it = lower_bound({p, 0});
        for (; it != end(); it = erase(it)) { // 删掉右侧不再凸的点
            if (turnRight(it->a - p, it->b)) {
                break;
            }
        }
        for (; it != begin() && prev(it) != begin(); erase(prev(it))) { // 删掉左侧不再凸的点
            if (turnRight(prev(prev(it))->b, p - prev(prev(it))->a)) {
                break;
            }
        }
        if (it != begin()) {
            prev(it)->b = p - prev(it)->a;
        }
        if (it == end()) {
            insert({p, {0, -1}});
        } else {
            insert({p, it->a - p});
        }
    }
};

// 完整的动态凸包（CG.md: ConvexHull）
struct DynamicConvexHull {
    UpperHull up, low;
    bool empty() const {
        return up.empty();
    }
    // 点是否在凸包内（含边界）
    bool contains(const Pnt<ll>& p) const {
        DynLine::cmp = 1;
        return up.contains(p) && low.contains(-p);
    }
    // 动态插入点
    void add(const Pnt<ll>& p) {
        DynLine::cmp = 1;
        up.add(p);
        low.add(-p);
    }
    // 直线 Ax + By = C 是否穿过凸包（凸包为空返回 false）
    bool isIntersect(ll A, ll B, ll C) const {
        DynLine::cmp = 0;
        if (empty()) {
            return false;
        }
        Pnt<ll> k = {-B, A};
        if (k.x < 0) {
            k = -k;
        }
        if (k.x == 0 && k.y < 0) {
            k.y = -k.y;
        }
        Pnt<ll> P = up.upper_bound({{0, 0}, k})->a;    // 法线方向上取极值的点
        Pnt<ll> Q = -low.upper_bound({{0, 0}, k})->a;  // 法线反方向上取极值的点
        // f(P)、f(Q) 是凸包上 f = Ax + By - C 的最大/最小值，异号（或为 0）即有公共点
        // 注意：原文档写的是 sign(...) * sign(...) > 0，实测得到的是「直线与凸包不相交」，
        // 与函数名恰好相反（相切于顶点、沿边贴住时也返回 false），这里改成 <= 0 使其名副其实
        return sgn(A * P.x + B * P.y - C) * sgn(A * Q.x + B * Q.y - C) <= 0;
    }
    friend ostream& operator<<(ostream& out, const DynamicConvexHull& ch) {
        for (const DynLine& line : ch.up) {
            out << "(" << line.a.x << "," << line.a.y << ")";
        }
        out << "/";
        for (const DynLine& line : ch.low) {
            out << "(" << -line.a.x << "," << -line.a.y << ")";
        }
        return out;
    }
};
