//https://github.com/Hinakomufu/mufu-library/blob/main/graph/tree/tree(segtree).cpp
#include <bits/stdc++.h>
using namespace std;
long inf=1e18;

template<typename U>
struct tree_monoid{
    using T=U;
    static T f(const T&a,const T&b){ return a+b; }
    static T e(){ return T(0); }
};

template<typename T,typename M = tree_monoid<T>> struct tree{
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

        struct segtree{ // multi element monoid
            private:
                //using T=typename M::T;
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
            vector<T> b(node_count);
            for(int v=0;v<node_count;++v)b[pos_[v]]=node_weight[v];
            node_seg=segtree(b);
            node_seg_built=true;
        }

        void build_edge_segtree(){
            vector<T> b(node_count,M::e());
            for(int v=0;v<node_count;++v){
                if(parent_edge_[v]!=-1) b[pos_[v]]=edge_weight[parent_edge_[v]];
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

        struct child_range{
            tree&tr;
            int v;
            bool include_heavy;

            child_range(tree&tr_,int const v_,bool const include_heavy_)
                : tr(tr_), v(v_), include_heavy(include_heavy_){}

            struct iterator{
                tree&tr;
                int v;
                int idx;
                bool include_heavy;

                iterator(tree&tr_,int const v_,int const idx_,bool const include_heavy_)
                    : tr(tr_), v(v_), idx(idx_), include_heavy(include_heavy_){
                    skip_nodes();
                }

                void skip_nodes(){
                    while(idx<static_cast<int>(tr.gr[v].size())){
                        int u=get<0>(tr.gr[v][idx]);
                        if(u==tr.parent_[v]||(!include_heavy&&u==tr.heavy_[v]))++idx;
                        else break;
                    }
                }

                int operator*() const{ return get<0>(tr.gr[v][idx]); }

                iterator&operator++(){
                    ++idx;
                    skip_nodes();
                    return *this;
                }

                bool operator==(iterator const&other) const{
                    return &tr==&other.tr&&v==other.v&&idx==other.idx;
                }

                bool operator!=(iterator const&other) const{ return !(*this==other); }
            };

            iterator begin(){
                tr.ensure_hld_built();
                return iterator(tr,v,0);
            }

            iterator end(){
                tr.ensure_hld_built();
                return iterator(tr,v,static_cast<int>(tr.gr[v].size()));
            }
        };

        struct node_proxy{
            tree&tr;
            int v;

            node_proxy(tree&tr_,int const v_)
                : tr(tr_), v(v_){}

            operator T() const{ return tr.node_weight[v]; }

            int pos(){
                tr.ensure_hld_built();
                return tr.pos_[v];
            }

            int parent(){
                tr.ensure_hld_built();
                return tr.parent_[v];
            }

            int heavy_child(){
                tr.ensure_hld_built();
                return tr.heavy_[v];
            }

            int depth(){
                tr.ensure_hld_built();
                return tr.depth_[v];
            }

            int lca(int const other) const{
                return tr.lca(v,other);
            }

            int heavy_path_head(){
                tr.ensure_hld_built();
                return tr.head_[v];
            }

            vector<int> heavy_path_vertices() const{
                tr.ensure_hld_built();
                vector<int> res;
                for(int u=v;u!=-1;u=tr.heavy_[u])res.push_back(u);
                return res;
            }

            child_range child(bool const include_heavy=true){
                tr.ensure_hld_built();
                return child_range(tr,v,include_heavy);
            }

            bool is_ancestor(int const other){
                tr.ensure_hld_built();
                return tr.pos_[v]<=tr.pos_[other]&&tr.pos_[other]<tr.pos_[v]+tr.subtree_[v];
            }

            bool is_leaf(){
                tr.ensure_hld_built();
                return tr.gr[v].size()==(tr.parent_[v]==-1?0:1);
            }

            struct update_proxy{
                tree&tr;
                int v;

                update_proxy(tree&tr_,int const v_)
                    : tr(tr_), v(v_){}

                update_proxy&operator=(T const x){
                    tr.ensure_node_seg_built();
                    tr.node_weight[v]=x;
                    tr.node_seg.update(tr.pos_[v])=x;
                    return *this;
                }

                update_proxy&operator+=(T const x){
                    tr.ensure_node_seg_built();
                    tr.node_weight[v]+=x;
                    tr.node_seg.update(tr.pos_[v]).apply(x);
                    return *this;
                }

                update_proxy&operator-=(T const x){ return *this+=-x; }
            };

            update_proxy update{tr,v};
        };

        struct edge_proxy{
            tree&tr;
            int eid;
            edge_proxy(tree&tr_,int const eid_): tr(tr_), eid(eid_){}

            operator T() const{ return tr.edge_weight[eid]; }

            int parent_node(){
                tr.ensure_hld_built();
                return tr.parent_[tr.child_edge_[eid]];
            }

            int child_node(){
                tr.ensure_hld_built();
                return tr.child_edge_[eid];
            }

            struct update_proxy{
                tree&tr;
                int eid;
                update_proxy(tree&tr_,int const eid_): tr(tr_), eid(eid_){}

                update_proxy&operator=(T const x){
                    tr.ensure_edge_seg_built();
                    tr.edge_weight[eid]=x;
                    int v=tr.child_edge_[eid];
                    tr.edge_seg.update(tr.pos_[v])=x;
                    return *this;
                }

                update_proxy&operator+=(T const x){
                    tr.ensure_edge_seg_built();
                    tr.edge_weight[eid]+=x;
                    int v=tr.child_edge_[eid];
                    tr.edge_seg.update(tr.pos_[v]).apply(x);
                    return *this;
                }

                update_proxy&operator-=(T const x){ return *this+=-x; }
            };

            update_proxy update{tr,eid};
        };

        struct path_proxy{
            tree&tr;
            int u,v;
            path_proxy(tree&tr_,int const u_,int const v_): tr(tr_), u(u_), v(v_){}

            T node_sum() const{ return tr.path_node_sum(u,v); }
            T edge_sum() const{ return tr.path_edge_sum(u,v); }
            int lca() const{ return tr.lca(u,v); }
            int node_count() const{ return tr.tree_node_count(u,v); }
            vector<int> vertices() const{ return tr.path_vertices(u,v); }
        };

        struct subtree_proxy{
            tree&tr;
            int v;
            subtree_proxy(tree&tr_,int const v_): tr(tr_), v(v_){}

            T node_sum() const{ return tr.subtree_node_sum(v); }
            T edge_sum() const{ return tr.subtree_edge_sum(v); }

            int node_count() const{
                tr.ensure_hld_built();
                return tr.subtree_[v];
            }

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
            ans=M::f(ans,edge_seg(pos_[u]+1,pos_[v]));
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
            return node_seg(pos_[v],pos_[v]+subtree_[v]-1);
        }

        T subtree_edge_sum(int const v){
            ensure_edge_seg_built();
            if(subtree_[v]<=1) return M::e();
            return edge_seg(pos_[v]+1,pos_[v]+subtree_[v]-1);
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
            node_weight(n,M::e()), edge_weight(), node_count(n), edge_count(0), parent_(n,-1), depth_(n,0),
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
};

/*  template



    for debug (xxxxx->component of T):
    s.debug([](const T& t){return to_string(static_cast<long>(t.xxxxx));});
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
