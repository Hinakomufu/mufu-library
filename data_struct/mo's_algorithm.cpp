#include <bits/stdc++.h>
using namespace std;

struct mo{
    private:
        struct query{
            int l,r,id;
            query(int const l=0,int const r=0,int const id=0): l(l), r(r), id(id){}
        };

        int n,block,q;
        bool sorted;
        vector<query> queries;

        void sort_query(){
            sort(queries.begin(),queries.end(),[&](const query&a,const query&b){
                int const ab=a.l/block;
                int const bb=b.l/block;
                if(ab!=bb) return ab<bb;
                return (ab&1)?(a.r>b.r):(a.r<b.r);
            });
            sorted=true;
        }

    public:
        mo(int const n) : n(n){
            block=max(1,static_cast<int>(sqrt(n)));
            q=0;
            sorted=false;
        }

        void add_query(int l,int r){
            queries.emplace_back(l,r,q++);
            sorted=false;
        }

        template<class OPS>
        void run(OPS&ops){
            if(!sorted)sort_query();
            int L=0,R=-1; // [L,R]

            for(auto&q:queries){
                while(L>q.l) ops.add(--L);
                while(R<q.r) ops.add(++R);
                while(L<q.l) ops.remove(L++);
                while(R>q.r) ops.remove(R--);
                ops.out(q.id);
            }
        }
};

/*





*/

struct operation{
    vector<int>&a;
    vector<int>freq;
    long pairs;
    vector<long>ans;

    operation(vector<int>&a,int q)
        : a(a), pairs(0), ans(q){
        int const mx=*max_element(a.begin(),a.end());
        freq.assign(mx+1,0);
    }

    void add(int const i){
        int x=a[i];
        pairs-=freq[x]/2;
        freq[x]++;
        pairs+=freq[x]/2;
    }

    void remove(int const i){
        int x=a[i];
        pairs-=freq[x]/2;
        freq[x]--;
        pairs+=freq[x]/2;
    }

    void out(int const id){
        ans[id]=pairs;
    }
};

int main(){
    // ABC242G
    int n;
    cin>>n;
    vector<int> a(n);
    for(int i=0;i<n;i++)cin>>a[i];

    int q;
    cin>>q;
    mo m(n);
    for(int i=0;i<q;i++){
        int l,r;
        cin>>l>>r;
        l--,r--;
        m.add_query(l,r);
    }

    operation op(a,q);
    m.run(op);
    for(int i=0;i<q;i++)cout<<op.ans[i]<<endl;
    return 0;
}
