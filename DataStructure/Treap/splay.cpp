template<typename T>
struct splaytree {
    // 成员变量
    struct NODE {
        int s[2];   // s[0] 是左儿子，s[1] 是右儿子
        int siz;    // 子树大小
        T key;      // 节点存储的值
        int fa;     // 父亲节点索引

        NODE(T val = T()) : siz(1), key(val), fa(0) {
            s[0] = s[1] = 0;
        }
    };

    std::vector<NODE> D;
    int root = 0; // 根节点的索引

    // 辅助函数（替代宏定义）
    inline int& ls(int x) { return D[x].s[0]; }
    inline int ls(int x) const { return D[x].s[0]; }
    inline int& rs(int x) { return D[x].s[1]; }
    inline int rs(int x) const { return D[x].s[1]; }
    inline int& fa(int x) { return D[x].fa; }
    inline int fa(int x) const { return D[x].fa; }

    // 构造函数
    splaytree() : root(0) {
        D.resize(1); // 0号位置作为空节点占位
    }

    int newp(const T& key = T()) {
        D.push_back(NODE(key));
        return D.size() - 1;
    }

    void maintain(int p) {
        if (p) {
            D[p].siz = D[ls(p)].siz + D[rs(p)].siz + 1;
        }
    }

    void clear(int x) {
        ls(x) = rs(x) = fa(x) = D[x].siz = 0;
        D[x].key = T();
    }

    bool get(int x) const {
        return x == rs(fa(x));
    }

    // 旋转 p 节点
    void rotate(int p) {
        int f = fa(p), ff = fa(f);
        if (!p || !f) return;
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

    // 伸展操作
    void splay(int p) {
        for (int f = fa(p); (f = fa(p)); rotate(p)) {
            if (fa(f)) {
                rotate(get(p) == get(f) ? f : p);
            }
        }
        this->root = p;
    }

    // 插入元素
    void insert(const T& val) {
        if (!root) {
            root = newp(val);
            return;
        }
        int cur = root, f = fa(cur);
        while (cur) {
            f = cur;
            cur = D[cur].s[val > D[cur].key];
        }
        cur = newp(val);
        fa(cur) = f;
        D[f].s[val > D[f].key] = cur;
        splay(cur);
    }

    // 删除元素
    void del(const T& val) {
        if (!root) return;
        int cur = root, p = 0;
        while (cur && D[cur].key != val) {
            p = cur;
            cur = D[cur].s[val > D[cur].key];
        }
        if (!cur) {
            if (p) splay(p);
            return;
        }
        splay(cur);
        if (!ls(cur) && !rs(cur)) {
            clear(cur);
            root = 0;
            return;
        }
        if (!ls(cur)) {
            root = rs(cur);
            fa(root) = 0;
            clear(cur);
            return;
        }
        if (!rs(cur)) {
            root = ls(cur);
            fa(root) = 0;
            clear(cur);
            return;
        }
        int L = ls(cur), R = rs(cur);
        root = L;
        fa(L) = 0;
        int mx = L;
        while (rs(mx)) mx = rs(mx);
        splay(mx);
        rs(mx) = R;
        fa(R) = mx;
        clear(cur);
        maintain(mx);
    }

    // 查询 val 的排名（以 1 为起始）
    int rank(const T& val) {
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

    // 查询第 rk 名的值
    T kth(int rk) {
        if (!root) return T();
        int cur = root;
        while (cur) {
            int sz = D[ls(cur)].siz + 1;
            if (sz > rk) {
                cur = ls(cur);
            } else if (sz == rk) {
                break;
            } else {
                rk -= sz;
                cur = rs(cur);
            }
        }
        if (cur) splay(cur);
        return D[cur].key;
    }

    // 查询严格小于 val 的最大值（前驱）
    T pre(const T& val) {
        if (!root) return T();
        int cur = root, p = 0;
        T ret = T();
        while (cur) {
            p = cur;
            if (D[cur].key >= val) {
                cur = ls(cur);
            } else {
                ret = D[cur].key;
                cur = rs(cur);
            }
        }
        if (p) splay(p);
        return ret;
    }

    // 查询严格大于 val 的最小值（后继）
    T nxt(const T& val) {
        if (!root) return T();
        int cur = root, p = 0;
        T ret = T();
        while (cur) {
            p = cur;
            if (D[cur].key <= val) {
                cur = rs(cur);
            } else {
                ret = D[cur].key;
                cur = ls(cur);
            }
        }
        if (p) splay(p);
        return ret;
    }
};