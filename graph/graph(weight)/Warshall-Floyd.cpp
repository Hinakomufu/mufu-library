#include <bits/stdc++.h>
using namespace std;

struct graph{
    int node;
    bool cal;
    long mx;
    vector<vector<long>> dist;
    long const inf=1e18;

    explicit graph(int const n): node(n), cal(false), mx(0),
                                 dist(n,vector<long>(n,1e18)){
        for(int i=0;i<n;i++){
            dist[i][i]=0;
        }
    }

    void calc(){
        for(int k=0;k<node;k++){
            for(int i=0;i<node;i++){
                if(dist[i][k]==inf) continue;
                for(int j=0;j<node;j++){
                    if(dist[k][j]==inf) continue;
                    long const nd=dist[i][k]+dist[k][j];
                    if(nd<dist[i][j]) dist[i][j]=nd;
                    //cout<<i<<" "<<j<<" "<<dist[i][j]<<endl;
                }
            }
        }
        mx=0;
        for(int i=0;i<node;i++){
            for(int j=0;j<node;j++){
                if(dist[i][j]<inf){ mx=max(mx,dist[i][j]); }
            }
        }
        cal=true;
    }

    void recalc(int const k){
        for(int i=0;i<node;i++){
            if(dist[i][k]==inf) continue;
            for(int j=0;j<node;j++){
                if(dist[k][j]==inf) continue;
                long const nd=dist[i][k]+dist[k][j];
                if(nd<dist[i][j]) dist[i][j]=nd;
            }
        }
    }

    void add_edge(int const u,int const v,long const w,bool const oneway=false){
        // for directed graphs only
        dist[u][v]=min(dist[u][v],w);
        if(!oneway){
            // for undirected and directed
            dist[v][u]=min(dist[v][u],w);
        }
        if(cal){
            recalc(u);
            recalc(v);
            mx=0;
            for(int i=0;i<node;i++){
                for(int j=0;j<node;j++){
                    if(dist[i][j]<inf){
                        mx=max(mx,dist[i][j]);
                    }
                }
            }
        }
    }

    long distance(int const u,int const v){
        if(!cal) calc();
        return (dist[u][v]>=inf?-1:dist[u][v]);
    }

    long max_dist(){
        if(!cal)calc();
        return mx;
    }

    void debug() const{
        for(int i=0;i<node;i++){
            for(int j=0;j<node;j++){
                cout<<dist[i][j]<<" ";
            }
            cout<<endl;
        }
    }

};

//
//
//
//
//

struct edge{
    int u;
    int v;
    long weight;
};

int main(){
    // ABC375F
    int n,m,q;
    cin>>n>>m>>q;
    vector<edge> v(m);
    for(int i=0;i<m;i++){
        int a,b;
        long c;
        cin>>a>>b>>c;
        a--,b--;
        v[i]={a,b,c};
    }
    vector<bool> u(m,true);
    vector<vector<int>> query(q);
    for(int i=0;i<q;i++){
        int op;
        cin>>op;
        if(op==1){
            int d;
            cin>>d;
            d--;
            u[d]=false;
            query[i]={1,d};
        } else{
            int x,y;
            cin>>x>>y;
            x--,y--;
            query[i]={2,x,y};
        }
    }

    reverse(query.begin(),query.end());
    vector<long> ans;
    graph g(n);
    for(int i=0;i<m;i++){
        if(u[i]){
            auto [a,b,c]=v[i];
            g.add_edge(a,b,c);
        }
    }

    for(const auto&que:query){
        if(que[0]==1){
            auto [a,b,c]=v[que[1]];
            g.add_edge(a,b,c);
        } else{
            long tmp=g.distance(que[1],que[2]);
            ans.push_back(tmp);
        }
    }

    reverse(ans.begin(),ans.end());
    for(long z:ans)cout<<z<<endl;
    return 0;
}
