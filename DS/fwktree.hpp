template <typename T>
class FenWick
{
private:
    vector<T> BIT;
    T lowbit(T x);
    int n;

public:
    FenWick(T n) : n(n)
    {
        this->BIT.resize(n, 0);
    }
    FenWick(vector<T> &ori) : FenWick(ori.size())
    {
        this->BIT = ori;
    }
    void set(int poi, T value);
    T query(int poi);
    T query(int l, int r);
};
template <typename T>
T FenWick<T>::lowbit(T x)
{
    return x & (-x);
}
template <typename T>
void FenWick<T>::set(int poi, T value)
{
    for (; poi < n; poi += (lowbit(poi)))
        (this->BIT[poi]) += value;
    return;
}
template <typename T>
T FenWick<T>::query(int poi)
{
    T ans = 0;
    for (; poi > 0; poi -= lowbit(poi))
    {
        ans += (this->BIT)[poi];
    }
    return ans;
}
template <typename T>
T FenWick<T>::query(int l, int r)
{
    return (this->query(r) - this->query(l - 1));
}
