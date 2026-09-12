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
