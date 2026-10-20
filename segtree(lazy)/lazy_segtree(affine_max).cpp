#include <bits/stdc++.h>
using namespace std;
constexpr long inf=1L<<60;

// mul>=0 only

struct T{
    long val;

    T(long const val_=0): val(val_){}
};

struct F{
    long mul; // mul>=0 only
    long add;

    static constexpr F id(){return {1,0};}

    bool operator==(F const&other) const{
        return mul==other.mul&&add==other.add;
    }

    bool operator!=(F const&other) const{
        return !(*this==other);
    }
};

struct Max{
    using T=::T;
    using F=::F;

    // merge 2 segments
    // ta(left),tb(right)
    // change depending on what to compute (sum,min,xor...)
    static T f(const T&ta,const T&tb){ return {max(ta.val,tb.val)}; }
    static T e(){ return -inf; }

    // apply operation f to segments, f(t)
    static T ap(const F&ff,const T&tt){
        return {ff.mul*tt.val+ff.add};
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

struct Min{
    using T=::T;
    using F=::F;

    // merge 2 segments
    // ta(left),tb(right)
    // change depending on what to compute (sum,min,xor...)
    static T f(const T&ta,const T&tb){ return {min(ta.val,tb.val)}; }
    static T e(){ return inf; }

    // apply operation f to segments, f(t)
    static T ap(const F&ff,const T&tt){
        return {ff.mul*tt.val+ff.add};
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

    long t;
    auto cond=[&](long val){ return val>=t; };
    int i_right=s.find_right(p,cond);
    int i_left =s.find_left(p,cond);

    s.update_range(l,r)(mul,add);
    for debug (xxxxx->component of T):
    s.debug([](const T& t){return to_string(static_cast<long>(t.xxxxx));});

// mul>=0 only
*/

int main(){

    long n,q;
    cin>>n>>q;
    vector<T>v(n);
    for(int i=0;i<n;i++)cin>>v[i].val;
    segtree<Min>s(v);

    while(q--){
        int op;
        cin>>op;
        if(op==1){
            int l,r,x;
            cin>>l>>r>>x;
            l--; // 0-index
            r--;
            s.update_range(l,r)(1,x);
        } else {
            int l,r;
            cin>>l>>r;
            l--;
            r--; // 0-index
            cout<<s(l,r).val<<endl;
        }
    }

}
