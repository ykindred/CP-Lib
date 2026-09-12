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
