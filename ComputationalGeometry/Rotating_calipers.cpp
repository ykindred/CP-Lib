// 旋转卡壳（CG.md「常用例题」中的凸包最大三角形 / 最大四边形）
// 为了复用静态凸包，本文件直接包含 Convex_hull.cpp（其中已包含 base.hpp）
#include "Convex_hull.cpp"

// 三点的二倍面积（恒非负），即 CG.md 例题里的 triangleAreaEx
//（Polygon_extra.cpp 里的 areaEx 是带符号的版本）
// 整数请留意溢出
template <class T>
inline T triangleAreaEx(const Pnt<T>& a, const Pnt<T>& b, const Pnt<T>& c) {
    return abs(crs(a, b, c));
}

// 凸包上四个点能构成的最大四边形的二倍面积（旋转卡壳，O(N^2)）
// 传入的点集需已按逆时针排好序（即 staticConvexHull 的结果），且点数 >= 4
// CG.md 例题「凸包上的点能构成的最大四角形的面积（旋转卡壳）」: rotatingCalipers
template <class T>
inline T rotatingCalipers(const vector<Pnt<T>>& p) {
    int n = (int)p.size();
    T ans = 0;
    auto S = [&](int x, int y, int z) -> T { return triangleAreaEx(p[x], p[y], p[z]); };
    auto nxt = [&](int i) -> int { return i == n - 1 ? 0 : i + 1; };
    for (int i = 0; i < n; i++) { // 枚举对角线 (i, j)
        int p1 = nxt(i), p2 = nxt(nxt(nxt(i)));
        for (int j = nxt(nxt(i)); nxt(j) != i; j = nxt(j)) {
            while (nxt(p1) != j && S(i, j, nxt(p1)) > S(i, j, p1)) {
                p1 = nxt(p1);
            }
            if (p2 == j) {
                p2 = nxt(p2);
            }
            while (nxt(p2) != i && S(i, j, nxt(p2)) > S(i, j, p2)) {
                p2 = nxt(p2);
            }
            ans = max(ans, S(i, j, p1) + S(i, j, p2));
        }
    }
    return ans; // 二倍面积
}

// 凸包上三个点能构成的最大三角形的二倍面积（逐点爬坡，均摊 O(N)）
// 传入的点集需已按逆时针排好序（即 staticConvexHull 的结果）
// CG.md 例题「凸包上的点能构成的最大三角形（暴力枚举）」，原题输出的是三个下标
template <class T>
inline T maxTriangleInHull(const vector<Pnt<T>>& p) {
    int n = (int)p.size();
    auto S = [&](int x, int y, int z) -> T { return triangleAreaEx(p[x], p[y], p[z]); };
    int i = 0, j = 1, k = 2;
    while (true) { // 每一步面积都严格变大，故必然结束
        T val = S(i, j, k);
        if (S((i + 1) % n, j, k) > val) {
            i = (i + 1) % n;
        } else if (S((i - 1 + n) % n, j, k) > val) {
            i = (i - 1 + n) % n;
        } else if (S(i, (j + 1) % n, k) > val) {
            j = (j + 1) % n;
        } else if (S(i, (j - 1 + n) % n, k) > val) {
            j = (j - 1 + n) % n;
        } else if (S(i, j, (k + 1) % n) > val) {
            k = (k + 1) % n;
        } else if (S(i, j, (k - 1 + n) % n) > val) {
            k = (k - 1 + n) % n;
        } else {
            break;
        }
    }
    return S(i, j, k); // 二倍面积
}

// 平面上任意点集中选 4 个点能构成的最大四边形的二倍面积（可以退化、可以有点重合）
// CG.md 例题「平面若干点能构成的最大四边形的面积（困难版）」：
// 凸包大小 <= 2 时退化，答案为 0；恰好为 3 时是凹四边形，枚举不在凸包上的点用大三角形减小三角形；
// 恰好为 4 时是凸四边形，用旋转卡壳。T 请用 ll 或 Real（要用到 Pnt 的 ==）
template <class T>
inline T maxQuadrilateralArea(vector<Pnt<T>> in) {
    auto hull = staticConvexHull(in, 0); // 不严格凸包
    int n = (int)hull.size();
    T ans = 0;
    if (n > 3) {
        ans = rotatingCalipers(hull);
    } else if (n == 3) {
        T area = triangleAreaEx(hull[0], hull[1], hull[2]);
        for (const Pnt<T>& it : in) {
            if (it == hull[0] || it == hull[1] || it == hull[2]) {
                continue;
            }
            T mn = min({triangleAreaEx(it, hull[0], hull[1]), triangleAreaEx(it, hull[0], hull[2]),
                        triangleAreaEx(it, hull[1], hull[2])});
            ans = max(ans, area - mn);
        }
    }
    return ans; // 二倍面积
}
