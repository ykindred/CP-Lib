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