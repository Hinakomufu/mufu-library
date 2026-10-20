#include <bits/stdc++.h>
using namespace std;

long inf=4e18;

struct semiring{
    static long add(long const a,long const b){ return min(a,b); }
    static long zero(){ return inf; }

    static long mul(long const a,long const b){ return a+b; }
    static long one(){ return 0; }

    static bool is_inf(long const x){ return x>=inf; } // for debug only
};

template<class T,class S>
struct matrix{
    int size;
    vector<T> cells;

    explicit matrix(int const n_)
        : size(n_), cells(n_*n_,S::zero()){}

    T&operator()(int const i,int const j){
        return cells[i*size+j];
    }

    const T&operator()(int const i,int const j) const{
        return cells[i*size+j];
    }

    static matrix identity(int n){
        matrix r(n);
        for(int i=0;i<n;i++) r(i,i)=S::one();
        return r;
    }

    matrix&operator=(std::initializer_list<T> init){
        int idx=0;
        fill(cells.begin(),cells.end(),S::zero());
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
                if(v==S::zero()) continue;
                for(int j=0;j<size;j++){
                    r(i,j)=S::add(r(i,j),S::mul(v,o(k,j)));
                }
            }
        }
        return r;
    }

    matrix&operator+=(const matrix&o){
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
                (*this)(i,j)=S::add((*this)(i,j),o(i,j));
        return *this;
    }

    matrix&operator*=(const matrix&o){
        matrix r(size);
        for(int i=0;i<size;i++){
            for(int k=0;k<size;k++){
                T const v=(*this)(i,k);
                if(v==S::zero()) continue;
                for(int j=0;j<size;j++){
                    r(i,j)=S::add(r(i,j),S::mul(v,o(k,j)));
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
                r(i,j)=S::add((*this)(i,j),o(i,j));
        return r;
    }

    matrix operator*(T const k) const{
        matrix r(size);
        for(int i=0;i<size;i++)
            for(int j=0;j<size;j++)
                r(i,j)=S::mul((*this)(i,j),k);
        return r;
    }

    friend matrix operator*(T const k,const matrix&m){
        matrix r(m.size);
        for(int i=0;i<m.size;i++)
            for(int j=0;j<m.size;j++)
                r(i,j)=S::mul(k,m(i,j));
        return r;
    }

    friend vector<T> operator*(const vector<T>&v,const matrix&m){
        int const n=m.size;
        vector<T> r(n,S::zero());
        for(int i=0;i<n;i++){
            T const vi=v[i];
            if(vi==S::zero()) continue;
            for(int j=0;j<n;j++){
                r[j]=S::add(r[j],S::mul(vi,m(i,j)));
            }
        }
        return r;
    }

    friend vector<T> operator*(const matrix&m,const vector<T>&v){
        int const n=m.size;
        vector<T> r(n,S::zero());
        for(int i=0;i<n;i++){
            T sum=S::zero();
            for(int j=0;j<n;j++){
                sum=S::add(sum,S::mul(m(i,j),v[j]));
            }
            r[i]=sum;
        }
        return r;
    }

    template<class U>
    explicit matrix(int const n_,const vector<U>&vec)
        : size(n_), cells(n_*n_,S::zero()){
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
        : size(n_), cells(n_*n_,S::zero()){
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
          cells(size*size,S::zero()){
        assert(size*size==static_cast<int>(init.size()));
        int idx=0;
        for(auto&x:init){
            if(idx>=size*size) break;
            cells[idx++]=x;
        }
    }

    // matrix{{1,2},{3,4}}
    matrix(std::initializer_list<std::initializer_list<T>> const init)
        : size(init.size()), cells(size*size,S::zero()){
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
        T s=S::zero();
        int const base=H*size;
        for(int j=0;j<size;j++){
            s=S::add(s,cells[base+j]);
        }
        return s;
    }

    T col_sum(int const w) const{
        int const W=(w%size+size)%size;
        T s=S::zero();
        for(int i=0;i<size;i++){
            s=S::add(s,cells[i*size+W]);
        }
        return s;
    }

    T total_sum() const{
        T s=S::zero();
        for(int i=0;i<size*size;i++){
            s=S::add(s,cells[i]);
        }
        return s;
    }

    void debug(bool const display_inf=false) const{
        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                T const v=(*this)(i,j);
                if(display_inf&&S::is_inf(v)) cout<<"inf";
                else cout<<v;
                if(j+1<size) cout<<' ';
            }
            cout<<'\n';
        }
    }
};

int main(){
    return 0;
}
