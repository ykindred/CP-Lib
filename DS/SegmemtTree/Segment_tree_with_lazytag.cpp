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