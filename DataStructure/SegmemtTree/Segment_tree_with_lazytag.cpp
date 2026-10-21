//这是一个示例实现，这是一个区间加法线段树
struct segment_tree_with_lazytag{
    #define lp 2*p
    #define rp 2*p+1
    vector<int> data;
    vector<int> lz_tag;
    segment_tree_with_lazytag(int n){
        data.resize(4*n+10);
        lz_tag.resize(4*n+10);
    }
    void apply(int nl ,int nr ,int p ,int val) {
        data[p] += (nr - nl + 1) * val;
        lz_tag[p] += val;
    }
    void push_down(int nl ,int nr ,int p){
        if(nl == nr) lz_tag[p] = 0;
        else {
            int mid = (nl+nr)>>1;
            apply(nl ,mid , lp);
            apply(mid+1 , nr , rp);
            lz_tag[p] = 0;
        }
    }
    void add(int nl , int nr ,int tl ,int tr ,int p ,int val) {
        if(tl<=nl&&tr>=nr){
            apply(nl,nr,p,val);
            return;
        }
        else {
            push_down(nl,nr,p);
            int mid = (nl + nr) >> 1;
            if(tl<=mid) add(nl,mid,tl,tr,lp,val);
            if(tr>mid) add(mid+1,nr,tl,tr,rp,val);
            data[p] = data[lp] + data[rp];
        }
    }
    int query(int nl ,int nr ,int tl , int tr ,int p) {
        if(tl<=nl && tr>=nr) return data[p];
        else {
            push_down(nl,nr,p);
            int mid = (nl+nr)>>1 , lt = 0 , rt = 0;
            if(tl<=mid) lt = query(nl,mid,tl,tr,lp);
            if(tr>mid) rt = query(mid+1,nr,tl,tr,rp);
            data[p] = data[lp] + data[rp];
            return lt + rt;
        }
    }
    #undef lp
    #undef rp
}