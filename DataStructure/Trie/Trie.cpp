struct D{
    int nxt[26] ={0};
    int cnt = 0;
};
struct Trie{
    vector<D>node;
    Trie(){node.resize(2);}
    int newp(){node.push_back({}); return node.size()-1;}
    //只给出insert的基本实现，简单来说，不能使用递归实现，查询时不断挪动指针向下
    //其余操作的查询操作均以此为基础，不再赘述
    void insert(const string& s){
        int n = s.size();
        int cur = 1;
        for(int i = 0;i<n;i++){
            auto val = s[i];
            if(!node[cur].nxt[val-'a']) {
                node[cur].nxt[val-'a'] = newp();
            }
            cur = node[cur].nxt[val-'a'] ;
        }
        node[cur].cnt++;
        return;
    }
};