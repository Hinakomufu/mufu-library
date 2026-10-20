#include <bits/stdc++.h>
using namespace std;
long inf=1e18;

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
mint modpow(mint a,long n){ return a.pow(n); }

struct M{
    using T=mint;
    static T f(const T&a,const T&b){ return a+b; }
    static T e(){ return 0; }
};

template<typename M>
struct segtree{ // single element monoid
    private:
        using T=typename M::T;
        int len;
        int size;
        vector<T> val;

        T get(int const pos) const{
            return val[size+pos];
        }

    public:
        // clang-format off
        segtree():len(0),size(1),val(2,M::e()){}
        segtree(segtree&&)=default;
        segtree&operator=(segtree&&)=default;
        segtree(const segtree&)=default;
        segtree&operator=(const segtree&)=default;
        // clang-format on

        explicit segtree(vector<T> const&arr){
            len=static_cast<int>(arr.size());
            size=1;
            while(len>size) size<<=1;
            val.assign(2*size,M::e());

            for(int i=0;i<len;i++){
                val[size+i]=arr[i];
            }
            for(int i=len;i<size;i++){
                val[size+i]=M::e();
            }
            for(int j=size-1;j>0;j--){
                val[j]=M::f(val[2*j],val[2*j+1]);
            }
        }

        explicit segtree(int n) : segtree(vector<T>(n,M::e())){}

        int seg_len() const{ return len; }
        int seg_size() const{ return size; }

        struct update_proxy{
            segtree&st;
            int pos;

            update_proxy(segtree&st_,int const p_) : st(st_), pos(p_){}

            update_proxy&operator=(T const x){
                int p=pos+st.size;
                st.val[p]=x;
                for(p>>=1;p;p>>=1){
                    st.val[p]=M::f(st.val[p<<1],st.val[p<<1|1]);
                }

                return *this;
            }

            update_proxy&operator++(){
                int p=pos+st.size;
                st.val[p]+=1;
                for(p>>=1;p;p>>=1){
                    st.val[p]=M::f(st.val[p<<1],st.val[p<<1|1]);
                }

                return *this;
            }

            T operator++(int){
                T old=st.get(pos);
                ++(*this);
                return old;
            }

            update_proxy&operator+=(T const delta){
                int p=pos+st.size;
                st.val[p]+=delta;
                for(p>>=1;p;p>>=1){
                    st.val[p]=M::f(st.val[p<<1],st.val[p<<1|1]);
                }

                return *this;
            }

            update_proxy&operator--(){
                int p=pos+st.size;
                st.val[p]-=1;
                for(p>>=1;p;p>>=1){
                    st.val[p]=M::f(st.val[p<<1],st.val[p<<1|1]);
                }

                return *this;
            }

            T operator--(int){
                T old=st.get(pos);
                --(*this);
                return old;
            }

            update_proxy&operator-=(T const delta){
                int p=pos+st.size;
                st.val[p]-=delta;
                for(p>>=1;p;p>>=1){
                    st.val[p]=M::f(st.val[p<<1],st.val[p<<1|1]);
                }
                return *this;
            }

            explicit operator T() const{
                return st.get(pos);
            }
        };

        update_proxy update(int const pos){
            return update_proxy(*this,pos);
        }

        T operator()(int const pos) const{
            return get(pos);
        }

        T operator[](int const pos) const{
            return val[pos];
        }

        T calc(int l,int r) const{ // [l,r]
            //cout<<l<<" "<<r<<endl;
            T left=M::e();
            T right=M::e();
            l+=size;
            r+=size+1; // [l,r] -> [l,r)
            while(l<r){
                if(l&1) left=M::f(left,val[l++]);
                if(r&1) right=M::f(val[--r],right);
                l>>=1;
                r>>=1;
            }
            return M::f(left,right);
        }

        T operator()(int const l,int const r) const{
            return calc(l,r);
        }

        T all() const{
            return calc(0,len-1);
        }

        T all_except(int const pos) const{
            T left=(pos>0)?calc(0,pos-1):M::e();
            T right=(pos<len-1)?calc(pos+1,len-1):M::e();
            return M::f(left,right);
        }

        // find the smallest i,such that p<=i<len and cond(f(p,i))==true.
        // ...,p=false,...,i-1=false,[i=true],i+1,...
        // if not found, return len
        int find_right(int const p,auto cond) const{
            if (p < 0 || p >= len) return len;
            T sm=M::e();
            int i=p+size;
            while(true){
                while((i&1)==0) i>>=1;
                if(cond(M::f(sm,val[i]))) break;
                sm=M::f(sm,val[i]);
                i++;
                if((i&-i)==i) return len;
            }
            while(i<size){
                i<<=1;
                if(!cond(M::f(sm,val[i]))){
                    sm=M::f(sm,val[i]);
                    i++;
                }
            }
            return i-size;
        }

        // find the largest i,such that 0<=i<p and cond(f(i,p))==true.
        // ...,i-1,[i=true],i+1=false,...,p=false,...
        // if not found, return -1
        int find_left(int p,auto cond) const{
            if(len==0||p<0) return -1;
            if(p>=len) p=len-1;
            T sm=M::e();
            int i=p+size+1;
            while(true){
                --i;
                while(i>1&&(i&1)) i>>=1;
                if(cond(M::f(val[i],sm))) break;
                sm=M::f(val[i],sm);
                if((i&-i)==i) return -1;
            }
            while(i<size){
                i=i*2+1;
                if(!cond(M::f(val[i],sm))){
                    sm=M::f(val[i],sm);
                    --i;
                }
            }
            return i-size;
        }

        void debug() const{
            // print only the original array part (leaves)
            for(int i=0;i<len;i++){
                cout<<val[size+i]<<(i==len-1?"\n":" ");
            }
        }

        void debug(T const limit=inf) const{
            // print only the original array part (leaves)
            if(limit==inf)limit/=10;
            for(int i=0;i<len;i++){
                T v=val[size+i];
                if(abs(v)>=limit){
                    if(v>=0) cout<<"inf";
                    else cout<<"-inf";
                } else cout<<v;
                if(i+1<len) cout<<" ";
            }
            cout<<"\n";
        }
};

/*  template
    vector<long>a(n);
    segtree<m>s(a);

    s.update(p)=x;
    long tmp=s(p);
    long ans=s(l, r);
    long ans2=s.all_except(r);

    long t;
    auto cond=[&](long val){ return val>=t; };
    int i_right=s.find_right(p,cond);
    int i_left =s.find_left(p,cond);
*/

int main(){
    // ABC222D
    int n;
    cin>>n;
    vector<int> a(n),b(n);
    for(int i=0;i<n;i++)cin>>a[i];
    for(int i=0;i<n;i++)cin>>b[i];
    vector<mint> base(3001,0);
    for(int x=a[0];x<=b[0];x++) base[x]=1;
    segtree<M> seg(base);

    for(int j=1;j<n;j++){
        vector<mint> nxt(3001,0);
        for(int x=a[j];x<=b[j];x++){
            nxt[x]=seg.calc(0,x);
        }
        seg=segtree<M>(nxt);
    }

    mint ans=seg.calc(0,3000);
    cout<<ans<<endl;
    return 0;
}
