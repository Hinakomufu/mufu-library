#include <bits/stdc++.h>
using namespace std;
long inf=1e18;

template<typename T> struct tree{
    private:
        // node values on tree

        vector<T> node_weight;
        vector<T> edge_weight;

        //

        int node_count;
        int edge_count;
        vector<int> parent_,depth_;
        vector<int> parent_edge_,child_edge_;
        vector<int> subtree_,heavy_;
        vector<int> head_,pos_,inv_pos_;
        int cur_pos=0;
        bool hld_built=false;
        bool node_seg_built=false;
        bool edge_seg_built=false;

        struct Type{
            T val;
            long len;

            Type(T const val_=0,long const len_=0): val(val_), len(len_){}
            static constexpr Type e(){ return {static_cast<T>(0),0}; }
        };

        struct Func{
            T mul;
            T add;

            static constexpr Func id(){ return {static_cast<T>(1),static_cast<T>(0)}; }

            bool operator==(Func const&other) const{
                return mul==other.mul&&add==other.add;
            }

            bool operator!=(Func const&other) const{ return !(*this==other); }
        };

        struct tree_monoid{
            using S=Type;
            using F=Func;

            // merge 2 segments
            // sa(left),sb(right)
            // change depending on what to compute (sum,min,xor...)
            static S f(const S&sa,const S&sb){ return {sa.val+sb.val,sa.len+sb.len}; }
            static S e(){ return S::e(); }

            // apply operation f to segments, f(t)
            static S ap(const F&ff,const S&ss){
                return {ff.mul*ss.val+ff.add*ss.len,ss.len};
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

        struct segtree{ // lazy operation
            public:
                using S=Type;
                using F=Func;
                using M=tree_monoid;

                // clang-format off
                segtree():len(0),size(1),value(2,M::e()),lazy(2,M::id()){}
                segtree(segtree&&)=default;
                segtree&operator=(segtree&&)=default;
                segtree(const segtree&)=default;
                segtree&operator=(const segtree&)=default;
                // clang-format on

                explicit segtree(vector<S> const&arr){
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

                S get(int const pos){
                    int k=pos+size;
                    vector<int> path;
                    for(int x=k;x>1;x>>=1) path.push_back(x>>1);
                    reverse(path.begin(),path.end());
                    for(int const p:path) push(p);

                    return value[k];
                }

                S operator()(int const pos){
                    return get(pos);
                }

                S calc(int l,int r){
                    auto query=[&](auto&&self,int a,int b,int k,int L,int R) ->S{
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

                S operator()(int const l,int const r){
                    return calc(l,r);
                }

                S all(){
                    return calc(0,len-1);
                }

                S all_except(int const pos){
                    S left=(pos>0)?calc(0,pos-1):M::e();
                    S right=(pos<len-1)?calc(pos+1,len-1):M::e();
                    return M::f(left,right);
                }

                void debug(function<string(const S&)> proj){
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
                vector<S> value;
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

        using M=tree_monoid;
        segtree node_seg;
        segtree edge_seg;

        void build_hld(int const root=0){
            cur_pos=0;
            depth_[root]=0;
            dfs_size(root,-1);
            dfs_hld(root,root);
            hld_built=true;
        }

        void build_node_segtree(){
            vector<Type> b(node_count);
            for(int v=0;v<node_count;++v)b[pos_[v]]=Type(node_weight[v],1);
            node_seg=segtree(b);
            node_seg_built=true;
        }

        void build_edge_segtree(){
            vector<Type> b(node_count,M::e());
            for(int v=0;v<node_count;++v){
                if(parent_edge_[v]!=-1) b[pos_[v]]=Type(edge_weight[parent_edge_[v]],1);
            }

            edge_seg=segtree(b);
            edge_seg_built=true;
        }

        void ensure_hld_built(){ if(!hld_built) build_hld(); }

        void ensure_node_seg_built(){
            ensure_hld_built();
            if(!node_seg_built) build_node_segtree();
        }

        void ensure_edge_seg_built(){
            ensure_hld_built();
            if(!edge_seg_built) build_edge_segtree();
        }

        int dfs_size(int const root,int const p){
            vector<int> order;
            order.reserve(node_count);
            vector<int> st;
            st.reserve(node_count);
            st.push_back(root);
            parent_[root]=p;
            subtree_.assign(node_count,1);
            fill(heavy_.begin(),heavy_.end(),-1);
            while(!st.empty()){
                int v=st.back();
                st.pop_back();
                order.push_back(v);
                for(auto [u, w, eid]:gr[v]){
                    if(u==parent_[v]) continue;
                    parent_[u]=v;
                    parent_edge_[u]=eid;
                    child_edge_[eid]=u;
                    depth_[u]=depth_[v]+1;
                    st.push_back(u);
                }
            }
            for(int i=static_cast<int>(order.size())-1;i>0;--i){
                int const v=order[i];
                int const par=parent_[v];
                subtree_[par]+=subtree_[v];
                if(heavy_[par]==-1||
                   subtree_[v]>subtree_[heavy_[par]]){
                    heavy_[par]=v;
                }
            }
            return subtree_[root];
        }

        void dfs_hld(int const root,int const h){
            vector<pair<int,int>> st;
            st.reserve(node_count);
            st.emplace_back(root,h);

            while(!st.empty()){
                auto [start, hd]=st.back();
                st.pop_back();
                for(int v=start;v!=-1;v=heavy_[v]){
                    head_[v]=hd;
                    inv_pos_[cur_pos]=v;
                    pos_[v]=cur_pos++;
                    for(auto it=gr[v].rbegin();it!=gr[v].rend();++it){
                        auto [u, w, eid]=*it;
                        if(u==parent_[v]||u==heavy_[v])continue;
                        st.emplace_back(u,u);
                    }
                }
            }
        }

        void path_node_apply(int u,int v,T mul,T add){
            ensure_node_seg_built();
            while(head_[u]!=head_[v]){
                if(depth_[head_[u]]<depth_[head_[v]])swap(u,v);
                node_seg.update_range(pos_[head_[u]],pos_[u])(mul,add);
                u=parent_[head_[u]];
            }
            if(depth_[u]>depth_[v]) swap(u,v);
            node_seg.update_range(pos_[u],pos_[v])(mul,add);
        }

        void path_edge_apply(int u,int v,T mul,T add){
            ensure_edge_seg_built();
            while(head_[u]!=head_[v]){
                if(depth_[head_[u]]<depth_[head_[v]])swap(u,v);
                edge_seg.update_range(pos_[head_[u]],pos_[u])(mul,add);
                u=parent_[head_[u]];
            }
            if(depth_[u]>depth_[v]) swap(u,v);
            if(u!=v)edge_seg.update_range(pos_[u]+1,pos_[v])(mul,add);
        }

        void subtree_node_apply(int const v,T mul,T add){
            ensure_node_seg_built();
            node_seg.update_range(pos_[v],pos_[v]+subtree_[v]-1)(mul,add);
        }

        void subtree_edge_apply(int const v,T mul,T add){
            ensure_edge_seg_built();
            if(subtree_[v]<=1) return;
            edge_seg.update_range(pos_[v]+1,pos_[v]+subtree_[v]-1)(mul,add);
        }

        enum class update_kind{ node,edge,path_node,path_edge,subtree_node,subtree_edge };

        struct update_proxy{
            tree&tr;
            update_kind kind;
            int u,v;

            update_proxy(tree&tr_,update_kind const kind_,int const u_,int const v_=-1)
                : tr(tr_), kind(kind_), u(u_), v(v_){}

            void assign(T x){
                switch(kind){
                    case update_kind::node:
                        tr.ensure_node_seg_built();
                        tr.node_weight[u]=x;
                        tr.node_seg.update_range(tr.pos_[u],tr.pos_[u])(0,x);
                        break;

                    case update_kind::edge:{
                        tr.ensure_edge_seg_built();
                        tr.edge_weight[u]=x;
                        int c=tr.child_edge_[u];
                        tr.edge_seg.update_range(tr.pos_[c],tr.pos_[c])(0,x);
                        break;
                    }

                    case update_kind::path_node:tr.path_node_apply(u,v,0,x);
                        break;
                    case update_kind::path_edge:tr.path_edge_apply(u,v,0,x);
                        break;
                    case update_kind::subtree_node:tr.subtree_node_apply(u,0,x);
                        break;
                    case update_kind::subtree_edge:tr.subtree_edge_apply(u,0,x);
                        break;
                }
            }

            void add(T x){
                switch(kind){
                    case update_kind::node:
                        tr.ensure_node_seg_built();
                        tr.node_weight[u]+=x;
                        tr.node_seg.update_range(tr.pos_[u],tr.pos_[u])(1,x);
                        break;

                    case update_kind::edge:{
                        tr.ensure_edge_seg_built();
                        tr.edge_weight[u]+=x;
                        int c=tr.child_edge_[u];
                        tr.edge_seg.update_range(tr.pos_[c],tr.pos_[c])(1,x);
                        break;
                    }

                    case update_kind::path_node:tr.path_node_apply(u,v,1,x);
                        break;
                    case update_kind::path_edge:tr.path_edge_apply(u,v,1,x);
                        break;
                    case update_kind::subtree_node:tr.subtree_node_apply(u,1,x);
                        break;
                    case update_kind::subtree_edge:tr.subtree_edge_apply(u,1,x);
                        break;
                }
            }

            update_proxy&operator=(T x){
                assign(x);
                return *this;
            }

            update_proxy&operator+=(T x){
                add(x);
                return *this;
            }

            update_proxy&operator-=(T x){
                add(-x);
                return *this;
            }
        };

        struct node_proxy{
            tree&tr;
            int v;
            update_proxy update;

            node_proxy(tree&tr_,int const v_): tr(tr_), v(v_), update(tr_,update_kind::node,v_){}

            operator T() const{
                if(tr.node_seg_built)return tr.node_seg.get(tr.pos_[v]).val;
                return tr.node_weight[v];
            }
        };

        struct edge_proxy{
            tree&tr;
            int eid;
            update_proxy update;
            edge_proxy(tree&tr_,int const eid_): tr(tr_), eid(eid_), update(tr_,update_kind::edge,eid_){}

            operator T() const{
                if(tr.edge_seg_built){
                    int c=tr.child_edge_[eid];
                    return tr.edge_seg.get(tr.pos_[c]).val;
                }
                return tr.edge_weight[eid];
            }

            int parent_node(){
                tr.ensure_hld_built();
                return tr.parent_[tr.child_edge_[eid]];
            }

            int child_node(){
                tr.ensure_hld_built();
                return tr.child_edge_[eid];
            }
        };

        struct path_proxy{
            tree&tr;
            int u,v;
            update_proxy node_update;
            update_proxy edge_update;

            path_proxy(tree&tr_,int u_,int v_)
                : tr(tr_), u(u_), v(v_),
                  node_update(tr_,update_kind::path_node,u_,v_),
                  edge_update(tr_,update_kind::path_edge,u_,v_){}

            T node_sum() const{ return tr.path_node_sum(u,v); }
            T edge_sum() const{ return tr.path_edge_sum(u,v); }
            int lca() const{ return tr.lca(u,v); }
            int node_count() const{ return tr.tree_node_count(u,v); }
            vector<int> vertices() const{ return tr.path_vertices(u,v); }
        };

        struct subtree_proxy{
            tree&tr;
            int v;
            update_proxy node_update;
            update_proxy edge_update;

            subtree_proxy(tree&tr_,int const v_):
                tr(tr_), v(v_),
                node_update(tr_,update_kind::subtree_node,v_),
                edge_update(tr_,update_kind::subtree_edge,v_){}

            T node_sum() const{ return tr.subtree_node_sum(v); }
            T edge_sum() const{ return tr.subtree_edge_sum(v); }
            int node_count() const{ return tr.subtree_[v]; }
            vector<int> vertices() const{ return tr.subtree_vertices(v); }
        };

        int tree_node_count(int const u,int const v){
            int const w=lca(u,v);
            return depth_[u]+depth_[v]-2*depth_[w]+1;
        }

        T path_node_sum(int u,int v){
            ensure_node_seg_built();
            T ans=M::e();
            while(head_[u]!=head_[v]){
                if(depth_[head_[u]]<depth_[head_[v]]) swap(u,v);
                ans=M::f(ans,node_seg(pos_[head_[u]],pos_[u]));
                u=parent_[head_[u]];
            }
            if(depth_[u]>depth_[v]) swap(u,v);
            ans=M::f(ans,node_seg(pos_[u],pos_[v]));
            return ans;
        }

        T path_edge_sum(int u,int v){
            ensure_edge_seg_built();
            T ans=M::e();
            while(head_[u]!=head_[v]){
                if(depth_[head_[u]]<depth_[head_[v]]) swap(u,v);
                ans=M::f(ans,edge_seg(pos_[head_[u]],pos_[u]));
                u=parent_[head_[u]];
            }
            if(depth_[u]>depth_[v]) swap(u,v);
            if(u!=v)ans=M::f(ans,edge_seg(pos_[u]+1,pos_[v]));
            return ans;
        }

        vector<int> path_vertices(int const u,int const v){
            ensure_hld_built();
            vector<int> left,right;
            int a=u,b=v;
            while(head_[a]!=head_[b]){
                if(depth_[head_[a]]>=depth_[head_[b]]){
                    for(int p=pos_[a];p>=pos_[head_[a]];--p) left.push_back(inv_pos_[p]);
                    a=parent_[head_[a]];
                } else{
                    for(int p=pos_[b];p>=pos_[head_[b]];--p) right.push_back(inv_pos_[p]);
                    b=parent_[head_[b]];
                }
            }
            if(depth_[a]>=depth_[b]){
                for(int p=pos_[a];p>=pos_[b];--p) left.push_back(inv_pos_[p]);
            } else{
                for(int p=pos_[b];p>pos_[a];--p) right.push_back(inv_pos_[p]);
                left.push_back(a);
            }
            reverse(right.begin(),right.end());
            left.insert(left.end(),right.begin(),right.end());
            return left;
        }

        int lca(int u,int v){
            ensure_hld_built();
            while(head_[u]!=head_[v]){
                if(depth_[head_[u]]<depth_[head_[v]]) swap(u,v);
                u=parent_[head_[u]];
            }
            return depth_[u]<depth_[v]?u:v;
        }

        T subtree_node_sum(int const v){
            ensure_node_seg_built();
            return node_seg(pos_[v],pos_[v]+subtree_[v]-1).val;
        }

        T subtree_edge_sum(int const v){
            ensure_edge_seg_built();
            if(subtree_[v]<=1) return M::e();
            return edge_seg(pos_[v]+1,pos_[v]+subtree_[v]-1).val;
        }

        vector<int> subtree_vertices(int const v){
            ensure_hld_built();
            vector<int> res;
            res.reserve(subtree_[v]);
            for(int p=pos_[v];p<pos_[v]+subtree_[v];++p){ res.push_back(inv_pos_[p]); }
            return res;
        }

    public:
        vector<vector<tuple<int,T,int>>> gr;

        explicit tree(int const n,vector<T> const&a):
            node_weight(a), edge_weight(), node_count(n), edge_count(0), parent_(n,-1), depth_(n,0),
            parent_edge_(n,-1), child_edge_(), subtree_(n,0), heavy_(n,-1), head_(n,-1),
            pos_(n,-1), inv_pos_(n,-1), gr(n){}

        explicit tree(int const n):
            node_weight(n,T{}), edge_weight(), node_count(n), edge_count(0), parent_(n,-1), depth_(n,0),
            parent_edge_(n,-1), child_edge_(), subtree_(n,0), heavy_(n,-1), head_(n,-1),
            pos_(n,-1), inv_pos_(n,-1), gr(n){}

        void add_edge(int u,int v,T w=0,bool const undirected=true){
            gr[u].emplace_back(v,w,edge_count);
            if(undirected)gr[v].emplace_back(u,w,edge_count);
            edge_weight.push_back(w);
            child_edge_.push_back(-1);
            edge_count++;
            hld_built=false;
            node_seg_built=false;
            edge_seg_built=false;
        }

        pair<int,int> diameter_edge() const{
            auto farthest=[&](int const start){
                vector<int> dist(node_count,-1);
                vector<int> st;
                st.reserve(node_count);
                st.push_back(start);
                dist[start]=0;
                int far=start;
                while(!st.empty()){
                    int v=st.back();
                    st.pop_back();
                    if(dist[v]>dist[far]){ far=v; }
                    for(auto [u, w, eid]:gr[v]){
                        if(dist[u]!=-1) continue;
                        dist[u]=dist[v]+1;
                        st.push_back(u);
                    }
                }
                return far;
            };
            int u=farthest(0);
            int v=farthest(u);
            return {u,v};
        }

        pair<int,int> diameter_weight() const{
            auto farthest=[&](int const start){
                vector<long> dist(node_count,-1);
                vector<int> st;
                st.reserve(node_count);
                st.push_back(start);
                dist[start]=0;
                int far=start;
                while(!st.empty()){
                    int v=st.back();
                    st.pop_back();
                    if(dist[v]>dist[far]){ far=v; }
                    for(auto [u, w, eid]:gr[v]){
                        if(dist[u]!=-1) continue;
                        dist[u]=dist[v]+w;
                        st.push_back(u);
                    }
                }
                return far;
            };
            int u=farthest(0);
            int v=farthest(u);
            return {u,v};
        }

        node_proxy node(int const v){ return node_proxy(*this,v); }
        edge_proxy edge(int const eid){ return edge_proxy(*this,eid); }

        path_proxy path(int const u,int const v){
            ensure_hld_built();
            return path_proxy(*this,u,v);
        }

        subtree_proxy subtree(int const v){
            ensure_hld_built();
            return subtree_proxy(*this,v);
        }

        int parent(int const v){
            ensure_hld_built();
            return parent_[v];
        }

        int depth(int const v){
            ensure_hld_built();
            return depth_[v];
        }

        int subtree_size(int const v){
            ensure_hld_built();
            return subtree_[v];
        }

        int pos(int const v){
            ensure_hld_built();
            return pos_[v];
        }

        void node_debug(){
            ensure_hld_built();
            for(int v=0;v<node_count;++v){
                if(node_seg_built) cout<<node_seg.get(pos_[v]).val;
                else cout<<node_weight[v];
                cout<<(v+1==node_count?'\n':' ');
            }
        }

        void edge_debug(){
            ensure_hld_built();
            for(int eid=0;eid<edge_count;++eid){
                if(edge_seg_built) cout<<edge_seg.get(pos_[child_edge_[eid]]).val;
                else cout<<edge_weight[eid];
                cout<<(eid+1==edge_count?'\n':' ');
            }
        }
};

/*  template


*/

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n,q;
    cin>>n>>q;

    vector<long> a(n);
    for(int i=0;i<n;i++)cin>>a[i];
    tree<long> tr(n,a);
    for(int i=1;i<n;i++){
        int p;
        cin>>p;
        tr.add_edge(i,p);
    }

    while(q--){
        int op;
        cin>>op;
        if(op==0){
            int u;
            long x;
            cin>>u>>x;
            tr.node(u).update+=x;
        } else{
            int u;
            cin>>u;
            cout<<tr.subtree(u).node_sum()<<'\n';
        }
    }

    return 0;
}
