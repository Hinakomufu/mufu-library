#include <bits/stdc++.h>
using namespace std;

struct wavelet_matrix{
    private:
        int height;
        int len;
        vector<vector<int>> prefix_one;

        int range_kth_smallest(int l,int r,int k) const{ // k:0-index
            ++r;
            int ans=0;
            for(int h=height-1;h>=0;--h){
                int const ones=prefix_one[h][r]-prefix_one[h][l];
                int const zeros=(r-l)-ones;
                if(k<zeros){
                    l-=prefix_one[h][l];
                    r-=prefix_one[h][r];
                }
                else{
                    ans|=1<<h;
                    k-=zeros;
                    int const mid=len-prefix_one[h][len];
                    l=mid+prefix_one[h][l];
                    r=mid+prefix_one[h][r];
                }
            }
            return ans;
        }

        int range_count_less(int l,int r,int const x) const{
            ++r;
            int ans=0;
            for(int h=height-1;h>=0;--h){
                int const ones=prefix_one[h][r]-prefix_one[h][l];
                int const zeros=(r-l)-ones;
                int const bit=x>>h&1;
                if(bit==0){
                    l-=prefix_one[h][l];
                    r-=prefix_one[h][r];
                }
                else{
                    ans+=zeros;
                    int const mid=len-prefix_one[h][len];
                    l=mid+prefix_one[h][l];
                    r=mid+prefix_one[h][r];
                }
            }
            return ans;
        }


    public:
        wavelet_matrix(vector<int> arr): height(0), len(arr.size()){
            int max_value=0;
            for(int const x:arr){ max_value=max(max_value,x); }
            while(max_value>=(1<<height)){ ++height; }
            prefix_one.resize(height);

            for(int h=height-1;h>=0;--h){
                vector<int> bit(len);
                vector<int> L,R;

                for(int i=0;i<len;++i){
                    bit[i]=arr[i]>>h&1;
                    if(bit[i]==0) L.emplace_back(arr[i]);
                    else R.emplace_back(arr[i]);
                }

                int const l_size=L.size();
                int const r_size=R.size();
                for(int i=0;i<l_size;++i){ arr[i]=L[i]; }
                for(int i=0;i<r_size;++i){ arr[l_size+i]=R[i]; }
                prefix_one[h].resize(len+1);
                for(int i=0;i<len;++i){ prefix_one[h][i+1]=prefix_one[h][i]+bit[i]; }
            }
        }

        struct range{
            wavelet_matrix const& wm;
            int l;
            int r;

            int kth_smallest(int const k) const{ // k:0-index
                assert(0<=k && k<=r-l);
                return wm.range_kth_smallest(l,r,k);
            }

            int kth_largest(int const k) const{ // k:0-index
                assert(0<=k && k<=r-l);
                return wm.range_kth_smallest(l,r,r-l-k);
            }

            int count_less(int const x) const{return wm.range_count_less(l,r,x);}

        };

        range operator()(int const l,int const r) const{
            assert(0<=l && l<=r && r<len);
            return {*this,l,r};
        }
};

int main(){
    int n,q;
    cin>>n>>q;
    vector<int>a(n);
    for(int i=0;i<n;i++)cin>>a[i];
    wavelet_matrix const wm(a);
    while(q--){
        int l,r,x;
        cin>>l>>r>>x;
        cout<<wm(l,r-1).count_less(x)<<endl;
    }

}
