// 返回每段起始下标
vector<int> lyndon_factors(const string& s) {
    int n = (int)s.size(), i = 0;
    vector<int> pos;
    while (i < n) {
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j]) {
            if (s[k] < s[j]) k = i;
            else k++;
            j++;
        }
        while (i <= k) {
            pos.push_back(i);
            i += j - k;
        }
    }
    return pos;
}
// 最小表示法可由 Lyndon 分解导出：取分解结果中最后一段的起始位置即可
