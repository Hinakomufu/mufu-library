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
//
int max_degree=2000000;
//

template<class mint>
vector<mint> convolution(vector<mint> a,vector<mint> b,int const n=max_degree){
    assert(0<=n&&n<=max_degree);
    assert(mint::can_ntt());
    auto [rank_2,primitive_root]=mint::ntt_info();

    auto ntt=[&](vector<mint>&res,bool const invert){
        int const MOD=mint::mod_val();
        int sz=res.size();
        assert(sz<=(1<<rank_2));
        static vector<int> rev;
        static vector<mint> roots{0,1};

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
                mint e=mint(primitive_root).pow((MOD-1)>>(k+1));
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
            int const inv_n=mint(sz).inv().v;
            for(auto&x:res){
                x.v=static_cast<long>(x.v)*inv_n%MOD;
            }
        }
    };

    if(a.empty()||b.empty()) return {};
    int sz=1;
    int const l=min(static_cast<int>(a.size()),n+1);
    int const r=min(static_cast<int>(b.size()),n+1);
    a.resize(l);
    b.resize(r);
    while(l+r-1>sz) sz<<=1;
    a.resize(sz);
    b.resize(sz);

    ntt(a,false);
    ntt(b,false);
    for(int i=0;i<sz;i++) a[i]*=b[i];
    ntt(a,true);
    a.resize(min(n+1,l+r-1));
    return a;
}

int main(){
    int n,m;
    cin>>n>>m;
    vector<mint>a(n),b(m);
    for(int i=0;i<n;i++)cin>>a[i];
    for(int i=0;i<m;i++)cin>>b[i];
    vector<mint> const c=convolution(a,b);
    for(int i=0;i<n+m-1;i++)cout<<c[i]<<(i==n+m-2?"\n":" ");
}
