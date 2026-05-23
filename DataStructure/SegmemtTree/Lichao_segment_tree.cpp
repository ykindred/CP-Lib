#include<bits/stdc++.h>
using TAG  = pair<long long,long long>;
using ll = long long;
using namespace std;
struct Lichao_segment_tree{
    vector<TAG>tag;
    Lichao_segment_tree(int n){
        tag.resize(4*n+10,{0,0});
    }
    void modify(int nl,int nr,int tl,int tr,TAG n_tag,int p){
        int mid = nl + ((nr-nl)>>1);
        if(nl>=tl&&nr<=tr){
            const auto&[k1,b1] = tag[p];
            const auto&[k2,b2] = n_tag;
            ll t1,b1,t2,b2;
            t1 = max(k1*nl+b1,k1*nr+b1);
            b1 = min(k1*nl+b1,k1*nr+b1);
            t2 = max(k2*nl+b2,k2*nr+b2);
            b2 = min(k2*nl+b2,k2*nr+b2);
            if(t1<=b2) {
                tag[p] = n_tag;
                return;
            }
            else if(t2<=b1){
                return;
            } 
            else{
                modify(nl,mid,tl,tr,n_tag,2*p);
                modify(mid+1,nr,tl,tr,n_tag,2*p+1);
            }
        }
        else{
            if(tl<=mid) modify(nl,mid,tl,tr,n_tag,2*p);
            if(tr>mid) modify(mid+1,nr,tl,tr,n_tag,2*p+1);
        }
    }
    ll query(int nl,int nr,int tp,int p){
        int mid = nl + ((nr-nl)>>1);
        const auto&[k,b] = tag[p];
        ll cnt = k*tp+b;
        if(nl==nr) return cnt;
        else if(tp<=mid) return max(cnt,query(nl,mid,tp,2*p));
        else return max(cnt,query(mid+1,nr,tp,2*p+1));
    }
};
