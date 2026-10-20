#include <bits/stdc++.h>
using namespace std;

long double f(long double x){
    return (x-5.0)*(x-5.0)+3.0;
}

long double ts(function<long double(long double)> const&f){
    long double l=0;
    long double r=1e9;

    long double h=1e-3;
    vector<long double>xs={1.0,10.0,100.0};
    int d=0;
    for(long double x:xs)d+=f(x+h)-2*f(x)+f(x-h)>0.0;
    bool dir=d>0;

    while(r-l>1e-12){
        long double m1=(l*2+r)/3;
        long double m2=(l+r*2)/3;

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
    return f((l+r)/2);
}


int main(){
    cout<<fixed<<setprecision(12)<<ts(f)<<endl;
    return 0;
}
