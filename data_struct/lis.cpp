#include<bits/stdc++.h>
using namespace std;

struct lis{
    private:
        vector<long> a;
        int n;
        bool increasing;
        bool strict_val;

        vector<int> L,R;
        int len;
        vector<int> pos;
        vector<vector<int>> cand;

        void compute_dp(vector<int>&D,bool const increase,bool const strict,bool const forward) const{
            vector<long> dp;
            dp.reserve(n);

            int const start=forward?0:n-1;
            int const end=forward?n:-1;
            int const step=forward?1:-1;

            for(int i=start;i!=end;i+=step){
                long x=increase?a[i]:-a[i];

                auto it=strict
                            ?lower_bound(dp.begin(),dp.end(),x)
                            :upper_bound(dp.begin(),dp.end(),x);

                int const k=it-dp.begin();
                if(it==dp.end()) dp.push_back(x);
                else *it=x;

                D[i]=k;
            }
        }

        void compute_all(){
            compute_dp(L,increasing,strict_val,true);
            compute_dp(R,!increasing,strict_val,false);

            len=*max_element(L.begin(),L.end())+1;
            cand.resize(len);

            for(int i=0;i<n;i++){
                if(L[i]+R[i]+1==len){
                    pos[i]=L[i];
                    cand[pos[i]].push_back(i);
                } else pos[i]=-1;
            }
        }

    public:
        explicit lis(const vector<long>&a_,bool const increasing_=true,bool const strict_=true)
            : a(a_), n(a_.size()), increasing(increasing_), strict_val(strict_),
              L(n), R(n), pos(n,-1){
            compute_all();
        }

        int length() const{ return len; }

        const vector<int>&positions() const{ return pos; }

        vector<int> use_position(bool const must_only=false) const{
            vector<int> res;

            if(must_only){
                vector<vector<int>> bucket(len);
                for(int i=0;i<n;i++){
                    if(L[i]+R[i]+1==len){
                        bucket[L[i]].push_back(i);
                    }
                }
                for(int j=0;j<len;j++){
                    if(bucket[j].size()==1){
                        res.push_back(bucket[j][0]);
                    }
                }
            } else{
                for(int i=0;i<n;i++){
                    if(pos[i]!=-1){
                        res.push_back(i);
                    }
                }
            }
            sort(res.begin(),res.end());
            return res;
        }

        vector<int> compute_lis(bool const increase=true,bool const strict=true, bool const forward=true) const {
            vector<int> D(n);
            compute_dp(D, increase,strict, forward);
            return D;
        }

        vector<long> restore(bool const lexi_min=true) const{
            vector<long> res;
            res.reserve(len);
            int last_idx=-1;

            for(int j=0;j<len;j++){
                long best_val=lexi_min?LONG_MAX:LONG_MIN;
                int best_idx=-1;

                for(int const i:cand[j]){
                    if(i<=last_idx) continue;
                    long const v=a[i];
                    if(lexi_min){
                        if(v<best_val){
                            best_val=v;
                            best_idx=i;
                        }
                    } else{
                        if(v>best_val){
                            best_val=v;
                            best_idx=i;
                        }
                    }
                }
                res.push_back(best_val);
                last_idx=best_idx;
            }
            return res;
        }

};

int main(){
    // ABC354F
    int t;
    cin>>t;
    while(t--){
        int n;
        cin>>n;
        vector<long> a(n);
        for(int i=0;i<n;i++)cin>>a[i];
        lis l(a);
        vector v=l.use_position();
        int s=v.size();
        cout<<s<<endl;
        for(int i=0;i<s;i++)cout<<v[i]+1<<(i==s-1?"\n":" ");
    }
    return 0;
}
