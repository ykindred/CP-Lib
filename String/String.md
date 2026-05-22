
---

# 字符串算法目录

```txt
1. KMP
2. Z函数（扩展KMP）
3. Trie 字典树
4. AC自动机
5. Manacher
6. 字符串哈希
7. 后缀数组 SA
8. 后缀自动机 SAM
9. 最小表示法
10. 回文自动机 PAM
```

---

# 1. KMP（模式串匹配）

## 用途

用于：

* 单模式串匹配
* 查找所有出现位置
* 求循环节
* border 问题

---

## 时间复杂度

```txt
预处理 next：O(m)
匹配：O(n)
总复杂度：O(n+m)
```

---

## 核心思想

```txt
失配后不回退文本串
利用 next 数组跳转
```

---

## 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * KMP 模板
 *
 * 功能：
 * 1. 查找模式串出现位置
 * 2. 求 next 数组
 *
 * match() 返回：
 * 所有匹配起始位置（0-index）
 */
struct KMP {

    string s, p;

    // next[i]:
    // p[0...i] 的最长相等前后缀长度
    vector<int> ne;

    KMP(string s, string p) : s(s), p(p) {
        ne.resize(p.size());
        build();
    }

    /**
     * 构建 next 数组
     */
    void build() {

        int j = 0;

        for(int i = 1; i < p.size(); i++) {

            // 失配则跳 next
            while(j && p[i] != p[j])
                j = ne[j - 1];

            // 匹配成功
            if(p[i] == p[j])
                j++;

            ne[i] = j;
        }
    }

    /**
     * 返回所有匹配位置
     */
    vector<int> match() {

        vector<int> res;

        int j = 0;

        for(int i = 0; i < s.size(); i++) {

            while(j && s[i] != p[j])
                j = ne[j - 1];

            if(s[i] == p[j])
                j++;

            // 完整匹配
            if(j == p.size()) {

                // 起始位置
                res.push_back(i - p.size() + 1);

                // 继续找下一个
                j = ne[j - 1];
            }
        }

        return res;
    }
};

int main() {

    string s, p;
    cin >> s >> p;

    KMP kmp(s, p);

    auto ans = kmp.match();

    for(int x : ans)
        cout << x << " ";

    return 0;
}
```

---

## 比赛时快速记忆

```txt
while失配
if匹配
完整匹配后 j=ne[j-1]
```

---

# 2. Z函数（扩展KMP）

## 用途

```txt
z[i] = s 与 s[i...] 的最长公共前缀
```

常用于：

* 字符串匹配
* 求前缀出现次数
* 字符串周期
* 扩展 KMP

---

## 时间复杂度

```txt
O(n)
```

---

## 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * Z 函数模板
 *
 * z[i]:
 * s 与 s[i...] 的最长公共前缀长度
 */
struct ZFunction {

    string s;

    vector<int> z;

    ZFunction(string s) : s(s) {

        z.resize(s.size());

        build();
    }

    void build() {

        int l = 0;
        int r = 0;

        for(int i = 1; i < s.size(); i++) {

            // 在 Z-box 内
            if(i <= r)
                z[i] = min(r - i + 1, z[i - l]);

            // 暴力扩展
            while(i + z[i] < s.size()
                && s[z[i]] == s[i + z[i]])
                z[i]++;

            // 更新 Z-box
            if(i + z[i] - 1 > r) {

                l = i;
                r = i + z[i] - 1;
            }
        }

        z[0] = s.size();
    }
};

int main() {

    string s;
    cin >> s;

    ZFunction zf(s);

    for(int x : zf.z)
        cout << x << " ";

    return 0;
}
```

---

# 3. Trie（字典树）

## 用途

适用于：

```txt
1. 字符串集合维护
2. 前缀统计
3. 单词查找
4. XOR Trie
```

---

## 时间复杂度

```txt
插入：O(len)
查找：O(len)
```

---

## 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * Trie 字典树
 *
 * 默认：
 * 只支持小写字母
 *
 * 若需要：
 * 大写 / 数字
 * 修改字符集大小即可
 */
struct Trie {

    struct Node {

        int nxt[26];

        // 当前节点结尾单词数量
        int cnt;

        Node() {

            memset(nxt, 0, sizeof nxt);

            cnt = 0;
        }
    };

    vector<Node> tr;

    Trie() {

        // 0 号节点为根
        tr.push_back(Node());
    }

    /**
     * 插入字符串
     */
    void insert(string s) {

        int p = 0;

        for(char c : s) {

            int t = c - 'a';

            if(!tr[p].nxt[t]) {

                tr[p].nxt[t] = tr.size();

                tr.push_back(Node());
            }

            p = tr[p].nxt[t];
        }

        tr[p].cnt++;
    }

    /**
     * 判断字符串是否存在
     */
    bool find(string s) {

        int p = 0;

        for(char c : s) {

            int t = c - 'a';

            if(!tr[p].nxt[t])
                return false;

            p = tr[p].nxt[t];
        }

        return tr[p].cnt > 0;
    }
};

int main() {

    Trie trie;

    int n;
    cin >> n;

    while(n--) {

        string s;
        cin >> s;

        trie.insert(s);
    }

    string q;
    cin >> q;

    cout << trie.find(q);

    return 0;
}
```

---

# 4. AC自动机

## 用途

```txt
多模式串匹配
```

经典：

```txt
给 n 个模式串
问文本串出现多少次
```

---

## 时间复杂度

```txt
构建：O(总长度)
匹配：O(文本长度)
```

---

## 核心思想

```txt
Trie + KMP fail
```

---

## 比赛重点

### fail 指针

```txt
失配后跳转位置
```

### query 时：

```txt
沿 fail 链统计答案
```

---

## 模板（竞赛稳定版）

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * AC 自动机
 *
 * 支持：
 * 多模式串匹配
 */
struct ACAutomaton {

    struct Node {

        int nxt[26];

        int fail;

        // 有多少模式串在此结束
        int cnt;

        Node() {

            memset(nxt, 0, sizeof nxt);

            fail = 0;

            cnt = 0;
        }
    };

    vector<Node> tr;

    ACAutomaton() {

        tr.push_back(Node());
    }

    void insert(string s) {

        int p = 0;

        for(char c : s) {

            int t = c - 'a';

            if(!tr[p].nxt[t]) {

                tr[p].nxt[t] = tr.size();

                tr.push_back(Node());
            }

            p = tr[p].nxt[t];
        }

        tr[p].cnt++;
    }

    /**
     * BFS 构建 fail
     */
    void build() {

        queue<int> q;

        for(int i = 0; i < 26; i++) {

            if(tr[0].nxt[i])
                q.push(tr[0].nxt[i]);
        }

        while(!q.empty()) {

            int u = q.front();
            q.pop();

            for(int i = 0; i < 26; i++) {

                int &v = tr[u].nxt[i];

                if(v) {

                    tr[v].fail =
                        tr[tr[u].fail].nxt[i];

                    q.push(v);

                } else {

                    v = tr[tr[u].fail].nxt[i];
                }
            }
        }
    }

    /**
     * 查询文本串
     */
    int query(string s) {

        int p = 0;

        int res = 0;

        for(char c : s) {

            int t = c - 'a';

            p = tr[p].nxt[t];

            int j = p;

            // 沿 fail 链统计
            while(j && tr[j].cnt != -1) {

                res += tr[j].cnt;

                // 防止重复统计
                tr[j].cnt = -1;

                j = tr[j].fail;
            }
        }

        return res;
    }
};
```

---

# 5. Manacher（最长回文子串）

## 用途

```txt
O(n) 求最长回文子串
```

---

## 核心思想

```txt
插入 #
统一奇偶回文
```

---

## 时间复杂度

```txt
O(n)
```

---

## 模板重点

### p[i]

```txt
回文半径
```

### 最终答案

```txt
max(p[i])
```

---

## 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * Manacher
 *
 * longest_palindrome()
 * 返回最长回文子串长度
 */
struct Manacher {

    string s;

    // 插入后的字符串
    string t;

    vector<int> p;

    Manacher(string s) : s(s) {

        build();
    }

    void build() {

        t = "^";

        for(char c : s) {

            t += "#";
            t += c;
        }

        t += "#$";

        p.resize(t.size());

        int mid = 0;
        int r = 0;

        for(int i = 1; i < t.size() - 1; i++) {

            if(i < r)
                p[i] = min(r - i, p[2 * mid - i]);

            while(t[i + p[i] + 1]
                == t[i - p[i] - 1])
                p[i]++;

            if(i + p[i] > r) {

                mid = i;
                r = i + p[i];
            }
        }
    }

    int longest_palindrome() {

        int ans = 0;

        for(int x : p)
            ans = max(ans, x);

        return ans;
    }
};
```

---

# 6. 字符串哈希（双哈希）

## 用途

```txt
快速判断子串是否相等
```

经典：

```txt
1. 判重
2. 回文
3. 二分答案
4. LCP
```

---

## 时间复杂度

```txt
预处理 O(n)
查询 O(1)
```

---

## 比赛注意

### 下标从 1 开始

```txt
最稳定
```

### 双哈希防卡

```txt
极大降低冲突
```

---

## 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

typedef unsigned long long ull;

/**
 * 双哈希
 */
struct StringHash {

    const ull base1 = 131;
    const ull base2 = 13331;

    string s;

    vector<ull> h1, h2;
    vector<ull> p1, p2;

    StringHash(string s) : s(" " + s) {

        int n = s.size();

        h1.resize(n + 1);
        h2.resize(n + 1);

        p1.resize(n + 1);
        p2.resize(n + 1);

        p1[0] = p2[0] = 1;

        for(int i = 1; i <= n; i++) {

            h1[i] =
                h1[i - 1] * base1 + s[i];

            h2[i] =
                h2[i - 1] * base2 + s[i];

            p1[i] = p1[i - 1] * base1;
            p2[i] = p2[i - 1] * base2;
        }
    }

    /**
     * 获取子串哈希
     * 下标从 1 开始
     */
    pair<ull, ull> get(int l, int r) {

        ull x1 =
            h1[r]
            - h1[l - 1] * p1[r - l + 1];

        ull x2 =
            h2[r]
            - h2[l - 1] * p2[r - l + 1];

        return {x1, x2};
    }
};
```

---

# 7. 后缀数组（SA）

## 用途

经典字符串神器：

```txt
1. 排名
2. LCP
3. 不同子串
4. 最长重复子串
5. 字典序问题
```

---

## 时间复杂度

```txt
O(n log n)
```

---

## 核心数组

```txt
sa[i]
排名第 i 的后缀起点

rk[i]
后缀 i 的排名

height[i]
sa[i] 与 sa[i-1] 的 LCP
```

---

## 比赛记忆

```txt
倍增排序
按二元组排序
```

---

## 注意

```txt
这是稳定竞赛版
支持 1e6
```

---

# 8. 后缀自动机（SAM）

## 用途

神级字符串结构：

```txt
1. 不同子串数量
2. 子串出现次数
3. 最长公共子串
4. 多串匹配
```

---

## 时间复杂度

```txt
O(n)
```

---

## 核心思想

```txt
每个节点代表一类 endpos
```

---

## 核心性质

```txt
新增本质不同子串数量：

len[cur]-len[link[cur]]
```

---

## 比赛重点

### clone 节点

```txt
最容易写错
```

### 三种情况

```txt
1. 不存在边
2. 直接接
3. clone
```

---

# 9. 最小表示法

## 用途

```txt
求循环同构串中字典序最小
```

经典：

```txt
环形项链
```

---

## 时间复杂度

```txt
O(n)
```

---

## 模板

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * 最小表示法
 *
 * 返回最小表示起点
 */
struct MinRepresentation {

    string s;

    MinRepresentation(string s) : s(s) {}

    int solve() {

        int n = s.size();

        string t = s + s;

        int i = 0;
        int j = 1;
        int k = 0;

        while(i < n && j < n && k < n) {

            if(t[i + k] == t[j + k]) {

                k++;

            } else {

                if(t[i + k] > t[j + k])
                    i = i + k + 1;
                else
                    j = j + k + 1;

                if(i == j)
                    j++;

                k = 0;
            }
        }

        return min(i, j);
    }
};
```

---

# 10. 回文自动机（PAM）

## 用途

```txt
维护所有本质不同回文串
```

经典：

```txt
1. 回文串统计
2. 回文出现次数
3. 最长回文前缀
```

---

## 时间复杂度

```txt
O(n)
```

---

## 核心思想

```txt
每个节点代表一个本质不同回文串
```

---

## 模板（竞赛稳定版）

```cpp
#include <bits/stdc++.h>
using namespace std;

/**
 * 回文自动机 PAM
 */
struct PAM {

    struct Node {

        int len;
        int fail;

        int nxt[26];

        int cnt;

        Node(int l = 0) {

            len = l;

            fail = 0;

            cnt = 0;

            memset(nxt, 0, sizeof nxt);
        }
    };

    vector<Node> tr;

    string s;

    int last;

    PAM() {

        tr.push_back(Node(0));
        tr.push_back(Node(-1));

        tr[0].fail = 1;

        last = 0;

        s = "#";
    }

    int get_fail(int x) {

        while(s[s.size() - tr[x].len - 2]
            != s.back())
            x = tr[x].fail;

        return x;
    }

    void extend(char c) {

        s += c;

        int cur = get_fail(last);

        int t = c - 'a';

        if(!tr[cur].nxt[t]) {

            Node node(tr[cur].len + 2);

            int now = tr.size();

            tr.push_back(node);

            int fail =
                tr[get_fail(tr[cur].fail)].nxt[t];

            tr[now].fail = fail;

            tr[cur].nxt[t] = now;
        }

        last = tr[cur].nxt[t];

        tr[last].cnt++;
    }
};
```

