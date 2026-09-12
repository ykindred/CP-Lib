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
