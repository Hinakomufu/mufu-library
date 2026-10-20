#include <bits/stdc++.h>
using namespace std;

long inf=4e18;

struct unionfind{
    private:
        struct node_info{
            int node_count=1;
            int edge_count=0;
        };

        vector<int> parent;
        vector<node_info> info;
        int components=0;
        int n;

    public:
        explicit unionfind(int const n): parent(n,-1), info(n), components(n), n(n){}

        int add_node(){
            parent.push_back(-1);
            info.push_back(node_info());
            components++;
            n++;
            // return number of new node
            return parent.size()-1;
        }

        int root(int const x){
            if(parent[x]==-1) return x;
            return parent[x]=root(parent[x]);
        }

        bool is_connected(int const x,int const y){
            return root(x)==root(y);
        }

        void unite(int const x,int const y){
            int rx=root(x);
            int ry=root(y);
            if(rx==ry){
                info[rx].edge_count++;
                return;
            }

            // Union by Size (default)
            if(info[rx].node_count<info[ry].node_count) swap(rx,ry);
            parent[ry]=rx;
            info[rx].node_count+=info[ry].node_count;
            info[rx].edge_count+=info[ry].edge_count+1;
            components--;
        }

        vector<int> root_nodes() const{
            vector<int> res;
            res.reserve(components);
            for(int i=0;i<n;i++){
                if(parent[i]==-1)res.push_back(i);
            }
            return res;
        }

        vector<vector<int>> group_nodes() {
            vector<int> const roots = root_nodes();
            int const x = components;
            unordered_map<int,int> mp;
            mp.reserve(x);
            for(int i=0;i<x;i++){
                mp[roots[i]] = i;
            }
            vector<vector<int>> res(x);
            for(int i=0;i<n;i++){
                int r = root(i);
                res[mp[r]].push_back(i);
            }
            return res;
        }

        int group_node_count(int const x=0){
            return info[root(x)].node_count;
        }

        vector<int> group_node_counts() const{
            vector<int> res;
            res.reserve(components);
            for(int i=0;i<n;i++){
                if(parent[i]==-1)res.push_back(info[i].node_count);
            }
            return res;
        }

        int group_edge_count(int const x=0){
            return info[root(x)].edge_count;
        }

        int group_count() const{
            return components;
        }
};

struct edge{
    int u,v;
    long weight;

    bool operator<(const edge&other) const{
        if(weight!=other.weight) return weight<other.weight;
        if(u!=other.u) return u<other.u;
        return v<other.v;
    }
};

/*



*/

int main(){
    // tessoku A67
    int n,m;
    cin>>n>>m;

    vector<edge> e(m);
    for(int i=0;i<m;i++){
        int u,v;
        long w;
        cin>>u>>v>>w;
        u--;
        v--;
        e[i]={u,v,w};
    }
    sort(e.begin(),e.end());
    unionfind uf(n);

    for(auto&[u,v,w]:e){
        if(!uf.is_connected(u,v)){
            uf.unite(u,v,w);
        }
    }
    if(uf.group_count()!=1) return -1;
    cout<<uf.group_weight()<<endl;
    return 0;
}
