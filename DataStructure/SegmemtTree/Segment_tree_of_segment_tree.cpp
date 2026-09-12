struct D{
    int val = 0; //按需配置
};
D operator+(const D& left, const D& right){return {left.val+right.val};} //按需重载
struct segment_tree_with_dynamic_points{
    vector<D> data;
    vector<pair<int,int>> nxt;
    vector<pair<int,int>> snxt;
    D e; //单位元
    segment_tree_with_dynamic_points(){
        data.resize(2);
        nxt.resize(2);
        snxt.resize(2);
    }
    int newp(){
        data.emplace_back(e);
        nxt.emplace_back();
        snxt.emplace_back();
        return data.size() -1;
    }
    void modify_1D(int nl,int nr,int tp,D val ,int p){
        if(nl==nr) data[p] = val;
        else{
            auto [lp,rp] = nxt[p]; //注意此处不能引用，避免扩容后悬垂
            int mid = (nl+nr)>>1;
            if(tp<=mid){
                if(lp==0) lp = newp();
                modify_1D(nl,mid,tp,val,lp);
            }
            if(tp>mid){
                if(rp==0) rp =newp();
                modify_1D(mid+1,nr,tp,val,rp);
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
    //实际上是modify 2D
    void modify(int nl,int nr,int nb,int nt, int ts,int tp,D val ,int p){
        modify_1D(nb,nt,tp,val,p);
        if(nl == nr) {
            return;
        }
        else{
            auto [lp,rp] = snxt[p];
            int mid = (nl+nr)>>1;
            if(ts<=mid){
                if(lp ==0) lp = newp();
                modify(nl,mid,nb,nt,ts,tp,val,lp);
            }
            if(ts > mid){
                if(rp == 0) rp =newp();
                modify(mid+1,nr,nb,nt,ts,tp,val,rp);
            }
            snxt[p].first = lp;
            snxt[p].second = rp;
        }
        return;
    }
    D query_1D(int nl,int nr ,int tl ,int tr ,int p) const{
        if(nl>=tl&&nr<=tr) return data[p];
        else{
            const auto&[lp,rp] = nxt[p];
            int mid = (nl+nr)>>1;
            D lt =e ,rt = e;
            if(lp!=0&&tl<=mid) lt = query_1D(nl,mid,tl,tr,lp);
            if(rp!=0&&tr>mid) rt = query_1D(mid+1,nr,tl,tr,rp);
            return lt+rt;
        }
    }
    D query(int nl,int nr ,int nb,int nt,int tl,int tr,int tb,int tt,int p) const {
        if(nl>=tl&&nr<=tr) return query_1D(nb,nt,tb,tt,p);
        else{
            const auto &[lp,rp] = snxt[p];
            int mid = (nl+nr)>>1;
            D lt = e , rt = e;
            if(lp!=0&&tl<=mid) lt = query(nl,mid,nb,nt,tl,tr,tb,tt,lp);
            if(rp!=0&&tr>mid) rt = query(mid+1,nr,nb,nt,tl,tr,tb,tt,rp);
            return lt+rt;
        }
    }
};