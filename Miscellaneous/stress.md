在同一目录放 4 个文件：

```text
problem/
├── stress.py   # 对拍脚本（本文件配套）
├── std.cpp     # 你要检查的程序
├── brute.cpp   # 可信的暴力/正确解
└── gen.cpp     # 随机数据生成器
```

打开终端，进入该目录：

```bash
cd /path/to/problem
python3 stress.py
# 或者, 保证std正确, 只验证时限(无需brute):
python3 stress.py time 2
```


```cpp
// gen.cpp
#include <bits/stdc++.h>
using namespace std;

int main() {
    mt19937 rng(42);
    auto rnd = [&](int l, int r) {
        return uniform_int_distribution<int>(l, r)(rng);
    };

    int n = rnd(1, 30);
    cout << n << '\n';
    for (int i = 0; i < n; ++i) {
        cout << rnd(-10, 10) << " \n"[i == n - 1];
    }
}
```