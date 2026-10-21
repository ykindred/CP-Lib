using ll = long long;

struct Line {
    mutable ll k, b, p; // p 存储当前直线与下一条直线的交点横坐标（有效区间右端点）
    bool operator<(const Line& o) const { return k < o.k; }
    bool operator<(ll x) const { return p < x; } // 用于 lower_bound 按交点二分查询
};

// 动态维护 max(k * x + b)，若求 min 则插入 (-k, -b) 并将结果取反即可
struct LineContainer : multiset<Line, less<>> {
    static const ll inf = LLONG_MAX;

    // 整数向下取整除法：用于精确计算两条直线的交点横坐标 floor(a / b)
    ll div(ll a, ll b) {
        return a / b - ((a ^ b) < 0 && a % b);
    }

    // 计算直线 x 与相邻下一条直线 y 的交点，并判断 y 是否已经被完全覆盖而失效
    bool isect(iterator x, iterator y) {
        if (y == end()) return x->p = inf, 0;
        if (x->k == y->k) x->p = x->b > y->b ? inf : -inf;
        else x->p = div(y->b - x->b, x->k - y->k);
        return x->p >= y->p;
    }

    // 插入一条任意斜率的直线 y = kx + b，均摊 O(log N)
    void add(ll k, ll b) {
        auto z = insert({k, b, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z); // 向右淘汰冗余后继
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y)); // 检查当前插入点本身是否冗余
        while ((y = x) != begin() && (--x)->p >= y->p) isect(x, erase(y)); // 向左淘汰冗余前驱
    }

    // 查询在给定 x 处的最大值 max(kx + b)，单次 O(log N)
    ll query(ll x) {
        auto l = *lower_bound(x); // 直接二分找到第一个有效右端点 >= x 的直线
        return l.k * x + l.b;
    }
};