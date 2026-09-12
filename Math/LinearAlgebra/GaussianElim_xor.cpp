template <int DIM>
struct GaussXor {
    array<bitset<DIM>, DIM> mat; // 存增广矩阵, 行从 0 到 n - 1, 列从 0 到 m

    array<int, DIM> ans;
    // 解异或线性方程组, 无解返回-1, 唯一解返回0, 无穷解返回自由变元数
    int solve(int n, int m) {
        // n 个方程, m 个未知量(即系数矩阵为 n 行 m 列)
        int r = 0, c = 0;
        for (; r < n && c < m; c++) {
            int pivot = r;
            while (pivot < n && !mat[pivot][c]) {
                pivot++;
            }
            if (pivot == n) {
                continue;
            }
            if (pivot != r) {
                swap(mat[pivot], mat[r]);
            }
            for (int i = 0; i < n; i++) {
                if (i != r && mat[i][c]) {
                    mat[i] ^= mat[r];
                }
            }
            r++;
        }

        for (int i = r; i < n; i++) {
            if (mat[i][m]) {
                return -1;
            }
        }
        if (r < m) {
            return m - r;
        }
        for (int i = 0; i < m; i++) {
            if (i < n) {
                ans[i] = mat[i][m];
            }
        }
        return 0;
    }
};
