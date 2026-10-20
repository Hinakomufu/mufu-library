#include <bits/stdc++.h>
using namespace std;

long f(long x){
    return (x-5)*(x-5)+3;
}

long ts(function<long(long)> const&f){
    long l=0;
    long r=1e9;

    long h=1;
    vector<long> xs={1,10,100};
    long d=0;
    for(long x:xs)d+=f(x+h)-2*f(x)+f(x-h)>0;
    bool dir=d>0;

    while(r-l>3){
        long m1=(l*2+r)/3;
        long m2=(l+r*2)/3;

        if(dir){
            // f''(x)>0
            if(f(m1)>f(m2)) l=m1;
            else r=m2;
        } else{
            // f''(x)<0
            if(f(m1)<f(m2)) l=m1;
            else r=m2;
        }
    }
    long best=f(l);
    for(long i=l;i<=r;i++){
        best=min(best,f(i));
    }
    return best;
}


int main(){
    cout<<ts(f)<<endl;
    return 0;
}
