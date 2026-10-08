struct splaytree {
#define ls(x) D[(x)].s[0]
#define rs(x) D[(x)].s[1]
#define fa(x) D[(x)].fa
    //成员变量
    struct NODE {
        int s[2], siz, key, fa;
        NODE() { s[0] = s[1] = siz = key = fa = 0; }
    };
    vector<NODE> D;
    int root = 0;

    splaytree() :root(0) {
        D.resize(1);
    }
    
    int newp(int key = 0) { D.push_back(NODE()); D.back().key = key; D.back().siz = 1; return D.size() - 1; }
    void maintain(int p) { D[p].siz = D[ls(p)].siz + D[rs(p)].siz + 1; }
    void clear(int x) { ls(x) = rs(x) = fa(x) = D[x].siz = D[x].key = 0; }
    bool get(int x) { return x == rs(fa(x)); }
    ////旋转p节点，其实无需区分左旋和右旋，总是将相反方向的节点接到父亲，然后父亲接到相反方向节点
    void rotate(int p) {
        int f = fa(p), ff = fa(f);
        if ((!p) || (!f)) return;
        int dir = get(p);
        
        D[f].s[dir] = D[p].s[dir ^ 1];
        if (D[f].s[dir]) fa(D[f].s[dir]) = f; 
        
        D[p].s[dir ^ 1] = f;
        if (ff) D[ff].s[get(f)] = p;
        fa(f) = p; 
        fa(p) = ff;
        
        maintain(f); 
        maintain(p); 
    }
    //*伸展，将p节点旋转为根
    //这样写需要确保rotate是安全的，即rotate不旋转空节点和根节点
    //每次splay至多向上翻转两步，无论如何最后一步总是rotate p，如果f和同时为左/右孩子，则先翻转p
    void splay(int p) {
        for (int f = fa(p); f = fa(p); rotate(p)) {
            if (fa(f)) {
                rotate(get(p) == get(f) ? f : p);
            }
        }
        this->root = p;
    }
    //插入，注意这里支持多元素，约定左子树<= ，右子树>
    void insert(int val) {
        if (!root) { root = newp(val); return; }
        int cur = root, f = 0;
        while (cur) {
            f = cur;
            cur = D[cur].s[val > D[cur].key];
        }
        cur = newp(val);
        fa(cur) = f;
        D[f].s[val > D[f].key] = cur;
        splay(cur);
    }

    void del(int val) {
        if (!root) return;
        int cur = root, p = 0;
        while (cur && D[cur].key != val) {
            p = cur; 
            cur = D[cur].s[val > D[cur].key];
        }
        if (!cur) { splay(p); return; }
        splay(cur);
        
        if (!ls(cur) && !rs(cur)) {
            clear(cur); root = 0; return;
        }
        if (!ls(cur)) {
            root = rs(cur); fa(root) = 0; clear(cur); return;
        }
        if (!rs(cur)) {
            root = ls(cur); fa(root) = 0; clear(cur); return;
        }
        
        int L = ls(cur), R = rs(cur);
        root = L; fa(L) = 0;
        int mx = L;
        while (rs(mx)) mx = rs(mx);
        splay(mx);
        
        rs(mx) = R; fa(R) = mx;
        clear(cur);
        maintain(mx);
    }
    //查询x的排名 ,从小到大，最小的排名为1
    int rank(int val) {
        int cur = root, p = 0, ret = 1;
        while (cur) {
            p = cur;
            if (D[cur].key < val) {
                ret += D[ls(cur)].siz + 1;
                cur = rs(cur);
            } else {
                cur = ls(cur);
            }
        }
        if (p) splay(p);
        return ret;
    }
    //查询第K名的值，**必须**确保查询合法
    int kth(int rk) {
        if (!root) return -1;
        int cur = root;
        while (cur) {
            int sz = D[ls(cur)].siz + 1;
            if (sz > rk) cur = ls(cur);
            else if (sz == rk) break;
            else {
                rk -= sz; 
                cur = rs(cur);
            }
        }
        splay(cur);
        return D[cur].key;
    }
    //考虑从根往下走，如果当前点大于等于 x，那前驱一定在左子树，我们往左走；
    //否则，前驱可能在这个点，也可能在这个点的右子树里，
    //总之不在左子树里。所以先用这个点更新答案，再进入它的右子树继续找。
    int pre(int val) {
        if (!root) return -1;
        int cur = root, ret = 0, p = 0;
        while (cur) {
            p = cur;
            if (D[cur].key >= val) cur = ls(cur);
            else { ret = D[cur].key; cur = rs(cur); }
        }
        if (p) splay(p);
        return ret;
    }
    //与上一条类似
    int nxt(int val) {
        if (!root) return -1;
        int cur = root, ret = 0, p = 0;
        while (cur) {
            p = cur;
            if (D[cur].key <= val) cur = rs(cur);
            else { ret = D[cur].key; cur = ls(cur); }
        }
        if (p) splay(p);
        return ret;
    }

#undef ls
#undef rs
#undef fa
};