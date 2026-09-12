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
