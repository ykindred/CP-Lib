template <typename Z>
vector<Z> batch_inv(const vector<Z>& a) {
    int n = a.size();
    vector<Z> pre(n + 1), suf(n + 1), ret(n);
    pre[0] = 1;
    for (int i = 0; i < n; i++) {
        pre[i + 1] = pre[i] * a[i];
    }
    Z inv_all = pre[n].inv();
    for (int i = n - 1; i >= 0; i--) {
        ret[i] = inv_all * pre[i];
        inv_all *= a[i];
    }
    return ret;
}

template <typename Z>
vector<Z> many_factorials(const vector<int>& q) {
    constexpr int LIM = 100000;
    int n = q.size();
    int mod = Z::mod();
    vector<Z> ans(n, Z(1));
    vector<int> need_inv(n);
    vector<pair<int, int>> reg;
    vector<pair<int, int>> odd;
    reg.reserve(n);
    odd.reserve(n * 8);
    for (int id = 0; id < n; id++) {
        int t = q[id];
        if (t >= mod) {
            ans[id] = 0;
            continue;
        }
        if (t >= mod / 2) {
            int r = mod - 1 - t;
            if ((r & 1) == 0) {
                ans[id] = -ans[id];
            }
            need_inv[id] = 1;
            t = r;
        }
        long long pow2 = 0;
        while (t > LIM) {
            int x = (t - 1) / 2;
            odd.emplace_back(x, id);
            pow2 += t / 2;
            t >>= 1;
        }
        ans[id] *= Z(2).pow(pow2 % (mod - 1));
        reg.emplace_back(t, id);
    }
    sort(reg.begin(), reg.end());
    int cur = 0;
    Z prod = 1;
    for (auto [t, id] : reg) {
        while (cur < t) {
            cur++;
            prod *= cur;
        }
        ans[id] *= prod;
    }
    sort(odd.begin(), odd.end());
    int ocur = -1;
    Z oprod = 1;
    for (auto [x, id] : odd) {
        while (ocur < x) {
            ocur++;
            oprod *= Z(2LL * ocur + 1);
        }
        ans[id] *= oprod;
    }
    vector<Z> need;
    vector<int> pos;
    for (int i = 0; i < n; i++) {
        if (need_inv[i]) {
            pos.emplace_back(i);
            need.emplace_back(ans[i]);
        }
    }
    auto invs = batch_inv(need);
    for (int i = 0; i < (int)pos.size(); i++) {
        ans[pos[i]] = invs[i];
    }
    return ans;
}
