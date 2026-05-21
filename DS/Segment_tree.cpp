struct D{
    int val = 0;//按需配置
};
D operator+(const D& left, const D& right){return {left.val+right.val};} //按需重载
struct segment_tree{
    vector<D>data;
    D e; //单位元
    segment_tree(int n){
        data.resize(4*n+10);
    }
    void modify(int nl,int nr ,int tp ,D val ,int p){
        if(nl==nr){
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