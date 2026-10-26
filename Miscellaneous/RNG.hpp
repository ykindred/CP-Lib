// random number giver
inline u64 RNG_64() {
    static u64 x_ = u64(chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count()) * 10150724397891781847ULL;
    x_ ^= x_ << 7;
    return x_ ^= x_ >> 9;
}
// gives random number of [0, lim)
inline u64 RNG(u64 lim) { return RNG_64() % lim; }
// gives random number of [l, r)
inline ll RNG(ll l, ll r) { return l + RNG_64() % (r - l); }
