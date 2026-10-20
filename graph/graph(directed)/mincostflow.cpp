#include <bits/stdc++.h>
using namespace std;

struct flow{
    explicit flow(int const n_,bool const min_cost_=true) : n(n_), min_cost(min_cost_), gr(n_){}

    private:
        struct edge{
            int to;
            int rev;
            long cap;
            long cost;
            int id;
        };

        int n;
        int id_cnt=0;
        bool min_cost;
        vector<vector<edge>> gr;
        vector<long> edge_flow;
        vector<vector<int>> used_paths;
        vector<long> used_flow;

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

    public:
        void add_edge(int const from,int const to,long const cap,long const cost,int const id){
            long const flow_cost=min_cost?cost:-cost;
            gr[from].push_back({to,static_cast<int>(gr[to].size()),cap,flow_cost,id});
            gr[to].push_back({from,static_cast<int>(gr[from].size())-1,0,-flow_cost,-1});
        }

        void add_edge(int const from,int const to,long const cap,long const cost){
            add_edge(from,to,cap,cost,id_cnt);
            edge_flow.push_back(0);
            id_cnt++;
        }

        pair<long,long> cost_flow(int start,int const goal,long max_flow){
            constexpr long INF=1LL<<60;
            long flow=0,cost=0;

            vector<long> h(n,0),dist(n);
            vector<int> prev_v(n),prev_e(n);

            { // Bellman-Ford
                fill(dist.begin(),dist.end(),INF);
                dist[start]=0;
                bool update=true;
                while(update){
                    update=false;
                    for(int v=0;v<n;v++){
                        if(dist[v]==INF) continue;
                        for(int i=0;i<static_cast<int>(gr[v].size());i++){
                            auto const&e=gr[v][i];
                            if(e.cap>0&&dist[e.to]>dist[v]+e.cost){
                                dist[e.to]=dist[v]+e.cost;
                                update=true;
                            }
                        }
                    }
                }
                for(int v=0;v<n;v++) h[v]=dist[v];
            }

            while(max_flow>0){ // Dijkstra
                priority_queue<pair<long,int>,vector<pair<long,int>>,greater<pair<long,int>>> pq;
                fill(dist.begin(),dist.end(),INF);
                dist[start]=0;
                pq.push({0,start});

                while(!pq.empty()){
                    auto [d, v]=pq.top();
                    pq.pop();
                    if(dist[v]<d) continue;
                    for(int i=0;i<static_cast<int>(gr[v].size());i++){
                        auto&e=gr[v][i];
                        if(e.cap>0){
                            long nd=d+e.cost+h[v]-h[e.to];
                            if(nd<dist[e.to]){
                                dist[e.to]=nd;
                                prev_v[e.to]=v;
                                prev_e[e.to]=i;
                                pq.push({nd,e.to});
                            }
                        }
                    }
                }

                if(dist[goal]==INF) break;

                for(int v=0;v<n;v++) h[v]+=dist[v];

                long d=max_flow;
                for(int v=goal;v!=start;v=prev_v[v]){
                    d=min(d,gr[prev_v[v]][prev_e[v]].cap);
                }


                vector<int> path;
                for(int v=goal;v!=start;v=prev_v[v]){
                    path.push_back(v);
                }
                path.push_back(start);
                reverse(path.begin(),path.end());

                used_paths.push_back(path);
                used_flow.push_back(d);
                max_flow-=d;
                flow+=d;
                cost+=d*h[goal];

                for(int v=goal;v!=start;v=prev_v[v]){
                    auto&e=gr[prev_v[v]][prev_e[v]];
                    e.cap-=d;
                    gr[v][e.rev].cap+=d;
                }
            }

            collect_edge_flow();
            if(!min_cost)cost*=-1;

            return {flow,cost};
        }

        const vector<long>&get_edge_flow() const{ return edge_flow; }

        void debug() const{
            for(int u=0;u<n;u++){
                for(auto const&e:gr[u]){
                    if(e.id>=0){
                        long const flow=gr[e.to][e.rev].cap;
                        long const cap_now=e.cap;
                        long const cap_org=flow+cap_now;

                        long const original_cost=min_cost?e.cost:-e.cost;
                        cout<<u<<" "<<e.to<<" "
                                <<flow<<"/"<<cap_org
                                <<" cost="<<flow*original_cost<<"\n";
                    }
                }
            }

            for(int i=0;i<static_cast<int>(used_paths.size());i++){
                cout<<"path "<<i<<" (flow="<<used_flow[i]<<"): ";
                for(int const v:used_paths[i]) cout<<v<<" ";
                cout<<"\n";
            }
        }
};

int main(){
    //pastbook2022 F
    int n,e;
    long w;
    cin>>n>>e>>w;

    flow f(n);
    for(int i=0;i<e;i++){
        int u,v;
        long c,d;
        cin>>u>>v>>c>>d;
        u--,v--;
        f.add_edge(u,v,c,d);
    }

    auto [flow, cost]=f.cost_flow(0,n-1,w);
    f.debug();
    cout<<cost<<endl;
    return 0;
}
