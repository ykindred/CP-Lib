struct Disjoint_set_union_with_delete
{
    vector<int> fa;
    vector<int> siz;
    vector<int> id;
    int tot;       

    // n 为初始逻辑节点数，q 为预计的最大删除次数（用于预留物理节点空间）
    Disjoint_set_union_with_delete(int n, int q){
        int max_nodes = n + q + 10; 
        fa.resize(max_nodes);
        iota(fa.begin(), fa.end(), 0);
        siz.resize(max_nodes, 1);
        
        id.resize(n);
        iota(id.begin(), id.end(), 0); 
        tot = n - 1; 
    }

    int find(int x){
        return x == fa[x] ? x : fa[x] = find(fa[x]);
    }

    void unite(int x, int y){
        int fx = find(id[x]), fy = find(id[y]);
        if(fx == fy){
            return;
        }
        if(siz[fx] <= siz[fy]){
            siz[fy] += siz[fx];
            fa[fx] = fy;
        }else
        {
            siz[fx] += siz[fy];
            fa[fy] = fx;
        }
        return;
    }

    void del(int x){
        int fx = find(id[x]);
        // 从原集合的 size 中扣除自己 (视具体题目需求，通常删除节点意味着原集合大小-1)
        siz[fx]--;
        
        // 为逻辑节点 x 分配一个新的独立物理节点
        tot++;
        id[x] = tot;
        fa[tot] = tot;
        siz[tot] = 1;
        return;
    }
    
    // 获取逻辑节点 x 所在集合的大小
    int get_size(int x){
        return siz[find(id[x])];
    }
};