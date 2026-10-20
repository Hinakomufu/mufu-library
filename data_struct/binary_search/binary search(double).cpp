#include <bits/stdc++.h>
using namespace std;

long double dbs(long const n) {

    auto f=[&](long double const x){
        return x*x*x+x>=n;
    };

    long double l=0;
    long double r=1e6;
    bool const bl=f(l);
    bool const br=f(r);
    if(bl==br)return (br?r:-1);
    bool const dir=!bl&&br;

    while(r-l>1e-12) {
        long double const m=(l+r)/2;
        if(dir) {
            // false→true
            if(f(m)) r=m;
            else l=m;
        } else {
            // true→false
            if(f(m)) l=m;
            else r=m;
        }
    }
    return dir?r:l;
}

int main() {

    long n;
    cin>>n;
    cout<<fixed<<setprecision(12)<<dbs(n)<<endl;
    return 0;
}
