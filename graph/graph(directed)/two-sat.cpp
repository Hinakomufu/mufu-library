#include <bits/stdc++.h>
using namespace std;

struct graph{
    private:
        int node;
        int edges;

    public:
        vector<vector<int>> gr;
        vector<vector<int>> rv;
        explicit graph(int const n): node(n), edges(0), gr(n), rv(n){}

        void add_edge(int u,int v){
            gr[u].emplace_back(v); // for directed graphs only
            rv[v].emplace_back(u);
            edges++;
        }

        int edge_count(int const n) const{
            return gr[n].size();
        }

        void add_nodes(int const new_n){
            if(new_n<=node) return;
            gr.resize(new_n);
            rv.resize(new_n);
            node=new_n;
        }

        void debug() const{
            cout<<"edges:"<<edges<<"\n";
            for(int u=0;u<node;u++){
                for(int const v:gr[u]){
                    cout<<u<<" "<<v<<"\n";
                }
            }
        }

        friend struct scc;
};

struct scc{
    private:
        const graph*gr;
        int comp_num=0;
        vector<vector<int>> comps;
        vector<int> comp_size;
        vector<int> comp_id;
        graph dag;
        vector<int> order;
        vector<char> has_cycle;

    public:
        explicit scc(const graph&g,bool const topo_sort=true)
            : gr(&g), dag(0){
            int const n=g.node;
            comp_id.assign(n,-1);
            vector<int> visited(n,0);
            int ver=1;
            vector<int> vec;

            auto dfs1=[&](auto self,int const v)->void{
                visited[v]=ver;
                for(int nxt:g.gr[v]){
                    if(visited[nxt]!=ver) self(self,nxt);
                }
                vec.push_back(v);
            };
            for(int i=0;i<n;i++){
                if(visited[i]!=ver) dfs1(dfs1,i);
            }
            ver++;

            auto dfs2=[&](auto self,int const v,vector<int>&comp,int cid)->void{
                visited[v]=ver;
                comp.push_back(v);
                comp_id[v]=cid;
                for(int nxt:g.rv[v]){
                    if(visited[nxt]!=ver) self(self,nxt,comp,cid);
                }
            };

            while(!vec.empty()){
                int const v=vec.back();
                vec.pop_back();
                if(visited[v]!=ver){
                    vector<int> comp;
                    dfs2(dfs2,v,comp,comp_num);
                    comps.push_back(comp);
                    comp_num++;
                }
            }

            comp_size.resize(comp_num);
            for(int i=0;i<comp_num;i++){
                comp_size[i]=comps[i].size();
            }

            dag=graph(comp_num);
            vector<int> indeg(comp_num,0);
            for(int u=0;u<n;u++){
                for(int const v:g.gr[u]){
                    int cu=comp_id[u];
                    int cv=comp_id[v];
                    if(cu!=cv){
                        dag.add_edge(cu,cv);
                    }
                }
            }

            fill(indeg.begin(),indeg.end(),0);
            for(int u=0;u<comp_num;u++){
                for(int v:dag.gr[u]) indeg[v]++;
            }

            has_cycle.assign(comp_num,false);
            for(int cid=0;cid<comp_num;cid++){
                if(comp_size[cid]>=2){
                    has_cycle[cid]=true;
                    continue;
                }
                int v=comps[cid][0];
                for(int to:g.gr[v]){
                    if(to==v){
                        has_cycle[cid]=true;
                        break;
                    }
                }
            }

            if(topo_sort){
                queue<int> q;
                vector<int> indeg_copy=indeg;
                for(int i=0;i<comp_num;i++) if(indeg_copy[i]==0) q.push(i);

                vector<int> topo_order;
                while(!q.empty()){
                    int u=q.front();
                    q.pop();
                    topo_order.push_back(u);
                    for(int v:dag.gr[u]){
                        if(--indeg_copy[v]==0) q.push(v);
                    }
                }

                vector<int> independent,non_independent;
                for(int cid:topo_order){
                    bool has_in=(indeg[cid]>0);
                    bool has_out=!dag.gr[cid].empty();
                    if(!has_in&&!has_out){
                        independent.push_back(cid);
                    } else{
                        non_independent.push_back(cid);
                    }
                }
                order.clear();
                order.insert(order.end(),non_independent.begin(),
                             non_independent.end());
                order.insert(order.end(),independent.begin(),independent.end());
            } else{
                order.resize(comp_num);
                iota(order.begin(),order.end(),0);
            }
        }

        int get_cmp_num() const{ return comp_num; }
        const vector<vector<int>>&get_comps() const{ return comps; }
        const vector<int>&get_comp_size() const{ return comp_size; }
        const vector<int>&get_comp_id() const{ return comp_id; }
        const vector<int>&get_order() const{ return order; }
        const graph&get_dag() const{ return dag; }

        vector<int> dist_to_cycle() const{
            vector<int> dist(gr->node,-1);
            queue<int> q;

            for(int v=0;v<gr->node;v++){
                if(has_cycle[comp_id[v]]){
                    dist[v]=0;
                    q.push(v);
                }
            }

            while(!q.empty()){
                int v=q.front();
                q.pop();
                for(int from:gr->rv[v]){
                    if(dist[from]==-1){
                        dist[from]=dist[v]+1;
                        q.push(from);
                    }
                }
            }
            return dist;
        }

        vector<int> count_reachable(bool scc_cmp=false) const{
            int const W=(comp_num+63)/64;
            vector<vector<uint64_t>> reach(comp_num,vector<uint64_t>(W,0));
            for(int i=0;i<comp_num;i++){ // self:reachable
                reach[i][i>>6]|=1ULL<<(i&63);
            }
            for(int idx=comp_num;idx-->0;){ // check in reverse topological order
                int u=order[idx];
                for(int v:dag.gr[u]){
                    for(int w=0;w<W;w++){
                        reach[u][w]|=reach[v][w];
                    }
                }
            }

            vector<int> scc_result(comp_num,0); // count vertices reachable from each SCC
            for(int u=0;u<comp_num;u++){
                int sum=0;
                for(int v=0;v<comp_num;v++){
                    if(reach[u][v>>6]&(1ULL<<(v&63))){
                        sum+=comp_size[v];
                    }
                }
                scc_result[u]=sum;
            }

            if(scc_cmp)return scc_result;
            vector<int> node_result(gr->node);
            for(int v=0;v<gr->node;v++){
                node_result[v]=scc_result[comp_id[v]];
            }
            return node_result;
        }

        void debug(bool const cmp_num=true,bool const cmp_sz=false,bool const scc_print=true) const{
            if(cmp_num)cout<<"cmp_num:";
            cout<<comp_num<<"\n";
            for(int const cid:order){
                if(cmp_num)cout<<cid<<":";
                if(cmp_sz)cout<<comp_size[cid];
                cout<<(cmp_num?"[":" ");
                for(int i=0;i<comp_size[cid];i++)cout<<comps[cid][i]<<(i==comp_size[cid]-1?"":" ");
                if(cmp_num)cout<<"]";
                cout<<"\n";
            }
            if(scc_print){
                cout<<"\n";
                dag.debug();
            }
        }
};

struct two_sat{
    private:
        int num;
        graph gr;
        bool solved;
        vector<int> answer;

        // a = true → x
        // a = false → x+n
        int id(int const x,bool const a) const{
            return x+(a?0:num);
        }

    public:
        explicit two_sat(int n) : num(n), gr(2*n), solved(false), answer(n,0){}

        // add clause: (x = a) OR (y = b)
        void add_clause_or(int const x,bool const a,int const y,bool const b){
            solved=false;
            int const X=id(x,a);
            int const NX=id(x,!a);
            int const Y=id(y,b);
            int const NY=id(y,!b);

            gr.add_edge(NX,Y); // ¬a → b
            gr.add_edge(NY,X); // ¬b → a
        }

        bool solve(){
            scc const s(gr,true);
            vector<int> const&cid=s.get_comp_id();

            for(int i=0;i<num;i++){
                if(cid[i]==cid[i+num]){
                    return false;
                }
                answer[i]=(cid[i]>cid[i+num]);
            }
            return solved=true;
        }

        vector<int> get_answer() {
            if(!solved) solve();
            return answer;
        }

        //
        //
        //
        //
        //

        // forbid: (x = a) AND (y = b)
        void forbid_pair(int x,bool a,int y,bool b){
            add_clause_or(x,!a,y,!b); // ¬(x=a) OR ¬(y=b)
        }

        // add condition: x = a (true or false)
        void force(int const x,bool const a){
            add_clause_or(x,a,x,a);
        }

        // add condition: x = y
        void force_equal(int const x,int const y){
            add_clause_or(x,false,y,true); // ¬x ∨ y
            add_clause_or(y,false,x,true); // ¬y ∨ x
        }

        // add condition: x != y (x xor y)
        void force_not_equal(int const x,int const y){
            add_clause_or(x,true,y,true);   //  x ∨ y
            add_clause_or(x,false,y,false); // ¬x ∨ ¬y
        }

        // forbid: P1 AND P2 AND ... AND Pk
        // Pi : (xi, ai)
        void forbid_all(const vector<pair<int,bool>>&P){
            int k=P.size();
            if(k==1){
                force(P[0].first,!P[0].second);
                return;
            }
            if(k==2){
                forbid_pair(P[0].first,P[0].second,
                            P[1].first,P[1].second);
                return;
            }

            vector<int> t(k-1);
            for(int i=0;i<k-1;i++){
                t[i]=num++;
                gr.add_nodes(2*num);
            }

            // (¬A1 ∨ t1)
            add_clause_or(P[0].first,!P[0].second,t[0],true);

            for(int i=1;i<k-1;i++){
                // (¬Ai ∨ ti)
                add_clause_or(P[i].first,!P[i].second,t[i],true);
                // (¬t(i-1) ∨ ti)
                add_clause_or(t[i-1],false,t[i],true);
            }

            // (¬Ak ∨ ¬t(k-2))
            add_clause_or(P[k-1].first,!P[k-1].second,t[k-2],false);
        }

        // at most one: P1,P2,...,Pk (true: 0 or 1)
        // Pi : (xi, ai)
        void at_most_one(const vector<pair<int,bool>>&P){
            int const n=P.size();
            if(n<=1) return;

            vector<int> s(n-1);
            for(int i=0;i<n-1;i++){
                s[i]=num++;
                gr.add_nodes(2*num);
            }

            add_clause_or(P[0].first,!P[0].second,s[0],true); // P1 → s1
            for(int i=1;i<n;i++){
                add_clause_or(P[i].first,!P[i].second,s[i-1],true); // Pi → si
                if(i<n-1)
                    add_clause_or(s[i-1],false,s[i],true);           // si-1 → si
                add_clause_or(P[i].first,!P[i].second,s[i-1],false); // Pi → ¬s(i-1)
            }
        }

        // at least one: P1,P2,...,Pk (true: 1 or more)
        // Pi : (xi, ai)
        void at_least_one(const vector<pair<int,bool>>&P){
            // forbid ¬P1 ∧ ¬P2 ∧ ... ∧ ¬Pn
            vector<pair<int,bool>> neg;
            neg.reserve(P.size());
            for(auto [x,a]:P){
                neg.emplace_back(x,!a);
            }
            forbid_all(neg);
        }
};

/*



*/

int main(){
    // ALPC_H
    int n,d;
    cin>>n>>d;
    vector<long> x(n),y(n);
    for(int i=0;i<n;i++)cin>>x[i]>>y[i];
    two_sat ts(n);
    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            if(labs(x[i]-x[j])<d)ts.forbid_pair(i,true,j,true);
            if(labs(x[i]-y[j])<d)ts.forbid_pair(i,true,j,false);
            if(labs(y[i]-x[j])<d)ts.forbid_pair(i,false,j,true);
            if(labs(y[i]-y[j])<d)ts.forbid_pair(i,false,j,false);
        }
    }

    if(!ts.solve()){
        cout<<"No"<<endl;
        return 0;
    }

    cout<<"Yes"<<endl;
    vector<int> ans=ts.get_answer();
    for(int i=0;i<n;i++)cout<<(ans[i]?x[i]:y[i])<<"\n";
    return 0;
}
