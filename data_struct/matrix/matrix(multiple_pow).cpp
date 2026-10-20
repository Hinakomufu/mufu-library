#include <bits/stdc++.h>
using namespace std;

template<class T>
struct matrix{
    int size;
    vector<T> cells;

    explicit matrix(int const n_)
        : size(n_), cells(n_*n_,T(0)){}

    T&operator()(int const i,int const j){
        return cells[i*size+j];
    }

    const T&operator()(int const i,int const j) const{
        return cells[i*size+j];
    }

    static matrix identity(int n){
        matrix r(n);
        for(int i=0;i<n;i++) r(i,i)=T(1);
        return r;
    }

    matrix&operator=(std::initializer_list<T> init){
        int idx=0;
        fill(cells.begin(), cells.end(), T(0));
        for(auto&x:init){
            if(idx>=size*size) break;
            cells[idx++]=x;
        }
        return *this;
    }

    matrix power(long k) const{
        matrix r=matrix::identity(size);
        matrix x=*this;

        while(k>0){
            if(k&1) r*=x;
            x*=x;
            k>>=1;
        }
        return r;
    }

    matrix&pow(long const k){
        *this=this->power(k);
        return *this;
    }

    matrix operator*(const matrix&o) const{
        matrix r(size);
        for(int i=0;i<size;i++){
            for(int k=0;k<size;k++){
                T const v=(*this)(i,k);
                if(v==T(0)) continue;
                int const o_k=k*size;
                int const r_i=i*size;
                for(int j=0;j<size;j++){
                    r.cells[r_i+j]+=v*o.cells[o_k+j];
                }
            }
        }
        return r;
    }

    matrix&operator+=(const matrix&o){
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
                (*this)(i,j)+=o(i,j);
        return *this;
    }

    matrix&operator-=(const matrix&o){
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
                (*this)(i,j)-=o(i,j);
        return *this;
    }

    matrix&operator*=(const matrix&o){
        matrix r(size);
        for(int i=0;i<size;i++){
            for(int k=0;k<size;k++){
                T const v=(*this)(i,k);
                if(v==T(0)) continue;
                int const o_k=k*size;
                int const r_i=i*size;
                for(int j=0;j<size;j++){
                    r.cells[r_i+j]+=v*o.cells[o_k+j];
                }
            }
        }
        *this=move(r);
        return *this;
    }

    matrix operator+(const matrix&o) const{
        matrix r(size);
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
                r(i,j)=(*this)(i,j)+o(i,j);
        return r;
    }

    matrix operator-(const matrix&o) const{
        matrix r(size);
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
                r(i,j)=(*this)(i,j)-o(i,j);
        return r;
    }

    matrix operator*(T const k) const{
        matrix r(size);
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
                r(i,j)=(*this)(i,j)*k;
        return r;
    }

    friend matrix operator*(T const k,const matrix&m){
        return m*k;
    }

    friend vector<T> operator*(const vector<T>&v,const matrix&m){
        int const n=m.size;
        vector<T> r(n,T(0));
        for(int i=0;i<n;i++){
            T const vi=v[i];
            if(vi==T(0)) continue;
            for(int j=0;j<n;j++){
                r[j]+=vi*m(i,j);
            }
        }
        return r;
    }

    friend vector<T> operator*(const matrix&m,const vector<T>&v){
        int const n=m.size;
        vector<T> r(n,T(0));
        for(int i=0;i<n;i++){
            T sum=0;
            for(int j=0;j<n;j++){
                sum+=m(i,j)*v[j];
            }
            r[i]=sum;
        }
        return r;
    }

    template<class U>
    explicit matrix(int const n_,const vector<U>&vec)
        : size(n_), cells(n_*n_,T(0)){
        int idx=0;
        int const limit=size*size;
        int const m=vec.size();

        for(int i=0;i<size&&idx<m&&idx<limit;i++){
            for(int j=0;j<size&&idx<m&&idx<limit;j++){
                (*this)(i,j)=vec[idx++];
            }
        }
    }

    // matrix(2,{1,2,3,4})
    explicit matrix(int const n_,std::initializer_list<T> const init)
        : size(n_), cells(n_*n_,T(0)){
        int idx=0;
        int const limit=size*size;
        for(auto&x:init){
            if(idx>=limit) break;
            (*this)(idx/size,idx%size)=T(x);
            idx++;
        }
    }

    // matrix{1,2,3,4}
    matrix(std::initializer_list<T> init)
        : size(static_cast<int>(std::sqrt(init.size()))),
          cells(size*size,T(0)){
        assert(size*size==static_cast<int>(init.size()));
        int idx=0;
        for(auto&x:init){
            if(idx>=size*size) break;
            cells[idx++]=x;
        }
    }

    // matrix{{1,2},{3,4}}
    matrix(std::initializer_list<std::initializer_list<T>> const init)
        : size(init.size()), cells(size*size,T(0)){
        int i=0;
        for(auto&row:init){
            int j=0;
            for(auto&x:row){
                if(i<size&&j<size){
                    (*this)(i,j)=T(x);
                }
                j++;
            }
            i++;
        }
    }

    T row_sum(int const h) const{
        int const H=(h%size+size)%size;
        T s=0;
        int const base=H*size;
        for(int j=0;j<size;j++){
            s+=cells[base+j];
        }
        return s;
    }

    T col_sum(int const w) const{
        int const W=(w%size+size)%size;
        T s=0;
        for(int i=0;i<size;i++){
            s+=cells[i*size+W];
        }
        return s;
    }

    T total_sum() const{
        T s=0;
        for(int i=0;i<size*size;i++){
            s+=cells[i];
        }
        return s;
    }

    void debug() const{
        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                cout<<(*this)(i,j);
                if(j+1<size) cout<<' ';
            }
            cout<<'\n';
        }
    }
};

template<class T>
struct matrix_pows{
    int size;
    vector<matrix<T>> dbl;

    explicit matrix_pows(const matrix<T>&A,int max_bits=60)
        : size(A.size), dbl(max_bits,matrix<T>(A.size)){
        dbl[0]=A;
        for(int i=1;i<max_bits;i++){
            dbl[i]=dbl[i-1]*dbl[i-1];
        }
    }

    matrix<T> pow(long k) const{
        matrix<T> r=matrix<T>::identity(size);
        int i=0;
        while(k>0){
            if(k&1) r*=dbl[i];
            k>>=1;
            i++;
        }
        return r;
    }
};

/*  template

    matrix<int> A=matrix{1,2,3,4,5,6,7,8,9};
    matrix<int> B=matrix(3,{1,2,3,4,5,6,7,8,9});
    matrix<int> C=matrix{{1,2,3},{4,5,6},{7,8,9}};

    vector<int> v(9);
    for(int i=0;i<9;i++) cin>>v[i];
    matrix<int> X(3,v);

    matrix<int> I=matrix<int>::identity(3);
    matrix<int> O=matrix<int>(3);
    A+=matrix{1,2,3,2,1,2,3,2,1};
    B(1,2)=3;
    C(2,1)*=2;
    cin>>C(0,0);
    matrix<int> D=A*X*X-B*X+C*I;
    D.debug();
*/

int main(){
    matrix<int> A=matrix{1,2,3,4,5,6,7,8,9};
    matrix<int> B=matrix(3,{1,2,3,4,5,6,7,8,9});
    matrix<int> C=matrix{{1,2,3},{4,5,6},{7,8,9}};

    vector<int> v(9);
    for(int i=0;i<9;i++) cin>>v[i];
    matrix<int> X(3,v);

    matrix<int> I=matrix<int>::identity(3);
    matrix<int> O=matrix<int>(3);
    A+=matrix{1,2,3,2,1,2,3,2,1};
    B(1,2)=3;
    C(2,1)*=2;
    cin>>C(0,0);
    matrix<int> D=A*X*X-B*X+C*I;
    D.debug();
    return 0;
}
