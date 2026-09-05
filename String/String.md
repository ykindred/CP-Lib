# 字符串算法板子（ACM 竞赛用）

统一环境：C++17，`#include <bits/stdc++.h>`，`using namespace std;`。
默认字符集为小写字母 `a-z`，其他字符集只需修改常量 `ALPHA` 与映射 `c - 'a'`。
所有代码均经过 GCC 13.2 编译与样例验证。

## 复杂度速查

| 算法 | 构建 | 单次查询/操作 | 内存 |
| --- | --- | --- | --- |
| 字符串哈希 | O(n) | O(1) / O(log n)（二分 LCP） | O(n) |
| KMP | O(n) | O(n)（整体匹配） | O(n) |
| Z 函数 | O(n) | O(1) | O(n) |
| Manacher | O(n) | O(1) | O(n) |
| Trie | O(Σlen) | O(len) | O(Σlen·ALPHA) |
| AC 自动机 | O(Σlen·ALPHA) | O(文本长度) | O(Σlen·ALPHA) |
| 后缀数组 | O(n log n) | O(1)（配合 ST 表） | O(n) |
| 后缀自动机 | O(n) | O(len) | O(n·ALPHA) |
| 回文自动机 | O(n) | O(1) | O(n·ALPHA) |
| 最小表示法 | O(n) | — | O(1) |
| Lyndon 分解 | O(n) | — | O(n) |

---

## 目录

1. 字符串哈希
2. KMP
3. Z 函数（扩展 KMP）
4. Manacher
5. Trie
6. AC 自动机
7. 后缀数组 SA
8. 后缀自动机 SAM（含广义 SAM）
9. 回文自动机 PAM
10. 最小表示法
11. Lyndon 分解（Duval）
12. 常用技巧与结论

---

## 1. 字符串哈希

双模哈希，支持子串 O(1) 判等、二分求 LCP、正反哈希判回文。

```cpp
struct StringHash {
    using ull = unsigned long long;
    static const ull B = 131;                      // 基数（可换随机大质数防卡）
    static const ull M1 = 1000000007ull;
    static const ull M2 = 1000000009ull;
    int n;
    vector<ull> h1, h2, p1, p2;

    StringHash(const string& s) : n((int)s.size()),
        h1(n + 1), h2(n + 1), p1(n + 1, 1), p2(n + 1, 1) {
        for (int i = 1; i <= n; i++) {
            p1[i] = p1[i - 1] * B % M1;
            p2[i] = p2[i - 1] * B % M2;
            h1[i] = (h1[i - 1] * B + s[i - 1]) % M1;
            h2[i] = (h2[i - 1] * B + s[i - 1]) % M2;
        }
    }
    // 子串 [l, r]（0-indexed，闭区间）的哈希值
    pair<ull, ull> get(int l, int r) const {
        ull v1 = (h1[r + 1] + M1 - h1[l] * p1[r - l + 1] % M1) % M1;
        ull v2 = (h2[r + 1] + M2 - h2[l] * p2[r - l + 1] % M2) % M2;
        return {v1, v2};
    }
    // 本串从 i 起与 o 串从 j 起的后缀的 LCP 长度（二分）
    int lcp(const StringHash& o, int i, int j) const {
        int lo = 0, hi = min(n - i, o.n - j);
        while (lo < hi) {
            int mid = (lo + hi + 1) >> 1;
            if (get(i, i + mid - 1) == o.get(j, j + mid - 1)) lo = mid;
            else hi = mid - 1;
        }
        return lo;
    }
};
```

用法示例：

```cpp
StringHash h(s);
if (h.get(l1, r1) == h.get(l2, r2)) { /* 两子串相等 */ }
// 回文判断：建正串与反串两个哈希，h.get(l, r) == rev.get(n-1-r, n-1-l)
// 字典序比较两个子串：先二分 LCP，再比较下一位字符
```

---

## 2. KMP

### 2.1 前缀函数

`pi[i]` = 以 `s[i]` 结尾的最长真前后缀匹配长度。

```cpp
vector<int> prefix_function(const string& s) {
    int n = (int)s.size();
    vector<int> pi(n);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    return pi;
}
```

### 2.2 匹配

```cpp
// 求模式串 t 在文本串 s 中的所有出现起始位置（0-indexed）
vector<int> kmp_find(const string& s, const string& t) {
    vector<int> pi = prefix_function(t), res;
    int j = 0;
    for (int i = 0; i < (int)s.size(); i++) {
        while (j > 0 && s[i] != t[j]) j = pi[j - 1];
        if (s[i] == t[j]) j++;
        if (j == (int)t.size()) {
            res.push_back(i - (int)t.size() + 1);
            j = pi[j - 1];
        }
    }
    return res;
}
```

### 2.3 周期相关

```cpp
// 最小周期 = n - pi[n-1]；s 有整周期 p 当且仅当 n % (n - pi[n-1]) == 0
// 例：判断 s 是否由 t 重复 k 次组成：s 为 t+t 的前缀且 n % m == 0（m = t.size()）
```

---

## 3. Z 函数（扩展 KMP）

`z[i]` = s 与 s[i...] 的最长公共前缀长度。

```cpp
vector<int> z_function(const string& s) {
    int n = (int)s.size();
    vector<int> z(n);
    z[0] = n;
    for (int i = 1, l = 0, r = 0; i < n; i++) {
        if (i <= r) z[i] = min(r - i + 1, z[i - l]);
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) z[i]++;
        if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
    }
    return z;
}

// 用法：求 t 在 s 中所有出现位置
// 对 p = t + '#' + s 求 z，若 z[i] == t.size() 则出现位置为 i - t.size() - 1
// 扩展 KMP（s 每个后缀与 t 的 LCP）：对 p = t + '#' + s 求 z，z[i + t.size() + 1]
```

---

## 4. Manacher

```cpp
struct Manacher {
    int n;          // 原串长度
    vector<int> d;  // 变换后串中以 i 为中心的最长回文半径（含中心字符）
    Manacher(const string& s) {
        string t = "#";
        for (char c : s) { t += c; t += '#'; }
        n = (int)s.size();
        int m = (int)t.size();
        d.assign(m, 0);
        for (int i = 0, l = 0, r = -1; i < m; i++) {
            int k = (i > r) ? 1 : min(d[l + r - i], r - i + 1);
            while (0 <= i - k && i + k < m && t[i - k] == t[i + k]) k++;
            d[i] = k;
            if (i + k - 1 > r) l = i - k + 1, r = i + k - 1;
        }
    }
    // s[l..r] 是否为回文
    bool is_pal(int l, int r) const { return d[l + r + 1] - 1 >= r - l + 1; }
    // 以 i 为中心的最长奇回文子串长度
    int odd_len(int i) const { return d[2 * i + 1] - 1; }
    // 以 i-1 与 i 之间为对称轴的最长偶回文子串长度
    int even_len(int i) const { return d[2 * i] - 1; }
};
```

---

## 5. Trie

改 `ALPHA = 2` 即得 01-Trie（可用于最大异或对、按位贪心等）。

```cpp
struct Trie {
    static const int ALPHA = 26;
    vector<array<int, ALPHA>> ch;
    vector<int> cnt;   // 以该节点结尾的串数

    Trie() { new_node(); }
    int new_node() {
        ch.push_back({});
        ch.back().fill(0);
        cnt.push_back(0);
        return (int)ch.size() - 1;
    }
    void insert(const string& s) {
        int u = 0;
        for (char c : s) {
            int x = c - 'a';
            if (!ch[u][x]) ch[u][x] = new_node();
            u = ch[u][x];
        }
        cnt[u]++;
    }
    bool find(const string& s) {
        int u = 0;
        for (char c : s) {
            int x = c - 'a';
            if (!ch[u][x]) return false;
            u = ch[u][x];
        }
        return cnt[u] > 0;
    }
};
```

---

## 6. AC 自动机

```cpp
struct ACAutomaton {
    static const int ALPHA = 26;
    vector<array<int, ALPHA>> ch;
    vector<int> fail, cnt;   // cnt: 以该节点为结尾的模式串数（build 后含 fail 链累加）
    vector<int> order;       // BFS 序（即 fail 树的拓扑序）
    vector<int> end_node;    // 第 i 个插入模式串对应的终止节点

    ACAutomaton() { new_node(); }
    int new_node() {
        ch.push_back({});
        ch.back().fill(0);
        fail.push_back(0);
        cnt.push_back(0);
        return (int)ch.size() - 1;
    }
    void insert(const string& s) {
        int u = 0;
        for (char c : s) {
            int x = c - 'a';
            if (!ch[u][x]) ch[u][x] = new_node();
            u = ch[u][x];
        }
        cnt[u]++;
        end_node.push_back(u);
    }
    void build() {
        queue<int> q;
        for (int c = 0; c < ALPHA; c++) if (ch[0][c]) q.push(ch[0][c]);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int c = 0; c < ALPHA; c++) {
                int v = ch[u][c];
                if (v) {
                    fail[v] = ch[fail[u]][c];
                    cnt[v] += cnt[fail[v]];      // 匹配到 v 即匹配其所有后缀模式
                    q.push(v);
                } else {
                    ch[u][c] = ch[fail[u]][c];   // 补全转移
                }
            }
        }
    }
    // 文本串中所有模式串的总出现次数
    int query_total(const string& t) {
        int u = 0, ans = 0;
        for (char c : t) {
            u = ch[u][c - 'a'];
            ans += cnt[u];
        }
        return ans;
    }
    // 每个模式串各自的出现次数（按 fail 树自底向上累加经过次数）
    vector<int> query_each(const string& t) {
        vector<int> f((int)ch.size(), 0);
        int u = 0;
        for (char c : t) { u = ch[u][c - 'a']; f[u]++; }
        for (int i = (int)order.size() - 1; i >= 0; i--) {
            int v = order[i];
            f[fail[v]] += f[v];
        }
        vector<int> res;
        for (int v : end_node) res.push_back(f[v]);
        return res;
    }
};
```

---

## 7. 后缀数组 SA

倍增 + 计数排序，O(n log n)。`sa` 后缀数组，`rk` 排名，`lcp[i] = LCP(sa[i-1], sa[i])`（i ≥ 1）。

```cpp
struct SuffixArray {
    int n;
    string s;
    vector<int> sa, rk, lcp;

    SuffixArray(const string& str) : s(str), n((int)str.size()) {
        sa.resize(n); rk.resize(n);
        iota(sa.begin(), sa.end(), 0);
        sort(sa.begin(), sa.end(), [&](int a, int b) { return s[a] < s[b]; });
        rk[sa[0]] = 0;
        for (int i = 1; i < n; i++)
            rk[sa[i]] = rk[sa[i - 1]] + (s[sa[i]] != s[sa[i - 1]]);
        int m = rk[sa[n - 1]] + 1;
        vector<int> cnt(n), tmp(n);
        for (int k = 1; rk[sa[n - 1]] != n - 1; k <<= 1, m = rk[sa[n - 1]] + 1) {
            int p = 0;
            for (int i = n - k; i < n; i++) tmp[p++] = i;        // 第二关键字
            for (int i = 0; i < n; i++) if (sa[i] >= k) tmp[p++] = sa[i] - k;
            fill(cnt.begin(), cnt.begin() + m, 0);               // 按第一关键字计数排序
            for (int i = 0; i < n; i++) cnt[rk[tmp[i]]]++;
            for (int i = 1; i < m; i++) cnt[i] += cnt[i - 1];
            for (int i = n - 1; i >= 0; i--) sa[--cnt[rk[tmp[i]]]] = tmp[i];
            vector<int> old = rk;                                // 更新排名
            rk[sa[0]] = 0;
            for (int i = 1; i < n; i++) {
                pair<int, int> cur = {old[sa[i]], sa[i] + k < n ? old[sa[i] + k] : -1};
                pair<int, int> pre = {old[sa[i - 1]], sa[i - 1] + k < n ? old[sa[i - 1] + k] : -1};
                rk[sa[i]] = rk[sa[i - 1]] + (cur != pre);
            }
        }
        build_lcp();
    }
    void build_lcp() {   // Kasai 算法
        lcp.assign(n, 0);
        int h = 0;
        for (int i = 0; i < n; i++) {
            if (rk[i] == 0) continue;
            int j = sa[rk[i] - 1];
            while (i + h < n && j + h < n && s[i + h] == s[j + h]) h++;
            lcp[rk[i]] = h;
            if (h) h--;
        }
    }
};
```

### 7.1 ST 表：任意两后缀 LCP

```cpp
struct SparseTable {
    vector<vector<int>> st;
    vector<int> lg;
    SparseTable(const vector<int>& a) {
        int n = (int)a.size();
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) lg[i] = lg[i >> 1] + 1;
        st.assign(lg[n] + 1, vector<int>(n));
        st[0] = a;
        for (int k = 1; k <= lg[n]; k++)
            for (int i = 0; i + (1 << k) <= n; i++)
                st[k][i] = min(st[k - 1][i], st[k - 1][i + (1 << (k - 1))]);
    }
    int query(int l, int r) const {   // [l, r]
        int k = lg[r - l + 1];
        return min(st[k][l], st[k][r - (1 << k) + 1]);
    }
};

// 后缀 i 与后缀 j 的 LCP：
// int a = min(sa.rk[i], sa.rk[j]), b = max(sa.rk[i], sa.rk[j]);
// int len = st.query(a + 1, b);
```

### 7.2 常用应用

- 本质不同子串数 = `n * (n + 1) / 2 - sum(lcp[1..n-1])`
- 最长公共子串（多串）：所有串用不同分隔符拼接后建 SA，二分长度，按 lcp ≥ mid 分组，看是否每组覆盖所有串
- 后缀排序字典序第 k 大：直接在 sa 上取
- 连续出现 k 次的子串最长长度：lcp 数组上滑动窗口最小值

---

## 8. 后缀自动机 SAM

```cpp
struct SuffixAutomaton {
    struct Node {
        int len, link;
        int next[26];
        Node() : len(0), link(-1) { memset(next, -1, sizeof(next)); }
    };
    vector<Node> t;
    vector<int> cnt;   // endpos 大小（执行 calc_endpos 后有效；初始时原节点为 1，克隆节点为 0）
    int last;

    SuffixAutomaton() { t.emplace_back(); cnt.push_back(0); last = 0; }

    void extend(int c) {
        int cur = (int)t.size();
        t.emplace_back();
        cnt.push_back(1);
        t[cur].len = t[last].len + 1;
        int p = last;
        while (p != -1 && t[p].next[c] == -1) {
            t[p].next[c] = cur;
            p = t[p].link;
        }
        if (p == -1) {
            t[cur].link = 0;
        } else {
            int q = t[p].next[c];
            if (t[p].len + 1 == t[q].len) {
                t[cur].link = q;
            } else {
                int clone = (int)t.size();
                t.push_back(t[q]);
                cnt.push_back(0);
                t[clone].len = t[p].len + 1;
                while (p != -1 && t[p].next[c] == q) {
                    t[p].next[c] = clone;
                    p = t[p].link;
                }
                t[q].link = t[cur].link = clone;
            }
        }
        last = cur;
    }
    void build(const string& s) {
        for (char c : s) extend(c - 'a');
    }
    // 桶排求拓扑序（len 升序）
    vector<int> topo() {
        int m = 0;
        for (auto& v : t) m = max(m, v.len);
        vector<int> bucket(m + 2), ord((int)t.size());
        for (auto& v : t) bucket[v.len]++;
        for (int i = 1; i <= m; i++) bucket[i] += bucket[i - 1];
        for (int i = 0; i < (int)t.size(); i++) ord[--bucket[t[i].len]] = i;
        return ord;
    }
    // 计算每个状态的 endpos 大小（只会修改 cnt 一次，请勿重复调用）
    void calc_endpos() {
        vector<int> ord = topo();
        for (int i = (int)ord.size() - 1; i >= 1; i--) {
            int u = ord[i];
            cnt[t[u].link] += cnt[u];
        }
    }
    // 本质不同子串数
    long long distinct_substrings() {
        long long ans = 0;
        for (int i = 1; i < (int)t.size(); i++) ans += t[i].len - t[t[i].link].len;
        return ans;
    }
    // 子串 p 出现次数（需先 calc_endpos）
    int occurrence(const string& p) {
        int u = 0;
        for (char c : p) {
            u = t[u].next[c - 'a'];
            if (u == -1) return 0;
        }
        return cnt[u];
    }
    // 本串与串 p 的最长公共子串长度
    int lcs(const string& p) {
        int u = 0, l = 0, ans = 0;
        for (char c : p) {
            int x = c - 'a';
            while (u && t[u].next[x] == -1) {
                u = t[u].link;
                l = t[u].len;
            }
            if (t[u].next[x] != -1) {
                u = t[u].next[x];
                l++;
                ans = max(ans, l);
            }
        }
        return ans;
    }
};

// 第 k 小的本质不同子串（DAG 上计数 + 贪心走）
string kth_distinct_substring(SuffixAutomaton& sam, long long k) {
    auto& t = sam.t;
    vector<int> ord = sam.topo();
    vector<long long> sum((int)t.size());
    for (int i = (int)ord.size() - 1; i >= 0; i--) {
        int u = ord[i];
        sum[u] = 1;
        for (int c = 0; c < 26; c++) if (t[u].next[c] != -1) sum[u] += sum[t[u].next[c]];
    }
    string res;
    int u = 0;
    while (k > 0) {
        for (int c = 0; c < 26; c++) {
            int v = t[u].next[c];
            if (v == -1) continue;
            if (k > sum[v]) k -= sum[v];
            else { res += char('a' + c); u = v; k--; break; }
        }
    }
    return res;
}
```

### 8.1 广义 SAM（多串）

```cpp
// 在 SuffixAutomaton 中追加此函数，每插入一个新串前将 last 置为 0
int insert(int last, int c) {
    if (t[last].next[c] != -1) {
        int p = last, q = t[p].next[c];
        if (t[p].len + 1 == t[q].len) return q;
        int clone = (int)t.size();
        t.push_back(t[q]);
        cnt.push_back(0);
        t[clone].len = t[p].len + 1;
        while (p != -1 && t[p].next[c] == q) {
            t[p].next[c] = clone;
            p = t[p].link;
        }
        t[q].link = clone;
        return clone;
    }
    int cur = (int)t.size();
    t.emplace_back();
    cnt.push_back(1);
    t[cur].len = t[last].len + 1;
    int p = last;
    while (p != -1 && t[p].next[c] == -1) {
        t[p].next[c] = cur;
        p = t[p].link;
    }
    if (p == -1) t[cur].link = 0;
    else {
        int q = t[p].next[c];
        if (t[p].len + 1 == t[q].len) t[cur].link = q;
        else {
            int clone = (int)t.size();
            t.push_back(t[q]);
            cnt.push_back(0);
            t[clone].len = t[p].len + 1;
            while (p != -1 && t[p].next[c] == q) {
                t[p].next[c] = clone;
                p = t[p].link;
            }
            t[q].link = t[cur].link = clone;
        }
    }
    return cur;
}

// 用法：
// SuffixAutomaton sam;
// for (string& s : all_strings) {
//     sam.last = 0;
//     for (char c : s) sam.last = sam.insert(sam.last, c - 'a');
// }
```

### 8.2 注意

- `next` 用定长数组：节点数上限约 `2n`，`n = 5e5` 时约 100MB；大串/大字符集可换 `unordered_map` 或 `map`
- SAM 的 parent 树 + 线段树合并可用于统计"每个节点代表的串在哪些位置出现"类问题

---

## 9. 回文自动机 PAM

```cpp
struct PalindromicTree {
    static const int ALPHA = 26;
    struct Node {
        int len, link, cnt;   // cnt: 出现次数（calc 后为真实值）
        int next[ALPHA];
        Node() : len(0), link(0), cnt(0) { memset(next, 0, sizeof(next)); }
    };
    vector<Node> t;
    string s;   // s[0] 为哨兵
    int last, n;

    PalindromicTree() {
        t.resize(2);
        t[0].len = -1; t[0].link = 0;   // 奇根
        t[1].len = 0;  t[1].link = 0;   // 偶根
        last = 1; n = 0; s = "#";
    }
    void add(char c) {
        n++;
        s.push_back(c);
        int x = c - 'a';
        int cur = last;
        while (s[n - t[cur].len - 1] != c) cur = t[cur].link;
        if (!t[cur].next[x]) {
            int node = (int)t.size();
            t.emplace_back();
            t[node].len = t[cur].len + 2;
            if (t[node].len == 1) {
                t[node].link = 1;
            } else {
                int u = t[cur].link;
                while (s[n - t[u].len - 1] != c) u = t[u].link;
                t[node].link = t[u].next[x];
            }
            t[cur].next[x] = node;
        }
        last = t[cur].next[x];
        t[last].cnt++;
    }
    // 计算每个回文串的出现次数（节点按 len 递增创建，倒序累加即可）
    void calc() {
        for (int i = (int)t.size() - 1; i >= 2; i--) t[t[i].link].cnt += t[i].cnt;
    }
};

// 用法：
// PalindromicTree pam;
// for (char c : s) pam.add(c);
// 本质不同回文子串数 = pam.t.size() - 2
// 最长回文子串长度 = max(t[i].len)
// 回文串出现次数 = 对应节点的 cnt（先 pam.calc()）
```

---

## 10. 最小表示法

返回循环同构串中字典序最小者的起始下标。

```cpp
int minimal_representation(const string& s) {
    int n = (int)s.size(), i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        char a = s[(i + k) % n], b = s[(j + k) % n];
        if (a == b) k++;
        else {
            if (a > b) i += k + 1;
            else j += k + 1;
            if (i == j) i++;
            k = 0;
        }
    }
    return min(i, j);
}
// 求最大表示：把 a > b 的比较方向取反（即 if (a < b) i += k + 1; else j += k + 1;）
```

---

## 11. Lyndon 分解（Duval）

串分解为字典序非增的 Lyndon 串（严格小于自身所有非平凡后缀）。

```cpp
// 返回每段起始下标
vector<int> lyndon_factors(const string& s) {
    int n = (int)s.size(), i = 0;
    vector<int> pos;
    while (i < n) {
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i;
            else k++;
            j++;
        }
        while (i <= k) {
            pos.push_back(i);
            i += j - k;
        }
    }
    return pos;
}
// 最小表示法可由 Lyndon 分解导出：取分解结果中最后一段的起始位置即可
```

---

## 12. 常用技巧与结论

- **拼接 + 分隔符**：求 t 在 s 中的匹配，对 `t + '#' + s` 跑 KMP/Z；分隔符必须不出现在原串（多串拼接时用不同分隔符）
- **二分 + 哈希**：LCP、字典序比较、最长公共子串等 O(n log n) 判定类问题
- **正反哈希判回文**：`h.get(l, r) == rev.get(n-1-r, n-1-l)`
- **周期**：最小周期 = `n - pi[n-1]`；有整周期 ⇔ `n % (n - pi[n-1]) == 0`；最小周期长度 = 最小 border 长度
- **周期引理（Fine–Wilf）**：若 p、q 都是 s 的周期且 `p + q ≤ n + gcd(p, q)`，则 `gcd(p, q)` 也是周期
- **border 论**：一个串的所有 border 长度可以分成 O(log n) 段等差数列
- **本质不同子串数**：SA：`n(n+1)/2 - Σlcp`；SAM：`Σ(len[v] - len[link[v]])`
- **字典序第 k 小**：SAM 上 DAG 计数贪心；SA 上直接取 sa[k-1]
- **循环同构判定**：两串循环同构 ⇔ 最小表示相同；s+s 包含所有循环位移
- **哈希防卡**：自然溢出（2^64）可被生日攻击构造冲突，重要场合用双哈希或随机 BASE；哈希值不要暴露给交互
- **01-Trie**：Trie 的 ALPHA 改为 2，可 O(bit) 求最大异或对、按位贪心
- **多串 LCP 问题**：后缀数组 + 二分 + 按 lcp 分组；或广义 SAM
- **注意下标**：SA/SAM 的 rk、sa、lcp 均为 0-indexed；PAM 的 s 内部 1-indexed（s[0] 是哨兵）


