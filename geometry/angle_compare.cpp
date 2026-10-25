//https://github.com/Hinakomufu/mufu-library/blob/main/geometry/angle_compare.cpp
#include <bits/stdc++.h>
using namespace std;

// Sort by polar angle (by counterclockwise)
// zero_2pi=true -> [0,2pi),zero_2pi=false -> (-pi,pi]
// if(angle_cmp(a,b)), a comes before b in polar angle order
bool angle_comparison(const pair<long,long>&a,const pair<long,long>&b,
                      const bool counterclockwise=true,const bool zero_2pi=true){
    auto group=[&](const pair<long,long>&p){
        if(p.second<0) return (zero_2pi?4:0); // Negative angle
        if(p.second==0&&p.first>=0) return 1; // Angle zero
        if(p.second>0) return 2;              // Positive angle
        return 3;                             // Angle pi
    };

    int const ca=group(a);
    int const cb=group(b);
    if(ca!=cb) return (counterclockwise?ca<cb:ca>cb);

    if(a.second==0&&b.second==0) return false; // a:0 or pi
    long const cross=a.first*b.second-a.second*b.first;
    return (counterclockwise?cross>0:cross<0);
}

bool angle_cmp(const pair<long,long>&a,const pair<long,long>&b){
    return angle_comparison(a,b,false,false);
}

int main(){
    // ABC442_E
    long n,q;
    cin>>n>>q;
    vector<pair<long,long>> v(n);
    for(long i=0;i<n;i++)cin>>v[i].first>>v[i].second;

    vector w=v;
    sort(w.begin(),w.end(),angle_cmp);
    long s=w.size();

    while(q--){
        long a,b;
        cin>>a>>b;
        a--;
        b--;

        auto A=v[a];
        auto B=v[b];

        long l=lower_bound(w.begin(),w.end(),A,angle_cmp)-w.begin();
        long r=upper_bound(w.begin(),w.end(),B,angle_cmp)-w.begin();
        if(!angle_cmp(B,A))cout<<r-l<<endl;
        else cout<<s+r-l<<endl;
    }

    return 0;
}
