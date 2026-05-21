// 离散化
// 许多题目在处理数据的时候，并不关心值本身，只关心值之间的大小关系
// 这个时候进行离散化是比较正确的决定
template <typename T> 
vector<T> Discretization(vector<T> o_array)
{
    vector<T> knum = o_array;
    sort(knum.begin(), knum.end());
    knum.erase(unique(knum.begin(), knum.end()),knum.end());
    for(int i = 0;i<o_array.size();i++){
        o_array[i] = lower_bound(knum.begin(),knum.end(),o_array[i]) - knum.begin();
    }
    return o_array;
}
//传入一个数组，传出离散化之后的数组 如 num = Normalization(num);
