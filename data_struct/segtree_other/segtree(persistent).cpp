#include <bits/stdc++.h>
using namespace std;

struct M{
    using T=long;
    static T f(const T&a,const T&b){ return a+b; }
    static T e(){ return 0; }
};

template<typename M>
struct segtree{
    private:
        using T=typename M::T;

        struct node{
            T val;
            int l;
            int r;

            node(T val_=M::e(),int const l_=-1,int const r_=-1)
                : val(val_), l(l_), r(r_){}
        };

        vector<node> nodes;
        vector<int> version;
        int len;

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
            nodes.emplace_back(M::f(nodes[lc].val,nodes[rc].val),lc,rc);
            return sz;
        }

        T get_node(int const root,int const l,int const r,int const pos) const{
            if(l==r) return nodes[root].val;
            int const mid=(l+r)/2;
            if(pos<=mid)return get_node(nodes[root].l,l,mid,pos);
            return get_node(nodes[root].r,mid+1,r,pos);
        }

        struct update_proxy{
            segtree&st;
            int ver;
            int pos;

            update_proxy(
                segtree&st_,
                int const ver_,
                int const pos_
            ): st(st_), ver(ver_), pos(pos_){}

            update_proxy&operator=(T const value){
                st.version[ver]=st.update_node(st.version[ver],0,st.len-1,pos,value);
                return *this;
            }

            update_proxy&operator+=(T const delta){
                T const value=st.get_node(st.version[ver],0,st.len-1,pos);
                *this=value+delta;
                return *this;
            }

            update_proxy&operator-=(T const delta){
                T const value=st.get_node(st.version[ver],0,st.len-1,pos);
                *this=value-delta;
                return *this;
            }
        };

        int update_node(int node,int const l,int const r,int const pos,T x){
            if(l==r){
                int const id=nodes.size();
                nodes.emplace_back(x);
                return id;
            }

            int const mid=(l+r)/2;
            int lc=nodes[node].l;
            int rc=nodes[node].r;

            if(pos<=mid)lc=update_node(lc,l,mid,pos,x);
            else rc=update_node(rc,mid+1,r,pos,x);

            int const id=nodes.size();
            nodes.emplace_back(M::f(nodes[lc].val,nodes[rc].val),lc,rc);
            return id;
        }

        T range_calc(int const root,int const l,int const r) const{
            T ans=M::e();
            auto dfs=[&](auto&&self,int node,int L,int R)->void{
                if(R<l||r<L) return;
                if(l<=L&&R<=r){
                    ans=M::f(ans,nodes[node].val);
                    return;
                }
                int const mid=(L+R)/2;
                self(self,nodes[node].l,L,mid);
                self(self,nodes[node].r,mid+1,R);
            };
            dfs(dfs,root,0,len-1);
            return ans;
        }

    public:
        segtree(vector<T> const&a,int const ver): version(ver), len(a.size()){
            nodes.reserve(1+2*len);
            int const initial_root=build(a,0,len-1);
            for(int i=0;i<ver;i++){version[i]=initial_root;}
        }

        void copy(int const from,int const to){ version[to]=version[from]; }
        T get(int const ver,int const pos) const{ return get_node(version[ver],0,len-1,pos); }

        update_proxy update(int const ver,int const pos){
            return update_proxy(*this,ver,pos);
        }

        T calc(int const ver,int const l,int const r) const{
            return range_calc(version[ver],l,r);
        }

        T operator()(int const ver,int const pos) const{ return get(ver,pos); }
        T operator()(int const ver,int const l,int const r) const{ return calc(ver,l,r); }
};

/*  template



*/

int main(){
    // ABC453 E
    int n,m,q;
    cin>>n>>m>>q;
    vector<long>v(m);
    segtree<M> st(v,n);

    while(q--){
        int op;
        cin>>op;

        if(op==1){
            int x,y;
            cin>>x>>y;
            x--;
            y--;
            st.copy(y,x);
        } else if(op==2){
            int x,y;
            long z;
            cin>>x>>y>>z;
            x--;
            y--;
            st.update(x,y)=z;
        } else{
            int x,l,r;
            cin>>x>>l>>r;
            x--;
            l--;
            r--;
            cout<<st(x,l,r)<<endl;
        }
    }
}
