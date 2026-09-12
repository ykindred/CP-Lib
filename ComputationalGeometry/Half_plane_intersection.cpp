// 半平面交：求多条直线的「左侧」半平面的交集（CG.md「二维凸包」的半平面交）
// 依赖 base.hpp；交点用 base.hpp 的 intersection 计算，故只能是浮点
#include "base.hpp"
// 说明：半平面用 Lin<Real> 表示，约定取直线左侧（逆时针方向一侧）的半平面
// 半平面交的边界也是直线，可用 Lin 的 p, v 描述（CG.md 用两点式的 Line）

// CG.md: halfcut，返回交多边形的顶点（逆时针）；交集为空 / 无限大时返回空
inline vector<Pr> halfPlaneIntersect(vector<Lr> lines) {
    auto leftOf = [](const Pr& p, const Lr& l) { // 点在直线的左侧（在直线上不算）
        return rls(p, l) > 0;
    };
    // 按方向向量的极角排序：先上半平面（含 +x 轴），再下半平面，同组内按叉积
    sort(lines.begin(), lines.end(), [](const Lr& l1, const Lr& l2) {
        int d1 = up_dn(l1.v), d2 = up_dn(l2.v);
        if (d1 != d2) {
            return d1 > d2;
        }
        return crs(l1.v, l2.v) > 0;
    });
    // 预处理：方向相同的平行直线只保留更紧的那条
    //（原文档是在主循环里遇到同向平行线时改写 ls[0] 并 assert(ls.size() == 1)，
    // 但只要队列里还留着更早的直线（很常见）这个断言就会失败，这里提前归并掉）
    vector<Lr> uniq;
    for (const Lr& l : lines) {
        if (!uniq.empty() && sgn(crs(l.v, uniq.back().v)) == 0 && dot(l.v, uniq.back().v) > 0) {
            if (!leftOf(uniq.back().p, l)) { // 新直线更紧
                uniq.back() = l;
            }
            continue;
        }
        uniq.push_back(l);
    }
    deque<Lr> ls;
    deque<Pr> ps;
    for (const Lr& l : uniq) {
        if (ls.empty()) {
            ls.push_back(l);
            continue;
        }
        // 队尾、队首已经不在新半平面内的交点直接弹掉
        while (!ps.empty() && !leftOf(ps.back(), l)) {
            ps.pop_back();
            ls.pop_back();
        }
        while (!ps.empty() && !leftOf(ps[0], l)) {
            ps.pop_front();
            ls.pop_front();
        }
        if (sgn(crs(l.v, ls.back().v)) == 0) { // 平行
            if (dot(l.v, ls.back().v) > 0) {   // 同向：保留更紧的那条
                if (!leftOf(ls.back().p, l)) {
                    assert(ls.size() == 1);
                    ls[0] = l;
                }
                continue;
            }
            return {}; // 反向：交集为空
        }
        ps.push_back(intersection(ls.back(), l));
        ls.push_back(l);
    }
    while (!ps.empty() && !leftOf(ps.back(), ls[0])) {
        ps.pop_back();
        ls.pop_back();
    }
    if (ls.size() <= 2) { // 交不出有面积的区域
        return {};
    }
    // 首尾两条直线反向平行时交集无界（或为空），按本函数的约定返回空
    //（原文档没有这个判断，此时求交会除以 0，base.hpp 的 intersection 会断言失败）
    if (sgn(crs(ls[0].v, ls.back().v)) == 0) {
        return {};
    }
    ps.push_back(intersection(ls[0], ls.back()));
    return vector<Pr>(ps.begin(), ps.end());
}
