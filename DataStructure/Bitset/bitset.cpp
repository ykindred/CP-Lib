//此代码需要引用板子中的翻转64位数
//此板子所有操作，需要保证bitset 的大小相同
using u64 = uint64_t;
struct bset {
    int size_;
    int word_count_;
    vector<u64> in;

    bset(int size, u64 x = 0) : size_(size), word_count_((size + 63) / 64), in(word_count_, 0) {
        for (int i = 0; i < 64 && i < size_; ++i)
            if ((x >> i) & 1) set(i);
        normalize();
    }
    bset(int size, const string& s) : size_(size), word_count_((size + 63) / 64), in(word_count_, 0) {
    int len = min((int)s.size(), size_);
    for (int i = 0; i < len; ++i)
        if (s[i] == '1') set(i);
    normalize();
}

    // 设置第 pi 位为 x（0 或 1）
    void set(int pi, int x = 1) {
        if (pi < 0 || pi >= size_) return;
        int idx = pi / 64, off = pi % 64;
        if (x) in[idx] |= (1ULL << off);
        else   in[idx] &= ~(1ULL << off);
    }

    // 查询第 pi 位的值
    bool test(int pi) const {
        assert(pi >= 0 && pi < size_);
        return (in[pi / 64] >> (pi % 64)) & 1;
    }

    // 将某位清零
    void reset(int pi) { set(pi, 0); }
    // 全部清零
    void reset() { fill(in.begin(), in.end(), 0); }

    // 翻转某一位
    void flip(int pi) {
        if (pi < 0 || pi >= size_) return;
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
        for (auto& w : in) w = reverse64(w);
        // 再交换块的位置
        std::reverse(in.begin(), in.end());
        // 如果 size_ 不是 64 的倍数，需要调整最后一块的移位
        if (size_ % 64 != 0) {
            int shift = 64 - (size_ % 64);
            // 整体右移 shift 位，同时保持高位为 0
            for (int i = 0; i < word_count_; ++i) {
                u64 cur = in[i] >> shift;
                if (i + 1 < word_count_)
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
    string to_string() const {
        string res;
        for (int i = size_ - 1; i >= 0; --i)
            res += test(i) ? '1' : '0';
        return res;
    }

    // 访问第 idx 个 64 位块（只读）
    u64 operator[](int idx) const {
        assert(idx >= 0 && idx < word_count_);
        return in[idx];
    }

    // 访问第 idx 个 64 位块（可写，使用后需调用 normalize()）
    u64& operator[](int idx) {
        assert(idx >= 0 && idx < word_count_);
        return in[idx];
    }

    // 确保超出 size_ 的高位为 0
    void normalize() {
        if (size_ % 64 != 0) {
            int last_bits = size_ % 64;
            u64 mask = (1ULL << last_bits) - 1;
            in.back() &= mask;
        }
    }

    friend bset operator&(const bset& a, const bset& b) {
        bset ret(a.size_);
        for (int i = 0; i < a.word_count_; ++i)
            ret.in[i] = a.in[i] & b.in[i];
        ret.normalize();
        return ret;
    }

    friend bset operator|(const bset& a, const bset& b) {
        bset ret(a.size_);
        for (int i = 0; i < a.word_count_; ++i)
            ret.in[i] = a.in[i] | b.in[i];
        ret.normalize();
        return ret;
    }

    friend bset operator^(const bset& a, const bset& b) {
        bset ret(a.size_);
        for (int i = 0; i < a.word_count_; ++i)
            ret.in[i] = a.in[i] ^ b.in[i];
        ret.normalize();
        return ret;
    }

    // 取反
    friend bset operator~(const bset& a) {
        bset ret(a.size_);
        for (int i = 0; i < a.word_count_; ++i)
            ret.in[i] = ~a.in[i];
        ret.normalize();
        return ret;
    }

    // ---------- 移位运算符 ----------
    // 右移 cnt 位（逻辑右移，高位补 0）
    friend bset operator>>(const bset& a, int cnt) {
        bset ret(a.size_);
        if (cnt >= a.size_) return ret;               // 全部移出，结果为 0
        if (cnt <= 0) return a;                 // 负数视为左移，这里暂不支持，直接返回原值

        int word_shift = cnt / 64;
        int bit_shift  = cnt % 64;

        for (int i = 0; i < a.word_count_; ++i) {
            u64 val = 0;
            int src = i + word_shift;
            if (src < a.word_count_) {
                val = a.in[src] >> bit_shift;
                if (bit_shift && src + 1 < a.word_count_)
                    val |= a.in[src + 1] << (64 - bit_shift);
            }
            ret.in[i] = val;
        }
        ret.normalize();
        return ret;
    }

    // 左移 cnt 位（低位补 0）
    friend bset operator<<(const bset& a, int cnt) {
        bset ret(a.size_);
        if (cnt >= a.size_) return ret;
        if (cnt <= 0) return a;

        int word_shift = cnt / 64;
        int bit_shift  = cnt % 64;

        for (int i = 0; i < a.word_count_; ++i) {
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

    // 加法：返回 a + b，忽略最高位进位（模 2^size_）
    friend bset operator+(const bset& a, const bset& b) {
        bset ret(a.size_);
        u64 carry = 0;
        for (int i = 0; i < a.word_count_; ++i) {
            u64 sum = a.in[i] + b.in[i] + carry;
            ret.in[i] = sum;
            carry = (sum < a.in[i]) || (carry && sum == a.in[i]); 
        }
        ret.normalize();
        return ret;
    }

    // 减法：返回 a - b，若 a < b 则按模 2^size_ 计算（即借位被忽略）
    friend bset operator-(const bset& a, const bset& b) {
        // 使用补码：a - b = a + (~b + 1)
        bset neg_b = ~b;
        // 加 1
        bset one(a.size_);
        one.set(0);
        neg_b = neg_b + one;
        return a + neg_b;
    }

    //比较
    friend bool operator==(const bset& a, const bset& b) {
        for (int i = 0; i < a.word_count_; ++i)
            if (a.in[i] != b.in[i]) return false;
        return true;
    }
    friend bool operator!=(const bset& a, const bset& b) { return !(a == b); }
};