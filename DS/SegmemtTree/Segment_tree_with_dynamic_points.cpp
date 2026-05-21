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