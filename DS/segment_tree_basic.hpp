template<typename TD,typename TT>
class segment_tree
{
private:
    int                 num__count;             //线段树中的元素个数
    int                 now_min;
    bool                has_build;              //线段树是否被构建
    vector<TD>          num;                    //存储原始数据的数组
    vector<TT>          tree;                   //存储树信息的数组
    function<TT(TT,TT)> __interval_merge;       //区间合并的函数，支持自定义
    function<TT(TD)>   __value_transform;      //将数组元素值与线段树维护值进行转换的函数
    void                __build_tree(int pL,int pR,int p);                  //内部的树构建实现
    TT                  __query_tree(int tL,int tR,int rL,int rR,int p);    //内部的区间查询实现
    void                __update_value_by_point(int tp,int rL,int rR,int p);//内部的单点更新实现    
public:
    //传入三个值，一个是初始的元素的vector ，一个是区间合并函数，一个是值->区间转换逻辑
    //模板中 TD 代表单点元素数据类型，TT代表区间数据类型，这样分开设计，是为了更好的兼容性，详细最底部的说明
    segment_tree(const vector<TD>& outside_vector,function<TT(TT,TT)> merge,function<TT(TD)> transform );
    segment_tree(const int size);
    ~segment_tree() =default;
    TT                  query(int l,int r); //对外的区间查询，合并规则遵循__interval_merge ，闭区间,0-based
    TT                  query(int poi);     //对外的区间查询, 合并规则遵循__interval_merge，闭区间 0-based
    void                set(int p,TD num);   //对外的单点更新，只需指定坐标和值
    void                build();            //外部的树构建
    void                clear();            //清理整棵树，因为不涉及自己实现的内存申请/回收，所以应该用不上
};
template<typename TD,typename TT>
segment_tree<TD,TT>::segment_tree(const vector<TD>& outside_vector,function<TT(TT,TT)> merge,function<TT(TD)> transform)
{
    this->num__count = outside_vector.size();
    num = outside_vector;
    this->tree.resize(4*this->num__count);
    this->has_build =false;
    this->__interval_merge = merge;
    this->__value_transform = transform;
    this->build();
}
template<typename TD,typename TT>
segment_tree<TD,TT>::segment_tree(const int size)
{
    segment_tree(vector<TD>(size));
}
template<typename TD,typename TT>
void segment_tree<TD,TT>::build()
{
    if(has_build) return;
    has_build =true;
    this->__build_tree(0,this->num__count-1,1);
    return;
}
template<typename TD,typename TT>
void segment_tree<TD,TT>::__build_tree(int pL,int pR,int p)
{
    if(pL==pR) tree[p] = this->__value_transform(num[pL]);
    else
    {
        int mid = (pL+pR)/2;
        this->__build_tree(pL,mid,2*p);
        this->__build_tree(mid+1,pR,2*p+1);
        tree[p] =__interval_merge(tree[2*p],tree[2*p+1]);
        return;
    }   
}
template<typename TD,typename TT>
TT segment_tree<TD,TT>::query(int l,int r)
{
    if(l==r) return this->__value_transform(this->num[l]);
    else
        return (this->__query_tree(l,r,0,this->num__count-1,1));
}
template<typename TD,typename TT>
TT segment_tree<TD,TT>::query(int poi)
{
    return this->query(poi,poi);
}

template<typename TD,typename TT>
TT segment_tree<TD,TT>::__query_tree(int tL,int tR,int rL,int rR,int p)
{
    //此处写法需要兼容多种运算
    TT total;
    int mid = (rL+rR)/2;
    if(tL==rL&&tR==rR) total = tree[p];
    else
    {
        if(tL<=mid&&tR<=mid)
            //只取左区间
            total = __query_tree(tL,tR,rL,mid,2*p);
        else if(tL<=mid&&tR>mid)
            //左右区间均覆盖，对半分
            //注意此处total不能直接相加
            total = (this->__interval_merge)(__query_tree(tL,mid,rL,mid,2*p),__query_tree(mid+1,tR,mid+1,rR,2*p+1));
        else if(tL>mid&&tR>mid)
            //只取右区间
            total = __query_tree(tL,tR,mid+1,rR,2*p+1);
    }
    return total;
}
template<typename TD,typename TT>
void segment_tree<TD,TT>::clear()
{
    this->num.clear();
    this->tree.clear();
    this->num__count = 0;
    this->has_build = 0;
}
template<typename TD,typename TT>
void segment_tree<TD,TT>::__update_value_by_point(int tp,int rL,int rR,int p)
{
    if(rL==rR&&rL==tp) tree[p] = this->__value_transform(num[tp]);
    else
    {
        if(rL<=tp&&rR>=tp)
        {
            int mid = (rL+rR)/2;
            //只用更新一边
            if(mid>=tp)
                this->__update_value_by_point(tp,rL,mid,2*p);
            else
                this->__update_value_by_point(tp,mid+1,rR,2*p+1);
            tree[p] = this->__interval_merge(tree[2*p],tree[2*p+1]);
        }
        else return;
    }
}
template<typename TD,typename TT>
void segment_tree<TD,TT>::set(int p,TD number)
{
    this->num[p] = number;
    __update_value_by_point(p,0,this->num__count-1,1);
}

/*
本写法兼容几乎所有需要使用线段树，不需要离散化，只需要单点修改的情形
以下仅提供使用样例，请先查看上面的注释部分

Example 1:查询区间最大值

//定义部分
segment_tree<ll,ll> seg
(
    num,
    [](auto l,auto r) ->auto
    {
        return max(l,r);
    },
    [](auto ele) -> auto
    {
        return ele;
    }
);

//查询 [l,r] 的最大值
auto ret = seg.query(l,r);
//修改 poi处的值 为x
seg.query(poi,x);
//poi 处的值 +1
seg.query(poi,seg.query(poi)+1);

这种写法比较基础，此模板适用于一些较为复杂的情况，如下
Example 2:查询区间公共因子个数
//一个元素num[i]为公共因子当且仅当，对于任意的1<=j<=n 有 num[j]%num[i] == 0;
//定义
semt<ll,pair<ll,ll>>seg
(
    num,
    [](auto l,auto r) ->pair<ll,ll>
    {
        int common_gcd = gcd(l.first,r.first);
        int total_count = 0;
        if(l.first ==common_gcd) total_count+=l.second;
        if(r.first == common_gcd) total_count+=r.second;
        return {common_gcd,total_count};
    }
    ,
    [](auto r) ->pair<ll,ll>
    {
        return {r,1};
    }
);
//查询 [l,r]中的最公共因子个数
auto [gcd,cnt] = seg.query(l,r);

cnt 即为所求
*/
