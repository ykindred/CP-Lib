template<typename T,int Rols,int Cols>
class Matrix{
private:
    vector<vector<T>> inside_value;
public:
    Matrix();
    template<typename U,int N1,int N2,int N3>
    friend Matrix<U,N1,N3> operator*(const Matrix<U,N1,N2>&LEFT_M,const Matrix<U,N2,N3>&RIGHT_M);

    friend Matrix<T,Rols,Cols> operator+(const Matrix<T,Rols,Cols>&LEFT_M,const Matrix<T,Rols,Cols>&RIGHT_M);
    friend Matrix<T,Rols,Cols> operator-(const Matrix<T,Rols,Cols>&LEFT_M,const Matrix<T,Rols,Cols>&RIGHT_M);
    vector<T>& operator[](int poi){
        return this->inside_value[poi];
    }
    const vector<T>& operator[](int poi) const{
        return this->inside_value[poi];
    }
};
//以下是成员函数的具体实现
template<typename T,int Rols,int Cols>
Matrix<T,Rols,Cols>::Matrix()
{
    this->inside_value.resize(Rols,vector<T>(Cols));
    return;
}
template<typename T,int N1,int N2,int N3>
Matrix<T,N1,N3> operator*(const Matrix<T,N1,N2>& LEFT_M,const Matrix<T,N2,N3>&RIGHT_M)
{
    Matrix<T,N1,N3> ret;
    for(int i = 0;i<N1;i++)
    {
        for(int j = 0;j<N3;j++)
        {
            T total = 0;
            for(int k = 0;k<N2;k++)
                total += LEFT_M[i][k]*  RIGHT_M[k][j];
            ret[i][j] = total;
        }
    }
    return ret;
}
template<typename T,int Rols,int Cols>
Matrix<T,Rols,Cols> operator+(const Matrix<T,Rols,Cols>&LEFT_M,const Matrix<T,Rols,Cols>&RIGHT_M)
{
    Matrix<T,Rols,Cols> ret;
    for(int i = 0;i<Rols;i++)
    {
        for(int j = 0;j<Cols;j++)
        {
            ret[i][j] = LEFT_M[i][j] + RIGHT_M[i][j];
        }
    }
    return ret;
}
template<typename T,int Rols,int Cols>
Matrix<T,Rols,Cols> operator-(const Matrix<T,Rols,Cols>&LEFT_M,const Matrix<T,Rols,Cols>&RIGHT_M)
{
    Matrix<T,Rols,Cols> ret;
    for(int i = 0;i<Rols;i++)
    {
        for(int j = 0;j<Cols;j++)
        {
            ret[i][j] = LEFT_M[i][j] - RIGHT_M[i][j];
        }
    }
    return ret;
}
//Matrix<ll,5,5> mat
