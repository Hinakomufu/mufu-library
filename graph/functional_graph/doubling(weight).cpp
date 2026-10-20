#include <bits/stdc++.h>
using namespace std;


struct graph{
    private:
        long node;
        vector<vector<long>> nxt;
        vector<vector<long>> cost;
        bool costed=false;

    public:
        template<class NextFunc,class CostFunc>
        graph(long const n,NextFunc next,CostFunc weight)
            : node(n), nxt(62,vector<long>(n)), cost(62,vector<long>(n)){
            costed=true;
            for(long i=0;i<node;i++){
                nxt[0][i]=next(i);
                cost[0][i]=weight(i);
            }
            for(long k=1;k<62;k++){
                for(long i=0;i<node;i++){
                    long const mid=nxt[k-1][i];
                    nxt[k][i]=nxt[k-1][mid];
                    cost[k][i]=cost[k-1][i]+cost[k-1][mid];
                }
            }
        }

        template<class NextFunc>
        graph(long const n,NextFunc next)
            : node(n), nxt(62,vector<long>(n)){
            for(long i=0;i<node;i++){
                nxt[0][i]=next(i);
            }
            for(long k=1;k<62;k++){
                for(long i=0;i<node;i++){
                    long const mid=nxt[k-1][i];
                    nxt[k][i]=nxt[k-1][mid];
                }
            }
        }

        long jump(long u,long const k) const{
            for(long b=0;b<62;b++){
                if(k&(1L<<b)) u=nxt[b][u];
            }
            return u;
        }

        long jump_cost(long u,long const k) const{
            if(!costed)return -1;
            long s=0;
            for(long b=0;b<62;b++){
                if(k&(1L<<b)){
                    s+=cost[b][u];
                    u=nxt[b][u];
                }
            }
            return s;
        }
};

/*

    auto next=[&](long const u){ return a[u]-1; };
    auto weight=[&](long const u){ return u+1; };
    graph g(n,next,weight);

    // when weight not need:
    graph g(n,next);
*/

int main(){
    // ABC438E
    long n,q;
    cin>>n>>q;

    vector<long> a(n);
    for(long i=0;i<n;i++)cin>>a[i];

    auto next=[&](long const u){ return a[u]-1; };
    auto weight=[&](long const u){ return u+1; };
    graph g(n,next,weight);

    while(q--){
        long x,y;
        cin>>y>>x;
        x--;
        cout<<g.jump_cost(x,y)<<endl;
    }

    return 0;
}
