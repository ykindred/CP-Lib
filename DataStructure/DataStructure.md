# 数据结构模板库

---

## 树状数组

```cpp
template <typename T>
class FenWick
{
private:
    vector<T> BIT;
    T lowbit(T x);
    int n;

public:
    FenWick(T n) : n(n)
    {
        this->BIT.resize(n, 0);
    }
    FenWick(vector<T> &ori) : FenWick(ori.size())
    {
        this->BIT = ori;
    }
    void set(int poi, T value);
    T query(int poi);
    T query(int l, int r);
};
template <typename T>
T FenWick<T>::lowbit(T x)
{
    return x & (-x);
}
template <typename T>
void FenWick<T>::set(int poi, T value)
{
    for (; poi < n; poi += (lowbit(poi)))
        (this->BIT[poi]) += value;
    return;
}
template <typename T>
T FenWick<T>::query(int poi)
{
    T ans = 0;
    for (; poi > 0; poi -= lowbit(poi))
    {
        ans += (this->BIT)[poi];
    }
    return ans;
}
template <typename T>
T FenWick<T>::query(int l, int r)
{
    return (this->query(r) - this->query(l - 1));
}
```

---

## 并查集

### 基础并查集

```cpp
//这是并查集的一个极简实现
struct Disjoint_set_unoin{
    vector<int>fa;
    Disjoint_set_unoin(int n){fa.resize(n);iota(fa.begin(),fa.end(),0);};
    int find(int n){return n==fa[n]?n:fa[n] = find(fa[n]);}
    void unite(int x,int y){fa[find(x)] = find(y);}
};
```

### 可回滚并查集

```cpp
struct Disjoint_set_union_with_rollback
{
    //由于要保留路径信息，因此不能再使用路径压缩
    vector<int>fa;
    vector<int>siz;
    stack<pair<int,int>> undost;
    Disjoint_set_union_with_rollback(int n){
        fa.resize(n);
        iota(fa.begin(),fa.end(),0);
        siz.resize(n,1);
    }
    int find(int x){
        return x==fa[x]?x:find(fa[x]);
    }
    void unite(int x,int y){
        int fx = find(x) ,fy = find(y);
        if(fx==fy){
            //虽然没有实际操作，但是为了确保timer一致性，push空操作
            undost.push({-1,-1});
            return;
        }
        if(siz[fx]<=siz[fy]){
            siz[fy] += siz[fx];
            fa[fx] = fy;
            undost.push({fx,fy});
        }else
        {
            siz[fx] += siz[fy];
            fa[fy] = fx;
            undost.push({fy,fx});
        }
        return;
    }
    void undo(){
        auto [x,y] = undost.top();
        undost.pop();
        if(x==-1) return;
        else{
            siz[y] -= siz[x];
            fa[x] = x;
            return;
        }
    }
};
```

### 带删除并查集

```cpp
struct Disjoint_set_union_with_delete
{
    vector<int> fa;
    vector<int> siz;
    vector<int> id;
    int tot;       

    // n 为初始逻辑节点数，q 为预计的最大删除次数（用于预留物理节点空间）
    Disjoint_set_union_with_delete(int n, int q){
        int max_nodes = n + q + 10; 
        fa.resize(max_nodes);
        iota(fa.begin(), fa.end(), 0);
        siz.resize(max_nodes, 1);
        
        id.resize(n);
        iota(id.begin(), id.end(), 0); 
        tot = n - 1; 
    }

    int find(int x){
        return x == fa[x] ? x : fa[x] = find(fa[x]);
    }

    void unite(int x, int y){
        int fx = find(id[x]), fy = find(id[y]);
        if(fx == fy){
            return;
        }
        if(siz[fx] <= siz[fy]){
            siz[fy] += siz[fx];
            fa[fx] = fy;
        }else
        {
            siz[fx] += siz[fy];
            fa[fy] = fx;
        }
        return;
    }

    void del(int x){
        int fx = find(id[x]);
        // 从原集合的 size 中扣除自己 (视具体题目需求，通常删除节点意味着原集合大小-1)
        siz[fx]--;
        
        // 为逻辑节点 x 分配一个新的独立物理节点
        tot++;
        id[x] = tot;
        fa[tot] = tot;
        siz[tot] = 1;
        return;
    }
    
    // 获取逻辑节点 x 所在集合的大小
    int get_size(int x){
        return siz[find(id[x])];
    }
};
```

---

## 线段树

### 基础线段树

```cpp
struct D{
    int val = 0;//按需配置
};
D operator+(const D& left, const D& right){return {left.val+right.val};} //按需重载
struct segment_tree{
    vector<D>data;
    D e; //单位元
    segment_tree(int n){
        data.resize(4*n+10);
    }
    void modify(int nl,int nr ,int tp ,D val ,int p){
        if(nl==nr){
            data[p] = val;
            return;
        }
        else{
            int mid = (nl+nr)>>1;
            if(tp<=mid) modify(nl,mid,tp,val,2*p);
            if(tp>mid) modify(mid+1,nr,tp,val,2*p+1);
            //push up
            data[p] = data[2*p] + data[2*p+1];
            return;
        }
    }
    D query(int nl,int nr,int tl,int tr,int p) const{
        if(nl>=tl&&nr<=tr) return data[p];
        else{
            int mid = (nl+nr)>>1;
            D lt = e ,rt = e;
            if(tl<=mid) lt = query(nl,mid,tl,tr,2*p);
            if(tr>mid) rt = query(mid+1,nr,tl,tr,2*p+1);
            return lt+rt;
        }
    }
};
```

### 懒标记线段树

```cpp
//懒标记线段树的实现较为简单，但本身由于TAG的存在，非常灵活
//特别是涉及多种操作的时候，要仔细想清楚TAG的与DATA的合并逻辑与优先级
//基础逻辑就一句话，在递归之前，必须下传TAG，在结束之后，必须根据子区间信息push up

//这里给出一个区间赋值/加和/查询 操作的线段树实现，实际实现时对照修改即可
#include<bits/stdc++.h>
using namespace std;
using ll = long long;
struct D{
    ll val = 0;
};
D operator+(const D&l_op,const D& r_op){return {l_op.val+r_op.val};};
struct TAG{
    ll add = 0;
    ll set = -1;
};

TAG operator+(const TAG&l_op,const TAG&r_op){ //r_op是优先级更高的tag
    if(r_op.set!=-1){
        return {r_op.add,r_op.set};
    }
    else{
        return {l_op.add+ r_op.add,l_op.set};
    }
}
TAG& operator+=(TAG& l_op,const TAG& r_op){
    l_op = l_op + r_op;
    return l_op;
}
struct Segment_tree_with_lazy_tag{
    vector<D> data;
    vector<TAG> lz_tag;
    D e; //单位元
    Segment_tree_with_lazy_tag(int n){
        data.resize(4*n);
        lz_tag.resize(4*n);
    }
    //计算区间信息与tag合并的函数
    D merge(const D& data,const TAG& tag,ll length){ 
        const auto[add,set] = tag;
        if(set!=-1){
            return {(set+add)*length};
        }
        else{
            return {data.val + add*length};
        }
    }
    //区间编辑
    void modify(int nl,int nr,int tl,int tr,TAG val,int p){
        if(nl>=tl&&nr<=tr){
            lz_tag[p] += val;
            return;
        }
        else{
            //递归前push down
            lz_tag[2*p] += lz_tag[p];
            lz_tag[2*p+1] += lz_tag[p];
            lz_tag[p] = TAG();

            int mid = nl +((nr-nl)>>1);
            if(tl<=mid) modify (nl,mid,tl,tr,val,2*p);
            if(tr>mid) modify (mid+1,nr,tl,tr,val,2*p+1);

            //push up
            data[p] = merge(data[2*p],lz_tag[2*p],(mid - nl+1)) + merge(data[2*p+1],lz_tag[2*p+1],(nr-mid));
            return ;
        }
    }
    //区间查询
    D query(int nl,int nr ,int tl,int tr,int p){
        if(nl>=tl&&nr<=tr){
            return merge(data[p],lz_tag[p],nr-nl+1);
        }
        else{
            //递归前push down
            lz_tag[2*p] += lz_tag[p];
            lz_tag[2*p+1] += lz_tag[p];
            lz_tag[p] = TAG();

            D lt = e;
            D rt = e;
            int mid = nl +((nr-nl)>>1);
            if(tl<=mid) lt = query(nl,mid,tl,tr,2*p);
            if(tr>mid) rt = query(mid+1,nr,tl,tr,2*p+1);

            //push up
            data[p] = merge(data[2*p],lz_tag[2*p],(mid - nl+1)) + merge(data[2*p+1],lz_tag[2*p+1],(nr-mid));
            return lt+rt;
        }
    }
};
```

### 动态开点线段树

```cpp
struct D{
    int val = 0; //按需配置
};
D operator+(const D& left, const D& right){return {left.val+right.val};} //按需重载
struct segment_tree_with_dynamic_points{
    vector<D> data;
    vector<pair<int,int>> nxt;
    D e; //单位元
    segment_tree_with_dynamic_points(){
        data.resize(2);
        nxt.resize(2);
    }
    int newp(){
        data.emplace_back();
        nxt.emplace_back();
        return data.size() -1;
    }
    void modify(int nl,int nr ,int tp,D val,int p){
        if(nl==nr) data[p] = val;
        else{
            auto [lp,rp] = nxt[p]; //注意此处不能引用，避免扩容后悬垂
            int mid = (nl+nr)>>1;
            if(tp<=mid){
                if(lp==0) lp = newp();
                modify(nl,mid,tp,val,lp);
            }
            if(tp>mid){
                if(rp==0) rp =newp();
                modify(mid+1,nr,tp,val,rp);
            }
            nxt[p].first = lp;
            nxt[p].second = rp;
            //push up;
            D lt =e ,rt = e;
            if(lp!=0) lt = data[lp];
            if(rp!=0) rt = data[rp];
            data[p] = lt + rt;
            
            return;
        }
    }
    D query(int nl,int nr ,int tl ,int tr ,int p) const{
        if(nl>=tl&&nr<=tr) return data[p];
        else{
            const auto&[lp,rp] = nxt[p];
            int mid = (nl+nr)>>1;
            D lt =e ,rt = e;
            if(lp!=0&&tl<=mid) lt = query(nl,mid,tl,tr,lp);
            if(rp!=0&&tr>mid) rt = query(mid+1,nr,tl,tr,rp);
            return lt+rt;
        }
    }
};
```

### 可持久化线段树（主席树）

```cpp
struct D{
    int val = 0;
};
D operator+ (const D& left ,const D& right){return {left.val+right.val};}
struct persistent_segment_tree{
    int cnt_v;
    vector<int> h; 
    vector<D> data;
    D e;    //单位元
    vector<pair<int,int>>nxt;
    persistent_segment_tree(D e) : e(e){
        data.resize(2,e);
        nxt.resize(2);
        h.resize(1);
        h[0] = 1;
    }
    int newp(){
        data.push_back(e);
        nxt.push_back({});
        return data.size() - 1;
    }
    //每次修改都创建新节点，不修改的部分复用原来的节点，因此递归新旧同时进行
    void modify(int nl,int nr ,int tp ,D val,int p ,int old_p){
        if(nl==nr){
            data[p] = val;
            return;
        }else{
            const auto [o_lp,o_rp] = nxt[old_p]; //不允许使用引用，以免悬垂指针
            ll mid  = (nl+nr)>>1;
            if(tp<=mid){
                nxt[p].second = o_rp;
                nxt[p].first = newp();
                modify(nl,mid,tp,val,nxt[p].first,o_lp);
            }
            else{
                nxt[p].first = o_lp;
                nxt[p].second = newp();
                modify(mid+1,nr,tp,val,nxt[p].second,o_rp);
            }
            //push up;
            const auto[lp,rp] = nxt[p];
            data[p] = data[lp] + data[rp];
        }
    }
    //查询和正常线段树完全一致
    D query(int nl,int nr ,int tl ,int tr ,int p) const{
        if(nl>=tl&&nr<=tr){
            return data[p];
        }
        else{
            const auto [lp,rp] = nxt[p];
            ll mid = (nl+nr)>>1;
            D lt = e ,rt = e;
            if(tl<=mid&&lp)
                lt = query(nl,mid,tl,tr,lp);
            if(tr>mid&&rp)
                rt = query(mid+1,nr,tl,tr,rp);
            return lt+rt;
        }
    }
    void update(int tk,int nl,int nr ,int tp ,D val){
        int old_head = h[tk];
        int new_head = newp();
        modify(nl ,nr ,tp,val,new_head,old_head);
        h[tk] = new_head;
    }
    int backup(int tk){
        h.push_back(h[tk]);
        return h.size()-1;
    }
};
```

### 标记永久化线段树

```cpp
//在线段树区间查询的时候，除了打懒标记并下传，还有一种写法
//那就是我们将标记"保留"在区间中，查询的时候累计路径上的所有标记并计算结果
//坏处很明显，这要求标记满足结合律和交换律，这显然是阿贝尔群的性质。
//好处是什么？由于这样实现不要求下传标记，因此可以很方便的进行 "持久化"

//下给出一个普通线段树的标记永久化区间和实现
//支持区间增加及查询
struct D
{
    int val = 0;
};
D operator+(const D&l_op,const D&r_op) {return {l_op.val+r_op.val};}
struct TAG
{
    int add = 0;
};
TAG operator+(const TAG&l_op,const TAG&r_op){ //r_op是优先级更高的tag
   return {l_op.add+r_op.add};
}
TAG& operator+=(TAG& l_op,const TAG& r_op){
    l_op = l_op + r_op;
    return l_op;
}
struct Segment_tree_with_persistent_marks
{
    vector<D>data;
    vector<TAG>tags;
    Segment_tree_with_persistent_marks(int n){
        data.resize(4*n+10);
        tags.resize(4*n+10);
    }
    D e;    //单位元
    D merge(const D&data, const TAG&tag,int length){
        return {data.val+tag.add*length};
    }
    void modify(int nl,int nr,int tl,int tr,TAG add,int p){
        //每次途经节点，累加实际受影响的交集长度贡献
        int overlap = min(nr, tr) - max(nl, tl) + 1;
        data[p] = merge(data[p],add,overlap); 
        if(nl>=tl&&nr<=tr){
            
            tags[p] += add; //标记留在此处，不再递归
            return;
        }
        else{
            int mid = (nl+((nr-nl)>>1));
            if(tl<=mid) modify(nl,mid,tl,tr,add,2*p);
            if(tr>mid) modify(mid+1,nr,tl,tr,add,2*p+1);
            return;
        }
    }

    //查询时，只计算父节点的标记和！
    D query(int nl,int nr,int tl,int tr,int p){
        if(nl>=tl&&nr<=tr){
            return data[p];
        }
        else{
            int mid = (nl + ((nr-nl)>>1));
            D lt = e;
            D rt = e;
            if(tl<=mid) lt = query(nl,mid,tl,tr,2*p);
            if(tr>mid) rt = query(mid+1,nr,tl,tr,2*p+1);

            int overlap = min(nr, tr) - max(nl, tl) + 1;
            return merge(lt + rt, tags[p], overlap);
        }
    }
};
```

---

## 字典树

### 基础字典树

```cpp
struct D{
    int nxt[26] ={0};
    int cnt = 0;
};
struct Trie{
    vector<D>node;
    Trie(){node.resize(2);}
    int newp(){node.push_back({}); return node.size()-1;}
    //只给出insert的基本实现，简单来说，不能使用递归实现，查询时不断挪动指针向下
    //其余操作的查询操作均以此为基础，不再赘述
    void insert(const string& s){
        int n = s.size();
        int cur = 1;
        for(int i = 0;i<n;i++){
            auto val = s[i];
            if(!node[cur].nxt[val-'a']) {
                node[cur].nxt[val-'a'] = newp();
            }
            cur = node[cur].nxt[val-'a'] ;
        }
        node[cur].cnt++;
        return;
    }
};
```

### 01字典树

```cpp
//01字典树一般用来解决位运算相关的问题
//如果原题可以按位贪心，可以从01字典树的角度思考
struct D{
    int nxt[2] = {0};
};
struct Trie_01{
    vector<D> node;
    Trie_01(){
        node.resize(2);
    }
    int newp(){
        node.push_back({});
        return node.size() - 1;
    }
    //插入一个数
    void insert(int num){
        int cur = 1;
        for(int i = 30;i>=0;i--){
            int bit = (num>>i) &1;
            if(node[cur].nxt[bit]==0){
                node[cur].nxt[bit]  = newp();
            }
            cur = node[cur].nxt[bit];
        }
    }
    //返回异或值最大的结果
    int query(int num) const{
        int cur = 1;
        int ret = 0;
        for(int i = 30;i>=0;i--){
            int bit = (num>>i)&1;
            if(node[cur].nxt[!bit] ==0){
                cur = node[cur].nxt[bit];
            }
            else{
                cur = node[cur].nxt[!bit];
                ret += (1<<i);
            }
        }
        return ret;
    }
};
```

### 可持久化01字典树

```cpp
//为01字典树添加了持久化操作
//思路与主席树类似，只要操作，就修改附近所有节点，然后复用之前节点
#include<bits/stdc++.h>
using namespace std;
struct D{
    int nxt[2] = {0};
    int cnt = 0;
};
struct Trie_01{
    vector<D> node;
    vector<int> h;
    Trie_01(){
        node.resize(2);
        h.resize(1);
        h[0] = 1;
    }
    int newp(){
        node.push_back({});
        return node.size() - 1;
    }
    //给版本k,插入一个数
    void insert(int tk,int num){
        int cur = newp();
        int o_cur = h[tk];
        h[tk] = cur;    //cur 更换新的头节点
        for(int i = 30;i>=0;i--){
            int bit = (num>>i)&1;
            node[cur].nxt[bit] = newp();
            node[cur].cnt = node[o_cur].cnt + 1;

            node[cur].nxt[!bit] = node[o_cur].nxt[!bit];

            cur = node[cur].nxt[bit];
            o_cur = node[o_cur].nxt[bit];
        }
        node[cur].cnt = node[o_cur].cnt + 1;
    }
    //复制版本k，并返回新的版本编号
    int copy(int tk){
        h.push_back(h[tk]);
        return h.size()-1;
    }
    //查询版本k的最大值
    int query(int tk,int num) const{
        int cur = h[tk];
        int ret = 0;
        for(int i = 30;i>=0;i--){
            int bit = (num>>i)&1;
            if(node[cur].nxt[!bit] ==0){
                cur = node[cur].nxt[bit];
            }
            else{
                cur = node[cur].nxt[!bit];
                ret += (1<<i);
            }
        }
        return ret;
    }
    //查询版本l 与版本r之间的最大值  [l,r] 双闭区间
    //往往这个更有效果，因为可以处理区间
    int query(int tl,int tr,int num) const{
        if(tl==0) return query(tr,num);
        int curl = h[tl - 1];
        int curr = h[tr];
        int ret = 0;
        for(int i = 30;i>=0;i--){
            int bit = (num>>i)&1;
            int cnt = node[node[curr].nxt[!bit]].cnt - node[node[curl].nxt[!bit]].cnt;
            if(cnt>0){
                //共同进入 !bit 分支
                ret += (1<<i);
                curl = node[curl].nxt[!bit];
                curr = node[curr].nxt[!bit];
            }
            else{
                //共同进入 bit分支
                curl = node[curl].nxt[bit];
                curr = node[curr].nxt[bit];
            }
        }
        return ret;
    }
};
```

---

## 平衡树

### 隐式Treap（无旋Treap / FHQ-Treap）

```cpp
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
```

---

## 其他数据结构与算法

### 稀疏表

```cpp
#include <bits/stdc++.h>
using namespace std;
template <typename S>
struct SpTable {
    int n;
    int log;
    vector<vector<S>> st;

    SpTable(const vector<S>& a) {
        build(a);
    }
    void build(const vector<S>& a) {
        n = a.size();
        log = 32 - __builtin_clz(n);
        st.assign(log, vector<S>(n));
        for (int i = 0; i < n; i++) {
            st[0][i] = a[i];
        }
        for (int j = 1; j < log; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                st[j][i] = op(st[j - 1][i], st[j - 1][i + (1 << (j - 1))]);
            }
        }
    }
    S query(int l, int r) {
        int j = 31 - __builtin_clz(r - l);
        return op(st[j][l], st[j][r - (1 << j)]);
    }
};
```

### 离散化

```cpp
// 离散化
// 许多题目在处理数据的时候，并不关心值本身，只关心值之间的大小关系
// 这个时候进行离散化是比较正确的决定
template <typename T> 
vector<T> Discretization(vector<T> o_array)
{
    vector<T> knum = o_array;
    sort(knum.begin(), knum.end());
    knum.erase(unique(knum.begin(), knum.end()),knum.end());
    for(int i = 0;i<o_array.size();i++){
        o_array[i] = lower_bound(knum.begin(),knum.end(),o_array[i]) - knum.begin();
    }
    return o_array;
}
//传入一个数组，传出离散化之后的数组 如 num = Normalization(num);
```

### 哈希表

```cpp
#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
struct chash {
    static ull splitmix64(ull x) {
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(ull x) const {
        static const ull F = 
            chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + F);
    }
};
template<typename K, typename V>
using HashMap = __gnu_pbds::gp_hash_table<K, V, chash>;
```

### 欧拉序

```cpp
//许多处理树上问题，特别是对 子树 进行操作的时候 ，往往利用欧拉序将树拍平后，再利用线段树维护
//实现了一个结构体，初始化n为节点数，并传入边（需人为确保是一棵树）
//调用成员函数.init后，会生成两个个长度为n的数组
//第一个为树的dfn序，注意默认视1为根节点
//第二个为子树索引，tp[p] 代表 p节点所在子树在dfn上对应的连续区间 ，请使用auto[lp,rp] = pt[p] 调用
struct GenerateEulerTourOrder
{
    vector<int>dfn;
    vector<pair<int,int>>tp;
    vector<vector<int>> g;
    GenerateEulerTourOrder(int n){
        dfn.reserve(n);
        tp.resize(n+1);
        g.resize(n+1);
    }
    void add(int u,int v){
        g[u].push_back(v);
        g[v].push_back(u);
    }
    void init(int head = 1){
        auto dfs = [this](auto&&self,int n,int p) ->void {
            auto & [lp,rp] = tp[n];
            dfn.emplace_back(n);
            lp = dfn.size()-1;
            for(auto nextp:g[n]){
                if(nextp== p) continue;
                else{
                    self(self,nextp,n);
                }
            }
            rp = dfn.size() - 1;
        };
        dfs(dfs,head,-1);
        return;
    }
};
```

### 回滚莫队

> 此模板仅包含思路注释，无具体实现代码。

```
//常规莫队在挪动指针时，总是双指针交替挪动，O(1)更新答案
//回滚莫队的思想是：
//对于一些可离线的查询，我们进行add操作比较简单，del操作比较复杂
//我们在查询的时候，先确保右指针单调挪动，每次右指针挪动前，把左指针回溯到最初始的状态（区块右边界）
//再将指针左移以匹配区间
//注意到这样就只有add操作，我们只需要维护一个操作栈即可正确回溯，注意到左指针最多挪动sqrt(n)次
```
