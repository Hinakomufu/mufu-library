#include <bits/stdc++.h>
using namespace std;

long lbs(){

    // false -> true
    // ...,false,false,[true],true,true...
    auto f=[&](long const x){
        return x*x>=5;
    };

    long l=0;
    long r=9e18;        // check overflows
    if(f(l))return l;   // check when f(l)=true
    if(!f(r))return -1; // check when f(r)=false

    while(l<r){
        long const m=l+(r-l+1)/2;
        if(f(m)) r=m;
        else l=m+1;
        //cout<<l<<" "<<r<<'\n';
    }
    return l;
}

int main(){
    cout<<lbs()<<endl;
    return 0;
}
