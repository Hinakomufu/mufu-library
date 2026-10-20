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

struct T{
    mint val;
    long len;

    T(long const val_=0,long const len_=0): val(val_),len(len_) {}
    static constexpr T e(){return {0,0};}
};

struct F{
    mint mul;
    mint add;

    static constexpr F id(){return {1,0};}

    bool operator==(F const&other) const{
        return mul==other.mul&&add==other.add;
    }

    bool operator!=(F const&other) const{return !(*this==other);}
};

struct M{
    using T=::T;
    using F=::F;

    // merge 2 segments
    // ta(left),tb(right)
    // change depending on what to compute (sum,min,xor...)
    static T f(const T&ta,const T&tb){ return {ta.val+tb.val,ta.len+tb.len}; }
    static T e(){ return T::e(); }

    // apply operation f to segments, f(t)
    static T ap(const F&ff,const T&tt){
        return {ff.mul*tt.val+ff.add*tt.len,tt.len};
    }

    // y=f(x) -> z=g(y), z=g(f(x))
    // left first, right next
    static F cmp(const F&ff,const F&fg){
        return {
        fg.mul*ff.mul,
        fg.mul*ff.add+fg.add
        };
    }

    static F id(){ return F::id(); }
};

template<typename M>
struct segtree{ // lazy operation
    public:
        using T=typename M::T;
        using F=typename M::F;

        explicit segtree(vector<T> const&arr){
            len=static_cast<int>(arr.size());
            size=1;
            while(len>size) size<<=1;
            value.assign(2*size,M::e());
            lazy.assign(2*size,M::id());

            for(int i=0;i<len;i++){
                value[size+i]=arr[i];
            }
            for(int i=len;i<size;i++){
                value[size+i]=M::e();
            }
            for(int j=size-1;j>0;j--){
                value[j]=M::f(value[2*j],value[2*j+1]);
            }
        }


        struct range_proxy{
            segtree&st;
            int l,r;

            range_proxy(segtree&st_,int const l_,int const r_)
                : st(st_), l(l_), r(r_){}

            template<class...Args>
            range_proxy&apply(Args&&...args){
                st.range_apply(l,r,typename M::F{std::forward<Args>(args)...});
                return *this;
            }

            template<class...Args>
            range_proxy&operator()(Args&&...args){
                st.range_apply(l,r,typename M::F{std::forward<Args>(args)...});
                return *this;
            }
        };

        range_proxy update_range(int l,int r){
            return range_proxy(*this,l,r);
        }

        T get(int const pos){
            int k=pos+size;
            vector<int> path;
            for(int x=k;x>1;x>>=1) path.push_back(x>>1);
            reverse(path.begin(),path.end());
            for(int const p:path) push(p);

            return value[k];
        }

        T operator()(int const pos){
            return get(pos);
        }

        T calc(int l,int r){
            auto query=[&](auto&&self,int a,int b,int k,int L,int R) ->T{
                if(R<a||b<L) return M::e();
                if(a<=L&&R<=b) return value[k];
                push(k);
                int mid=(L+R)/2;
                return M::f(
                    self(self,a,b,k*2,L,mid),
                    self(self,a,b,k*2+1,mid+1,R)
                );
            };
            return query(query,l,r,1,0,size-1);
        }

        T operator()(int const l, int const r){
            return calc(l, r);
        }

        T all(){
            return calc(0,len-1);
        }

        T all_except(int const pos){
            T left=(pos>0)?calc(0,pos-1):M::e();
            T right=(pos<len-1)?calc(pos+1,len-1):M::e();
            return M::f(left,right);
        }

        void debug(function<string(const T&)> proj){
            // apply all pending lazy values to leaves
            push_all(1);
            // print only the original array part (leaves)
            for(int i=0;i<len;i++){
                cout<<proj(value[size+i])<<(i+1==len?"\n":" ");
            }
        }

    private:
        int len;
        int size;
        vector<T> value;
        vector<F> lazy;

        void range_apply(int const l,int const r,typename M::F f){
            auto dfs=[&](auto&&self,int k,int L,int R)->void{
                if(R<l||r<L) return;
                if(l<=L&&R<=r){
                    value[k]=M::ap(f,value[k]);
                    lazy[k]=M::cmp(lazy[k],f);
                    return;
                }
                push(k);
                int mid=(L+R)/2;
                self(self,k*2,L,mid);
                self(self,k*2+1,mid+1,R);
                value[k]=M::f(value[k*2],value[k*2+1]);
            };
            dfs(dfs,1,0,size-1);
        }

        void push(int k){
            if(lazy[k]!=M::id()){
                value[k<<1]=M::ap(lazy[k],value[k<<1]);
                value[k<<1|1]=M::ap(lazy[k],value[k<<1|1]);
                lazy[k<<1]=M::cmp(lazy[k<<1],lazy[k]);
                lazy[k<<1|1]=M::cmp(lazy[k<<1|1],lazy[k]);
                lazy[k]=M::id();
            }
        }

        void push_all(int const k){
            if(k>=size) return;
            push(k);
            push_all(k*2);
            push_all(k*2+1);
        }

};

/*  template

s.update_range(l,r)(mul,add);
for debug (xxxxx->component of T):
s.debug([](const T& t){return to_string(static_cast<long>(t.xxxxx));});

*/

int main(){
    // ALPC_K
    int n,q;
    cin>>n>>q;
    vector<T> v(n);
    for(int i=0;i<n;i++){
        long a;
        cin>>a;
        v[i]={a,1};
    }
    segtree<M> s(v);

    for(int i=0;i<q;i++){
        int op;
        cin>>op;
        if(op==0){
            int l,r,b,c;
            cin>>l>>r>>b>>c;
            s.update_range(l,r-1)(b,c);
        } else{
            int l,r;
            cin>>l>>r;
            cout<<s(l,r-1).val<<endl;
        }
        //s.debug([](const T& t){return to_string(static_cast<long>(t.val));});
    }
    return 0;
}
