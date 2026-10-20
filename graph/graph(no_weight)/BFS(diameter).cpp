#include <bits/stdc++.h>
using namespace std;

struct graph{
    int node;
    vector<vector<int>> vec;

    int diameter_length=-1;
    vector<int> end_node1;
    vector<int> end_node2;
    int center_node=-1;
    pair<int,int> center_edge={-1,-1};
    bool found_diameter=false;

    explicit graph(int const n): node(n), vec(n){}

    void add_edge(int u,int v,bool const undirected=true){
        vec[u].emplace_back(v);                // for directed graphs only
        if(undirected) vec[v].emplace_back(u); // for undirected and directed
    }

    int edge_count(int const n) const{
        return vec[n].size();
    }

    void bfs(int const start,vector<int>&walk,vector<int>&parent) const{
        // i:initial notes
        if(walk[start]!=-1)return;
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
                    parent[nxt]=fnt;
                }
            }
        }
    }

    // function to restore path
    static vector<int> restore_path(int const start,int const goal,vector<int> const&parent){
        vector<int> path;
        int cur=goal;
        while(cur!=-1){
            path.push_back(cur);
            if(cur==start) break;
            cur=parent[cur];
        }
        if(path.back()!=start) return {};
        reverse(path.begin(),path.end());
        return path;
    }

    vector<int> restore_path(int start,int const goal) const{
        if(start==goal) return {start};

        vector<int> walk(node,-1),parent(node,-1);
        deque<int> que;
        que.push_back(start);
        walk[start]=0;

        while(!que.empty()){
            int f=que.front();
            que.pop_front();
            for(int nxt:vec[f]){
                if(walk[nxt]==-1){
                    walk[nxt]=walk[f]+1;
                    parent[nxt]=f;
                    que.push_back(nxt);
                }
            }
        }

        if(walk[goal]==-1) return {};

        vector<int> path;
        int cur=goal;
        while(cur!=-1){
            path.push_back(cur);
            if(cur==start) break;
            cur=parent[cur];
        }
        reverse(path.begin(),path.end());
        return path;
    }

    vector<int> restore_path(pair<int,int> const&p)const{
        int const s=p.first;
        int const g=p.second;
        return restore_path(s,g);
    }

    //
    //
    //
    //
    //

    void find_diameter(){
        found_diameter=true;
        if(node==0)return;
        vector<int> walk(node,-1),parent(node,-1);
        bfs(0,walk,parent);
        int u=max_element(walk.begin(),walk.end())-walk.begin();

        walk.assign(node,-1);
        parent.assign(node,-1);
        bfs(u,walk,parent);
        int a,b;
        diameter_length=*max_element(walk.begin(),walk.end());
        for(int i=0;i<node;i++){
            if(walk[i]==diameter_length)end_node1.push_back(i);
            if(diameter_length%2==0&&walk[i]==diameter_length/2)center_node=i;
            if(diameter_length%2==1&&walk[i]==diameter_length/2)a=i;
            if(diameter_length%2==1&&walk[i]==diameter_length/2+1)b=i;
        }

        sort(end_node1.begin(),end_node1.end());
        if(diameter_length%2==1)center_edge={a,b};
        if(diameter_length%2==0){
            end_node2.push_back(-1);
            return;
        }

        int const v=end_node1[0];
        vector<int> walk2(node,-1),parent2(node,-1);
        bfs(v,walk2,parent2);
        for(int i=0;i<node;i++){
            if(walk2[i]==diameter_length){
                end_node2.push_back(i);
            }
        }
        sort(end_node2.begin(),end_node2.end());
    }

    pair<int,int> diameter_pair(){
        if(!found_diameter)find_diameter();
        if(node==0)return {-1,-1};
        if(node==1)return {0,0};
        int u=end_node1[0];
        int v=(diameter_length%2==0?end_node1[1]:end_node2[0]);
        if(u>v)swap(u,v);
        return {u,v};
    }
};

/*

g.find_diameter();
cout<<g.diameter_length<<endl;
vector path=g.restore_path(g.diameter_pair());

(diameter_length:even) end_node1-center_node-end_node1
(diameter_length:odd) end_node1-center_edge-end_node2

*/

int main(){
    int n;
    cin>>n;

    // define number of edges (need change)
    graph g(n);
    for(int i=0;i<n-1;i++){
        // 0_indexed
        int u,v;
        cin>>u>>v;
        u--;
        v--;
        g.add_edge(u,v);
    }

    g.find_diameter();
    cout<<g.diameter_length<<endl;
    vector path=g.restore_path(g.diameter_pair());
    for(int const v:path){
        cout<<v+1<<" ";
    }
    cout<<endl;
    return 0;
}
