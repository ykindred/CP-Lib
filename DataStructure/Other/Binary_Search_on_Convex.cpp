using ll = long long;
using i128 = __int128;

struct p {
    ll x = 0;
    ll y = 0;
};

struct vec {
    ll x = 0;
    ll y = 0;
    vec(const p& l, const p& r) : x(r.x - l.x), y(r.y - l.y) {}
};

struct convex {
    int cur = 0;
    int ptr = 0; 
    vector<p> pt;
    convex(int max_n = 200005) : cur(0), ptr(0), pt(max_n) {}
    i128 cross(const vec& v1, const vec& v2) const {
        return (i128)v1.x * v2.y - (i128)v1.y * v2.x;
    }


    ll eval(int i, ll x) const {
        return pt[i].x * x + pt[i].y;
    }

    void insert(p v) {
        if (cur > 0 && pt[cur - 1].x == v.x) {
            if (v.y <= pt[cur - 1].y) return; 
            cur--;                        
        }
        while (cur > 1 && cross(vec(pt[cur - 2], pt[cur - 1]), vec(pt[cur - 1], v)) >= 0) {
            cur--;
        }
        pt[cur++] = v;
    }

    // 【写法一】通用二分查询：适用于查询 x 无单调性的情况，单次 O(log n)，常数极小
    ll find(ll x) const {
        int l = 0, r = cur - 2, ans = cur - 1;
        while (l <= r) {
            int mid = (l + r) >> 1;
            // 因为函数值在凸壳上单峰，若 mid 的值 >= mid + 1 的值，说明峰值在 mid 或其左侧
            if (eval(mid, x) >= eval(mid + 1, x)) {
                ans = mid;
                r = mid - 1;
            } else {
                l = mid + 1;
            }
        }
        return eval(ans, x);
    }

    // 【写法二】单调指针查询：若查询的 x 也是单调递增的，用这个均摊 O(1)，比二分快数倍
    ll find_mono(ll x) {
        if (ptr >= cur) ptr = cur - 1;
        while (ptr < cur - 1 && eval(ptr, x) <= eval(ptr + 1, x)) {
            ptr++;
        }
        return eval(ptr, x);
    }
};