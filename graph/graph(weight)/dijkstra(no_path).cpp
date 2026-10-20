#include <bits/stdc++.h>
using namespace std;

struct graph{
    int node;
    vector<vector<pair<int,long>>> gr;

    explicit graph(int const n): node(n), gr(n){}

    void add_edge(int u,int v,long w,bool const undirected=true){
        gr[u].emplace_back(v,w);
        if(undirected) gr[v].emplace_back(u,w);
    }

    vector<long> dijkstra_all(int start){
        vector<long> dist(node,1e18);
        dist[start]=0;
        priority_queue<pair<long,int>,vector<pair<long,int>>,greater<>> pq;
        pq.push({0,start});

        while(!pq.empty()){
            auto [d,u]=pq.top();
            pq.pop();
            if(d>dist[u]) continue;
            for(auto [v,w]:gr[u]){
                if(dist[v]>dist[u]+w){
                    dist[v]=dist[u]+w;
                    pq.push({dist[v],v});
                }
            }
        }
        return dist;
    }

    long dijkstra_one(int start,int const goal){
        vector<long> dist(node,1e18);
        dist[start]=0;
        priority_queue<pair<long,int>,vector<pair<long,int>>,greater<>> pq;
        pq.push({0,start});
        while(!pq.empty()){
            auto [d, u]=pq.top();
            pq.pop();
            if(d>dist[u]) continue;
            if(u==goal) return d;

            for(auto [v, w]:gr[u]){
                if(dist[v]>dist[u]+w){
                    dist[v]=dist[u]+w;
                    pq.push({dist[v],v});
                }
            }
        }
        return 1e18;
    }
};

int main(){
    // suit for tessoku A64
    int n,m;
    cin>>n>>m;

    graph g(n);
    for(int i=0;i<m;i++){
        // 0_indexed
        int u,v;
        long w;
        cin>>u>>v>>w;
        u--;
        v--;
        g.add_edge(u,v,w);
    }

    vector<long> const dist=g.dijkstra_all(0);

    for(int i=0;i<n;i++){
        if(dist[i]==1e18){
            cout<<-1<<endl;
        } else{
            cout<<dist[i]<<endl;
        }
    }

    return 0;
}
