// 二维凸包（CG.md「二维凸包」中的静态凸包、点与凸包的位置关系）
// 依赖 base.hpp
#include "base.hpp"
// 说明：点与凸包的位置关系也可以直接用 base.hpp 的 inside(p, polygon)
// 注意：这里返回的凸包是逆时针顺序，与 base.hpp 中 Polygon 的约定一致
//（CG.md 原版把 cross 的两个参数写反了，得到的是顺时针凸包）

// 二维静态凸包（Andrew 算法），O(N log N)
// flag = 1：不把凸包边上的点、重复顶点加入凸包（严格）；flag = 0：加入
// 返回逆时针顺序、不含重复首尾点的凸包（点数 <= 2 时原样返回）
// CG.md: staticConvexHull
template <class T>
inline vector<Pnt<T>> staticConvexHull(vector<Pnt<T>> A, int flag = 1) {
    int n = (int)A.size();
    if (n <= 2) { // 特判
        return A;
    }
    vector<Pnt<T>> ans(n * 2);
    sort(A.begin(), A.end());
    // 逆时针凸包：出现非左转（叉积 <= 0）就回退；flag = 0 时允许边上的点
    auto bad = [&](T v) { return flag ? (v <= 0) : (v < 0); };
    int now = -1;
    for (int i = 0; i < n; i++) { // 维护下凸包
        while (now > 0 && bad(crs(ans[now - 1], ans[now], A[i]))) {
            now--;
        }
        ans[++now] = A[i];
    }
    int pre = now;
    for (int i = n - 2; i >= 0; i--) { // 维护上凸包
        while (now > pre && bad(crs(ans[now - 1], ans[now], A[i]))) {
            now--;
        }
        ans[++now] = A[i];
    }
    ans.resize(now); // 去掉重复的首尾点
    return ans;
}

// 点与凸包的位置关系：0 点在凸包外；1 在凸包上（含边与顶点）；2 在凸包内
// 传入的凸包需已按逆时针排好序（即 staticConvexHull 的结果）
// CG.md: contains(Point p, vector<Point> A)
template <class T>
inline int pointInHull(const Pnt<T>& p, const vector<Pnt<T>>& A) {
    int n = (int)A.size();
    bool in = false;
    for (int i = 0; i < n; i++) {
        Pnt<T> a = A[i] - p, b = A[(i + 1) % n] - p;
        if (a.y > b.y) {
            swap(a, b);
        }
        if (a.y <= 0 && 0 < b.y && crs(a, b) < 0) {
            in = !in;
        }
        if (crs(a, b) == 0 && dot(a, b) <= 0) { // 点在该边上
            return 1;
        }
    }
    return in ? 2 : 0;
}
