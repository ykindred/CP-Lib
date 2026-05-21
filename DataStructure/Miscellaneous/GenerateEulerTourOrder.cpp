//许多处理树上问题，特别是对 子树 进行操作的时候 ，往往利用欧拉序将树拍平后，再利用线段树维护
//实现了一个结构体，初始化n为节点数，并传入边（需人为确保是一棵树）
//调用成员函数.init后，会生成两个个长度为n的数组
//第一个为树的dfn序，注意默认视1为根节点
//第二个为子树索引，tp[p] 代表 p节点所在子树在dfn上对应的连续区间 ，请使用auto[lp,rp] = pt[p] 调用
struct GenerateEulerTourOrder
{
    vector<int>dfn;
    vector<pair<int,int>>tp;
    vector<vector<int>> g;
    GenerateEulerTourOrder(int n){
        dfn.reserve(n);
        tp.resize(n+1);
        g.resize(n+1);
    }
    void add(int u,int v){
        g[u].push_back(v);
        g[v].push_back(u);
    }
    void init(int head = 1){
        auto dfs = [this](auto&&self,int n,int p) ->void {
            auto & [lp,rp] = tp[n];
            dfn.emplace_back(n);
            lp = dfn.size()-1;
            for(auto nextp:g[n]){
                if(nextp== p) continue;
                else{
                    self(self,nextp,n);
                }
            }
            rp = dfn.size() - 1;
        };
        dfs(dfs,head,-1);
        return;
    }
};
