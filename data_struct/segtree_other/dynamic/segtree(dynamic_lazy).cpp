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

    constexpr modint operator-() const{
        return modint(v==0?0:mod-v);
    }

    static constexpr modint raw(int v){
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

    static constexpr T e(){ return {0,0}; }
};

struct F{
    mint mul;
    mint add;

    static constexpr F id(){ return {1,0}; }

    bool operator==(F const&other) const{
        return mul==other.mul&&add==other.add;
    }

    bool operator!=(F const&other) const{
        return !(*this==other);
    }
};

struct m{
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
struct segtree{ // dynamic, lazy operation
    public:
        using T=typename M::T;
        using F=typename M::F;

    private:
        struct node{
            T val;
            F lazy;
            node*l=nullptr;
            node*r=nullptr;
            node(long len): val{0,len}, lazy(M::id()){}
        };

        long len;
        node*root=nullptr;

        T value(node*t,long const l,long const r){
            if(t) return t->val;
            return T{0,r-l};
        }

        void apply(node*t,F const&f){
            t->val=M::ap(f,t->val);
            t->lazy=M::cmp(t->lazy,f);
        }

        void push(node*t,long const l,long const r){
            if(!t||r-l==1) return;

            long const mid=l+(r-l)/2;

            if(!t->l) t->l=new node(mid-l);
            if(!t->r) t->r=new node(r-mid);

            if(t->lazy!=M::id()){
                apply(t->l,t->lazy);
                apply(t->r,t->lazy);
                t->lazy=M::id();
            }
        }

        void pull(node*t,long const l,long const r){
            long const mid=l+(r-l)/2;
            t->val=M::f(
                value(t->l,l,mid),
                value(t->r,mid,r)
            );
        }

        void range_apply(node*&t,long const l,long const r,long const a,long const b,F const&f){
            if(r<=a||b<=l) return;
            if(!t) t=new node(r-l);
            if(a<=l&&r<=b){
                apply(t,f);
                return;
            }
            push(t,l,r);
            long const mid=l+(r-l)/2;
            range_apply(t->l,l,mid,a,b,f);
            range_apply(t->r,mid,r,a,b,f);
            pull(t,l,r);
        }

        T calc(node*t,long const l,long const r,long const a,long const b,F const&carry) const{
            if(r<=a||b<=l) return M::e();
            if(a<=l&&r<=b){
                T v=t?t->val:T{0,r-l};
                return M::ap(carry,v);
            }
            F next=carry;
            if(t) next=M::cmp(t->lazy,carry);
            long const mid=l+(r-l)/2;
            return M::f(
                calc(t?t->l:nullptr,l,mid,a,b,next),
                calc(t?t->r:nullptr,mid,r,a,b,next)
            );
        }

        struct update_proxy{
            segtree&st;
            long l,r;
            update_proxy(segtree&st_,long const l_,long const r_): st(st_), l(l_), r(r_){}

            template<class...Args>
            update_proxy&apply(Args&&...args){
                st.range_apply(st.root,0,st.len,l,r,F{std::forward<Args>(args)...});
                return *this;
            }

            template<class...Args>
            update_proxy&operator()(Args&&...args){
                return apply(std::forward<Args>(args)...);
            }
        };

        void debug(node*t,long const l,long const r,F const&carry,function<string(const T&)> proj) const{
            if(!t) return;
            if(r-l==1){
                T v=M::ap(carry,t->val);
                cout<<l<<" "<<proj(v)<<"\n";
                return;
            }
            F next=M::cmp(t->lazy,carry);
            long const mid=l+(r-l)/2;
            debug(t->l,l,mid,next,proj);
            debug(t->r,mid,r,next,proj);
        }

    public:
        explicit segtree(long const n) : len(n){}

        update_proxy update_range(long l,long const r){
            return update_proxy(*this,l,r+1);
        }

        T calc(long const l,long const r) const{
            if(l>r) return M::e();
            return calc(root,0,len,l,r+1,M::id());
        }

        T get(long const p) const{
            return calc(p,p);
        }

        T operator()(long const p) const{
            return get(p);
        }

        T operator()(long const l,long const r) const{
            return calc(l,r);
        }

        T all() const{
            if(len==0) return M::e();
            return calc(0,len-1);
        }

        T all_except(long const pos) const{
            if(len==0) return M::e();
            T left=pos>0?calc(0,pos-1):M::e();
            T right=pos<len-1?calc(pos+1,len-1):M::e();
            return M::f(left,right);
        }

        void debug(function<string(const T&)> proj) const{
            if(len==0) return;
            debug(root,0,len,M::id(),proj);
        }
};

/*  template

s.update_range(l,r)(mul,add);
for debug (xxxxx->component of T):
s.debug([](const T& t){return to_string(static_cast<long>(t.xxxxx));});

*/

int main(){
    int n,q;
    cin>>n>>q;
    segtree<m> s(n);

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
