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

// ================= 使用示例 =================
StringHash h(s);
if (h.get(l1, r1) == h.get(l2, r2)) { /* 两子串相等 */ }
// 回文判断：建正串与反串两个哈希，h.get(l, r) == rev.get(n-1-r, n-1-l)
// 字典序比较两个子串：先二分 LCP，再比较下一位字符
