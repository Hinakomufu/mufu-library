#include <bits/stdc++.h>
using namespace std;

int check(const pair<long,long>&p){
    if(p.second>0) return 0;
    if(p.second==0&&p.first>=0) return 0;
    return 1;
}

bool angle_cmp(const pair<long,long>&a,const pair<long,long>&b){
    int ca=check(a),cb=check(b);
    if(ca!=cb) return ca<cb;
    return a.first*b.second-a.second*b.first<0;
}

int main(){
    // suit for ABC442_E
    long n,q;
    cin>>n>>q;
    vector<pair<long,long>> v(n);
    for(long i=0;i<n;i++){
        cin>>v[i].first>>v[i].second;
    }

    vector w=v;
    sort(w.begin(),w.end(),angle_cmp);

    while(q--){
        long a,b;
        cin>>a>>b;
        a--;
        b--;

        auto A=v[a];
        auto B=v[b];

        if(!angle_cmp(B,A)){
            long l=lower_bound(w.begin(),w.end(),A,angle_cmp)-w.begin();
            long r=upper_bound(w.begin(),w.end(),B,angle_cmp)-w.begin();
            cout<<r-l<<endl;
        } else{
            long l=lower_bound(w.begin(),w.end(),A,angle_cmp)-w.begin();
            long r1=w.size();
            long r2=upper_bound(w.begin(),w.end(),B,angle_cmp)-w.begin();
            cout<<r1+r2-l<<endl;
        }

    }

    return 0;
}
