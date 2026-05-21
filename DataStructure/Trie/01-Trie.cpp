//01字典树一般用来解决位运算相关的问题
//如果原题可以按位贪心，可以从01字典树的角度思考
struct D{
    int nxt[2] = {0};
};
struct Trie_01{
    vector<D> node;
    Trie_01(){
        node.resize(2);
    }
    int newp(){
        node.push_back({});
        return node.size() - 1;
    }
    //插入一个数
    void insert(int num){
        int cur = 1;
        for(int i = 30;i>=0;i--){
            int bit = (num>>i) &1;
            if(node[cur].nxt[bit]==0){
                node[cur].nxt[bit]  = newp();
            }
            cur = node[cur].nxt[bit];
        }
    }
    //返回异或值最大的结果
    int query(int num) const{
        int cur = 1;
        int ret = 0;
        for(int i = 30;i>=0;i--){
            int bit = (num>>i)&1;
            if(node[cur].nxt[!bit] ==0){
                cur = node[cur].nxt[bit];
            }
            else{
                cur = node[cur].nxt[!bit];
                ret += (1<<i);
            }
        }
        return ret;
    }
};