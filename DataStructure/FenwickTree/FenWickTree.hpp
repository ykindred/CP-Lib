template <typename T>
struct FenWick
{
#define lowbit(x) = (x) & ((-x))
    vector<T> BIT;
    int n;
    FenWick(T n) : n(n){
        this->BIT.resize(n, 0);
    }
    FenWick(vector<T> &ori) : FenWick(ori.size()){
        this->BIT = ori;
    }
    void set(int poi, T value){
        for (; poi < n; poi += (lowbit(poi)))
        (this->BIT[poi]) += value;
        return;
    };
    T query(int poi){
        T ans = 0;
        for (; poi > 0; poi -= lowbit(poi)){
            ans += (this->BIT)[poi];
        }   
        return ans;
    };
    T query(int l, int r){
        return (this->query(r) - this->query(l - 1));
    };
};

