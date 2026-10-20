#include <bits/stdc++.h>
using namespace std;

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

        vector<vector<int>> group_nodes(){
            vector<int> const roots=root_nodes();
            int const x=components;
            unordered_map<int,int> mp;
            mp.reserve(x);
            for(int i=0;i<x;i++){
                mp[roots[i]]=i;
            }
            vector<vector<int>> res(x);
            for(int i=0;i<n;i++){
                int r=root(i);
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

struct graph{
    private:
        void spanning_reset(){
            for(auto&e:edges)e.in_spanning_tree=false;
        }

    public:
        struct node{
            int id;
        };

        struct edge{
            int u,v;
            bool in_spanning_tree=false;

            edge(int const a,int const b){
                if(a<=b) u=a,v=b;
                else u=b,v=a;
            }

            bool operator==(edge const&other) const{
                return u==other.u&&v==other.v;
            }

            bool operator<(edge const&other) const{
                if(u!=other.u) return u<other.u;
                return v<other.v;
            }
        };

        int node_count;
        int edge_count;
        vector<node> nodes;
        vector<edge> edges;
        vector<vector<pair<int,int>>> vec; // adjacent nodes,edge_number
        bool spanning=false;

        explicit graph(int const n): node_count(n), edge_count(0), vec(n){
            nodes.reserve(n);
            for(int i=0;i<n;i++)nodes.emplace_back(i);
        }

        void add_edge(int x,int y,bool const undirected=true){
            edges.emplace_back(x,y);
            vec[x].emplace_back(y,edge_count);                // for directed graphs only
            if(undirected) vec[y].emplace_back(x,edge_count); // for undirected and directed
            edge_count++;
        }

        int edge_size(int const n) const{
            return vec[n].size();
        }

        void bfs(int const start,vector<int>&walk) const{
            // i:initial notes
            if(walk[start]!=-1)return;
            deque<int> que;
            que.push_back(start);
            walk[start]=0;

            while(!que.empty()){
                int const fnt=que.front();
                que.pop_front();

                for(auto [nxt,_]:vec[fnt]){
                    if(walk[nxt]==-1){
                        que.push_back(nxt);
                        walk[nxt]=walk[fnt]+1;
                    }
                }
            }
        }

        int walk_count(vector<int> const&walk) const{
            int count=0;
            for(int i=0;i<node_count;++i){
                if(walk[i]!=-1) count++;
            }
            return count;
        }

        void spanning_dfs(int const start=0){
            if(spanning)spanning_reset();
            vector<bool> visited(node_count,false);
            auto dfs=[&](auto&&self,int const v)->void{
                visited[v]=true;

                for(auto [nxt,e]:vec[v]){
                    if(visited[nxt]) continue;

                    edges[e].in_spanning_tree=true;
                    self(self,nxt);
                }
            };
            dfs(dfs,start);
            spanning=true;
        }

        void spanning_bfs(int const start=0){
            if(spanning)spanning_reset();
            vector<bool> visited(node_count,false);
            deque<int> que;
            visited[start]=true;
            que.push_back(start);
            while(!que.empty()){
                int const v=que.front();
                que.pop_front();
                for(auto [nxt,e]:vec[v]){
                    if(visited[nxt]) continue;
                    visited[nxt]=true;
                    edges[e].in_spanning_tree=true;
                    que.push_back(nxt);
                }
            }
            spanning=true;
        }

        void spanning_uf(){
            if(spanning) spanning_reset();
            unionfind uf(node_count);
            for(int i=0;i<edge_count;i++){
                int const u=edges[i].u;
                int const v=edges[i].v;
                if(uf.is_connected(u,v)) continue;
                uf.unite(u,v);
                edges[i].in_spanning_tree=true;
            }
            spanning=true;
        }

        vector<int> spanning_path(int const start,int const goal) const{
            if(!spanning) return {};
            vector<int> parent(node_count,-1);
            deque<int> que;
            parent[start]=start;
            que.push_back(start);
            while(!que.empty()){
                int const v=que.front();
                que.pop_front();
                if(v==goal) break;
                for(auto [nxt,e]:vec[v]){
                    if(!edges[e].in_spanning_tree) continue;
                    if(parent[nxt]!=-1) continue;
                    parent[nxt]=v;
                    que.push_back(nxt);
                }
            }
            if(parent[goal]==-1) return {};
            vector<int> path;
            for(int v=goal;v!=start;v=parent[v]){
                path.push_back(v);
            }
            path.push_back(start);
            reverse(path.begin(),path.end());
            return path;
        }
};

/*


*/

int main(){
    // ABC472E
    int t;
    cin>>t;
    while(t--){
        int n,m;
        cin>>n>>m;
        graph g(n);
        for(int i=0;i<m;i++){
            int a,b;
            cin>>a>>b;
            a--;
            b--;
            g.add_edge(a,b);
        }

        g.spanning_uf();
        vector<int>parity(n,-1);
        deque<int>q;
        q.push_back(0);
        parity[0]=0;
        while(!q.empty()){
            int v=q.front();
            q.pop_front();
            for(auto [nxt,e]:g.vec[v]){
                if(!g.edges[e].in_spanning_tree) continue;
                if(parity[nxt]!=-1) continue;

                parity[nxt]=parity[v]^1;
                q.push_back(nxt);
            }
        }

        bool exist=false;
        vector<int>ans={};
        for(auto[u,v,used]:g.edges){
            if(exist)break;
            if(used)continue;
            if(parity[u]!=parity[v])continue;
            exist=true;
            ans=g.spanning_path(u,v);
        }

        if(!exist)cout<<-1<<endl;
        else{
            int x=ans.size();
            cout<<x<<endl;
            for(int i=0;i<x;i++){
                cout<<ans[i]+1<<(i==x-1?"\n":" ");
            }

        }
    }
    return 0;
}
