#include <bits/stdc++.h>
using namespace std;

//ABC430_E

vector<int> z_algorithm(const string &s) {
    int n=s.size();
    vector<int> z(n);
    int l=0,r=0;
    for(int i=1;i<n;i++) {
        if(i<=r) z[i]=min(r-i+1,z[i-l]);
        while(i+z[i]<n&&s[z[i]]==s[i+z[i]]) z[i]++;
        if(i+z[i]-1>r) l=i,r=i+z[i]-1;
    }
    return z;
}

int main() {

    int t;
    cin>>t;
    while(t--) {
        string a,b,s;
        cin>>a>>b;
        s=b+'#'+a+a;
        vector z=z_algorithm(s);

        int ans=1e9;
        int bl=b.length();
        int sl=s.length();

        for(int i=bl+1;i<=sl-bl;i++) {
            if(z[i]>=bl){
                ans=min(ans,i-bl-1);
            }
        }
        cout<<(ans==1e9?-1:ans)<<endl;
    }
    return 0;
}
