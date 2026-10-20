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

    T(long const val_=0,long const len_=1): val(val_), len(len_){}
    static constexpr T e(){ return {0,0}; }
};

struct F{
    mint mul;
    mint add;

    static constexpr F id(){ return {1,0}; }

    bool operator==(F const&other) const{
        return mul==other.mul&&add==other.add;
    }

    bool operator!=(F const&other) const{ return !(*this==other); }
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
struct segtree{
    private:
        using T=typename M::T;
        using F=typename M::F;

        struct node{
            T val;
            F lazy;
            int l;
            int r;

            node(
                T val_=M::e(),
                F lazy_=M::id(),
                int const l_=-1,
                int const r_=-1
            ): val(val_), lazy(lazy_), l(l_), r(r_){}
        };

        vector<node> nodes;
        vector<int> version;
        int len;
        int initial_root;

        int root(int const ver) const{ return ver==-1?initial_root:version[ver]; }

        int build(vector<T> const&a,int const l,int const r){
            if(l==r){
                int const sz=nodes.size();
                nodes.emplace_back(a[l]);
                return sz;
            }
            int const mid=(l+r)/2;
            int lc=build(a,l,mid);
            int rc=build(a,mid+1,r);
            int const sz=nodes.size();
            nodes.emplace_back(M::f(nodes[lc].val,nodes[rc].val),M::id(),lc,rc);
            return sz;
        }

        T get_node(int const root,int const l,int const r,int const pos,F const&lazy) const{
            if(l==r) return M::ap(lazy,nodes[root].val);
            int const mid=(l+r)/2;
            F const next_lazy=M::cmp(lazy,nodes[root].lazy);
            if(pos<=mid)return get_node(nodes[root].l,l,mid,pos,next_lazy);
            return get_node(nodes[root].r,mid+1,r,pos,next_lazy);
        }

        int apply_node(int node,F const&f){
            int const id=nodes.size();
            nodes.emplace_back(
                M::ap(f,nodes[node].val),
                M::cmp(nodes[node].lazy,f),
                nodes[node].l,
                nodes[node].r
            );
            return id;
        }

        struct update_proxy{
            segtree&st;
            int ver;
            int l;
            int r;

            update_proxy(
                segtree&st_,
                int const ver_,
                int const l_,
                int const r_
            ): st(st_), ver(ver_), l(l_), r(r_){}

            template<class...Args>
            update_proxy&operator()(Args&&...args){
                F f{std::forward<Args>(args)...};
                st.version[ver]=st.update_node(
                    st.version[ver],
                    0,st.len-1,
                    l,r,f
                );

                return *this;
            }
        };

        int copy_node(int dst,int src,int const l,int const r,int const ql,int const qr){
            if(r<ql||qr<l) return dst;
            if(ql<=l&&r<=qr) return src;
            int const mid=(l+r)/2;
            int dlc=nodes[dst].l;
            int drc=nodes[dst].r;
            if(nodes[dst].lazy!=M::id()){
                dlc=apply_node(dlc,nodes[dst].lazy);
                drc=apply_node(drc,nodes[dst].lazy);
            }
            int slc=nodes[src].l;
            int src_r=nodes[src].r;
            if(nodes[src].lazy!=M::id()){
                slc=apply_node(slc,nodes[src].lazy);
                src_r=apply_node(src_r,nodes[src].lazy);
            }
            int new_lc=copy_node(
                dlc,slc,
                l,mid,
                ql,qr
            );
            int new_rc=copy_node(
                drc,src_r,
                mid+1,r,
                ql,qr
            );
            int const sz=nodes.size();
            nodes.emplace_back(
                M::f(nodes[new_lc].val,nodes[new_rc].val),
                M::id(),
                new_lc,new_rc
            );
            return sz;
        }

        int update_node(int node,int const l,int const r,int const ql,int const qr,F const&f){
            if(r<ql||qr<l){ return node; }
            if(ql<=l&&r<=qr){ return apply_node(node,f); }
            int const mid=(l+r)/2;
            int lc=nodes[node].l;
            int rc=nodes[node].r;
            if(nodes[node].lazy!=M::id()){
                lc=apply_node(lc,nodes[node].lazy);
                rc=apply_node(rc,nodes[node].lazy);
            }
            int const new_lc=update_node(lc,l,mid,ql,qr,f);
            int const new_rc=update_node(rc,mid+1,r,ql,qr,f);
            int const id=nodes.size();
            nodes.emplace_back(
                M::f(nodes[new_lc].val,nodes[new_rc].val),
                M::id(),
                new_lc,
                new_rc
            );
            return id;
        }

        T range_calc(int const root,int const l,int const r) const{
            auto dfs=[&](auto&&self,int node,int L,int R,F const&lazy)->T{
                if(R<l||r<L) return M::e();
                if(l<=L&&R<=r)return M::ap(lazy,nodes[node].val);
                int const mid=(L+R)/2;
                F const next_lazy=M::cmp(lazy,nodes[node].lazy);
                return M::f(
                    self(self,nodes[node].l,L,mid,next_lazy),
                    self(self,nodes[node].r,mid+1,R,next_lazy)
                );
            };

            return dfs(dfs,root,0,len-1,M::id());
        }

    public:
        segtree(vector<T> const&a,int const ver): version(ver), len(a.size()){
            nodes.reserve(1+2*len);
            initial_root=build(a,0,len-1);
            for(int i=0;i<ver;i++){ version[i]=initial_root; }
        }

        int initial() const{return initial_root;}
        void copy(int const from,int const to){ version[to]=root(from); }

        void copy_range(int const from,int const to,int const l,int const r){
            version[to]=copy_node(version[to],root(from),0,len-1,l,r);
        }

        T get(int const ver,int const pos) const{ return get_node(root(ver),0,len-1,pos,M::id()); }

        update_proxy update_range(int const ver,int const l,int const r){
            return update_proxy(*this,ver,l,r);
        }

        T calc(int const ver,int const l,int const r) const{ return range_calc(root(ver),l,r); }

        T operator()(int const ver,int const pos) const{ return get(ver,pos); }
        T operator()(int const ver,int const l,int const r) const{ return calc(ver,l,r); }
};

/*  template



*/

int main(){
    int n,q;
    cin>>n>>q;
    vector<T> a(n,T());
    for(int i=0;i<n;i++)cin>>a[i].val;
    segtree<M> st(a,q);

    for(int i=0;i<q;i++){
        int op;
        cin>>op;
        if(op==0){
            int k,l,r;
            mint b,c;
            cin>>k>>l>>r>>b>>c;
            st.copy(k,i);
            st.update_range(i,l,r-1)(b,c);
        } else if(op==1){
            int k,s,l,r;
            cin>>k>>s>>l>>r;
            st.copy(k,i);
            st.copy_range(s,i,l,r-1);
        } else{
            int k,l,r;
            cin>>k>>l>>r;
            st.copy(k,i);
            cout<<st(k,l,r-1).val<<endl;
        }
    }
}
