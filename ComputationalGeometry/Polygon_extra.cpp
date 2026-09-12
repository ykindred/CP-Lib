// 平面多边形补充（CG.md「平面多边形」中 base.hpp 未覆盖的部分）
// 依赖 base.hpp
#include "base.hpp"
// 说明：点在多边形内、线段在多边形内、任意多边形面积 base.hpp 已提供，见 inside / area

// 三点的有向二倍面积（逆时针为正），也等于以 p2 - p1、p3 - p1 为邻边的平行四边形有向面积
// CG.md: areaEx（原文档函数体里写的是未定义的 b, c, a，这里按语义补全）
// 整数请留意溢出，ll 可用 base.hpp 的 crs128
template <class T>
inline T areaEx(const Pnt<T>& p1, const Pnt<T>& p2, const Pnt<T>& p3) {
    return crs(p1, p2, p3);
}

// 判断四个点能否组成矩形/正方形（可以处理浮点数、共点的情况）
// 返回 2 构成正方形；1 构成矩形；0 其它（CG.md: isSquare）
template <class T>
inline int isSquare(vector<Pnt<T>> x) {
    assert((int)x.size() == 4);
    sort(x.begin(), x.end());
    vector<Pr> p(4);
    for (int i = 0; i < 4; i++) {
        p[i] = Pr(x[i]);
    }
    Real d01 = dis(p[0], p[1]), d23 = dis(p[2], p[3]);
    Real d02 = dis(p[0], p[2]), d13 = dis(p[1], p[3]);
    if (cmp(d01, d23) == 0 && sgn(d01) != 0 && cmp(d02, d13) == 0 && sgn(d02) != 0 &&
        parallel(p[1] - p[0], p[3] - p[2]) && parallel(p[2] - p[0], p[3] - p[1]) &&
        orthogonal(p[1] - p[0], p[2] - p[0])) {
        return cmp(d01, d02) == 0 ? 2 : 1;
    }
    return 0;
}

// 任意多边形上（边上，含顶点）的网格点个数，仅能处理整数：Σ gcd(|Δx|, |Δy|)
// CG.md: onPolygonGrid，皮克定理用
inline ll onPolygonGrid(const vector<Pnt<ll>>& p) {
    int n = (int)p.size();
    ll ans = 0;
    for (int i = 0; i < n; i++) {
        const Pnt<ll>& a = p[i];
        const Pnt<ll>& b = p[(i + 1) % n];
        ans += gcd(abs(a.x - b.x), abs(a.y - b.y));
    }
    return ans;
}

// 任意多边形内部的网格点个数，仅能处理整数（皮克定理：S = n + s / 2 - 1）
// CG.md: inPolygonGrid
inline ll inPolygonGrid(const vector<Pnt<ll>>& p) {
    int n = (int)p.size();
    ll ans = 0; // 二倍有向面积
    for (int i = 0; i < n; i++) {
        const Pnt<ll>& a = p[i];
        const Pnt<ll>& b = p[(i + 1) % n];
        const Pnt<ll>& c = p[(i + 2) % n];
        ans += b.y * (a.x - c.x);
    }
    ans = abs(ans);
    return (ans - onPolygonGrid(p)) / 2 + 1;
}
