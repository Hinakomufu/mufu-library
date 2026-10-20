#include <bits/stdc++.h>
using namespace std;
long inf=1e18;

struct M{
    using T=long;
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

// mp[val[i]]->a[i]
pair<vector<long>,vector<long>> // compressed values,reverse map
    compress(const vector<long>&a){
    int const n=a.size();
    vector<long> t=a; // copy original values
    sort(t.begin(),t.end());
    t.erase(unique(t.begin(),t.end()),t.end());

    vector<long> b(n);
    for(long i=0;i<n;i++){
        long const idx=lower_bound(t.begin(),t.end(),a[i])-t.begin();
        b[i]=idx; // compressed index (0-based)
    }

    // b: compressed values, t: reverse map
    return {b,t};
}

vector<long> // count
    direct_count(vector<long> const&v,
                 bool const right_dir=true,
                 bool const larger=true,
                 bool const strict=true){
    long const n=v.size();
    auto [val, mp]=compress(v);

    vector<long> cnt(n);
    long const mx=*max_element(val.begin(),val.end());
    vector<long> const a(mx+1,0L);
    segtree<M> s(a);

    long const start=right_dir?n-1:0;
    long const end=right_dir?-1:n;
    long const step=right_dir?-1:1;

    for(long i=start;i!=end;i+=step){
        long const x=val[i];

        if(larger){ // larger: i<j -> s[i]<s[j]
            long const lim=strict?x+1:x;
            if(0<=lim){
                cnt[i]=s(lim,mx);
            }
        } else{ // smaller: i<j -> s[i]>s[j]
            long const lim=strict?x-1:x;
            if(0<=lim){
                cnt[i]=s(0,lim);
            }
        }
        ++s.update(x);
    }
    return cnt;
}

long inversion(vector<long> const&v){
    vector<long> const u=direct_count(v,true,false,true);
    long val=0;
    for(long const x:u) val+=x;
    return val;
}

int main(){
    // ABC436_F
    int n;
    cin>>n;
    vector<long> a(n);
    for(int i=0;i<n;i++){
        cin>>a[i];
    }

    long ans=0;
    vector l=direct_count(a,false,false,true);
    vector r=direct_count(a,true,false,true);
    for(int i=0;i<n;i++){
        ans+=(l[i]+1)*(r[i]+1);
    }
    cout<<ans<<endl;

    return 0;
}
