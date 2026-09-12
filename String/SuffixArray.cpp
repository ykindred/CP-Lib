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
