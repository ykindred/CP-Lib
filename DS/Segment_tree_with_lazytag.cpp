#include<bits/stdc++.h>
using namespace std;
struct D{
    int val = 0;//按需配置
};
struct TAG{
    int add = 0;
};
D operator+(const D& left, const D& right){return {left.val+right.val};} //按需重载
D operator*(const D&left,const int& right){return {left.val*right};} //按需重载
TAG operator+(const TAG&left ,const TAG& right){return {left.add+right.add};}//按需重载
TAG operator+=(TAG& left,const TAG& right) {return left = left+right;}//按需重载
struct segment_tree{
    vector<D>data;
    vector<TAG>lz_tag;
    D e; //单位元
    segment_tree(int n){
        data.resize(4*n+10);
        lz_tag.resize(4*n+10);
    }
    //注意事项:
    //只要遍历到这个区间，那么需要push_down ，并且确保return 前更新为真值
    //只要有递归向下的操作，结束前必须push_up
    void modify(int nl,int nr ,int tp ,D val ,int p){
        if(nl==nr){
            //不需递归，下传值
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
    void add(int nl,int nr ,int tl,int tr,D val,int p){
        if(nl>=tl&&nr<=tr){
            //不需递归，懒标记打于此
            data[p] = data[p] + val*(nr-nl+1);

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