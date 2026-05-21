#include "exgcd.hpp"
inline array<i128, 2> excrt(const vector<ll>& b, const vector<ll>& m) {
    int n = b.size();
    i128 nowb = b[0], nowm = m[0];
    for (int i = 1; i < n; i++) {
        ll A = nowm % m[i], B = m[i];
        i128 C = b[i] - nowb;
        auto [y1, _, g] = exgcd(A, B);
        if (C % g != 0) {
            return { -1, -1 };
        }
        
        i128 M = m[i] / g;
        i128 y = (i128)y1 * (C / g) % M;
        y = (y < 0) ? y + M : y;
        nowb += y * nowm;
        nowm *= M;
        nowb %= nowm;
    }
    return { nowb, nowm };
}
