//
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
template<class T,std::enable_if_t<std::is_integral_v<T>,int> =0> mint modpow(mint const a,T const n){ return a.pow(n); }

struct T{
    mint a;
    mint b;

    T(mint const a=0,mint const b=0) : a(a), b(b){}
    static constexpr T e(){ return {1,0}; }
};

struct M{
    using T=::T;

    // merge 2 segments
    // ta(left),tb(right)
    // change depending on what to compute (sum,min,xor...)
    static T f(const T&ta,const T&tb){return{ta.a*tb.a,tb.a*ta.b+tb.b};}
    static T e(){ return T::e(); }
};

template<typename M>
struct segtree{ // dynamic, multiple element monoid
    private:
        using T=typename M::T;

        struct node{
            T val=M::e();
            node*l=nullptr;
            node*r=nullptr;
        };

        long len;
        node*root=nullptr;

        void set(node*&t,long const l,long const r,long const p,T x){
            if(!t) t=new node();
            if(r-l==1){
                t->val=x;
                return;
            }

            long const mid=l+(r-l)/2;
            if(p<mid) set(t->l,l,mid,p,x);
            else set(t->r,mid,r,p,x);
            T lv=t->l?t->l->val:M::e();
            T rv=t->r?t->r->val:M::e();
            t->val=M::f(lv,rv);
        }

        void update(long const p,T x){
            set(root,0,len,p,x);
        }

        T calc(node*t,long const l,long const r,long const a,long const b) const{
            if(!t||r<=a||b<=l) return M::e();
            if(a<=l&&r<=b) return t->val;
            long const mid=l+(r-l)/2;
            return M::f(
                calc(t->l,l,mid,a,b),
                calc(t->r,mid,r,a,b)
            );
        }

        struct update_proxy{
            segtree&st;
            long pos;

            update_proxy(segtree&st_,long const p_)
                : st(st_), pos(p_){}

            update_proxy&operator=(T const x){
                st.update(pos,x);
                return *this;
            }

            explicit operator T() const{
                return st.get(pos);
            }
        };

        template<typename F>
        long find_right(node*t,long const l,long const r,long const p,T&sm,F&cmp) const{
            if(r<=p) return len;
            if(p<=l){
                T v=t?t->val:M::e();
                if(cmp(M::f(sm,v))){
                    sm=M::f(sm,v);
                    return len;
                }
                if(r-l==1) return l;
            }
            long const mid=l+(r-l)/2;
            long const res=find_right(t?t->l:nullptr,l,mid,p,sm,cmp);
            if(res!=len) return res;
            return find_right(t?t->r:nullptr,mid,r,p,sm,cmp);
        }

        template<typename F>
        long find_left(node*t,long const l,long const r,long const p,T&sm,F&cmp) const{
            if(p<=l) return -1;
            if(r<=p){
                T v=t?t->val:M::e();
                if(cmp(M::f(v,sm))){
                    sm=M::f(v,sm);
                    return -1;
                }
                if(r-l==1) return l;
            }
            long const mid=l+(r-l)/2;
            long const res=find_left(t?t->r:nullptr,mid,r,p,sm,cmp);
            if(res!=-1) return res;
            return find_left(t?t->l:nullptr,l,mid,p,sm,cmp);
        }

        void debug(node*t,long const l,long const r) const{
            if(!t) return;
            if(r-l==1){
                cout<<l<<" "<<t->val<<"\n";
                return;
            }
            long const mid=l+(r-l)/2;
            debug(t->l,l,mid);
            debug(t->r,mid,r);
        }

    public:
        explicit segtree(long const n) : len(n){}

        update_proxy update(long const p){
            return update_proxy(*this,p);
        }

        T calc(long const l,long const r) const{
            if(l>r) return M::e();
            return calc(root,0,len,l,r+1);
        }

        T get(node*t,long const l,long const r,long const p) const{
            if(!t) return M::e();
            if(r-l==1) return t->val;
            long const mid=l+(r-l)/2;
            if(p<mid) return get(t->l,l,mid,p);
            return get(t->r,mid,r,p);
        }

        T get(long const p) const{
            if(p<0||p>=len) return M::e();
            return get(root,0,len,p);
        }

        T operator()(long const l,long const r) const{
            return calc(l,r);
        }

        T operator()(long const p) const{
            return get(p);
        }

        T all() const{
            if(len==0)return M::e();
            return calc(0,len-1);
        }

        T all_except(long const pos) const{
            if(len==0) return M::e();
            T left=(pos>0)?calc(0,pos-1):M::e();
            T right=(pos<len-1)?calc(pos+1,len-1):M::e();
            return M::f(left,right);
        }

        template<typename F>
        long find_right(long const p,F cmp) const{
            if(p>=len) return len;
            T sm=M::e();
            return find_right(root,0,len,p,sm,cmp);
        }

        template<typename F>
        long find_left(long const p,F cmp) const{
            if(p<=0) return -1;
            T sm=M::e();
            return find_left(root,0,len,p,sm,cmp);
        }

        void debug() const{
            if(len==0) return;
            debug(root,0,len);
        }
};

/*  template


*/

int main(){
    int n,q;
    cin>>n>>q;
    segtree<M> s(n);

    while(q--){
        int op;
        cin>>op;
        if(op==0){
            int p;
            mint c,d;
            cin>>p>>c>>d;
            s.update(p)={c,d};
        } else{
            int l,r;
            mint x;
            cin>>l>>r>>x;
            auto [A,B]=s(l,r-1);
            //cout<<A<<" "<<B<<endl;
            cout<<A*x+B<<endl;
        }
    }
    return 0;
}
