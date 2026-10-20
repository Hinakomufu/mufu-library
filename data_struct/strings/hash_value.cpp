#include <bits/stdc++.h>
using namespace std;

constexpr long base1=911,mod1=1e9+7;
constexpr long base2=3571,mod2=1e9+9;

struct hash_string {
    vector<long> h1,h2,pow1,pow2;

    hash_string(const string &s) {
        int n=s.size();
        h1.assign(n+1,0);
        h2.assign(n+1,0);
        pow1.assign(n+1,1);
        pow2.assign(n+1,1);
        for(int i=0;i<n;i++) {
            h1[i+1]=(h1[i]*base1+s[i])%mod1;
            h2[i+1]=(h2[i]*base2+s[i])%mod2;
            pow1[i+1]=(pow1[i]*base1)%mod1;
            pow2[i+1]=(pow2[i]*base2)%mod2;
        }
    }

    pair<long,long> get_hash(int l,int r) const {
        long x1=(h1[r]-h1[l]*pow1[r-l]%mod1+mod1)%mod1;
        long x2=(h2[r]-h2[l]*pow2[r-l]%mod2+mod2)%mod2;
        return {x1,x2};
    }
};

int main() {
    int t;
    cin>>t;
    while(t--) {
        string a,b;
        cin>>a>>b;
        a+=a;
        int n=a.size();
        DoubleHash da(a);
        DoubleHash db(b);
        auto target=db.get_hash(0,b.size());

        int ans=-1;
        for(int i=0;i<n;i++) {
            if(da.get_hash(i,i+n)==target) {
                ans=i;
                break;
            }
        }
        cout<<ans<<endl;
    }
    return 0;
}
