#include <bits/stdc++.h>
using namespace std;

long lbs(){

    // true -> false
    // ...,true,true,[true],false,false,...
    auto f=[&](long const x){
        return x*x>=5;
    };

    long l=0;
    long r=9e18;        // check overflows
    if(!f(l))return -1; // check when f(l)=false
    if(f(r))return r;   // check when f(r)=true

    while(l<r){
        long const m=l+(r-l+1)/2;
        if(f(m)) l=m;
        else r=m-1;
        //cout<<l<<" "<<r<<'\n';
    }
    return l;
}

int main(){
    cout<<lbs()<<endl;
    return 0;
}
