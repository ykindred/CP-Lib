#include "testlib.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// 将 total 随机拆分为 cnt 个正整数，返回 vector<int>，每个元素 >= 1 且总和 = total
vector<int> split_n(int cnt, int total) {
    if (cnt > total) {
        quitf(_fail, "tcnt (%d) cannot be greater than ntotal (%d)", cnt, total);
    }
    vector<int> parts;
    // 先给每组分配 1，剩余 total - cnt 再随机分配
    int remaining = total - cnt;
    // 生成 cnt-1 个分割点，范围 [0, remaining]
    vector<int> points;
    for (int i = 0; i < cnt - 1; ++i) {
        points.push_back(rnd.next(0, remaining));
    }
    sort(points.begin(), points.end());
    // 计算各段长度
    int prev = 0;
    for (int i = 0; i < cnt - 1; ++i) {
        parts.push_back(points[i] - prev + 1); // +1 是因为基础 1
        prev = points[i];
    }
    parts.push_back(remaining - prev + 1);
    // 随机打乱各组顺序，使分配更随机
    shuffle(parts.begin(), parts.end());
    return parts;
}

int main(int argc, char* argv[]) {
    // 注册生成器，第一个参数为命令行参数个数，第二个为参数数组，第三个为版本
    registerGen(argc, argv, 1);

    // 读取参数，必须提供
    int tcnt = opt<int>("tcnt");
    int ntotal = opt<int>("ntotal");
    int vmin = opt<int>("vmin");
    int vmax = opt<int>("vmax");

    // 参数合法性检查
    if (tcnt <= 0 || ntotal <= 0) {
        quitf(_fail, "tcnt and ntotal must be positive integers");
    }
    if (vmin > vmax) {
        quitf(_fail, "vmin (%d) must be <= vmax (%d)", vmin, vmax);
    }

    // 拆分 ntotal
    vector<int> ns = split_n(tcnt, ntotal);

    // 输出第一行：数据组数
    cout << tcnt << "\n";

    // 逐组输出
    for (int i = 0; i < tcnt; ++i) {
        int n = ns[i];
        cout << n << "\n";
        // 生成 n 个 [vmin, vmax] 之间的随机整数
        for (int j = 0; j < n; ++j) {
            if (j) cout << ' ';
            cout << rnd.next(vmin, vmax);
        }
        cout << "\n";
    }

    return 0;
}