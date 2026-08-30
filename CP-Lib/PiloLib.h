
//宏
#define endl '\n'
#define rep(i,from,to) for(int i = from ; i<=top ; i++)
//using 
using ll = long long;
using u32 = unsigned int;
using u64 = unsigned long long;

namespace Pilo{
//各类工具函数
template<typename T>
bool inrange(T x,T L,T R) {return x>=L&&x<=R;}



//各类运算符重载



//各类数据结构
//这是并查集的一个极简实现
struct Disjoint_set_unoin{
    vector<int>fa;
    Disjoint_set_unoin(int n){fa.resize(n);iota(fa.begin(),fa.end(),0);};
    int find(int n){return n==fa[n]?n:fa[n] = find(fa[n]);}
    void unite(int x,int y){fa[find(x)] = find(y);}
};


}