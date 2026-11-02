//
#include <bits/stdc++.h>
using namespace std;
long inf=1e18;

struct Sum{
    using T=long;
    static T f(const T&a,const T&b){ return a+b; }
    static T e(){ return 0; }
};

template<typename M>
struct segtree{ // dynamic, single element monoid
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

            update_proxy&operator+=(T const delta){
                st.update(pos,st.get(pos)+delta);
                return *this;
            }

            update_proxy&operator-=(T const delta){
                st.update(pos,st.get(pos)-delta);
                return *this;
            }

            update_proxy&operator++(){
                st.update(pos,st.get(pos)+1);
                return *this;
            }

            T operator++(int){
                T old=st.get(pos);
                ++(*this);
                return old;
            }

            update_proxy&operator--(){
                st.update(pos,st.get(pos)-1);
                return *this;
            }

            T operator--(int){
                T old=st.get(pos);
                --(*this);
                return old;
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
    // ABC482C
    long h,w,n,q;
    cin>>h>>w>>n>>q;
    segtree<Sum> sh(h),sw(w);
    for(int i=0;i<n;i++){
        long r,c;
        cin>>r>>c;
        r--;
        c--;
        sh.update(r)++;
        sw.update(c)++;
    }
    for(int i=0;i<q;i++){
        int op;
        long x,y;
        cin>>op>>x>>y;
        x--;
        y--;
        if(op==1)cout<<sh(x,y)<<endl;
        else cout<<sw(x,y)<<endl;
    }
}
