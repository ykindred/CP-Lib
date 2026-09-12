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
