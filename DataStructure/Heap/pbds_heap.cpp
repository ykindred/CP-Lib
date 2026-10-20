#include <iostream>
#include <vector>
#include <ext/pb_ds/priority_queue.hpp>

using namespace std;

// ===================== pb_ds 堆模板区 =====================

// 1. 基础类型别名 (解决与 std::priority_queue 的命名冲突)
// 大根堆 (默认)
template<typename T>
using pb_max_heap = __gnu_pbds::priority_queue<T, less<T>, __gnu_pbds::pairing_heap_tag>;

// 小根堆
template<typename T>
using pb_min_heap = __gnu_pbds::priority_queue<T, greater<T>, __gnu_pbds::pairing_heap_tag>;

// 2. 自定义结构体
struct Node {
    int id;       // 节点编号 (通常用于映射迭代器)
    long long val; // 权值
};

// 3. 自定义比较器 (用于结构体)
struct Cmp {
    // 这里写 > 是小根堆，写 < 是大根堆
    bool operator()(const Node& a, const Node& b) const {
        return a.val > b.val; 
    }
};

// 带自定义比较器的配对堆别名
using pb_node_heap = __gnu_pbds::priority_queue<Node, Cmp, __gnu_pbds::pairing_heap_tag>;
// 定义迭代器类型，方便后续写代码
using point_it = pb_node_heap::point_iterator;

// ==========================================================

const int MAXN = 100005;

// 用于记录每个 id 对应的堆中迭代器，初始全为 nullptr (即 nullptr 泛型的等价，pb_ds中为 0/NULL)
point_it it_pos[MAXN]; 

void solve() {
    pb_node_heap h1, h2;

    // ------------------------------------------------
    // 场景 1: 插入并记录迭代器 (O(1))
    // ------------------------------------------------
    it_pos[1] = h1.push({1, 50});
    it_pos[2] = h1.push({2, 30});
    it_pos[3] = h1.push({3, 40});

    // ------------------------------------------------
    // 场景 2: 动态修改权值 (Decrease-Key / Modify, 均摊 O(log n))
    // ------------------------------------------------
    // 比如我们要把 id=1 的节点权值修改为 10
    if (it_pos[1] != nullptr) {
        h1.modify(it_pos[1], {1, 10}); 
        // 此时 id=1 (val=10) 自动浮到小根堆顶
    }

    // ------------------------------------------------
    // 场景 3: 任意节点删除 (均摊 O(log n))
    // ------------------------------------------------
    // 直接在堆中删掉 id=3 的节点
    if (it_pos[3] != nullptr) {
        h1.erase(it_pos[3]);
        it_pos[3] = nullptr; // 记得清空，防止出现野指针
    }

    // ------------------------------------------------
    // 场景 4: 堆的合并 (O(1))
    // ------------------------------------------------
    h2.push({4, 15});
    h2.push({5, 5});

    // 将 h2 的元素全部合并进 h1
    // 注意：操作后 h1 包含所有元素，而 h2 变为空堆 (size = 0)
    h1.join(h2);

    // ------------------------------------------------
    // 场景 5: 常规访问与弹出 (均摊 O(log n))
    // ------------------------------------------------
    cout << "当前堆内元素(按升序): \n";
    while (!h1.empty()) {
        Node u = h1.top();
        h1.pop();
        // 弹出后，该节点在堆中的迭代器就失效了，更新映射
        it_pos[u.id] = nullptr; 
        
        cout << "ID: " << u.id << ", Val: " << u.val << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}