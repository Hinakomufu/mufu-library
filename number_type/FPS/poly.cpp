//https://github.com/Hinakomufu/mufu-library/blob/main/number_type/FPS/poly.cpp
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

template<class mint>
struct combination{
    private:
        int max_n;
        vector<mint> factorial,inv_factorial;

    public:
        explicit combination(int n): max_n(n){
            factorial.resize(n+1);
            inv_factorial.resize(n+1);

            factorial[0]=1;
            for(long i=1;i<=n;i++) factorial[i]=factorial[i-1]*i;

            inv_factorial[n]=factorial[n].inv();
            for(long i=n;i>0;i--) inv_factorial[i-1]=inv_factorial[i]*i;
        }

        template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
        mint fact(T n) const{
            if(n<0||n>max_n) return mint(1);
            return factorial[n];
        }

        template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
        mint invfact(T n) const{
            if(n<0||n>max_n) return mint(1);
            return inv_factorial[n];
        }

        template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
        mint C(T n,T r) const{
            if(r<0||r>n) return mint(0);
            if(n<=max_n)return factorial[n]*inv_factorial[r]*inv_factorial[n-r];
            mint res=1;
            for(T i=0;i<r;++i){
                res*=mint(n-i);
                res/=mint(i+1);
            }
            return res;
        }

        template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
        mint P(T n,T const r) const{
            if(r<0||r>n) return mint(0);
            if(n<=max_n)return factorial[n]*inv_factorial[n-r];
            mint res=1;
            for(T i=0;i<r;++i){
                res*=mint(n-i);
            }
            return res;
        }
};

// clang-format off
using mint=modint<>;
static combination<mint> comb(1000000);
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint modpow(mint const a,T const n){ return a.pow(n); }
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint fact(T n){ return comb.fact(n); }
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint invfact(T n){ return comb.invfact(n); }
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint c(T n,T r){ return comb.C(n,r); }
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint p(T n,T r){ return comb.P(n,r); }
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint h(T n,T r){ return comb.C(n+r-1,r); }

// clang-format on
//
int max_degree=2000000;
//

struct poly{
    private:
        vector<mint> func;

        int size() const{ return static_cast<int>(func.size()); }

        static void ntt(vector<mint>&res,bool const invert){
            constexpr int MOD=mint::mod_val();
            constexpr int primitive_root=mint::ntt_info().second;
            int const sz=res.size();
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
                if(i<rev[i])swap(res[i],res[rev[i]]);
            }

            for(int len=1;len<sz;len<<=1){
                for(int i=0;i<sz;i+=len<<1){
                    for(int j=0;j<len;++j){
                        int const u=res[i+j].v;
                        int const v=static_cast<long>(res[i+j+len].v)*roots[len+j].v%MOD;
                        int x=u+v;
                        if(x>=MOD)x-=MOD;
                        int y=u-v;
                        if(y<0)y+=MOD;
                        res[i+j].v=x;
                        res[i+j+len].v=y;
                    }
                }
            }

            if(invert){
                reverse(res.begin()+1,res.end());
                int const inv_n=mint(sz).inv().v;
                for(auto&x:res)x.v=static_cast<long>(x.v)*inv_n%MOD;
            }
        }

        static void convolution(vector<mint>&a,vector<mint>&b,int const n=max_degree){
            assert(0<=n&&n<=max_degree);
            if(a.empty()||b.empty()){
                a.clear();
                return;
            }

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
        }

        static void transposed_ntt(vector<mint>&res,bool const invert){
            int const sz=res.size();
            if(!invert){
                ntt(res,true);
                reverse(res.begin()+1,res.end());
                for(auto&x:res)x*=mint(sz);
            } else{
                reverse(res.begin()+1,res.end());
                ntt(res,false);
                int const inv_n=mint(sz).inv().v;
                for(auto&x:res)x.v=static_cast<long>(x.v)*inv_n%mint::mod_val();
            }
        }

        static vector<mint> transposed_convolution(vector<mint> a,vector<mint> b){
            int const n=static_cast<int>(a.size());
            int const m=static_cast<int>(b.size());
            int sz=1;
            while(sz<n) sz<<=1;
            a.resize(sz);
            b.resize(sz);
            ntt(b,false);
            transposed_ntt(a,true);
            for(int i=0;i<sz;i++)a[i]*=b[i];
            transposed_ntt(a,false);
            a.resize(n-m+1);
            return a;
        }

        static vector<mint> inv_series(const vector<mint>&a,int const n){
            assert(n>=0);
            assert(!a.empty()&&a[0]!=mint(0));
            vector<mint> res(1);
            res[0]=a[0].inv();
            int sz=1;
            while(sz<n+1){
                sz<<=1;
                sz=min(sz,n+1);
                vector<mint> f(min(static_cast<int>(a.size()),sz));
                for(int i=0;i<static_cast<int>(f.size());i++) f[i]=a[i];
                vector<mint> g=res;
                convolution(f,g,sz-1);
                f.resize(sz);
                for(int i=0;i<sz;i++) f[i]=-f[i];
                f[0]+=mint(2);
                convolution(res,f,sz-1);
            }
            res.resize(n+1);
            return res;
        }

        static void shrink_vec(vector<mint>&a){
            while(static_cast<int>(a.size())>1&&!a.empty()&&a.back()==mint(0)) a.pop_back();
        }

        poly composition_power2(const poly&g) const{
            int const n=g.size();
            assert(n>0);
            assert((n&(n-1))==0);
            constexpr int k=1;
            vector<mint> q(2*n);
            for(int i=0;i<n;i++)q[i]=-g[i];

            auto rec=[&](auto&self,int const N,int const K,vector<mint> Q)->vector<mint>{
                if(N==1){
                    vector<mint> p(2*K);

                    vector<mint> rf=func;
                    reverse(rf.begin(),rf.end());
                    for(int i=0;i<K;i++)p[2*i]=rf[i];
                    return p;
                }

                vector<mint> R(2*N*K);
                for(int i=0;i<2*N*K;i++)R[i]=(i%2==0?Q[i]:-Q[i]);

                vector<mint> QQ=Q;
                vector<mint> RR=R;
                convolution(QQ,RR,4*N*K-1);
                QQ.resize(4*N*K);

                for(int i=0;i<2*N*K;i++)QQ[2*N*K+i]+=Q[i]+R[i];
                vector<mint> nxt_Q(2*N*K);
                for(int j=0;j<2*K;j++){
                    for(int i=0;i<N/2;i++){
                        nxt_Q[N*j+i]=QQ[(2*N)*j+(2*i)];
                    }
                }

                vector<mint> const nxt_p=self(self,N/2,K*2,nxt_Q);
                vector<mint> pq(4*N*K);
                for(int j=0;j<2*K;j++){
                    for(int i=0;i<N/2;i++){
                        pq[(2*N)*j+(2*i+1)]+=nxt_p[N*j+i];
                    }
                }

                vector<mint> p(2*N*K);
                for(int i=0;i<2*N*K;i++)p[i]+=pq[2*N*K+i];
                pq.pop_back();
                vector<mint> const x=transposed_convolution(pq,R);
                for(int i=0;i<2*N*K;i++)p[i]+=x[i];
                return p;
            };
            vector<mint> const p=rec(rec,n,k,q);

            vector<mint> output(n);
            for(int i=0;i<n;i++)output[i]=p[i];
            reverse(output.begin(),output.end());
            return poly(output);
        }

        vector<mint> power_projection(const poly&wt,int const m) const{
            const poly&f=*this;
            assert(f[0]==mint(0));
            assert(m>=0);
            if(f.is_zero()){ return vector<mint>(m+1,mint(0)); }

            if(f[0]!=mint(0)){
                mint const c=f[0];
                poly h=f;
                h[0]=0;
                vector<mint> A=h.power_projection(wt,m);
                for(int p=0;p<=m;p++)A[p]*=invfact(p);
                vector<mint> B(m+1);
                mint pw=1;
                for(int q=0;q<=m;q++){
                    B[q]=pw*invfact(q);
                    pw*=c;
                }

                poly pa(A);
                poly const pb(B);
                pa*=pb;

                A=pa.coeff();
                A.resize(m+1);
                for(int i=0;i<=m;i++)A[i]*=fact(i);
                return A;
            }

            int n=1;
            while(n<f.size()||n<wt.size())n<<=1;

            poly ff=f;
            poly ww=wt;
            ff.func.resize(n);
            ww.func.resize(n);

            vector<mint> P(2*n);
            vector<mint> Q(2*n);

            reverse(ww.func.begin(),ww.func.end());
            for(int i=0;i<n;i++){
                P[i]=ww[i];
                Q[i]=-ff[i];
            }

            int k=1;

            while(n>1){
                vector<mint> R(2*n*k);

                for(int i=0;i<2*n*k;i++){ R[i]=(i%2==0?Q[i]:-Q[i]); }

                vector<mint> PQ=P;
                vector<mint> QQ=Q;
                vector<mint> RR=R;
                vector<mint> RQ=R;

                convolution(PQ,RR,4*n*k-1);
                convolution(QQ,RQ,4*n*k-1);

                PQ.resize(4*n*k);
                QQ.resize(4*n*k);

                for(int i=0;i<2*n*k;i++){
                    PQ[2*n*k+i]+=P[i];
                    QQ[2*n*k+i]+=Q[i]+R[i];
                }

                fill(P.begin(),P.end(),mint(0));
                fill(Q.begin(),Q.end(),mint(0));

                for(int j=0;j<2*k;j++){
                    for(int i=0;i<n/2;i++){
                        P[n*j+i]=PQ[(2*n)*j+(2*i+1)];
                        Q[n*j+i]=QQ[(2*n)*j+(2*i)];
                    }
                }

                n>>=1;
                k<<=1;

                P.resize(2*n*k);
                Q.resize(2*n*k);
            }

            vector<mint> res(k);

            for(int i=0;i<k;i++)
                res[i]=P[2*i];

            reverse(res.begin(),res.end());
            res.resize(m+1);
            return res;
        }

    public:
        explicit poly(): func({mint(1)}){}
        explicit poly(const vector<mint>&v): func(v){}

        template<class...Args,std::enable_if_t<(std::is_convertible_v<Args,long>&&...),int> =0>
        explicit poly(Args...args): func{mint(args)...}{ shrink_vec(func); }

        mint&operator[](int const i){ return func[i]; }

        mint operator[](int const i) const{
            if(i<0||i>=static_cast<int>(func.size()))return mint(0);
            return func[i];
        }

        static poly zero(int const n=0){ return poly(vector<mint>(n+1)); } // blank poly (until nth degree)
        static poly one(int const n=0){
            int const m=max(n,0);
            poly res=zero(m);
            res[0]=mint(1);
            return res;
        }

        static poly linear(int const n=1){
            int const m=max(n,1);
            poly res=zero(m);
            res[1]=mint(1);
            return res;
        }

        static poly factorial_poly(int const n=0){
            int const m=max(n,0);
            poly res=zero(m);
            for(int i=0;i<=m;i++){ res[i]=fact(i); }
            return res;
        }

        bool operator==(const poly&other) const{ return func==other.func; }
        bool operator!=(const poly&other) const{ return !(*this==other); }
        int degree() const{ return static_cast<int>(func.size())-1; }
        bool is_zero() const{ return (func.size()==1&&func[0]==mint(0)); }

        const vector<mint>&coeff() const{ return func; }

        poly&truncate(int const n){
            if(n<0){
                func={mint(0)};
                return *this;
            }
            if(size()>n+1)func.resize(n+1);
            shrink_vec(func);
            return *this;
        }

        poly operator-() const{ return (*this)*mint(-1); }

        poly operator+(mint const c) const{
            poly res=*this;
            res[0]+=c;
            return res;
        }

        poly operator-(mint const c) const{
            poly res=*this;
            res[0]-=c;
            return res;
        }

        poly operator*(mint const c) const{
            vector<mint> res=func;
            for(auto&x:res)x*=c;
            return poly(res);
        }

        friend poly operator+(mint const c,const poly&f){ return f+c; }
        friend poly operator-(mint const c,const poly&f){ return (-f)+c; }

        template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
        friend poly operator+(T c,const poly&f){ return f+mint(c); }

        template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
        friend poly operator-(T c,const poly&f){ return mint(c)-f; }

        template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0>
        friend poly operator*(T c,const poly&f){ return f*mint(c); }

        poly operator/(mint const c) const{ return *this*c.inv(); }

        poly&operator*=(mint const c){
            for(auto&x:func)x*=c;
            shrink_vec(func);
            return *this;
        }

        poly&operator/=(mint const c){ return *this*=c.inv(); }

        poly operator+(const poly&other) const{
            int const sz=max(size(),other.size());
            vector<mint> res(sz);
            for(int i=0;i<sz;i++)res[i]=(*this)[i]+other[i];
            return poly(res);
        }

        poly operator-(const poly&other) const{
            int const sz=max(size(),other.size());
            vector<mint> res(sz);
            for(int i=0;i<sz;i++)res[i]=(*this)[i]-other[i];
            return poly(res);
        }

        poly operator*(const poly&other) const{
            auto res=func;
            auto rhs=other.func;
            convolution(res,rhs,max_degree);
            return poly(res);
        }

        poly&mul(const poly&other,int const n){
            auto rhs=other.func;
            convolution(func,rhs,n);
            return *this;
        }

        poly&operator+=(const poly&other){
            func.resize(max(size(),other.size()));
            for(int i=0;i<other.size();i++)
                func[i]+=other[i];
            shrink_vec(func);
            return *this;
        }

        poly&operator-=(const poly&other){
            func.resize(max(size(),other.size()));
            for(int i=0;i<other.size();i++)
                func[i]-=other[i];
            shrink_vec(func);
            return *this;
        }

        poly&operator*=(const poly&other){
            *this=(*this)*other;
            return *this;
        }

        poly prefix_sum() const{
            vector<mint> res(size());
            mint sum=0;
            for(int i=0;i<size();i++){
                sum+=func[i];
                res[i]=sum;
            }
            return poly(res);
        }

        poly taylor_shift(mint const c) const{
            int const n=degree();
            vector<mint> res(n+1);
            for(int i=0;i<=n;i++)res[i]=func[i]*fact(i);
            reverse(res.begin(),res.end());

            vector<mint> b(n+1);
            mint d=1;
            for(int i=0;i<=n;i++){
                b[i]=d*invfact(i);
                d*=c;
            }

            convolution(res,b,n);
            reverse(res.begin(),res.end());
            for(int i=0;i<=n;i++)res[i]*=invfact(i);
            return poly(res);
        }

        mint bostan_mori(const poly&q,long k) const{
            poly P=*this;
            poly Q=q;
            while(k>0){
                int const need=max(P.degree()+Q.degree(),2*Q.degree())+1;
                int sz=1;
                while(sz<need)sz<<=1;
                vector<mint> pv=P.func;
                vector<mint> qv=Q.func;
                pv.resize(sz);
                qv.resize(sz);
                ntt(pv,false);
                ntt(qv,false);
                vector<mint> s(sz);
                vector<mint> t(sz);
                for(int i=0;i<sz;++i){
                    int const j=(i+(sz>>1))&(sz-1);
                    s[i]=pv[i]*qv[j];
                    t[i]=qv[i]*qv[j];
                }
                ntt(s,true);
                ntt(t,true);
                vector<mint> np;
                vector<mint> nq;
                np.resize((need+1)/2);
                nq.resize((need+1)/2);
                if(k&1){ for(int i=0;i<static_cast<int>(np.size());++i)np[i]=s[2*i+1]; } else{
                    for(int i=0;i<static_cast<int>(np.size());++i)np[i]=s[2*i];
                }
                for(int i=0;i<static_cast<int>(nq.size());++i)nq[i]=t[2*i];
                P=poly(np);
                Q=poly(nq);
                k>>=1;
            }
            return P[0]/Q[0];
        }

        poly inv(int const n,bool const sparse=false) const{
            assert(n>=0);
            assert(!func.empty()&&func[0]!=mint(0));
            if(!sparse)return poly(inv_series(func,n));
            vector<mint> res(n+1);
            mint const inv_0=func[0].inv();
            res[0]=inv_0;
            vector<pair<int,mint>> nz;
            for(int i=1;i<size()&&i<=n;i++){
                if(func[i]!=mint(0))nz.emplace_back(i,func[i]);
            }
            for(int i=1;i<=n;i++){
                mint s=0;
                for(auto [d,a]:nz){
                    if(d>i) break;
                    s+=a*res[i-d];
                }
                res[i]=-s*inv_0;
            }
            return poly(res);
        }

        poly div(const poly&other,int const n=-1) const{
            assert(n>=-1);
            assert(!other.func.empty());
            if(n==-1){
                int const x=degree();
                int const y=other.degree();
                if(x<y)return poly(mint(0));
                int const k=x-y;

                vector<mint> const rf(func.rbegin(),func.rend());
                vector<mint> const rg(other.func.rbegin(),other.func.rend());

                poly const F(rf);
                poly const G(rg);
                poly Q=F.div(G,k);
                Q.func.resize(k+1);
                reverse(Q.func.begin(),Q.func.end());
                return Q;
            }
            assert(other[0]!=mint(0));
            auto res=func;
            poly inv_other=other.inv(n);
            convolution(res,inv_other.func,n);
            return poly(res);
        }

        poly operator/(const poly&other) const{ return div(other); }
        poly&operator/=(const poly&other){ return *this=div(other); }

        poly operator%(const poly&other) const{
            if(degree()<other.degree())
                return *this;

            poly const q=div(other);
            poly r=*this-q*other;
            r.truncate(other.degree()-1);
            return r;
        }

        poly&operator%=(const poly&other){ return *this=*this%other; }

        mint operator()(const mint&x) const{
            mint res=0;
            for(int i=degree();i>=0;i--)
                res=res*x+(*this)[i];
            return res;
        }

        poly composition(const poly&g) const{
            assert(!g.func.empty());
            int const original_size=g.size();
            int n=1;
            while(n<original_size) n<<=1;
            if(n==original_size)return composition_power2(g);

            poly padded_f=*this;
            poly padded_g=g;
            padded_f.func.resize(n);
            padded_g.func.resize(n);
            poly res=padded_f.composition_power2(padded_g);

            res.func.resize(original_size);
            shrink_vec(res.func);
            return res;
        }

        poly compositional_inverse() const{
            const int n=degree();
            if(n<0) return poly();
            assert((*this)[0]==mint(0));
            if(n==0)return *this;
            assert((*this)[1]!=mint(0));

            mint const ic=(*this)[1].inv();
            poly f=*this;
            f*=ic;
            poly wt=poly::zero(n);
            wt[n]=1;

            vector<mint> const A=f.power_projection(wt,n);
            poly g=poly::zero(n-1); // g[n-i]=n*A[i]/i
            for(int i=1;i<=n;i++){ g[n-i]=mint(n)*A[i]/mint(i); }
            poly h(vector<mint>(g.coeff().begin(),g.coeff().end())); // h=(g/x)^(-1/n)

            h=h.pow(-1L*mint(n).inv().v,n-1);
            h.truncate(n-1);
            poly res=poly::zero(n);
            for(int i=0;i<h.size();i++)res[i+1]=h[i];
            mint pw=1;

            for(int i=0;i<=n;i++){
                res[i]*=pw;
                pw*=ic;
            }
            return res;
        }

        poly operator()(const poly&g) const{ return composition(g); }

        poly deriv() const{
            int const n=func.size();
            if(n<=1) return poly(mint(0));
            vector<mint> res(n-1);
            for(int i=1;i<n;i++) res[i-1]=func[i]*mint(i);
            return poly(res);
        }

        poly integral(int const n) const{
            assert(n>=0);
            vector<mint> r(n+1);
            for(int i=0;i<n&&i<static_cast<int>(func.size());i++){
                r[i+1]=func[i]/mint(i+1);
            }
            return poly(r);
        }

        poly integral() const{
            if(func.empty()) return poly(mint(0));
            long const n=
                    (static_cast<long>(func.size())-1)+2;
            assert(n<=numeric_limits<int>::max());
            return integral(static_cast<int>(n));
        }

        poly log(int const n,bool const sparse=false) const{
            assert(n>=0);
            assert(!func.empty()&&func[0]==mint(1));
            if(n==0) return poly(mint(0));
            return deriv().mul(inv(n,sparse),n).integral(n);
        }

        poly exp(int const n,bool const sparse=false) const{
            assert(n>=0);
            assert(!func.empty()&&func[0]==mint(0));
            if(!sparse){
                poly res(mint(1));
                int sz=1;
                while(sz<n+1){
                    sz<<=1;
                    sz=min(sz,n+1);
                    poly g=res;
                    poly log_g=g.log(sz-1);
                    poly f=*this;
                    f.truncate(sz-1);
                    g*=poly(mint(1))-log_g+f;
                    g.truncate(sz-1);
                    res=g;
                }
                return res;
            }
            vector<mint> res(n+1);
            res[0]=mint(1);
            vector<pair<int,mint>> nz;
            for(int i=1;i<size()&&i<=n;i++){
                if(func[i]!=mint(0))
                    nz.emplace_back(i,mint(i)*func[i]);
            }
            for(int i=1;i<=n;i++){
                mint s=0;
                for(auto [d,a]:nz){
                    if(d>i) break;
                    s+=a*res[i-d];
                }
                res[i]=s/mint(i);
            }
            return poly(res);
        }

        poly pow(long const k,int const n,bool const sparse=false) const{
            if(k<0)return inv(n,sparse).pow(-k,n,sparse);
            assert(k>=0);
            assert(n>=0);
            if(k==0){
                poly res=zero(n);
                res[0]=mint(1);
                return res;
            }
            if(func.empty()) return zero(n);

            int d=0;
            while(d<size()&&(*this)[d]==mint(0)) ++d;
            if(d==size()) return zero(n);
            if(d>0&&k>n/d) return zero(n);
            long const shift=1L*d*k;
            if(shift>n) return zero(n);

            mint const a=(*this)[d];
            poly f(vector<mint>(func.begin()+d,func.end()));
            f/=a;
            int const need=n-static_cast<int>(shift);
            poly res;
            if(need==0){
                res=poly(mint(1));
            } else if(!sparse){
                res=(f.log(need)*mint(k)).exp(need);
            } else{
                vector<mint> ans(need+1);
                ans[0]=mint(1);

                vector<pair<int,mint>> nz;
                for(int i=1;i<f.size()&&i<=need;i++){
                    if(f[i]!=mint(0))nz.emplace_back(i,f[i]);
                }
                vector<mint> g(need);
                for(int i=0;i<need;i++){
                    mint s=0;
                    for(auto [j,fj]:nz){
                        if(j>i) break;
                        s-=fj*g[i-j];
                    }
                    for(auto [j,fj]:nz){
                        if(j>i+1) break;
                        s+=mint(k)*mint(j)*fj*ans[i-j+1];
                    }
                    g[i]=s;
                    ans[i+1]=s/mint(i+1);
                }
                res=poly(ans);
            }
            res*=a.pow(k);

            vector<mint> ans(n+1);
            for(int i=0;i<res.size()&&i+shift<n+1;i++)ans[i+shift]=res[i];
            return poly(ans);
        }

        static poly product(vector<poly> fs){
            struct cmp{
                bool operator()(const poly&a,const poly&b) const{
                    return a.degree()>b.degree();
                }
            };

            priority_queue<poly,vector<poly>,cmp> pq;
            for(auto&f:fs)
                pq.push(move(f));
            while(pq.size()>1){
                poly a=pq.top();
                pq.pop();
                poly b=pq.top();
                pq.pop();
                pq.push(a*b);
            }
            return pq.empty()?poly(mint(1)):pq.top();
        }

        // return f(a*r^i), i:[0,m]
        vector<mint> geometric_evaluation(mint const a,mint const r,int const m) const{
            assert(m>=0);
            int const n=size();
            vector<mint> res(m+1);
            if(n==0) return res;
            if(r==mint(0)){
                mint f_a=0;
                for(int i=n-1;i>=0;i--)f_a=f_a*a+(*this)[i];
                res[0]=f_a;
                for(int i=1;i<=m;i++)res[i]=(*this)[0];
                return res;
            }
            mint const r_inv=r.inv();
            int const L=n+m+1;
            vector<mint> pw(L),ipw(L); // pw[k] = r^{C(k,2)}, ipw[k] = r^{−C(k,2)}
            pw[0]=ipw[0]=1;
            mint rp=1,rip=1; // r^{k}, r^{−k}
            for(int k=1;k<L;k++)pw[k]=pw[k-1]*rp,ipw[k]=ipw[k-1]*rip,rp*=r,rip*=r_inv;
            poly u=poly::zero(n);
            poly v=poly::zero(L);
            mint ap=1;
            for(int j=0;j<n;j++)u[n-1-j]=(*this)[j]*ap*ipw[j],ap*=a;
            for(int k=0;k<L;k++)v[k]=pw[k];
            poly w=u*v;
            for(int i=0;i<=m;i++)
                res[i]=w[n-1+i]*ipw[i];
            return res;
        }

        static poly geometric_interpolation(const vector<mint>&y,const mint a,const mint r){
            int const n=static_cast<int>(y.size());
            if(n==0)return poly(mint(0));
            if(n==1)return poly(y[0]);

            if(r==mint(0)){ return poly(y[1],(y[0]-y[1])/a); }
            assert(a!=mint(0));

            vector<mint> pow_r(n+1);
            vector<mint> pow_invr(n+1);
            vector<mint> inv_s(n);

            mint s=1;
            mint const inv_r=r.inv();

            pow_r[0]=1;
            pow_invr[0]=1;
            inv_s[0]=1;

            for(int i=0;i<n;i++){
                if(i>0){ s*=mint(1)-pow_r[i]; }
                pow_r[i+1]=pow_r[i]*r;
                pow_invr[i+1]=pow_invr[i]*inv_r;
            }

            inv_s[n-1]=s.inv();
            for(int i=n-2;i>=0;i--){ inv_s[i]=inv_s[i+1]*(mint(1)-pow_r[i+1]); }
            s*=mint(1)-pow_r[n];

            vector<mint> yy=y;
            mint v=1;

            for(int i=0;i<n;i++){
                yy[i]*=v;
                yy[i]*=inv_s[i];
                yy[i]*=inv_s[n-i-1];
                if(i&1)yy[i]=-yy[i];

                if(i!=n-1){ v*=pow_invr[n-2-i]; }
            }

            vector<mint> f1(n+1);
            v=1;

            for(int i=0;i<=n;i++){
                if(i==0||i==n){ f1[i]=v; } else{ f1[i]=v*s*inv_s[i]*inv_s[n-i]; }
                if(i&1)f1[i]=-f1[i];
                v*=pow_r[i];
            }

            poly const g(yy);
            vector<mint> const f2=g.geometric_evaluation(mint(1),r,n-1);

            poly const F1(f1);
            poly const F2(f2);
            poly res=F1*F2;
            res.truncate(n-1);

            vector<mint> coeff=res.coeff();
            coeff.resize(n,mint(0));
            reverse(coeff.begin(),coeff.end());
            mint const inv_a=a.inv();
            v=1;

            for(int i=0;i<n;i++){
                coeff[i]*=v;
                v*=inv_a;
            }
            return poly(coeff);
        }

        void debug(int const l,int const r,bool const line_break=false) const{
            int const step=(l<=r?1:-1);
            for(int i=l;;i+=step){
                cout<<(*this)[i];
                if(i==r) break;
                cout<<(line_break?'\n':' ');
            }
            cout<<'\n';
        }

        void debug(int n=-1,bool const line_break=false) const{
            if(n<-1){
                cout<<'\n';
                return;
            }
            if(n==-1){
                if(func.empty()){
                    cout<<0<<'\n';
                    return;
                }
                n=degree();
            }
            for(int i=0;i<=n;i++){
                cout<<(*this)[i];
                if(i!=n) cout<<(line_break?'\n':' ');
            }
            cout<<'\n';
        }

        //
        vector<mint> multipoint_evaluation(const vector<mint>&x_vals) const;

        //
};

poly x=poly(0,1);
poly x2=poly(0,0,1);

/*

    poly p=poly::zero(n); // blank poly (until n-th degree, n+1 terms)
    for(int i=0;i<=n;i++)cin>>p[i]

*/
int main(){
    int n;
    long m;
    cin>>n>>m;
    poly p=poly::one();
    for(int i=0;i<n;i++){
        mint a;
        cin>>a;
        poly q=poly::one(a);
        q[a]=-1;
        p*=q;
    }
    p*=(1-x);
    cout<<poly::one().bostan_mori(p,m)<<endl;
    return 0;
}
