#include <bits/stdc++.h>
using namespace std;

struct graph{
    private:
        int node;
        vector<vector<int>> gr;

    public:
        explicit graph(int const n): node(n), gr(n){}

        void add_edge(int u,int v,bool const undirected=true){
            gr[u].emplace_back(v);                // for directed graphs only
            if(undirected) gr[v].emplace_back(u); // for undirected and directed
        }

        int edge_count(int const n) const{
            return gr[n].size();
        }

        int dfs(int const start,vector<bool>&walk) const{
            if(walk[start]) return 0;

            int walked=0;
            stack<int> st;
            st.push(start);
            walk[start]=true;
            walked++;

            while(!st.empty()){
                int const v=st.top();
                st.pop();

                for(int u:gr[v]){
                    if(!walk[u]){
                        walk[u]=true;
                        walked++;
                        st.push(u);
                    }
                }
            }
            return walked;
        }

};

/*



*/

int main(){

    return 0;
}
