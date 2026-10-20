#include <bits/stdc++.h>
using namespace std;

struct graph{
    int node;
    vector<vector<int>> vec;

    explicit graph(int const n): node(n), vec(n){}

    void add_edge(int u,int v,bool const undirected=true){
        vec[u].emplace_back(v);                // for directed graphs only
        if(undirected) vec[v].emplace_back(u); // for undirected and directed
    }

    int edge_count(int const n) const{
        return vec[n].size();
    }

    int bfs(int const start,vector<int>&walk) const{
        // i:initial notes
        if(walk[start]!=-1)return -1;
        int res=0;
        deque<int> que;
        que.push_back(start);
        walk[start]=0;

        while(!que.empty()){
            int const fnt=que.front();
            que.pop_front();

            for(int nxt:vec[fnt]){
                if(walk[nxt]==-1){
                    que.push_back(nxt);
                    walk[nxt]=walk[fnt]+1;
                    res=max(walk[nxt],res);
                }
            }
        }
        return res;
    }

    vector<int> restore_path(int const start,int const goal) const{
        vector<int> parent(node,-1);
        queue<int> q;
        q.push(start);
        parent[start]=start;

        while(!q.empty()){
            int x=q.front();
            q.pop();
            if(x==goal) break;
            for(int y:vec[x]){
                if(parent[y]==-1){
                    parent[y]=x;
                    q.push(y);
                }
            }
        }

        vector<int> path;
        int cur=goal;
        while(cur!=parent[cur]){
            path.push_back(cur);
            cur=parent[cur];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
        return path;
    }

    void bfs_all(vector<int>&walk) const{
        for(int start=0;start<node;++start){
            bfs(start,walk);
        }
    }

    int walk_count(vector<int> const&walk) const{
        int count=0;
        for(int i=0;i<node;++i){
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
};

int main(){
    int t;
    cin>>t;
    while(t--){
        int n,m;
        cin>>n>>m;

        graph g(n);
        for(int i=0;i<m;i++){
            int u,v;
            cin>>u>>v;
            u--;
            v--;
            g.add_edge(u,v);
        }

    }
}
