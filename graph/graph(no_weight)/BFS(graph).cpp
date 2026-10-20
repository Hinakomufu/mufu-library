#include <bits/stdc++.h>
using namespace std;

struct graph {
    int node;
    vector<vector<int>> vec;

    explicit graph(int const n): node(n), vec(n) {
    }

    void add_edge(int u,int v,bool const undirected=true) {
        vec[u].emplace_back(v); // for directed graphs only
        if(undirected) vec[v].emplace_back(u); // for undirected and directed
    }

    int edge_count(int const n) const {
        return vec[n].size();
    }

    int bfs(int const start,vector<int> &walk) const {
        // i:initial notes
        if(walk[start]!=-1)return -1;
        int res=0;
        deque<int> que;
        que.push_back(start);
        walk[start]=0;

        while(!que.empty()) {
            int const fnt=que.front();
            que.pop_front();

            for(int nxt:vec[fnt]) {
                if(walk[nxt]==-1) {
                    que.push_back(nxt);
                    walk[nxt]=walk[fnt]+1;
                    res=max(walk[nxt],res);
                }
            }
        }
        return res;
    }

    void bfs_all(vector<int> &walk) const {
        for(int start=0; start<node; ++start) {
            bfs(start,walk);
        }
    }

    int walk_count(vector<int> const&walk) const {
        int count=0;
        for(int i=0; i<node; ++i){
            if(walk[i]!=-1) count++;
        }
        return count;
    }

    void debug() const{
        for(int i=0;i<node;i++){
            for(int const v:vec[i]){
                cout<<v<<" ";
            }
            cout<<endl;
        }
    }

    //
    //
    //
    //
    //

    // function to judge N-partite graph
    bool is_partite(int const N,vector<int>&walk) const {
        bfs_all(walk);
        for(int u=0;u<node;++u) {
            for(int const v:vec[u]) {
                if(walk[u]!=-1&&walk[v]!=-1) {
                    if(walk[u]%N==walk[v]%N) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
};

//
//
//
//
//

int main() {
    // tenkei 026
    int n;
    cin>>n;

    graph g(n);
    for(int i=0;i<n-1;i++) {
        int u,v;
        cin>>u>>v;
        u--;
        v--;
        g.add_edge(u,v);
    }

    //g.debug();
    vector<int> walk(n,-1),walk2(n,-1);
    int start=-1;
    deque<int> que;
    que.push_back(0);
    walk[0]=0;

    while(!que.empty()) {
        int const fnt=que.front();
        que.pop_front();
        bool end=true;

        for(int nxt:g.vec[fnt]) {
            if(walk[nxt]==-1) {
                que.push_back(nxt);
                walk[nxt]=walk[fnt]+1;
                end=false;
            }
        }
        if(end){
            start=fnt;
            break;
        }
    }

    g.bfs(start,walk2);
    vector<int>ans0,ans1;
    for(int i=0;i<n;i++){
        if(walk2[i]%2==0)ans0.push_back(i);
        else ans1.push_back(i);
    }

    int x=ans0.size();
    int y=ans1.size();
    if(x>=y){
        for(int i=0;i<n/2;i++)cout<<ans0[i]+1<<(i==n/2-1?"\n":" ");
    }
    else for(int i=0;i<n/2;i++)cout<<ans1[i]+1<<(i==n/2-1?"\n":" ");
    return 0;
}
