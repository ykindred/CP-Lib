using u64 = uint64_t;

template<int S>
struct __bitset {
    static constexpr int WORD_COUNT = (S + 63) / 64;
    std::vector<u64> in;

    __bitset(u64 x = 0) : in(WORD_COUNT, 0) {
        for (int i = 0; i < 64 && i < S; ++i)
            if ((x >> i) & 1) set(i);
        normalize();
    }

    __bitset(const std::string& s) : in(WORD_COUNT, 0) {
        int len = std::min((int)s.size(), S);
        for (int i = 0; i < len; ++i)
            if (s[len - 1 - i] == '1') set(i);   // 字符串低位在右侧
        normalize();
    }

    // 设置第 pi 位为 x（0 或 1）
    void set(int pi, int x = 1) {
        if (pi < 0 || pi >= S) return;
        int idx = pi / 64, off = pi % 64;
        if (x) in[idx] |= (1ULL << off);
        else   in[idx] &= ~(1ULL << off);
    }

    // 查询第 pi 位的值
    bool test(int pi) const {
        assert(pi >= 0 && pi < S);
        return (in[pi / 64] >> (pi % 64)) & 1;
    }

    // 将某位清零
    void reset(int pi) { set(pi, 0); }
    // 全部清零
    void reset() { std::fill(in.begin(), in.end(), 0); }

    // 翻转某一位
    void flip(int pi) {
        if (pi < 0 || pi >= S) return;
        in[pi / 64] ^= (1ULL << (pi % 64));
    }

    // 翻转所有位（高位会保持不变）
    void flip() {
        for (auto& w : in) w = ~w;
        normalize();
    }

    // 反转整个 bitset（第 0 位与第 S-1 位互换）
    void reverse() {
        // 先翻转每个 64 位块内部的位
        for (auto& w : in) w = __builtin_bitreverse64(w);
        // 再交换块的位置
        std::reverse(in.begin(), in.end());
        // 如果 S 不是 64 的倍数，需要调整最后一块的移位
        if (S % 64 != 0) {
            int shift = 64 - (S % 64);
            // 整体右移 shift 位，同时保持高位为 0
            for (int i = 0; i < WORD_COUNT; ++i) {
                u64 cur = in[i] >> shift;
                if (i + 1 < WORD_COUNT)
                    cur |= in[i + 1] << (64 - shift);
                in[i] = cur;
            }
        }
        normalize();
    }

    // 返回位数为 1 的个数
    int count() const {
        int ans = 0;
        for (auto w : in) ans += __builtin_popcountll(w);
        return ans;
    }

    // 转换为字符串（最高位在前）
    std::string to_string() const {
        std::string res;
        for (int i = S - 1; i >= 0; --i)
            res += test(i) ? '1' : '0';
        return res;
    }

    // 访问第 idx 个 64 位块（只读）
    u64 operator[](int idx) const {
        assert(idx >= 0 && idx < WORD_COUNT);
        return in[idx];
    }

    // 访问第 idx 个 64 位块（可写，使用后需调用 normalize()）
    u64& operator[](int idx) {
        assert(idx >= 0 && idx < WORD_COUNT);
        return in[idx];
    }

    // 确保超出 S 的高位为 0
    void normalize() {
        if (S % 64 != 0) {
            int last_bits = S % 64;
            u64 mask = (1ULL << last_bits) - 1;
            in.back() &= mask;
        }
    }

    friend __bitset<S> operator&(const __bitset<S>& a, const __bitset<S>& b) {
        __bitset<S> ret;
        for (int i = 0; i < WORD_COUNT; ++i)
            ret.in[i] = a.in[i] & b.in[i];
        ret.normalize();
        return ret;
    }

    friend __bitset<S> operator|(const __bitset<S>& a, const __bitset<S>& b) {
        __bitset<S> ret;
        for (int i = 0; i < WORD_COUNT; ++i)
            ret.in[i] = a.in[i] | b.in[i];
        ret.normalize();
        return ret;
    }

    friend __bitset<S> operator^(const __bitset<S>& a, const __bitset<S>& b) {
        __bitset<S> ret;
        for (int i = 0; i < WORD_COUNT; ++i)
            ret.in[i] = a.in[i] ^ b.in[i];
        ret.normalize();
        return ret;
    }

    // 取反
    friend __bitset<S> operator~(const __bitset<S>& a) {
        __bitset<S> ret;
        for (int i = 0; i < WORD_COUNT; ++i)
            ret.in[i] = ~a.in[i];
        ret.normalize();
        return ret;
    }

    // ---------- 移位运算符 ----------
    // 右移 cnt 位（逻辑右移，高位补 0）
    friend __bitset<S> operator>>(const __bitset<S>& a, int cnt) {
        __bitset<S> ret;
        if (cnt >= S) return ret;               // 全部移出，结果为 0
        if (cnt <= 0) return a;                 // 负数视为左移，这里暂不支持，直接返回原值

        int word_shift = cnt / 64;
        int bit_shift  = cnt % 64;

        for (int i = 0; i < WORD_COUNT; ++i) {
            u64 val = 0;
            int src = i + word_shift;
            if (src < WORD_COUNT) {
                val = a.in[src] >> bit_shift;
                if (bit_shift && src + 1 < WORD_COUNT)
                    val |= a.in[src + 1] << (64 - bit_shift);
            }
            ret.in[i] = val;
        }
        ret.normalize();
        return ret;
    }

    // 左移 cnt 位（低位补 0）
    friend __bitset<S> operator<<(const __bitset<S>& a, int cnt) {
        __bitset<S> ret;
        if (cnt >= S) return ret;
        if (cnt <= 0) return a;

        int word_shift = cnt / 64;
        int bit_shift  = cnt % 64;

        for (int i = 0; i < WORD_COUNT; ++i) {
            u64 val = 0;
            int src = i - word_shift;
            if (src >= 0) {
                val = a.in[src] << bit_shift;
                if (bit_shift && src - 1 >= 0)
                    val |= a.in[src - 1] >> (64 - bit_shift);
            }
            ret.in[i] = val;
        }
        ret.normalize();
        return ret;
    }

    // 加法：返回 a + b，忽略最高位进位（模 2^S）
    friend __bitset<S> operator+(const __bitset<S>& a, const __bitset<S>& b) {
        __bitset<S> ret;
        u64 carry = 0;
        for (int i = 0; i < WORD_COUNT; ++i) {
            u64 sum = a.in[i] + b.in[i] + carry;
            ret.in[i] = sum;
            carry = (sum < a.in[i]) || (carry && sum == a.in[i]); 
        }
        ret.normalize();
        return ret;
    }

    // 减法：返回 a - b，若 a < b 则按模 2^S 计算（即借位被忽略）
    friend __bitset<S> operator-(const __bitset<S>& a, const __bitset<S>& b) {
        // 使用补码：a - b = a + (~b + 1)
        __bitset<S> neg_b = ~b;
        // 加 1
        __bitset<S> one;
        one.set(0);
        neg_b = neg_b + one;
        return a + neg_b;
    }

    //比较
    friend bool operator==(const __bitset<S>& a, const __bitset<S>& b) {
        for (int i = 0; i < WORD_COUNT; ++i)
            if (a.in[i] != b.in[i]) return false;
        return true;
    }
    friend bool operator!=(const __bitset<S>& a, const __bitset<S>& b) { return !(a == b); }
};
using bitset = __bitset;