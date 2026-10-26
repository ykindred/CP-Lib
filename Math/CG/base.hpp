#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;
using Real = long double;
using i128 = __int128;
constexpr Real PI = numbers::pi;
constexpr Real EPS = 1e-8;

inline int sgn(Real x) {
    return (x > EPS) - (x < -EPS);
}
inline int sgn(ll x) {
    return (x > 0) - (x < 0);
}

inline int cmp(Real x, Real y) {
    return sgn(x - y);
}
inline int cmp(ll x, ll y) {
    return (x > y) - (x < y);
}

inline Real arccos(Real c) {
    c = max(c, Real(-1.0));
    c = min(c, Real(1.0));
    return acosl(c);
}

template <class T>
struct Pnt {
    T x, y;
    Pnt(T x = 0, T y = 0) : x(x), y(y) {}
    template <class U> operator Pnt<U>() const {
        return Pnt<U>(U(x), U(y));
    }

    Pnt& operator+=(const Pnt& o) & {
        return x += o.x, y += o.y, *this;
    }
    Pnt& operator-=(const Pnt& o) & {
        return x -= o.x, y -= o.y, *this;
    }
    Pnt& operator*=(T o) & {
        return x *= o, y *= o, *this;
    }
    Pnt& operator/=(T o) & {
        return x /= o, y /= o, *this;
    }
    Pnt operator-() const {
        return Pnt(-x, -y);
    }

    friend Pnt operator+(Pnt a, const Pnt& b) {
        return a += b;
    }
    friend Pnt operator-(Pnt a, const Pnt& b) {
        return a -= b;
    }
    friend Pnt operator*(Pnt a, T b) {
        return a *= b;
    }
    friend Pnt operator*(T a, Pnt b) {
        return b *= a;
    }
    friend Pnt operator/(Pnt a, T b) {
        return a /= b;
    }
    bool operator<(const Pnt& o) const {
        if (x != o.x) return x < o.x;
        return y < o.y;
    }
    bool operator>(const Pnt& o) const {
        return o < *this;
    }
    bool operator==(const Pnt& o) const {
        return (cmp(x, o.x) == 0) && (cmp(y, o.y) == 0);
    }
    bool operator!=(const Pnt& o) const {
        return !(*this == o);
    }
    friend istream& operator>>(istream& is, Pnt<T>& p) {
        return is >> p.x >> p.y;
    }
    friend ostream& operator<<(ostream& os, const Pnt<T>& p) {
        return os << '(' << p.x << ", " << p.y << ')';
    }
};
using Pr = Pnt<Real>;
#define Pt Pnt<T>

template <class T>
T dot(const Pt& a, const Pt& b) {
    return a.x * b.x + a.y * b.y;    
}
template <class T>
T dot(const Pt& o, const Pt& a, const Pt& b) {
    return dot(a - o, b - o);
}

template <class T>
T crs(const Pt& a, const Pt& b) {
    return a.x * b.y - a.y * b.x;
}
template <class T>
T crs(const Pt& o, const Pt& a, const Pt& b) {
    return crs(a - o, b - o);
}

inline i128 crs128(const Pnt<ll>& a, const Pnt<ll>& b) {
    return (i128)a.x * b.y - (i128)a.y * b.x;
}
inline i128 dot128(const Pnt<ll>& a, const Pnt<ll>& b) {
    return (i128)a.x * b.x + (i128)a.y * b.y;
}


template <typename T>
T len2(const Pt& p) {
    return dot(p, p);
}
inline Real len(const Pr& p) {
    return sqrtl(len2(p));
}

template <typename T>
T dis2(const Pt& a, const Pt& b) {
    return dot(a - b, a - b);
}
inline Real dis(const Pr& a, const Pr& b) {
    return sqrtl(dis2(a, b));
}

template <class T>
T manhattan(const Pt& a, const Pt& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

inline Pr norm(Pr a) {
    // 非零
    return a / len(a);
}

inline Real agl(const Pr& a, const Pr& b) {
    // 需保证非零
    Real val = fabsl(crs(a, b));
    return fabsl(atan2l(val, dot(a, b)));
}

inline Real agl(Real a, Real b, Real c) {
    // 余弦定理
    return arccos((a * a + b * b - c * c) / (2.0 * a * b));
}

inline Pr rotate(const Pr& a, Real theta) {
    Real c = cosl(theta), s = sinl(theta);
    return Pr(c * a.x - s * a.y, s * a.x + c * a.y);
}
inline Pr rotate(const Pr& o, const Pr& a, Real theta) {
    // a 绕 o 转 theta
    return o + rotate(a - o, theta);
}

template <class T>
Pt perp(const Pt& a) {
    return { -a.y, a.x };
}
template <class T>
Pt perp(const Pt& o, const Pt& a) {
    return o + perp(a - o);
}

inline Real toDeg(Real x) {
    // 转角度
    return x * 180 / PI;
}
inline Real toArc(Real x) {
    // 转弧度
    return PI / 180 * x;
}


template <typename T>
struct Seg {
    Pt a, b;
    Seg(const Pt& a = {}, const Pt& b = {}) : a(a), b(b) {}
    template<class U> operator Seg<U>() const { // 自动类型匹配
        return Seg<U>(Pnt<U>(a), Pnt<U>(b));
    }
};
#define St Seg<T>
using Sr = Seg<Real>;

template <class T>
struct Lin {
    Pt p, v;
    Lin(Pt p = {}, Pt v = {}) : p(p), v(v) {}
    Lin(const St& s) : p(s.a), v(s.b - s.a) {}
    template<class U> operator Lin<U>() const { // 自动类型匹配
        return Lin<U>(Pnt<U>(p), Pnt<U>(v));
    }
};
#define Lt Lin<T>
using Lr = Lin<Real>;

enum Relation { 
    ONBACK = -2, 
    CW, // clockwise
    ON,
    CCW, // counter-clockwise
    ONFRONT
};

template <class T>
int rls(const Pt& p, const Lt& l) { // relation
    return sgn(crs(l.v, p - l.p));
}

template <class T>
int rls(const Pt& p, const St& s) {
    int tmp = rls(p, Lin(s));
    if (tmp != 0) {
        return tmp;
    }
    if (sgn(dot(s.a, s.b, p)) < 0) {
        return ONBACK;
    } else if (sgn(dot(s.b, s.a, p)) < 0) {
        return ONFRONT;
    }
    return ON;
}

inline Pr project(const Pr& p, const Lr& l) {
    // 需保证l.v非零
    Pr t = l.v * dot(p - l.p, l.v);
    return l.p + t / len2(l.v);
}

inline Pr reflect(const Pr& p, const Lr& l) {
    return project(p, l) * 2 - p;
}

template <class T>
bool parallel(const Pt& a, const Pt& b) {
    return sgn(crs(a, b)) == 0;
}
template <class T>
bool orthogonal(const Pt& a, const Pt& b) {
    return sgn(dot(a, b)) == 0;
}


// 线段相交, 快速排斥实验
template <class T>
bool box_joint(const St& a, const St& b) { // 快速排斥实验
    Pt A = { a.a.x, a.a.y }, 
        B = { a.b.x, a.b.y }, 
        C = { b.a.x, b.a.y },
        D = { b.b.x, b.b.y };
    return 
        cmp(min(A.x, B.x), max(C.x, D.x)) <= 0 && 
        cmp(min(C.x, D.x), max(A.x, B.x)) <= 0 && 
        cmp(min(A.y, B.y), max(C.y, D.y)) <= 0 && 
        cmp(min(C.y, D.y), max(A.y, B.y)) <= 0;
}

// 跨立实验
template <class T>
bool straddle(const St& a, const St& b) {
    return sgn(crs(a.a, a.b, b.a)) * sgn(crs(a.a, a.b, b.b)) <= 0;
}

template <class T>
bool intersect(const St a, const St& b) {
    return box_joint(a, b) && 
           straddle(a, b) && 
           straddle(b, a);
}

// 直线相交
template <class T>
inline bool intersect(const Lt& a, const Lt& b) {
    if (sgn(crs(a.v, b.v)) != 0) {
        return true;
    } else {
        return sgn(crs(a.v, b.p - a.p)) == 0;
    }
}

// 直线交点
inline Pr intersection(const Lr& a, const Lr& b) {
    // 需保证不平行
    assert(sgn(crs(a.v, b.v)) != 0);
    return a.p + a.v * crs(b.p - a.p, b.v) / crs(a.v, b.v);
}

// 点-直线距离
inline Real dist(const Pr& a, const Lr& l) {
    return fabsl(crs(l.v, a - l.p) / len(l.v));
}
// 点-线段距离
inline Real dist(const Pr& p, const Sr& s) {
    Pr ab = s.b - s.a, 
       ap = p - s.a, 
       ba = s.a - s.b,
       bp = p - s.b;
    if (sgn(dot(ab, ap)) < 0) {
        return len(ap);
    } else if (sgn(dot(ba, bp)) < 0) {
        return len(bp);
    }
    return fabsl(crs(ab, ap)) / len(ab);
}

// 线段-线段距离
inline Real dist(const Sr& a, const Sr& b) {
    if (intersect(a, b)) {
        return 0;
    }
    return min({ 
        dist(a.a, b),
        dist(a.b, b),
        dist(b.a, a),
        dist(b.b, a)
    });
}

template <class T>
using Polygon = vector<Pnt<T>>; // 需要保证逆时针存储
inline Real area(const Polygon<Real>& pol) {
    Real ret = 0;
    int n = pol.size();
    for (int i = 0; i < n; i++) {
        ret += crs(pol[i], pol[(i + 1) % n]);
    }
    return abs(ret) / 2;
}

enum Inside {
    OUT = 0,
    ONSEG,
    IN
};
template <class T>
inline int inside(const Pt& p, const Polygon<T>& pol) {
    int wn = 0;
    int n = pol.size();
    
    for (int i = 0; i < n; i++) {
        const auto& v1 = pol[i];
        const auto& v2 = pol[(i + 1) % n];
        if (rls(p, Seg(v1, v2)) == ON) {
            return ONSEG;
        }
        int s1 = sgn(v1.y - p.y);
        int s2 = sgn(v2.y - p.y);
        int s3 = sgn(crs(v2 - v1, p - v1));
        if (s1 <= 0) {
            if (s2 > 0 && s3 > 0) {
                wn++;
            }
        } else {
            if (s2 <= 0 && s3 < 0) {
                wn--;
            }
        }
    }
    if (wn != 0) {
        return IN;
    } else {
        return OUT;
    }
}

inline int inside(const Sr& s, const Polygon<Real>& pol) {
    if (inside(s.a, pol) * inside(s.b, pol) == 0) {
        return OUT;
    }
    int n = pol.size();
    for (int i = 0; i < n; i++) {
        const auto& p1 = pol[i];
        const auto& p2 = pol[(i + 1) % n];
        Seg edge(p1, p2);
        if (sgn(crs(s.b - s.a, edge.a - s.a)) * sgn(crs(s.b - s.a, edge.b - s.a)) < 0 &&
            sgn(crs(edge.b - edge.a, s.a - edge.a)) * sgn(crs(edge.b - edge.a, s.b - edge.a)) < 0) {
            return OUT;
        }
    }
    
    vector<Pr> pts;
    pts.emplace_back(s.a);
    pts.emplace_back(s.b);
    for (int i = 0; i < n; i++) {
        if (rls(pol[i], s) == ON) {
            pts.emplace_back(pol[i]);
        }
    }
    auto param = [&](const Pr& p) -> Real {
        Pr v = s.b - s.a;
        if (fabsl(v.x) >= fabsl(v.y)) {
            return (p.x - s.a.x) / v.x;
        } else {
            return (p.y - s.a.y) / v.y;
        }
    };
    sort(pts.begin(), pts.end(), [&](const Pr& u, const Pr& v) {
        return param(u) < param(v);
    });
    pts.erase(unique(pts.begin(), pts.end()), pts.end());

    bool flag = false;
    for (const auto& p : pts) {
        if (inside(p, pol) == ONSEG) {
            flag = true;
        }
    }
    for (int i = 0; i + 1 < (int)pts.size(); i++) {
        if (pts[i] == pts[i + 1]) {
            continue;
        }
        Pr mid = (pts[i] + pts[i + 1]) / 2;
        int t = inside(mid, pol);
        if (t == OUT) {
            return OUT;
        }
    }

    if (flag) {
        return ONSEG;
    }
    return IN;
}

template <class T>
inline bool is_convex(const Polygon<T>& pol) {
    // 允许相邻边共线
    int n = pol.size();
    Pt v1;
    Pt v2 = pol[1] - pol[0];
    for (int i = 1; i < n; i++) {
        v1 = v2;
        v2 = pol[(i + 1) % n] - pol[i];
        if (sgn(crs(v1, v2)) < 0) {
            return false;
        }
    }
    return true;
}

// 1 : 上半, 0 : 原点, -1 : 下半
template <class T>
inline int up_dn(const Pt& p) {
    if (sgn(p.y) == 0) {
        return sgn(p.x);
    }
    return sgn(p.y);
}

template <class T>
inline int polar_cmp(const Pt& lt, const Pt& rt) {
    int a = up_dn(lt), b = up_dn(rt);
    if (a != b) {
        return (a < b) ? -1 : 1;
    }
    auto c = crs(lt, rt);
    return -sgn(c);
}

struct Circle {
    Pr o;
    Real r;
};



#undef Pt
#undef Pr
#undef St
#undef Sr
#undef Lt
#undef Lr