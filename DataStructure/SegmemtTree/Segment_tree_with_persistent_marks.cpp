//在线段树区间查询的时候，除了打懒标记并下传，还有一种写法
//那就是我们将标记”保留“在区间中，查询的时候累计路径上的所有标记并计算结果
//坏处很明显，这要求标记满足结合律和交换律，这显然是阿贝尔群的性质。
//好处是什么？由于这样实现不要求下传标记，因此可以很方便的进行 “持久化”
//下给出一个普通线段树的标记永久化区间和实现
//支持区间增加及查询
#include<bits/stdc++.h>
using namespace std;
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
        if(nl>=tl&&nr<=tr){
            tags[p] += add; //标记留在此处，不再递归
            return;
        }
        else{
            int mid = (nl+((nr-nl)>>1));
            if(tl<=mid) modify(nl,mid+1,tl,tr,add,2*p);
            if(tr>mid) modify(mid+1,nr,tl,tr,add,2*p+1);
            return;
        }
    }

    D query(int nl,int nr,int tl,int tr,int p){
        if(nl>=tl&&nr<=tr){
            return merge(data[p],tags[p],nr-nl+1);
        }
        else{
            int mid = (nl + ((nr-nl)>>1));
            D lt = e;
            D rt = e;
            if(tl<=mid) lt = query(nl,mid,tl,tr,2*p);
            if(tr>mid) rt = query(mid+1,nr,tl,tr,2*p+1);
            //考虑交集

            return merge(lt+rt,tags[p],max(tl-nr+1,nl-tr+1));
        }
    }
};
