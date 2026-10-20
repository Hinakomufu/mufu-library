#include <bits/stdc++.h>
using namespace std;
constexpr long inf=1L<<62;

struct line_equation{
    private:
        long a,b; // y=ax+b
    public:
        line_equation() : a(0), b(0){}
        line_equation(long const a_,long const b_) : a(a_), b(b_){}

        bool operator<(const line_equation&other) const{
            if((this)->slope()!=other.slope())return (this)->slope()<other.slope();
            return (this)->intercept_y()>other.intercept_y();
        }

        long slope() const{ return a; }
        long intercept_y() const{ return b; }
        long y_val(long const x) const{ return a*x+b; }
        void debug() const{ cout<<a<<" "<<b<<endl; }
};

struct Min{
    static bool is_better(long const&x,long const&y){ return x<y; }
    static long id(){ return inf; }
};

struct Max{
    static bool is_better(long const&x,long const&y){ return x>y; }
    static long id(){ return -inf; }
};

template<typename M>
struct li_chao_tree{
    private:
        static constexpr long x_min=static_cast<long>(-1e9);
        static constexpr long x_max=static_cast<long>(1e9);

        struct Node{
            line_equation line;
            int left=0;
            int right=0;
        };

        vector<Node> tree;
        int root=0;

        int new_node(){
            tree.emplace_back();
            return static_cast<int>(tree.size())-1;
        }

        int add_line_impl(line_equation L,int k,long const l,long const r){
            if(k==0){
                k=new_node();
                tree[k].line=L;
                return k;
            }
            long m=l+(r-l)/2;
            if(M::is_better(L.y_val(m),tree[k].line.y_val(m))){
                swap(L,tree[k].line);
            }
            if(l==r) return k;
            if(M::is_better(L.y_val(l),tree[k].line.y_val(l))){
                tree[k].left=add_line_impl(L,tree[k].left,l,m);
            } else if(M::is_better(L.y_val(r),tree[k].line.y_val(r))){
                tree[k].right=add_line_impl(L,tree[k].right,m+1,r);
            }
            return k;
        }

        long calc_impl(long const x,int const k,long const l,long const r) const{
            if(k==0) return M::id();
            long res=tree[k].line.y_val(x);
            if(l==r) return res;
            long const m=l+(r-l)/2;
            if(x<=m){
                long tmp=calc_impl(x,tree[k].left,l,m);
                if(M::is_better(tmp,res)) res=tmp;
            } else{
                long tmp=calc_impl(x,tree[k].right,m+1,r);
                if(M::is_better(tmp,res)) res=tmp;
            }
            return res;
        }

    public:
        li_chao_tree(){
            tree.reserve(1<<20);
            tree.emplace_back();
        }

        void add_line(line_equation const&L){
            root=add_line_impl(L,root,x_min,x_max);
        }

        void add_line_function(long const a,long const b){ // y=ax+b
            root=add_line_impl(line_equation(a,b),root,x_min,x_max);
        }

        long calc(long const x) const{
            return calc_impl(x,root,x_min,x_max);
        }

        long operator()(long const x) const{ return calc(x); }
};

int main(){
    // EDPC Z
    int n;
    long c;
    cin>>n>>c;

    vector<long> h(n);
    for(int i=0;i<n;i++)cin>>h[i];
    li_chao_tree<Min> tr;
    vector<long> dp(n);
    dp[0]=0;

    tr.add_line_function(-2*h[0],h[0]*h[0]+dp[0]);

    for(int i=1;i<n;i++){
        long x=h[i];
        dp[i]=x*x+c+tr(x); // min_j(-2*h[j]*x+h[j]^2+dp[j])
        tr.add_line_function(-2*x,x*x+dp[i]);
    }

    cout<<dp[n-1]<<endl;
}
