template <int DIM = 64>
struct LinearBasis {
    array<ull, DIM> p = {};
    int cnt = 0;
    bool zero = 0;
    
    bool insert(ull x) {
        for (int i = DIM - 1; i >= 0; i--) {
            if ((x & (1ULL << i)) == 0) {
                continue;
            }
            if (!p[i]) {
                p[i] = x;
                cnt++;
                return true;
            }
            x ^= p[i];
        }
        zero = true;
        return false;
    }
    
    bool check(ull x) {
        for (int i = DIM - 1; i >= 0; i--) {
            if ((x & (1ULL << i)) == 0) {
                continue;
            }
            if (!p[i]) {
                return false;
            }
            x ^= p[i];
        }
        return true;
    }
    
    ull max() {
        ull ret = 0;
        for (int i = DIM - 1; i >= 0; i--) {
            if ((ret ^ p[i]) > ret) {
                ret ^= p[i];
            }
        }
        return ret;
    }
    
    ull min() {
        if (zero) {
            return 0;
        }
        for (int i = 0; i < DIM; i++) {
            if (p[i]) {
                return p[i];
            }
        }
    }
    
    // 合并
    friend bool operator+(const LinearBasis& a, const LinearBasis& b) {
        LinearBasis ret = a;
        for (int i = 0; i < DIM; i++) {
            if (b.p[i]) {
                ret.insert(b.p[i]);
            }
        }
        return ret;
    }
};
