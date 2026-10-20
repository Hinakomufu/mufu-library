#include <bits/stdc++.h>
using namespace std;
long inf=1e18;

struct T{
    long a;
    long b;

    T() : a(0), b(0){}
    T(long const x) : a(x), b(x){}
    static constexpr T e(){ return T(); }
};

struct M{
    using T=::T;

    // merge 2 segments
    // ta(left),tb(right)
    // change depending on what to compute (sum,min,xor...)
    static T f(const T&ta,const T&tb){
        T t;
        t.a=ta.a;
        t.b=tb.b;
        return t;
    }

    static T e(){ return T::e(); }
};

template<typename M>
struct segtree{ // multi element monoid
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

        explicit segtree(int n): segtree(vector<T>(n,M::e())){}

        int seg_len() const{ return len; }
        int seg_size() const{ return size; }

        struct update_proxy{
            segtree&st;
            int pos;
            T&ref;

            update_proxy(segtree&st_,int p_)
                : st(st_), pos(p_), ref(st_.val[st_.size+p_]){}

            ~update_proxy(){
                int p=pos+st.size;
                for(p>>=1;p;p>>=1){
                    st.val[p]=M::f(st.val[p<<1],st.val[p<<1|1]);
                }
            }

            operator T&(){ return ref; }
            operator const T&() const{ return ref; }
            T*operator->(){ return &ref; }

            update_proxy&operator=(T const&x){
                ref=x;
                return *this;
            }

            update_proxy&apply(T const&delta){
                ref=M::f(ref,delta);
                return *this;
            }

            template<typename...Args>
            update_proxy&operator()(Args&&...args){
                ref=M::f(ref,T{std::forward<Args>(args)...});
                return *this;
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
            if(p<0||p>=len) return len;
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

        void debug(function<string(const T&)> proj) const{
            for(int i=0;i<len;i++){
                cout<<proj(val[size+i])<<(i+1<len?" ":"\n");
            }
        }
};

/*  template

    s.update(x)(1,1);

    long t;
    auto cond=[&](long val){ return val>=t; };
    int i_right=s.find_right(p,cond);
    int i_left =s.find_left(p,cond);

    for debug (xxxxx->component of T):
    s.debug([](const T& t){return to_string(static_cast<long>(t.xxxxx));});
*/

int main(){
    int n,q;
    cin>>n>>q;
    vector<T> v(n);
    for(int i=0;i<n;i++){
        int x;
        cin>>x;
        v[i].a=1;
    }
    segtree<M> s(v);
    while(q--){
        int op;
        cin>>op;
        if(op==1){
            int i,x;
            cin>>i>>x;
            i--;
            s.update(i)=T(x);
        } else{
            int l,r;
            cin>>l>>r;
            l--;
            r--;

            cout<<s(l,r).a<<endl;
        }
    }
    return 0;
}
