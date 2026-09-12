int minimal_representation(const string& s) {
    int n = (int)s.size(), i = 0, j = 1, k = 0;
    while (i < n && j < n && k < n) {
        char a = s[(i + k) % n], b = s[(j + k) % n];
        if (a == b) k++;
        else {
            if (a > b) i += k + 1;
            else j += k + 1;
            if (i == j) i++;
            k = 0;
        }
    }
    return min(i, j);
}
// 求最大表示：把 a > b 的比较方向取反（即 if (a < b) i += k + 1; else j += k + 1;）
