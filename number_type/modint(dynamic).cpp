#include <bits/stdc++.h>
using namespace std;

struct modint{
    static int mod;
    int v;

    modint(long const _v=0){
        long x=_v%mod;
        if(x<0) x+=mod;
        v=static_cast<int>(x);
    }

    operator int() const{ return v; }
    static int mod_val(){ return mod; }

    constexpr modint operator-() const{
        return modint(v==0?0:mod-v);
    }

    static modint raw(int v){
        modint x;
        x.v=v;
        return x;
    }

    friend std::istream&operator>>(std::istream&is,modint&m){
        long x;
        is>>x;
        m=modint(x);
        return is;
    }

    friend std::ostream&operator<<(std::ostream&os,const modint&m){
        return os<<m.v;
    }

    modint&operator=(long const x){
        long y=x%mod;
        if(y<0) y+=mod;
        v=static_cast<int>(y);
        return *this;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    modint operator+(T x) const{
        return *this+modint(x);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend modint operator+(T x,const modint&m){
        return modint(x)+m;
    }

    modint operator+(const modint&other) const{
        int x=v+other.v;
        if(x>=mod) x-=mod;
        return modint(x);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    modint operator-(T x) const{
        return *this-modint(x);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend modint operator-(T x,const modint&m){
        return modint(x)-m;
    }

    modint operator-(const modint&other) const{
        int x=v-other.v;
        if(x<0) x+=mod;
        return modint(x);
    }

    modint operator*(const modint&other) const{
        return modint(1LL*v*other.v%mod);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    modint operator*(T x) const{
        return *this*modint(x);
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend modint operator*(T x,const modint&m){
        return modint(x)*m;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    modint pow(T n) const{
        if(n<0) return inv().pow(-n);
        modint r(1),x(*this);
        while(n>0){
            if(n&1) r=r*x;
            x=x*x;
            n>>=1;
        }
        return r;
    }

    modint inv() const{ // mod:prime only
        return pow(mod-2);
    }

    modint operator/(const modint&other) const{
        return *this*other.inv();
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    modint operator/(T x) const{
        return *this*modint(x).inv();
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend modint operator/(T x,const modint&m){
        return modint(x)*m.inv();
    }

    modint&operator+=(const modint&other){
        v+=other.v;
        if(v>=mod) v-=mod;
        return *this;
    }

    modint&operator-=(const modint&other){
        v-=other.v;
        if(v<0) v+=mod;
        return *this;
    }

    modint&operator*=(const modint&other){
        v=static_cast<int>(1LL*v*other.v%mod);
        return *this;
    }

    modint&operator/=(const modint&other){
        return *this*=other.inv();
    }

    modint&operator++(){
        v++;
        if(v==mod) v=0;
        return *this;
    }

    modint operator++(int){
        modint tmp=*this;
        ++*this;
        return tmp;
    }

    modint&operator--(){
        if(v==0) v=mod-1;
        else v--;
        return *this;
    }

    modint operator--(int){
        modint tmp=*this;
        --*this;
        return tmp;
    }

    friend bool operator==(const modint&a,const modint&b){
        return a.v==b.v;
    }

    friend bool operator!=(const modint&a,const modint&b){
        return a.v!=b.v;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    bool operator==(T x) const{
        return v==x;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    bool operator!=(T x) const{
        return v!=x;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend bool operator==(T x,const modint&m){
        return x==m.v;
    }

    template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
    friend bool operator!=(T x,const modint&m){
        return x!=m.v;
    }
};

int modint::mod=998244353; // dummy mod
using mint=modint;

int main(){
    int t;
    cin>>t;
    while(t--){
        int x,y,p;
        cin>>x>>y>>p;

        modint::mod=p;
        cout<<mint(x).pow(y)<<endl;
    }
    return 0;
}
