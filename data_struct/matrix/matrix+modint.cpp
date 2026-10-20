#include <bits/stdc++.h>
using namespace std;

template<int mod=998244353>
//template<int mod=1000000007>
struct modint{
    int v;

    constexpr modint(long const _v=0){
        long x=_v%mod;
        if(x<0) x+=mod;
        v=static_cast<int>(x);
    }

    constexpr operator int() const{ return v; }
    static constexpr int mod_val(){ return mod; }
    constexpr modint operator-() const{ return modint(v==0?0:mod-v); }

    static constexpr modint raw(int v){
        modint x;
        x.v=v;
        return x;
    }

    // NTT info: {max power of 2, primitive root}
    static constexpr pair<int,int> ntt_info(){
        if(mod==120586241) return {20,74066978};
        if(mod==167772161) return {25,17};
        if(mod==469762049) return {26,30};
        if(mod==754974721) return {24,362};
        if(mod==880803841) return {23,211};
        if(mod==943718401) return {22,663003469};
        if(mod==998244353) return {23,31};
        if(mod==1045430273) return {20,363};
        if(mod==1051721729) return {20,330};
        if(mod==1053818881) return {20,2789};
        return {-1,-1};
    }

    static constexpr bool can_ntt(){ return ntt_info().first>=0; }

    friend std::istream&operator>>(std::istream&is,modint&m){
        long x;
        is>>x;
        m=modint(x);
        return is;
    }

    friend std::ostream&operator<<(std::ostream&os,const modint&m){
        return os<<m.v;
    }

    constexpr modint&operator=(long const x){
        long y=x%mod;
        if(y<0) y+=mod;
        v=static_cast<int>(y);
        return *this;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    constexpr modint operator+(T x) const{
        return *this+modint(x);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend constexpr modint operator+(T x,const modint&m){
        return modint(x)+m;
    }

    constexpr modint operator+(const modint&other) const{
        int x=v+other.v;
        if(x>=mod) x-=mod;
        return modint(x);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    constexpr modint operator-(T x) const{
        return *this-modint(x);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend constexpr modint operator-(T x,const modint&m){
        return modint(x)-m;
    }

    constexpr modint operator-(const modint&other) const{
        int x=v-other.v;
        if(x<0) x+=mod;
        return modint(x);
    }

    constexpr modint operator*(const modint&other) const{
        return modint(1LL*v*other.v%mod);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    constexpr modint operator*(T x) const{
        return *this*modint(x);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend constexpr modint operator*(T x,const modint&m){
        return modint(x)*m;
    }

    constexpr modint pow(long n) const{
        if(n<0) return inv().pow(-n);
        modint r(1),x(*this);
        while(n>0){
            if(n&1) r=r*x;
            x=x*x;
            n>>=1;
        }
        return r;
    }

    constexpr modint inv() const{ // mod:prime only
        return pow(mod-2);
    }

    constexpr modint operator/(const modint&other) const{
        return *this*other.inv();
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    constexpr modint operator/(T x) const{
        return *this*modint(x).inv();
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend constexpr modint operator/(T x,const modint&m){
        return modint(x)*m.inv();
    }

    constexpr modint&operator+=(const modint&other){
        v+=other.v;
        if(v>=mod) v-=mod;
        return *this;
    }

    constexpr modint&operator-=(const modint&other){
        v-=other.v;
        if(v<0) v+=mod;
        return *this;
    }

    constexpr modint&operator*=(const modint&other){
        v=static_cast<int>(1LL*v*other.v%mod);
        return *this;
    }

    constexpr modint&operator/=(const modint&other){
        return *this*=other.inv();
    }

    constexpr modint&operator++(){
        v++;
        if(v==mod) v=0;
        return *this;
    }

    constexpr modint operator++(int){
        modint tmp=*this;
        ++*this;
        return tmp;
    }

    constexpr modint&operator--(){
        if(v==0) v=mod-1;
        else v--;
        return *this;
    }

    constexpr modint operator--(int){
        modint tmp=*this;
        --*this;
        return tmp;
    }

    friend constexpr bool operator==(const modint&a,const modint&b){
        return a.v==b.v;
    }

    friend constexpr bool operator!=(const modint&a,const modint&b){
        return a.v!=b.v;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    constexpr bool operator==(T x) const{
        return v==x;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    constexpr bool operator!=(T x) const{
        return v!=x;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend constexpr bool operator==(T x,const modint&m){
        return x==m.v;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend constexpr bool operator!=(T x,const modint&m){
        return x!=m.v;
    }
};

using mint=modint<>;
mint modpow(mint const a,long const n){ return a.pow(n); }

template<class T=mint>
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

    static T linear_recurrence(long n,const vector<T>&recurrence,const vector<T>&initial){
        int k=recurrence.size();
        vector<T> init(k,T(0));
        for(int i=0;i<static_cast<int>(initial.size())&&i<k;i++){
            init[i]=initial[i];
        }
        if(n<k) return init[n];

        matrix<T> M(k);
        for(int j=0;j<k;j++) M(0,j)=recurrence[j];
        for(int i=1;i<k;i++) M(i,i-1)=T(1);
        matrix<T> P=M.power(n-(k-1));

        vector<T> vec(k);
        for(int i=0;i<k;i++) vec[i]=init[k-1-i];

        vector<T> res=P*vec;
        return res[0];
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

matrix(int,std::initializer_list<int>)->matrix<mint>;
mint recur(long const n,vector<mint> const&recurrence,vector<mint> const&initial){
    return matrix<mint>::linear_recurrence(n,recurrence,initial);
}

int main(){
    // math 056 (mod1e9+7)
    long n;
    cin>>n;
    vector<mint>r={1,1,1};
    vector<mint>i={1,1,2};
    cout<<recur(n-1,r,i)<<endl;
    return 0;
}
