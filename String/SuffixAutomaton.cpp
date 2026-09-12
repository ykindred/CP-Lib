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
