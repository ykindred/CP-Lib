#include <iostream>
#include <ext/rope> // 必须包含的头文件

using namespace std;
using namespace __gnu_cxx; // 必须声明的命名空间

int main() {
    // 1. 定义：可以存 int，也可以直接用 crope（即 rope<char>）处理字符串
    rope<int> rp; 
    crope str_rp = "HelloWorld";

    // 2. 尾部添加元素
    rp.push_back(10);
    rp.push_back(20);
    rp.push_back(30); // 当前序列: [10, 20, 30]

    // 3. 在下标 k 处插入元素（0-indexed），复杂度 O(log n)
    rp.insert(1, 99); // 在下标 1 处插入 99 -> [10, 99, 20, 30]

    // 4. 删除：从下标 pos 开始删除 len 个元素，复杂度 O(log n)
    rp.erase(2, 1);   // 从下标 2 开始删 1 个 -> [10, 99, 30]

    // 5. 提取子区间：从 pos 开始长度为 len 的子序列，复杂度 O(log n)
    rope<int> sub = rp.substr(1, 2); // 提取 [99, 30]

    // 6. 区间剪切与粘贴（比如把区间 [l, r] 移到最前面）
    // 相当于：截取子串 + 原处删除 + 头部拼接，全部 O(log n)
    rp = rp.substr(1, 2) + rp.substr(0, 1); // 变成 [99, 30, 10]

    // 7. 单点修改：注意 rp[k] 返回的是只读引用，修改最好用 replace
    rp.replace(0, 888); // 将下标 0 的元素替换为 888 -> [888, 30, 10]

    // 8. 遍历与访问
    for (int i = 0; i < rp.size(); i++) {
        cout << rp[i] << " "; // 单次访问 O(log n)
    }
    return 0;
}