#include <bits/stdc++.h>
using namespace std;

struct functional_graph{
    private:
        int node;
        vector<int> nxt;
        vector<vector<int>> rev;

        bool analyzed=false;
        vector<int> cycle_id;
        vector<int> cycle_pos;
        vector<int> dist_to_cycle;
        vector<int> cycle_entry;
        vector<vector<int>> cycle_nodes;
        vector<int> cycle_len;

        void analyze_cycles(){
            cycle_id.assign(node,-1);
            cycle_pos.assign(node,-1);
            dist_to_cycle.assign(node,-1);
            cycle_entry.assign(node,-1);
            cycle_nodes.clear();
            cycle_len.clear();

            vector<int> state(node,0);
            int cid=0;
            for(int start=0;start<node;start++){
                if(state[start]!=0) continue;
                int u=start;
                while(state[u]==0){
                    state[u]=1;
                    u=nxt[u];
                }
                if(state[u]==1){
                    vector<int> cyc;
                    int v=u;
                    int pos=0;
                    do{
                        cyc.push_back(v);
                        cycle_id[v]=cid;
                        cycle_pos[v]=pos++;
                        dist_to_cycle[v]=0;
                        cycle_entry[v]=v;
                        state[v]=2;
                        v=nxt[v];
                    } while(v!=u);
                    int L=cyc.size();
                    cycle_len.push_back(L);
                    cycle_nodes.push_back(cyc);
                    cid++;
                }

                queue<int> q;
                q.push(u);
                while(!q.empty()){
                    int x=q.front();
                    q.pop();
                    for(int y:rev[x]){
                        if(state[y]==2) continue;
                        dist_to_cycle[y]=dist_to_cycle[x]+1;
                        cycle_id[y]=cycle_id[x];
                        cycle_pos[y]=-1;
                        cycle_entry[y]=cycle_entry[x];
                        state[y]=2;
                        q.push(y);
                    }
                }
            }
            analyzed=true;
        }

    public:

        explicit functional_graph(int const n)
            : node(n), nxt(n), rev(n), cycle_id(n), cycle_pos(n),
              dist_to_cycle(n), cycle_entry(n){}

        void add_direct_edge(int const u,int const v){
            nxt[u]=v;
            rev[v].push_back(u);
            analyzed=false;
        }

        vector<int> cycle_component(int const u,bool const sorted=false){
            if(!analyzed) analyze_cycles();
            int const cid=cycle_id[u];
            if(cid==-1) return {};
            vector<int> res=cycle_nodes[cid];
            if(sorted) sort(res.begin(),res.end());
            return res;
        }

        int cycle_size(int const u){
            if(!analyzed) analyze_cycles();
            int const cid=cycle_id[u];
            if(cid==-1) return -1;
            return cycle_len[cid];
        }

        vector<int> reachable_vertices(int const u,bool const sorted=false){
            if(!analyzed) analyze_cycles();
            vector<int> res;
            int x=u;
            while(x!=cycle_entry[u]){
                res.push_back(x);
                x=nxt[x];
            }
            int const cid=cycle_id[u];
            queue<int> q;
            bool found=false;
            for(int v:cycle_nodes[cid]){
                if(v==u)found=true;
                if(found)res.push_back(v);
                else q.push(v);
            }
            while(!q.empty()){
                res.push_back(q.front());
                q.pop();
            }
            if(sorted) sort(res.begin(),res.end());
            return res;
        }

        int reachable_count(int const u){
            if(!analyzed) analyze_cycles();
            return dist_to_cycle[u]+cycle_len[cycle_id[u]];
        }

        bool reachable(int const u,int const v){
            if(!analyzed) analyze_cycles();
            if(cycle_id[u]!=cycle_id[v]) return false;
            if(cycle_pos[u]!=-1&&cycle_pos[v]!=-1) return true;
            if(cycle_pos[u]==-1&&cycle_pos[v]!=-1) return true;
            int x=u;
            while(x!=cycle_entry[u]){
                if(x==v) return true;
                x=nxt[x];
            }
            return false;
        }

        int distance(int const u,int const v){
            if(!analyzed) analyze_cycles();
            if(cycle_id[u]==cycle_id[v]){
                int const cid=cycle_id[u];
                int const L=cycle_len[cid];
                if(cycle_pos[u]==-1&&cycle_pos[v]==-1){
                    int x=u;
                    int d=0;
                    while(x!=cycle_entry[u]){
                        if(x==v) return d;
                        x=nxt[x];
                        d++;
                    }
                    return -1;
                }
                if(cycle_pos[u]==-1&&cycle_pos[v]!=-1){
                    int const entry=cycle_entry[u];
                    int const du=dist_to_cycle[u];
                    int const dv=(cycle_pos[v]-cycle_pos[entry]+L)%L;
                    return du+dv;
                }
                if(cycle_pos[u]!=-1&&cycle_pos[v]!=-1){
                    return (cycle_pos[v]-cycle_pos[u]+L)%L;
                }
            }
            return -1;
        }

        vector<int> restore_path(int const u,int const v){
            if(!analyzed) analyze_cycles();
            if(!reachable(u,v)) return {};
            vector<int> path;
            if(cycle_pos[u]!=-1&&cycle_pos[v]!=-1){
                const vector<int>&cyc=cycle_nodes[cycle_id[u]];
                int const L=cyc.size();
                int const pos_u=cycle_pos[u];
                int const pos_v=cycle_pos[v];
                path.push_back(u);
                for(int k=1;k<=(pos_v-pos_u+L)%L;k++){
                    int const idx=(pos_u+k)%L;
                    path.push_back(cyc[idx]);
                }
                return path;
            }
            if(cycle_pos[u]==-1&&cycle_pos[v]==-1){
                int x=u;
                while(true){
                    path.push_back(x);
                    if(x==v) return path;
                    x=nxt[x];
                }
            }
            if(cycle_pos[u]==-1&&cycle_pos[v]!=-1){
                int x=u;
                while(x!=cycle_entry[u]){
                    path.push_back(x);
                    x=nxt[x];
                }
                path.push_back(x);
                const vector<int>&cyc=cycle_nodes[cycle_id[u]];
                int const L=cyc.size();
                int const pos_entry=cycle_pos[x];
                int const pos_v=cycle_pos[v];
                for(int k=1;k<=(pos_v-pos_entry+L)%L;k++){
                    int const idx=(pos_entry+k)%L;
                    path.push_back(cyc[idx]);
                }
                return path;
            }
            return {};
        }
};

//
//
//
//
//

int main(){
    // ABC357E
    int n;
    cin>>n;

    functional_graph g(n);
    for(int i=0;i<n;i++){
        // 0_indexed
        int v;
        cin>>v;
        v--;
        g.add_direct_edge(i,v);
    }

    long ans=0;
    for(int i=0;i<n;i++){
        ans+=g.reachable_count(i);
    }
    cout<<ans<<endl;
    return 0;
}
