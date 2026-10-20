#include <bits/stdc++.h>
using namespace std;

// only for x<1e6; if x>1e6, use longint

// sum in [0,x] of floor((an+b)/c) (c!=0)
long floor_sum(long const&x,long a,long b,long const&m){
    function<long(long,long)> div_floor=[&](long const p,long const q){
        if(q<0)return div_floor(-p,-q);
        if(p>=0) return p/q;
        return -((-p+q-1)/q);
    };
    function<long(long,long)> mod_floor=[&](long const p,long const q){
        if(q<0) return mod_floor(p,-q);
        long r=p%q;
        if(r<0) r+=q;
        return r;
    };

    if(m==0)return 0;
    if(m<0)return floor_sum(x,-a,-b,-m);
    if(a<0)return -floor_sum(x,-a,m-b-1,m);
    if(a==0)return (x+1)*div_floor(b,m);
    long res=0;
    if(a>=m){
        res+=x*(x+1)*div_floor(a,m)/2;
        a=mod_floor(a,m);
    }
    if(b>=m||b<0){
        res+=(x+1)*div_floor(b,m);
        b=mod_floor(b,m);
    }
    long const y=div_floor(a*x+b,m);
    long const z=mod_floor(a*x+b,m);
    if(y!=0)res+=y+floor_sum(y-1,m,z,a);
    return res;
}

// sum in [l,r] of floor((an+b)/m)
long range_floor_sum(long const l,long const r,long const a,long const b,long const m){
    long val=0;
    if(l<=r)val+=floor_sum(r,a,b,m);
    if(l>0)val-=floor_sum(l-1,a,b,m);
    return val;
}

int main(){
    // ARC111E
    int t;
    cin>>t;
    while(t--){
        long a,b,c,d;
        cin>>a>>b>>c>>d;
        long max_i=(d-2)/(c-b);
        cout<<max_i-floor_sum(max_i,c,a,d)+floor_sum(max_i,b,a-1,d)<<endl;
    }
    return 0;
}
