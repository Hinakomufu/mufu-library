#include <bits/stdc++.h>
using namespace std;
long inf=1e18;

struct tour{
    int node_id;
    long node_weight; // weight of node
    long edge_weight; // weight of parent->v (root:0)
    int depth;        // dist of root->v

    tour(int const nid,long const w_node,long const w_edge,int const depth)
        : node_id(nid), node_weight(w_node), edge_weight(w_edge), depth(depth){}

};

struct graph{
    private:
        int node;
        vector<long> w_node;
        bool toured;
        vector<int> walk,nin_,nout_,ein_,eout_,parent_,depth_;
        vector<tour> euler;

        struct edge{
            int u,v;
            long w;

            edge(int const a,int const b,long const w_): w(w_){
                if(a<=b) u=a,v=b;
                else u=b,v=a;
            }

            bool operator==(edge const&other) const{
                return u==other.u&&v==other.v&&w==other.w;
            }

            bool operator<(edge const&other) const{
                if(w!=other.w) return w<other.w;
                if(u!=other.u) return u<other.u;
                return v<other.v;
            }
        };

        void dfs_walk(int nid,int const p,long w_par,int d,bool const negative_exit){
            parent_[nid]=p;
            depth_[nid]=d;
            if(p!=-1){
                euler.emplace_back(nid,w_node[nid],w_par,d);
            } else{
                euler.emplace_back(nid,w_node[nid],0,d); // root
            }
            if(nin_[nid]==-1)nin_[nid]=static_cast<int>(euler.size())-1;
            for(auto [u,w,next_eid]:gr[nid]){
                if(u==p) continue;
                ein_[next_eid]=static_cast<int>(euler.size());
                dfs_walk(u,nid,w,d+1,negative_exit);
                if(negative_exit)euler.emplace_back(nid,-w_node[u],-w,d);
                else euler.emplace_back(nid,0,0,d);
                eout_[next_eid]=static_cast<int>(euler.size())-1;
            }
            nout_[nid]=static_cast<int>(euler.size())-1;
        }

    public:
        vector<edge> edges;
        vector<vector<tuple<int,long,int>>> gr; // (to, edge_weight, edge_id)
        explicit graph(int const n): node(n), w_node(n,0), toured(false), gr(n){}
        explicit graph(int const n,long const w): node(n), w_node(n,w), toured(false), gr(n){}

        graph(int const n,const vector<long>&node_weight)
            : node(n), w_node(node_weight), toured(false), gr(n){}

        void add_edge(int u,int v,long w=0,bool const undirected=true){
            toured=false;
            int eid=edges.size();
            edges.push_back({u,v,w});
            gr[u].emplace_back(v,w,eid);
            if(undirected) gr[v].emplace_back(u,w,eid);
        }

        int edge_count(int const n) const{
            return gr[n].size();
        }

        void run_euler(int const root=0,bool const negative_exit=true){
            toured=true;

            nin_.assign(node,-1);
            nout_.assign(node,-1);
            ein_.assign(node,-1);
            eout_.assign(node,-1);
            parent_.assign(node,-1);
            depth_.assign(node,-1);
            euler.clear();
            euler.reserve(2*node);

            dfs_walk(root,-1,0,0,negative_exit);
        }

        vector<tour>&get_euler(int const root=0,bool const negative_exit=true){
            if(!toured)run_euler(root,negative_exit);
            return euler;
        }

        int nin(int const v) const{ return nin_[v]; }
        int nout(int const v) const{ return nout_[v]; }
        int ein(int const v) const{ return ein_[v]; }
        int eout(int const v) const{ return eout_[v]; }
        int depth(int const v) const{ return depth_[v]; }
        int parent(int const v) const{ return parent_[v]; }
};

struct m{
    using T=::tour;

    // merge 2 segments
    // ta(left),tb(right)
    // change depending on what to compute (sum,min,xor...)
    static T f(const T&ta,const T&tb){
        return {(ta.depth<=tb.depth?ta.node_id:tb.node_id),
                ta.node_weight+tb.node_weight,
                ta.edge_weight+tb.edge_weight,
                min(ta.depth,tb.depth)
                };
    }

    static T e(){ return {-1,0,0,static_cast<int>(1e9)}; }
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
    segtree<m> s(g.get_euler(0,true));

    s.update(g.ein(x))->edge_weight=y;
    s.update(g.eout(x))->edge_weight=-y;

    int l=g.nin(x),r=g.nin(y);
    if(l>r) swap(l,r);
    int lca=s.calc(l,r).node_id;

    for debug (xxxxx->component of T):
    s.debug([](const T& t){return to_string(static_cast<long>(t.xxxxx));});
*/

int main(){
    // ABC294G
    long n;
    cin>>n;
    graph g(n);
    for(long i=0;i<n-1;i++){
        long u,v,w;
        cin>>u>>v>>w;
        u--,v--;
        g.add_edge(u,v,w);
    }

    segtree<m> s(g.get_euler());

    auto dist=[&](long x,long y){
        int l=g.nin(x),r=g.nin(y);
        if(l>r) swap(l,r);
        int lca=s(l,r).node_id;
        return s(0,g.nin(x)).edge_weight+s(0,g.nin(y)).edge_weight-2*s(0,g.nin(lca)).edge_weight;
    };
    int q;
    cin>>q;
    while(q--){
        int op;
        long x,y;
        cin>>op>>x>>y;
        if(op==1){
            x--;
            s.update(g.ein(x))->edge_weight=y;
            s.update(g.eout(x))->edge_weight=-y;
            //s.debug([](const tour&t){ return to_string(t.edge_weight); });
        } else{
            cout<<dist(x-1,y-1)<<endl;
        }
    }
    return 0;
}
