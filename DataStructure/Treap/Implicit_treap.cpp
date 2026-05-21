struct Implicit_treap {
    vector<int> val;
    vector<int> siz;
    vector<unsigned int> pri;
    vector<pair<int, int>> nxt;
    vector<bool> rev;   // 区间翻转懒标记
    int root;
    mt19937 rng;

    Implicit_treap() : root(0), rng(random_device{}()) {
        val.push_back(0);
        siz.push_back(0);
        pri.push_back(0);
        nxt.push_back({0, 0});
        rev.push_back(false);
    }

    int newp(int v) {
        val.push_back(v);
        siz.push_back(1);
        pri.push_back(rng());
        nxt.push_back({0, 0});
        rev.push_back(false);
        return siz.size() - 1;
    }

    void push_up(int p) {
        if (p) {
            const auto [lp, rp] = nxt[p];
            siz[p] = siz[lp] + siz[rp] + 1;
        }
    }

    void push_down(int p) {
        if (p && rev[p]) {
            // 1. 实际执行翻转：交换左右儿子
            swap(nxt[p].first, nxt[p].second);
            // 2. 交换后，再取出当前真正的左右儿子去下传标记
            const auto [lp, rp] = nxt[p];
            if (lp) rev[lp] = !rev[lp];
            if (rp) rev[rp] = !rev[rp];
            // 3. 清除当前节点的标记
            rev[p] = false;
        }
    }

    pair<int, int> split(int p, int k) {
        if (!p) return {0, 0};
        
        push_down(p); 

        const auto [lp, rp] = nxt[p];
        if (siz[lp] + 1 <= k) {
            auto [rx, ry] = split(rp, k - siz[lp] - 1);
            nxt[p].second = rx;
            push_up(p);
            return {p, ry};
        } else {
            auto [lx, ly] = split(lp, k);
            nxt[p].first = ly;
            push_up(p);
            return {lx, p};
        }
    }

    int merge(int x, int y) {
        if (!x || !y) return x | y;
        push_down(x); 
        push_down(y);

        if (pri[x] > pri[y]) {
            nxt[x].second = merge(nxt[x].second, y);
            push_up(x);
            return x;
        } else {
            nxt[y].first = merge(x, nxt[y].first);
            push_up(y);
            return y;
        }
    }

    // --- 0-based 对外接口 ---

    // 在下标 pos 处插入值 v (插入后它将成为新的 array[pos])
    // 允许 pos 取 0 到 siz[root] (即支持在首尾插入)
    void insert(int pos, int v) {
        auto [x, y] = split(root, pos);
        root = merge(merge(x, newp(v)), y);
    }

    // 删除下标为 pos 的节点 (0 <= pos < siz[root])
    void erase(int pos) {
        auto [x, z] = split(root, pos + 1); 
        auto [lx, del] = split(x, pos);    
        root = merge(lx, z); 
    }

    // 反转区间 [l, r] (0 <= l <= r < siz[root])
    void reverse_range(int l, int r) {
        if(l >= r) return;
        auto [x, z] = split(root, r + 1); 
        auto [lx, y] = split(x, l);       
        rev[y] = !rev[y]; 
        root = merge(merge(lx, y), z);
    }
};