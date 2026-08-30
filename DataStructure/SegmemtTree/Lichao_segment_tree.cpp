#include <bits/stdc++.h>
using namespace std;

using TAG = pair<double, double>;  // (斜率 k, 截距 b)

struct LichaoTree {
    struct Node {
        int lc, rc;      // 左右孩子编号，0 表示空
        TAG line;        // 当前节点保存的线段
    };

    vector<Node> tr;     // 节点池
    double L, R;         // 整个定义域 [L, R]
    const double INF = 4e18;
    int root;

    LichaoTree(double l, double r) : L(l), R(r) {
        tr.push_back({0, 0, {0, -INF}});  // 0 号位置占位，不使用
        root = new_node();
    }
    int new_node() {
        tr.push_back({0, 0, {0, -INF}});
        return (int)tr.size() - 1;
    }
    // 计算线段 line 在 x 处的值
    double eval(const TAG& line, double x) const {
        return line.first * x + line.second;
    }
    // 在节点 p 对应的区间 [l, r] 中插入线段 nw
    void add_line(int p, double l, double r, TAG nw) {
        TAG& cur = tr[p].line;
        double mid = (l + r) / 2;

        double cur_l = eval(cur, l), cur_r = eval(cur, r);
        double nw_l = eval(nw, l), nw_r = eval(nw, r);
        // 新线段完全优于当前线段
        if (nw_l >= cur_l && nw_r >= cur_r) {
            cur = nw;
            return;
        }
        // 新线段完全劣于当前线段
        if (nw_l <= cur_l && nw_r <= cur_r) {
            return;
        }

        // 交叉：根据中点值决定保留哪条，较差的一条下传
        if (eval(nw, mid) > eval(cur, mid)) {
            swap(cur, nw);
        }

        // 现在 nw 是需要下传的线段，判断它在哪一侧可能更优
        if (eval(nw, l) > eval(cur, l)) {
            if (!tr[p].lc) tr[p].lc = new_node();
            add_line(tr[p].lc, l, mid, nw);
        } else if (eval(nw, r) > eval(cur, r)) {
            if (!tr[p].rc) tr[p].rc = new_node();
            add_line(tr[p].rc, mid + 1, r, nw);
        }
    }
    void modify(int p, double l, double r, double ql, double qr, TAG nw) {
        if (ql <= l && r <= qr) {
            add_line(p, l, r, nw);
            return;
        }
        double mid = (l + r) / 2;
        if (ql <= mid) {
            if (!tr[p].lc) tr[p].lc = new_node();
            modify(tr[p].lc, l, mid, ql, qr, nw);
        }
        if (qr > mid) {
            if (!tr[p].rc) tr[p].rc = new_node();
            modify(tr[p].rc, mid + 1, r, ql, qr, nw);
        }
    }
    double query(int p, double l, double r, double x) {
        if (p == 0) return -INF; 
        double res = eval(tr[p].line, x);
        double mid = (l + r) / 2;
        if (l == r) return res;
        if (x <= mid) {
            return max(res, query(tr[p].lc, l, mid, x));
        } else {
            return max(res, query(tr[p].rc, mid + 1, r, x));
        }
    }
    void insert_line(TAG line) {
        add_line(root, L, R, line);
    }
    void insert_segment(double ql, double qr, TAG line) {
        modify(root, L, R, ql, qr, line);
    }
    double ask(double x) {
        return query(root, L, R, x);
    }
};
/*
int main() {
    LichaoTree lct(0.0, 1000.0);  // 定义域 [0, 1000]
    lct.insert_line({1.5, 2.0});  // y = 1.5x + 2
    lct.insert_segment(10.0, 50.0, {-0.5, 30.0}); // 在 [10,50] 插入 y = -0.5x + 30
    cout << lct.ask(20.0) << endl;  // 查询 x=20 处的最大值
}*/