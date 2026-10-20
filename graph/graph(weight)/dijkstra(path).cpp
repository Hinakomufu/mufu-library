#include <bits/stdc++.h>
using namespace std;

struct graph {
    int node;
    vector<vector<pair<int,long>>> gr;

    explicit graph(int const n): node(n), gr(n) {
    }

    void add_edge(int u,int v,long w,bool const undirected=true) {
        gr[u].emplace_back(v,w);
        if(undirected) gr[v].emplace_back(u,w);
    }

    vector<long> dijkstra_all(int start,vector<int> &parent) {
        vector<long> dist(node,1e18);
        dist[start]=0;
        parent.assign(node,-1);
        priority_queue<pair<long,int>,vector<pair<long,int>>,greater<>> pq;
        pq.push({0,start});

        while(!pq.empty()) {
            auto [d,u]=pq.top();
            pq.pop();
            if(d>dist[u]) continue;
            for(auto [v,w]:gr[u]) {
                if(dist[v]>dist[u]+w) {
                    dist[v]=dist[u]+w;
                    parent[v]=u;
                    pq.push({dist[v],v});
                }
            }
        }
        return dist;
    }

    static vector<int> restore_path(int const start,
                                    int const goal,
                                    const vector<int> &parent) {
        vector<int> path;
        int cur=goal;
        while(cur!=-1) {
            path.push_back(cur);
            if(cur==start) break;
            cur=parent[cur];
        }
        if(path.back()!=start) return {};
        reverse(path.begin(),path.end());
        return path;
    }
};


int main() {
    // suit for tessoku B64
    int n,m;
    cin>>n>>m;

    graph g(n);
    for(int i=0;i<m;i++) {
        // 0_indexed
        int u,v;
        long w;
        cin>>u>>v>>w;
        u--;
        v--;
        g.add_edge(u,v,w);
    }

    vector<int> parent;
    vector<long> const dist=g.dijkstra_all(0,parent);
    vector<int>path=g.restore_path(0,n-1,parent);
    int l=path.size();

    for(int i=0;i<l;i++) {
        cout<<path[i]+1<<(i==l-1?"\n":" ");
    }


    return 0;
}
