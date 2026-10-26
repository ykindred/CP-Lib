#pragma once
#include "../head.hpp"

// 维护 B 位无符号整数的子集异或值；B 的范围为 $1\le B\le64$。
// 默认构造空线性基；p 存基向量，rank 是秩，zero 表示能否由非空子集异或得到零。
template <int B = 64>
struct LinearBasis {
    static_assert(1 <= B && B <= 64);
    array<u64, B> p{};
    int rank = 0;
    bool zero = false; 

    // 传入一个 B 位整数 x，将它并入线性基；返回是否增加了秩。
    // 若 x 与已有元素线性相关，则记录存在非空零异或子集；时间为 $O(B)$。
    bool insert(u64 x) {
        if constexpr (B < 64) assert((x >> B) == 0);
        for (int i = B - 1; i >= 0; i--) {
            if (!(x >> i & 1)) continue;
            if (p[i]) x ^= p[i];
            else {
                p[i] = x;
                rank++;
                return true;
            }
        }
        zero = true;
        return false;
    }
    // 传入 x，返回它能否由当前元素的某个子集异或得到，允许空集。
    bool contains(u64 x) const {
        for (int i = B - 1; i >= 0; i--) {
            if (x >> i & 1) x ^= p[i];
        }
        return x == 0;
    }
    // 传入初始值 x，返回 x 与任意子集异或后的最大值；默认从零开始。
    u64 max_xor(u64 x = 0) const {
        for (int i = B - 1; i >= 0; i--) x = max(x, x ^ p[i]);
        return x;
    }
    // 传入初始值 x，返回 x 与任意子集异或后的最小值；默认从零开始。
    u64 min_xor(u64 x = 0) const {
        for (int i = B - 1; i >= 0; i--) x = min(x, x ^ p[i]);
        return x;
    }
    // 传入另一个线性基 b，将其代表的元素并入当前对象，不返回新对象。
    void merge(const LinearBasis& b) {
        auto q = b.p;
        zero |= b.zero;
        for (u64 x : q) if (x) insert(x);
    }
    
    // 无需参数，返回按主元从低到高排列、已消去低主元的基向量。
    // 该数组按二进制编号组合时，异或值递增；构造时间为 $O(B^2)$，可缓存用于多次查询。
    vector<u64> ordered() const {
        auto q = p;
        vector<u64> ret;
        for (int i = 0; i < B; i++) {
            if (!q[i]) continue;
            for (int j = 0; j < i; j++) {
                if (q[i] >> j & 1) q[i] ^= q[j];
            }
            ret.push_back(q[i]);
        }
        return ret;
    }

    // 传入从零开始的编号 k，返回第 k 小的不同异或值；越界返回 nullopt。
    // nonempty 为真时排除空集；若非空子集也能得到零，仍保留零。单次时间为 $O(B^2)$。
    optional<u64> kth(u64 k, bool nonempty = false) const {
        if (nonempty && !zero) {
            if (k == numeric_limits<u64>::max()) return nullopt;
            k++;
        }
        if (rank < 64 && k >= (u64(1) << rank)) return nullopt;
        auto q = ordered();
        u64 ret = 0;
        for (int i = 0; i < rank; i++) if (k >> i & 1) ret ^= q[i];
        return ret;
    }
};
