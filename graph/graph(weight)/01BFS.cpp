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

    int edge_count(int const n) const{
        return gr[n].size();
    }

    void bfs(int const start,vector<long>&dist) const{
        // i:initial notes
        deque<int> dq;
        dist[start]=0;
        dq.push_front(start);

        while(!dq.empty()){
            int v=dq.front();
            dq.pop_front();

            for(auto [to, w]:gr[v]){
                if(dist[to]>dist[v]+w){
                    dist[to]=dist[v]+w;
                    if(w==0) dq.push_front(to);
                    else dq.push_back(to);
                }
            }
        }
    }
};

//
//
//
//
//

int main(){
    // ABC400D
    int h,w;
    cin>>h>>w;
    auto pos=[&](int x,int y){ return x*w+y; };
    auto outside=[&](int x,int y){ return x<0||x>=h||y<0||y>=w; };

    vector<string> v(h);
    for(int i=0;i<h;i++)cin>>v[i];
    int sx,sy,gx,gy;
    cin>>sx>>sy>>gx>>gy;
    sx--,sy--,gx--,gy--;

    graph g(h*w);

    vector<int> dx={1,0,-1,0};
    vector<int> dy={0,1,0,-1};
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            for(int d=0;d<4;d++){
                int x=i+dx[d];
                int y=j+dy[d];
                if(outside(x,y))continue;
                if(v[x][y]=='.'){
                    g.add_edge(pos(i,j),pos(x,y),0,false);
                } else if(v[x][y]=='#'){
                    g.add_edge(pos(i,j),pos(x,y),1,false);
                }
            }
            for(int d=0;d<4;d++){
                int x=i+dx[d];
                int y=j+dy[d];
                int x2=i+2*dx[d];
                int y2=j+2*dy[d];
                if(outside(x2,y2))continue;
                if(v[x][y]=='#'||v[x2][y2]=='#'){
                    g.add_edge(pos(i,j),pos(x2,y2),1,false);
                }
            }
        }
    }

    vector<long> dist(h*w,1e9);
    g.bfs(pos(sx,sy),dist);
    cout<<dist[pos(gx,gy)]<<endl;
    return 0;
}
