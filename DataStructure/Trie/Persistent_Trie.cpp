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