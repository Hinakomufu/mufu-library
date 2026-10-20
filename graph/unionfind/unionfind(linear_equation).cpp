#include <bits/stdc++.h>
using namespace std;

template<class T>
struct unionfind{
    private:
        struct node_info{ // A[x]=a_val*A[parent[x]]+b_val
            int node_count=1;
            int edge_count=0;
            T a_val=1;
            T b_val=0;
            T parameter=0;
            bool is_specified=false;
        };

        vector<int> parent;
        vector<node_info> info;
        int components=0;
        int n;

        int root(int const x){
            int const p=parent[x];
            if(p==-1) return x;
            int const r=root(p);

            T ax=info[x].a_val;
            T bx=info[x].b_val;
            T ap=info[p].a_val;
            T bp=info[p].b_val;
            // A[x]=ax*A[p]+bx , A[p]=ap*A[r]+bp
            // => A[x]=ax*ap*A[r]+ax*bp+bx
            info[x].a_val=ax*ap;
            info[x].b_val=ax*bp+bx;
            return parent[x]=r;
        }

    public:
        explicit unionfind(int const n_):
            parent(n_,-1), info(n_), components(n_), n(n_){}

        int add_node(){
            parent.push_back(-1);
            info.push_back(node_info());
            components++;
            // return number of new node
            return n++;
        }

        bool is_connected(int const x,int const y){
            return root(x)==root(y);
        }

        pair<T,T> get_coef(int x){
            root(x);
            return {info[x].a_val,info[x].b_val};
        }

        bool add_edge(int x,int y,T coef_x,T coef_y,T value){
            // coef_x*A[x]+coef_y*A[y]=value
            if(coef_x==0||coef_y==0) return false;
            int rx=root(x);
            int ry=root(y);
            auto [px,qx]=get_coef(x);
            auto [py,qy]=get_coef(y);

            if(info[rx].is_specified&&info[ry].is_specified){
                T Ax=px*info[rx].parameter+qx;
                T Ay=py*info[ry].parameter+qy;
                return (coef_x*Ax+coef_y*Ay==value);
            }
            if(rx==ry){
                T ca=coef_x*px+coef_y*py;
                T cb=value-(coef_x*qx+coef_y*qy);
                if(ca==0) return (cb==0);
                info[rx].is_specified=true;
                info[rx].parameter=cb/ca;
                return true;
            }

            if(info[rx].node_count<info[ry].node_count){
                swap(rx,ry);
                swap(x,y);
                swap(coef_x,coef_y);
                swap(px,py);
                swap(qx,qy);
            }

            parent[ry]=rx;
            info[rx].node_count+=info[ry].node_count;
            info[rx].edge_count+=info[ry].edge_count+1;
            components--;

            // A[x]=px*A[rx]+qx, A[y]=py*A[ry]+qy
            // coef_x*A[x]+coef_y*A[y]=value -> A[ry]=a_val*A[rx]+b_val
            info[ry].a_val=(-coef_y*px)/(coef_x*py);
            info[ry].b_val=(value-coef_x*qx-coef_y*qy)/(coef_y*py);

            if(info[ry].is_specified){
                info[rx].is_specified=true;
                info[rx].parameter=(info[ry].parameter-info[ry].b_val)/info[ry].a_val;
            }

            return true;
        }

        int group_node_count(int const x=0){
            return info[root(x)].node_count;
        }

        int group_edge_count(int const x=0){
            return info[root(x)].edge_count;
        }

        vector<int> get_component_nodes(int const x){
            int const r=root(x);
            vector<int> nodes;
            for(int i=0;i<n;i++){
                if(root(i)==r) nodes.push_back(i);
            }
            return nodes;
        }

        int group_count() const{
            return components;
        }

        T val(int const x=0,T const param=0){
            int r=root(x);
            auto [px, qx]=get_coef(x);
            if(info[r].is_specified)return px*info[r].parameter+qx;
            return px*param+qx;
        }

        bool set_val(int const x,T v){
            int rx=root(x);
            auto [px, qx]=get_coef(x);
            T Ax=px*v+qx;
            if(info[rx].is_specified)return (Ax==info[rx].parameter);
            info[rx].is_specified=true;
            info[rx].parameter=Ax;
            return true;
        }

        tuple<bool,T,T> solve_coef(int const x,int const y){
            if(root(x)!=root(y))return {false,0,0};
            auto [px, qx]=get_coef(x);
            auto [py, qy]=get_coef(y);
            // A[x]=px*A[r]+qx
            // A[y]=py*A[r]+qy
            // return a,b such that A[x]=a*A[y]+b.
            return {true,px/py,(qx*py-px*qy)/py};
        }

        tuple<bool,T> solve_linear(int const x,int const y,T coef_x,T coef_y){
            int r=root(x);
            if(r!=root(y)) return {false,0};

            auto [_,a,b]=solve_coef(x,y); // A[x]=a*A[y]+b
            T coefficient=coef_x*a+coef_y;
            if(coefficient==0)return {true,coef_x*b};

            if(!info[r].is_specified) return {false,0};
            T Ay=val(y);
            return {true,coefficient*Ay+coef_x*b};
        }
};

/*
    !!!
    if coef_x or coef_y can be values other than ±1, T=long cannot be used.
    !!!

    solve_coef(x,y):
    A[x]=px*A[r]+qx
    A[y]=py*A[r]+qy
    if A[x] and A[y] are connected,
    return true,a,b such that A[x]=a*A[y]+b.
    if not connected, return false.

    solve_linear(u,v,coef_x,coef_y):
    if the value of the linear expression is uniquely determined,
    return true and val such that val=cx*A[x]+cy*A[y].
    if not, return false.

    auto [f,a,b]=uf.solve_coef(x,y);
    auto [f,ans]=uf.solve_linear(u,v,coef_x,coef_y);

    coef_x*A[x]+coef_y*A[y]=value
    (coef_x,coef_y,value)
    v=u+w -> (1,-1,w)
    u+v=w -> (1,1,w)
    v=u   -> (1,-1,0)
*/

int main(){
    // tessoku 068
    int n,q;
    cin>>n>>q;

    unionfind<long> uf(n); //0_indexed

    for(int i=0;i<q;i++){
        int t,x,y;
        long v;
        cin>>t>>x>>y>>v;
        x--;
        y--;
        if(t==0)uf.add_edge(x,y,1,1,v); // u+v=w
        else{
            auto [f,a,b]=uf.solve_coef(x,y);
            if(f)cout<<(v-b)/a<<endl;
            else cout<<"Ambiguous"<<endl;
        }
    }

    return 0;
}
