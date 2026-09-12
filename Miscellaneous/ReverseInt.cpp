// ---------- 8 位 ----------
static inline uint8_t reverse8(uint8_t x) {
    x = (uint8_t)(((x & 0x55U) << 1) | ((x & 0xAAU) >> 1));
    x = (uint8_t)(((x & 0x33U) << 2) | ((x & 0xCCU) >> 2));
    x = (uint8_t)(((x & 0x0FU) << 4) | ((x & 0xF0U) >> 4));
    return x;
}

// ---------- 16 位 ----------
static inline uint16_t reverse16(uint16_t x) {
    x = (uint16_t)(((x & 0x5555U) << 1) | ((x & 0xAAAAU) >> 1));
    x = (uint16_t)(((x & 0x3333U) << 2) | ((x & 0xCCCCU) >> 2));
    x = (uint16_t)(((x & 0x0F0FU) << 4) | ((x & 0xF0F0U) >> 4));
    x = (uint16_t)(((x & 0x00FFU) << 8) | ((x & 0xFF00U) >> 8));
    return x;
}

// ---------- 32 位 ----------
static inline uint32_t reverse32(uint32_t x) {
//            0123456789012345678901234567890123456789
    x = ((x & 0x55555555U) << 1) | ((x & 0xAAAAAAAAU) >> 1);
    x = ((x & 0x33333333U) << 2) | ((x & 0xCCCCCCCCU) >> 2);
    x = ((x & 0x0F0F0F0FU) << 4) | ((x & 0xF0F0F0F0U) >> 4);
    x = ((x & 0x00FF00FFU) << 8) | ((x & 0xFF00FF00U) >> 8);
    x = ((x & 0x0000FFFFU) << 16) | ((x & 0xFFFF0000U) >> 16);
    return x;
}

// ---------- 64 位 ----------
static inline uint64_t reverse64(uint64_t x) {
    //        012345678901234567890123456789012345678901234567890123456789
    x = ((x & 0x5555555555555555ULL) << 1) | ((x & 0xAAAAAAAAAAAAAAAAULL) >> 1);
    x = ((x & 0x3333333333333333ULL) << 2) | ((x & 0xCCCCCCCCCCCCCCCCULL) >> 2);
    x = ((x & 0x0F0F0F0F0F0F0F0FULL) << 4) | ((x & 0xF0F0F0F0F0F0F0F0ULL) >> 4);
    x = ((x & 0x00FF00FF00FF00FFULL) << 8) | ((x & 0xFF00FF00FF00FF00ULL) >> 8);
    x = ((x & 0x0000FFFF0000FFFFULL) << 16) | ((x & 0xFFFF0000FFFF0000ULL) >> 16);
    x = ((x & 0x00000000FFFFFFFFULL) << 32) | ((x & 0xFFFFFFFF00000000ULL) >> 32);
    return x;
}

// ---------- 128 位 ----------
static inline unsigned __int128 reverse128(unsigned __int128 x) {
    uint64_t lo = (uint64_t)x;                 // 低 64 位
    uint64_t hi = (uint64_t)(x >> 64);         // 高 64 位
    // 翻转整个 128 位：低块翻转后移到高位，高块翻转后移到低位
    return ((unsigned __int128)reverse64(lo) << 64) | reverse64(hi);
}
//有符号数，直接借用无符号数
static inline int8_t  reverse_int8 (int8_t  x) { return (int8_t) reverse8 ((uint8_t) x);  }
static inline int16_t reverse_int16(int16_t x) { return (int16_t)reverse16((uint16_t)x); }
static inline int32_t reverse_int32(int32_t x) { return (int32_t)reverse32((uint32_t)x); }
static inline int64_t reverse_int64(int64_t x) { return (int64_t)reverse64((uint64_t)x); }
static inline __int128 reverse_int128(__int128 x) {
    return (__int128)reverse128((unsigned __int128)x);
}