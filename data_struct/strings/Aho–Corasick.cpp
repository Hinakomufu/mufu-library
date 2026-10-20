#include <bits/stdc++.h>
using namespace std;

// number of alphabets
static constexpr int num=26;

struct aho_corasick{

    struct node{
        int nxt[num];
        int fail;
        bool out;

        node() : fail(0), out(false){
            fill_n(nxt,num,-1);
        }
    };

    vector<node> nodes;

    aho_corasick(){
        nodes.reserve(200);
        nodes.emplace_back();
    }

    void insert(const string&s){
        int v=0;
        for(char c:s){
            int x=c-'a';
            if(nodes[v].nxt[x]==-1){
                nodes[v].nxt[x]=nodes.size();
                nodes.emplace_back();
            }
            v=nodes[v].nxt[x];
        }
        nodes[v].out=true;
    }

    void build(){
        queue<int> q;

        for(int c=0;c<num;c++){
            int u=nodes[0].nxt[c];
            if(u!=-1){
                nodes[u].fail=0;
                q.push(u);
            } else{
                nodes[0].nxt[c]=0;
            }
        }

        while(!q.empty()){
            int v=q.front();
            q.pop();
            int f=nodes[v].fail;
            nodes[v].out=nodes[v].out||nodes[f].out;

            for(int c=0;c<num;c++){
                int u=nodes[v].nxt[c];
                if(u!=-1){
                    nodes[u].fail=nodes[f].nxt[c];
                    q.push(u);
                } else{
                    nodes[v].nxt[c]=nodes[f].nxt[c];
                }
            }
        }
    }

};

int main(){

    return 0;
}
