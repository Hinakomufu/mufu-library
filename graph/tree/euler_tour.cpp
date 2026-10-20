#include <bits/stdc++.h>
using namespace std;

struct tour{
    int v;            // node
    long edge_weight; // weight of parent->v (root:0)
    long node_weight; // weight of node
    int depth;        // dist of root->v

    tour(int const v,long const w_edge,long const w_node,int const depth)
        : v(v), edge_weight(w_edge), node_weight(w_node), depth(depth){}

    tour&operator+=(long const w){
        node_weight+=w;
        return *this;
    }

    tour&operator-=(long const w){
        node_weight-=w;
        return *this;
    }
};

struct graph{
    private:
        int node;
        vector<long> w_node;
        bool toured;
        vector<int> tin,tout,parent,depth;
        vector<tour> euler;
        int timer=0;

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

    public:
        vector<edge> edges;
        vector<vector<pair<int,long>>> gr;
        explicit graph(int const n): node(n), w_node(n,0), toured(false), gr(n){}
        explicit graph(int const n,long const w): node(n), w_node(n,w), toured(false), gr(n){}

        graph(int const n,const vector<long>&node_weight)
            : node(n), w_node(node_weight), toured(false), gr(n){}

        void add_edge(int u,int v,long w=0,bool const undirected=true){
            toured=false;
            edges.push_back({u,v,w});
            gr[u].emplace_back(v,w);
            if(undirected) gr[v].emplace_back(u,w);
        }

        int edge_count(int const n) const{
            return gr[n].size();
        }

        void run_euler(int root=0,bool negative_exit=true){
            toured=true;

            tin.assign(node,-1);
            tout.assign(node,-1);
            parent.assign(node,-1);
            depth.assign(node,-1);
            euler.clear();
            euler.reserve(2*node);
            timer=-1;

            vector<long> edge_weight(node,0);
            vector<long> node_weight(node,0);
            stack<pair<int,int>> st;
            st.emplace(root,0);
            depth[root]=0;
            parent[root]=-1;

            while(!st.empty()){
                auto&p=st.top();
                int v=p.first;
                int&idx=p.second;
                if(idx==0){
                    timer++;
                    tin[v]=timer;
                    euler.emplace_back(v,edge_weight[v],w_node[v],depth[v]);
                }
                if(idx<static_cast<int>(gr[v].size())){
                    auto [u,w]=gr[v][idx++];
                    if(u==parent[v]) continue;
                    parent[u]=v;
                    edge_weight[u]=w;
                    node_weight[u]=w_node[u];
                    depth[u]=depth[v]+1;
                    st.emplace(u,0);
                } else{
                    tout[v]=timer;
                    timer++;
                    long ew=negative_exit?-edge_weight[v]:0;
                    long nw=negative_exit?-w_node[v]:0;

                    euler.emplace_back(v,ew,nw,depth[v]);
                    st.pop();
                }
            }
        }

        vector<tour>&get_euler(int const root=0,bool negative_exit=true){
            if(!toured)run_euler(root,negative_exit);
            return euler;
        }

        int get_tin(int const v) const{ return tin[v]; }
        int get_tout(int const v) const{ return tout[v]; }
        int get_depth(int const v) const{ return depth[v]; }
        int get_parent(int const v) const{ return parent[v]; }
};

int main(){
    
    return 0;
}
