#include <bits/stdc++.h>
using namespace std;

struct graph{
    private:
        int node;
        int edges;

    public:
        vector<vector<int>> gr;
        vector<vector<int>> rv;
        explicit graph(int const n): node(n), edges(0), gr(n), rv(n){}

        void add_edge(int u,int v){
            gr[u].emplace_back(v); // for directed graphs only
            rv[v].emplace_back(u);
            edges++;
        }

        int edge_count(int const n) const{
            return gr[n].size();
        }

        void add_nodes(int const new_n){
            if(new_n<=node) return;
            gr.resize(new_n);
            rv.resize(new_n);
            node=new_n;
        }

        void debug() const{
            cout<<"edges:"<<edges<<"\n";
            for(int u=0;u<node;u++){
                for(int const v:gr[u]){
                    cout<<u<<" "<<v<<"\n";
                }
            }
        }

        friend struct scc;
};

struct scc{
    private:
        const graph*gr;
        int comp_count_=0;
        vector<vector<int>> comps_;
        vector<int> comp_size_;
        vector<int> comp_id_;
        graph dag;
        vector<int> order_;
        vector<char> has_cycle;

    public:
        explicit scc(const graph&g,bool const topo_sort=true)
            : gr(&g), dag(0){
            int const n=g.node;
            comp_id_.assign(n,-1);
            vector<int> visited(n,0);
            int ver=1;
            vector<int> vec;

            auto dfs1=[&](auto self,int const v)->void{
                visited[v]=ver;
                for(int nxt:g.gr[v]){
                    if(visited[nxt]!=ver) self(self,nxt);
                }
                vec.push_back(v);
            };
            for(int i=0;i<n;i++){
                if(visited[i]!=ver) dfs1(dfs1,i);
            }
            ver++;

            auto dfs2=[&](auto self,int const v,vector<int>&comp,int cid)->void{
                visited[v]=ver;
                comp.push_back(v);
                comp_id_[v]=cid;
                for(int nxt:g.rv[v]){
                    if(visited[nxt]!=ver) self(self,nxt,comp,cid);
                }
            };

            while(!vec.empty()){
                int const v=vec.back();
                vec.pop_back();
                if(visited[v]!=ver){
                    vector<int> comp;
                    dfs2(dfs2,v,comp,comp_count_);
                    comps_.push_back(comp);
                    comp_count_++;
                }
            }

            comp_size_.resize(comp_count_);
            for(int i=0;i<comp_count_;i++){
                comp_size_[i]=comps_[i].size();
            }

            dag=graph(comp_count_);
            vector<int> indeg(comp_count_,0);
            for(int u=0;u<n;u++){
                for(int const v:g.gr[u]){
                    int cu=comp_id_[u];
                    int cv=comp_id_[v];
                    if(cu!=cv){
                        dag.add_edge(cu,cv);
                    }
                }
            }

            fill(indeg.begin(),indeg.end(),0);
            for(int u=0;u<comp_count_;u++){
                for(int v:dag.gr[u]) indeg[v]++;
            }

            has_cycle.assign(comp_count_,false);
            for(int cid=0;cid<comp_count_;cid++){
                if(comp_size_[cid]>=2){
                    has_cycle[cid]=true;
                    continue;
                }
                int v=comps_[cid][0];
                for(int to:g.gr[v]){
                    if(to==v){
                        has_cycle[cid]=true;
                        break;
                    }
                }
            }

            if(topo_sort){
                queue<int> q;
                vector<int> indeg_copy=indeg;
                for(int i=0;i<comp_count_;i++) if(indeg_copy[i]==0) q.push(i);

                vector<int> topo_order;
                while(!q.empty()){
                    int u=q.front();
                    q.pop();
                    topo_order.push_back(u);
                    for(int v:dag.gr[u]){
                        if(--indeg_copy[v]==0) q.push(v);
                    }
                }

                vector<int> independent,non_independent;
                for(int cid:topo_order){
                    bool has_in=(indeg[cid]>0);
                    bool has_out=!dag.gr[cid].empty();
                    if(!has_in&&!has_out){
                        independent.push_back(cid);
                    } else{
                        non_independent.push_back(cid);
                    }
                }
                order_.clear();
                order_.insert(order_.end(),non_independent.begin(),
                             non_independent.end());
                order_.insert(order_.end(),independent.begin(),independent.end());
            } else{
                order_.resize(comp_count_);
                iota(order_.begin(),order_.end(),0);
            }
        }

        int comp_count() const{ return comp_count_; }
        const vector<int>& comps(int const cid) const {return comps_[cid];}
        int comp_size(int const cid) const {return comp_size_[cid];}
        int comp_id(int const v) const {return comp_id_[v];}
        int order(int const i) const {return order_[i];}

        const graph&scc_dag() const{ return dag; }

        vector<int> dist_to_cycle() const{
            vector<int> dist(gr->node,-1);
            queue<int> q;

            for(int v=0;v<gr->node;v++){
                if(has_cycle[comp_id_[v]]){
                    dist[v]=0;
                    q.push(v);
                }
            }

            while(!q.empty()){
                int const v=q.front();
                q.pop();
                for(int from:gr->rv[v]){
                    if(dist[from]==-1){
                        dist[from]=dist[v]+1;
                        q.push(from);
                    }
                }
            }
            return dist;
        }

        vector<int> count_reachable(bool const scc_cmp=false) const{
            int const W=(comp_count_+63)/64;
            vector<vector<uint64_t>> reach(comp_count_,vector<uint64_t>(W,0));
            for(int i=0;i<comp_count_;i++){ // self:reachable
                reach[i][i>>6]|=1ULL<<(i&63);
            }
            for(int idx=comp_count_;idx-->0;){ // check in reverse topological order
                int const u=order_[idx];
                for(int const v:dag.gr[u]){
                    for(int w=0;w<W;w++){
                        reach[u][w]|=reach[v][w];
                    }
                }
            }

            vector<int> scc_result(comp_count_,0); // count vertices reachable from each SCC
            for(int u=0;u<comp_count_;u++){
                int sum=0;
                for(int v=0;v<comp_count_;v++){
                    if(reach[u][v>>6]&(1ULL<<(v&63))){
                        sum+=comp_size_[v];
                    }
                }
                scc_result[u]=sum;
            }

            if(scc_cmp)return scc_result;
            vector<int> node_result(gr->node);
            for(int v=0;v<gr->node;v++){
                node_result[v]=scc_result[comp_id_[v]];
            }
            return node_result;
        }

        void debug(bool const cmp_cnt=true,bool const cmp_sz=false,bool const scc_print=true) const{
            if(cmp_cnt)cout<<"cmp_count:";
            cout<<comp_count_<<"\n";
            for(int const cid:order_){
                if(cmp_cnt)cout<<cid<<":";
                if(cmp_sz)cout<<comp_size_[cid];
                cout<<(cmp_cnt?"[":" ");
                for(int i=0;i<comp_size_[cid];i++)cout<<comps_[cid][i]<<(i==comp_size_[cid]-1?"":" ");
                if(cmp_cnt)cout<<"]";
                cout<<"\n";
            }
            if(scc_print){
                cout<<"\n";
                dag.debug();
            }
        }
};

/*  template

    graph g(n);
    scc const sg(g);
    int k=sg.comp_num();
    const vector<int>&comps=sg.comps(cid);
    int sz=sg.get_comp_size(cid);
    int cid=sg.get_comp_id(cid);
    graph const&dag=sg.get_dag();
    const vector<int>&order=sg.order();

    sg.debug();
*/

int main(){
    // tessoku 021
    int n,m;
    cin>>n>>m;
    graph g(n);
    for(int i=0;i<m;i++){
        int a,b;
        cin>>a>>b;
        a--,b--;
        g.add_edge(a,b);
    }

    scc const sg(g);
    long ans=0;
    int s=sg.comp_count();
    for(int i=0;i<s;i++){
        long x=sg.comp_size(i);
        ans+=x*(x-1)/2;
    }

    cout<<ans<<endl;
    return 0;
}
