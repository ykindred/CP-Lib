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
