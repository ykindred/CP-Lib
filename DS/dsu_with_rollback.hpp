//带撤销并查集
//只能撤销上一个操作
struct dsu_with_rollback{
    vector<int>fa;
    vector<int>rank;
    stack<tuple<int,int>>st;
    //查找父节点
    int find(int x);
    //合并两个节点（按秩合并）
    void unite(int x,int y);
    //撤销上一次操作
    void undo();
    //构造函数
    dsu_with_rollback(int n);
};
//以下是成员函数的具体实现
dsu_with_rollback::dsu_with_rollback(int n){
    this->fa.resize(n);
    this->rank.resize(n,1);
    for(int i = 0;i<n;i++)
        fa[i] = i;
}
int dsu_with_rollback::find(int x){
//不能使用路径压缩
if(fa[x]==x) return x;
else{
    return this->find(fa[x]);
}
}
void dsu_with_rollback::unite(int x,int y){
    //按秩合并
    int fx = find(x),fy = find(y);
    if(fx==fy){
        st.push({-1,-1});
        return;
    }
    if(rank[fx]<rank[fy]){
        rank[fy] +=rank[fx];
        fa[fx] = fy;
        this->st.push({fx,fy});
    }else{
        rank[fx]+=rank[fy];
        fa[fy] = fx;
        this->st.push({fy,fx});
    }
}
void dsu_with_rollback::undo(){
    if(this->st.empty()){
        cerr<<"RuntimeError：Tring to undo when the stack is empty"<<endl;
        exit(-1);
    }else{
        auto &[u,v] = st.top();
        st.pop();
        if(u==-1&&v==-1) return;
        rank[v] -=rank[u];
        fa[u] = u;
    }
    return;
}
//Written by Pilocode
