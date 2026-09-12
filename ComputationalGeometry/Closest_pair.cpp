// 平面最近点对（set 解法，严格 O(N log N)）（CG.md「常用例题」）
// 依赖 base.hpp
#include "base.hpp"
// 借助 set 按 y 有序，横向用当前最优答案剪枝，比分治法稍快
// 返回最近点对距离的平方（整数坐标，避免开根号丢精度），需要距离时自行 sqrtl

// CG.md 例题「平面最近点对（set 解）」
inline ll closestPairDis2(vector<Pnt<ll>> in) {
    int n = (int)in.size();
    assert(n >= 2);
    ll d = dis2(in[0], in[1]); // 设定阈值的平方
    sort(in.begin(), in.end());
    set<Pnt<ll>> S; // 存 (y, x)，即交换坐标后按 y 排序，方便上下查找
    for (int i = 0, h = 0; i < n; i++) {
        Pnt<ll> now = {in[i].y, in[i].x};
        while (d && d <= (in[i].x - in[h].x) * (in[i].x - in[h].x)) { // 删除横坐标超过阈值的点
            S.erase(Pnt<ll>{in[h].y, in[h].x});
            h++;
        }
        auto it = S.lower_bound(now);
        for (auto k = it; k != S.end() && (k->x - now.x) * (k->x - now.x) < d; k++) {
            d = min(d, dis2(*k, now));
        }
        if (it != S.begin()) {
            for (auto k = prev(it); (k->x - now.x) * (k->x - now.x) < d; k--) {
                d = min(d, dis2(*k, now));
                if (k == S.begin()) {
                    break;
                }
            }
        }
        S.insert(now);
    }
    return d;
}
