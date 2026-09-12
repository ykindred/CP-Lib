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

// ================= 续 =================
// 来自小节：2.2 匹配

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

// ================= 续 =================
// 来自小节：2.3 周期相关

// 最小周期 = n - pi[n-1]；s 有整周期 p 当且仅当 n % (n - pi[n-1]) == 0
// 例：判断 s 是否由 t 重复 k 次组成：s 为 t+t 的前缀且 n % m == 0（m = t.size()）
