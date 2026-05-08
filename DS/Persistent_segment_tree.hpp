struct node{
    ll value = 0;
    int lp = -1;
    int rp = -1;
};
struct persistent_segment_tree
{
    //datastructure
    int tot;
    int cntarray;
    int n;
    vector<node>d;    //区间信息
    vector<int>h;   //数组k的最后一次编辑的头节点位置

    int newp();
    void update(int nl,int nr,int tp,int p,ll value,int rawp);
    void update(int tk,int tp ,ll value);

    ll query(int nl,int nr,int tl,int tr,int p);
    ll query(int tk,int tl,int tr);

    void copy(int k);
    persistent_segment_tree(int n,int q);
};
//以下是成员函数的具体实现
persistent_segment_tree::persistent_segment_tree(int n,int q) :n(n){
    this->tot = 0;
    this->d.resize(40*(n+q));
    this->h.resize(n+q+5);
    h[1] = 0;
    this->cntarray = 1;
}
int persistent_segment_tree::newp(){
    return ++tot;
}
//单点更新
void persistent_segment_tree::update(int nl,int nr,int tp,int p,ll value,int rawp){
    if(nl==nr){
        this->d[p] ={value,-1,-1};
    }
    else{
        int mid = (nl+nr)/2;
        int rrp = rawp==-1?-1:this->d[rawp].rp,rlp = rawp==-1?-1:this->d[rawp].lp;
        if(tp<=mid){
            this->d[p].rp = rrp;
            this->d[p].lp = this->newp();
            this->update(nl,mid,tp,this->d[p].lp,value,rlp);
        }
        else{
            this->d[p].lp = rlp;
            this->d[p].rp = this->newp();
            this->update(mid+1,nr,tp,this->d[p].rp,value,rrp);
        }
        int lp = this->d[p].lp,rp = this->d[p].rp;
        this->d[p].value = (lp==-1?0:this->d[lp].value)+(rp==-1?0:this->d[rp].value);
        return;
    }
}
void persistent_segment_tree::update(int tk,int tp,ll value){
    int rawp = this->h[tk];
    this->h[tk] = newp();
    this->update(0,n-1,tp,h[tk],value,rawp);
}
//区间查询
ll persistent_segment_tree::query(int nl,int nr,int tl,int tr,int p){
    const auto [value,lp,rp] = this->d[p];
    if(nl>tr||nr<tl) return 0;
    if(nl>=tl&&nr<=tr) return value;
    else{
        ll ret = 0;
        int mid = (nl+nr)/2;
        if(lp!=-1) ret+=query(nl,mid,tl,tr,lp);
        if(rp!=-1) ret+=query(mid+1,nr,tl,tr,rp);
        return ret;
    }
}
ll persistent_segment_tree::query(int tk,int tl,int tr){
    int fp = this->h[tk];
    return this->query(0,n-1,tl,tr,fp);
}
void persistent_segment_tree::copy(int o){
    this->h[++cntarray] = h[o];
}
//Written by Pilocode
