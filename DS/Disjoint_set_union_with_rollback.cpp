struct Disjoint_set_union_with_rollback
{
    //由于要保留路径信息，因此不能再使用路径压缩
    vector<int>fa;
    vector<int>siz;
    stack<pair<int,int>> undost;
    Disjoint_set_union_with_rollback(int n){
        fa.resize(n);
        iota(fa.begin(),fa.end(),0);
        siz.resize(n,1);
    }
    int find(int x){
        return x==fa[x]?x:find(fa[x]);
    }
    void unite(int x,int y){
        int fx = find(x) ,fy = find(y);
        if(fx==fy){
            //虽然没有实际操作，但是为了确保timer一致性，push空操作
            undost.push({-1,-1});
            return;
        }
        if(siz[fx]<=siz[fy]){
            siz[fy] += siz[fx];
            fa[fx] = fy;
            undost.push({fx,fy});
        }else
        {
            siz[fx] += siz[fy];
            fa[fy] = fx;
            undost.push({fy,fx});
        }
        return;
    }
    void undo(){
        auto [x,y] = undost.top();
        undost.pop();
        if(x==-1) return;
        else{
            siz[y] -= siz[x];
            fa[x] = x;
            return;
        }
    }
};
