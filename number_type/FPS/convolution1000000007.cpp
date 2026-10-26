//https://github.com/Hinakomufu/mufu-library/blob/main/number_type/FPS/convolution1000000007.cpp
#include <bits/stdc++.h>
using namespace std;

template<int mod>
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

//
int max_degree=2000000;
//

constexpr long MOD=1000000007;
using mint=modint<MOD>;

template<class mint>
vector<mint> convolution1000000007(vector<mint> a,vector<mint> b,int const n=max_degree){
    assert(0<=n&&n<=max_degree);

    auto conv=[]<class T>(vector<T> A,vector<T> B,int const N){
        auto ntt=[](vector<T>&res,bool const invert){
            int const MOD=T::mod_val();
            int sz=res.size();

            static vector<int> rev;
            static vector<T> roots{0,1};

            if(static_cast<int>(rev.size())!=sz){
                int const k=__builtin_ctz(sz);
                rev.resize(sz);

                for(int i=0;i<sz;++i){
                    rev[i]=(rev[i>>1]>>1)
                           |((i&1)<<(k-1));
                }
            }

            if(static_cast<int>(roots.size())<sz){
                int k=__builtin_ctz(roots.size());
                roots.resize(sz);

                while((1<<k)<sz){
                    T e=T(3).pow((MOD-1)>>(k+1));
                    for(int i=1<<(k-1);i<(1<<k);++i){
                        roots[2*i]=roots[i];
                        roots[2*i+1]=roots[i]*e;
                    }
                    ++k;
                }
            }

            for(int i=0;i<sz;++i){
                if(i<rev[i])
                    swap(res[i],res[rev[i]]);
            }

            for(int len=1;len<sz;len<<=1){
                for(int i=0;i<sz;i+=len<<1){
                    for(int j=0;j<len;++j){
                        int const u=res[i+j].v;
                        int const v=static_cast<long>(res[i+j+len].v)
                                    *roots[len+j].v%MOD;
                        int x=u+v;
                        if(x>=MOD) x-=MOD;
                        int y=u-v;
                        if(y<0) y+=MOD;
                        res[i+j].v=x;
                        res[i+j+len].v=y;
                    }
                }
            }

            if(invert){
                reverse(res.begin()+1,res.end());
                int const inv_n=T(sz).inv().v;
                for(auto&x:res){
                    x.v=static_cast<long>(x.v)*inv_n%MOD;
                }
            }
        };

        if(A.empty()||B.empty()) return vector<T>{};
        int sz=1;
        int const l=min(static_cast<int>(A.size()),N+1);
        int const r=min(static_cast<int>(B.size()),N+1);
        A.resize(l);
        B.resize(r);
        while(l+r-1>sz) sz<<=1;
        A.resize(sz);
        B.resize(sz);

        ntt(A,false);
        ntt(B,false);
        for(int i=0;i<sz;i++) A[i]*=B[i];
        ntt(A,true);
        A.resize(min(N+1,l+r-1));
        return A;
    };

    constexpr long p1=998244353;
    constexpr long p2=1004535809;
    constexpr long p3=469762049;

    using mint1=modint<p1>;
    using mint2=modint<p2>;
    using mint3=modint<p3>;

    int const a_sz=a.size();
    int const b_sz=b.size();

    vector<mint1> a1(a_sz),b1(b_sz);
    vector<mint2> a2(a_sz),b2(b_sz);
    vector<mint3> a3(a_sz),b3(b_sz);

    for(int i=0;i<a_sz;i++){
        a1[i]=a[i].v;
        a2[i]=a[i].v;
        a3[i]=a[i].v;
    }

    for(int i=0;i<b_sz;i++){
        b1[i]=b[i].v;
        b2[i]=b[i].v;
        b3[i]=b[i].v;
    }

    auto c1=conv(a1,b1,n);
    auto c2=conv(a2,b2,n);
    auto c3=conv(a3,b3,n);

    vector<mint> c(a_sz+b_sz-1);
    auto crt=[](long r1,long const r2,long const r3){
        mint2 const t2=(mint2(r2)-mint2(r1))/mint2(p1);
        mint3 const t3=(mint3(r3)-mint3(r1)-mint3(p1)*t2.v)/(mint3(p1)*mint3(p2));
        return mint(r1)+mint(p1)*t2.v+mint(p1)*mint(p2)*t3.v;
    };

    for(int i=0;i<a_sz+b_sz-1;i++){ c[i]=crt(c1[i].v,c2[i].v,c3[i].v); }

    return c;
}

int main(){
    int n,m;
    cin>>n>>m;
    vector<mint> a(n),b(m);
    for(int i=0;i<n;i++)cin>>a[i];
    for(int i=0;i<m;i++)cin>>b[i];

    vector<mint> c=convolution1000000007(a,b);
    for(int i=0;i<n+m-1;i++){cout<<c[i]<<(i==n+m-2?'\n':' ');}
}
