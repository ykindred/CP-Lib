// 平面几何杂项（CG.md「平面几何必要初始化」中 base.hpp 未覆盖的部分）
// 依赖 base.hpp：Real / EPS / sgn / cmp
#include "base.hpp"

// 实数域 gcd（CG.md: fgcd(ld x, ld y)）
inline Real fgcd(Real x, Real y) {
    return fabsl(y) < EPS ? fabsl(x) : fgcd(y, fmodl(x, y));
}

// 近似相等（等价于 cmp(x, y) == 0）
// CG.md: equal(T x, S y)
template <class T, class S>
inline bool equal(T x, S y) {
    return -EPS < x - y && x - y < EPS;
}

// 字符串读入浮点数：截断到 k 位小数后转为整数（放大 10^k 倍）
// 定点化之后就能用整数精确运算，避免浮点误差。CG.md: read(int k = Knum)
inline ll readReal(int k = 4) {
    string s;
    cin >> s;

    int num = 0;
    int it = (int)s.find('.');
    if (it != -1) {                   // 存在小数点
        num = (int)s.size() - it - 1; // 计算小数位数
        s.erase(s.begin() + it);      // 删除小数点
    }
    if (num > k) {                       // 小数位过多时截断（原文档未处理这种情况）
        s.erase(s.size() - (num - k));   // 删掉末尾多余的 num - k 位
        num = k;
    }
    for (int i = 1; i <= k - num; i++) { // 补全小数位数
        s += '0';
    }
    return stoll(s);
}
