// 闵可夫斯基和：两个凸包合成的大凸包（CG.md「二维凸包」的闵可夫斯基和）
// 依赖 base.hpp
#include "base.hpp"
// 两个凸包都需按逆时针顺序给出（即 staticConvexHull 的结果，见 Convex_hull.cpp）
// T 请用 ll 或 Real（函数内部用到了 sgn）

// CG.md: mincowski（原文档拼写），返回逆时针的闵可夫斯基和凸包
template <class T>
inline vector<Pnt<T>> minkowskiSum(const vector<Pnt<T>>& P1, const vector<Pnt<T>>& P2) {
    int n = (int)P1.size(), m = (int)P2.size();
    vector<Pnt<T>> V1(n), V2(m);
    for (int i = 0; i < n; i++) {
        V1[i] = P1[(i + 1) % n] - P1[i]; // 边向量，已按极角有序
    }
    for (int i = 0; i < m; i++) {
        V2[i] = P2[(i + 1) % m] - P2[i];
    }
    vector<Pnt<T>> ans = {P1.front() + P2.front()};
    int i = 0, j = 0;
    while (i < n && j < m) { // 按极角归并两条边链
        Pnt<T> val = sgn(crs(V1[i], V2[j])) > 0 ? V1[i++] : V2[j++];
        ans.push_back(ans.back() + val);
    }
    while (i < n) {
        ans.push_back(ans.back() + V1[i++]);
    }
    while (j < m) {
        ans.push_back(ans.back() + V2[j++]);
    }
    return ans;
}
