#include <bits/stdc++.h>
using namespace std;

struct flow{
    explicit flow(int const n_) : n(n_), gr(n_), level(n_), prog(n_){}

    private:
        struct edge{
            int to;
            int rev;
            long cap;
            int id;
        };

        int n;
        int id_cnt=0;
        vector<vector<edge>> gr;

        int start_edge=-1;
        int goal_edge=-1;
        vector<int> level,prog;
        long flow_val=0;
        vector<long> edge_flow;
        vector<int> min_cut;

        bool bfs(int const s,int const t){
            fill(level.begin(),level.end(),-1);
            queue<int> q;
            level[s]=0;
            q.push(s);
            while(!q.empty()){
                int const v=q.front();
                q.pop();
                for(auto&e:gr[v]){
                    if(e.cap>0&&level[e.to]<0){
                        level[e.to]=level[v]+1;
                        q.push(e.to);
                    }
                }
            }
            return level[t]>=0;
        }

        long dfs(int const v,int const t,long const f){
            if(v==t) return f;
            for(int&i=prog[v];i<static_cast<int>(gr[v].size());i++){
                edge&e=gr[v][i];
                if(e.cap>0&&level[e.to]==level[v]+1){
                    long const ret=dfs(e.to,t,min(f,e.cap));
                    if(ret>0){
                        e.cap-=ret;
                        gr[e.to][e.rev].cap+=ret;
                        return ret;
                    }
                }
            }
            return 0;
        }

        void collect_edge_flow(){
            for(int u=0;u<n;u++){
                for(auto const&e:gr[u]){
                    if(e.id>=0){
                        long const flowed=gr[e.to][e.rev].cap;
                        edge_flow[e.id]=flowed;
                    }
                }
            }
        }

        vector<int> compute_reachable(int const s) const{
            vector<int> vis(n,0);
            queue<int> q;
            vis[s]=1;
            q.push(s);
            while(!q.empty()){
                int const v=q.front();
                q.pop();
                for(auto const&e:gr[v]){
                    if(e.cap>0&&!vis[e.to]){
                        vis[e.to]=1;
                        q.push(e.to);
                    }
                }
            }
            return vis;
        }

        void collect_min_cut_edges(int const s){
            min_cut.clear();
            auto const vis=compute_reachable(s);
            for(int u=0;u<n;u++){
                for(auto const&e:gr[u]){
                    if(e.id>=0&&vis[u]&&!vis[e.to]){
                        min_cut.push_back(e.id);
                    }
                }
            }
        }

    public:
        void add_edge(int const from,int const to,long const cap,int const id){
            gr[from].push_back({to,static_cast<int>(gr[to].size()),cap,id});
            gr[to].push_back({from,static_cast<int>(gr[from].size())-1,0,-1});
        }

        void add_edge(int const from,int const to,long const cap){
            add_edge(from,to,cap,id_cnt);
            edge_flow.push_back(0);
            id_cnt++;
        }

        void run_flow(int const start=-1,int const goal=-1){
            flow_val=0;
            start_edge=(start==-1?max(0,n-2):start);
            goal_edge=(goal==-1?max(1,n-1):goal);
            while(bfs(start_edge,goal_edge)){
                fill(prog.begin(),prog.end(),0);
                long f;
                while((f=dfs(start_edge,goal_edge,1L<<60))>0){
                    flow_val+=f;
                }
            }
            collect_edge_flow();
            collect_min_cut_edges(start_edge);
        }

        long get_maxflow() const{ return flow_val; }
        const vector<long>&get_edge_flow() const{ return edge_flow; }
        const vector<int>&get_min_cut() const{ return min_cut; }

        void debug(bool const sort_info=false) const{
            struct Info{
                int u,to;
                long flow,cap_org;
                bool is_cut;
            };
            vector<Info> es;
            vector<char> is_cut(id_cnt,0);
            for(int const id:min_cut) is_cut[id]=1;

            for(int u=0;u<n;u++){
                for(auto const&e:gr[u]){
                    if(e.id>=0){
                        long const flow=gr[e.to][e.rev].cap;
                        long const cap_now=e.cap;
                        long const cap_org=flow+cap_now;

                        es.emplace_back(u,e.to,flow,cap_org,is_cut[e.id]);
                    }
                }
            }

            if(sort_info){
                sort(es.begin(),es.end(),[&](auto const&A,auto const&B){
                    auto key=[&](auto const&X){
                        if(X.u==start_edge) return 0;
                        if(X.to==goal_edge) return 2;
                        return 1;
                    };
                    int const ka=key(A);
                    int const kb=key(B);
                    if(ka!=kb) return ka<kb;
                    if(A.u!=B.u) return A.u<B.u;
                    return A.to<B.to;
                });
            }
            for(auto const&e:es){
                cout<<e.u<<" "<<e.to<<" "
                        <<e.flow<<"/"<<e.cap_org;
                if(e.is_cut) cout<<" *";
                cout<<"\n";
            }
        }
};

int main(){
    // tessoku A68
    int n,m;
    cin>>n>>m;

    flow f(n);

    for(int i=0;i<m;i++){
        int u,v;
        long w;
        cin>>u>>v>>w;
        u--,v--;
        f.add_edge(u,v,w);
    }

    f.run_flow(0,n-1);
    cout<<f.get_maxflow()<<endl;
    //f.debug();
    return 0;
}
