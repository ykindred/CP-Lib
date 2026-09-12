// 依赖：
#include "../Templates/Matrix.hpp"

template <typename S, int N>
struct GaussJordan {
    Matrix<S, N> A;
    ColVector<S, N> x;
    ColVector<S, N> b;
    
    int solve(int n, int m) {
        int r = 0;
        for (int c = 0; r < n && c < m; c++) {
            int pivot = r;
            while (pivot < n && A[pivot][c] == S()) {
                pivot++;
            }
            if (pivot == n) {
                continue;
            }
            if (pivot != r) {
                swap(A.a[pivot], A.a[r]);
                swap(b.a[pivot], b.a[r]);
            }
            S inv = S(1) / A[r][c];
            for (int j = c; j < m; j++) {
                A[r][j] = A[r][j] * inv;
            }
            b[r] = b[r] * inv;
            
            for (int i = 0; i < n; i++) {
                if (i != r && A[i][c] != S()) {
                    S factor = A[i][c];
                    for (int j = c; j < m; j++) {
                        A[i][j] = A[i][j] - factor * A[r][j];
                    }
                    b[i] = b[i] - factor * b[r];
                }
            }
            r++;
        }
        
        for (int i = r; i < n; i++) {
            if (b[i] != S()) {
                return -1;
            }
        }
        
        for (int i = 0; i < n; i++) {
            x[i] = S();
        }
        
        for (int i = 0; i < r; i++) {
            int maj = -1;
            for (int j = 0; j < m; j++) {
                if (A[i][j] != S()) {
                    maj = j;
                    break;
                }
            }
            if (maj != -1) {
                x[maj] = b[i];
            }
        }
        if (r < n) {
            return n - r;
        } else {
            return 0;
        }
    }
};
