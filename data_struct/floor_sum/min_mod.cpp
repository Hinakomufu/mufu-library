#include <bits/stdc++.h>
using namespace std;

// only for x<1e6; if x>1e6, use longint

// Store the positions where the prefix minimum is updated
pair<vector<long>,vector<long>>min_mod_segments(long const a,long const b,long const m){
    // p/q<=(m-a)/m<=r/s
    long p=0,q=1;
    long r=1,s=1;

    long det_l=m-a;
    long det_r=a;

    long pos=0;
    long y=b;

    vector<long> X={0};
    vector<long> DX;

    while(y){
        // Update r/s
        long k=det_r/det_l;
        det_r%=det_l;
        if(det_r==0){
            --k;
            det_r=det_l;
        }
        r+=k*p;
        s+=k*q;
        while(true){
            // ceil((det_l-y)/det_r)
            k=max(0L,(det_l-y+det_r-1)/det_r);
            if(det_l-k*det_r<=0)break;
            det_l-=k*det_r;
            p+=k*r;
            q+=k*s;

            // aq-pm=det_l
            k=y/det_l;
            y-=k*det_l;
            pos+=q*k;
            X.push_back(pos);
            DX.push_back(q);
        }

        k=det_l/det_r;
        det_l-=k*det_r;
        p+=k*r;
        q+=k*s;
    }
    return {X,DX};
}

// min in [0,x] of (ai+b)mod m (m!=0)
long min_mod(long const&x,long a,long b,long m){
    function<long(long,long)> mod_floor=[&](long const p,long const q){
        if(q<0) return mod_floor(p,-q);
        long r=p%q;
        if(r<0) r+=q;
        return r;
    };

    if(x<0||m==0) return -1;
    if(m<0) return min_mod(x,a,b,-m);
    if(a<0) return min_mod(x,-a,a*x+b,m);
    a=mod_floor(a,m);
    b=mod_floor(b,m);
    if(a==0) return b;

    long g=gcd(a,m);
    long const bg=b%g;

    a/=g;
    b/=g;
    m/=g;

    auto[X,DX]=min_mod_segments(a,b,m);
    long pos=0;

    for(int i=0;i+1<static_cast<int>(X.size());++i){
        long const xl=X[i];
        long const xr=X[i+1];
        if(xr<=x){
            pos=xr;
            continue;
        }
        pos=xl+(x-pos)/DX[i]*DX[i];
        break;
    }
    long const reduced=(a*pos+b)%m;
    return bg+g*reduced;
}

int main(){
    int t;
    cin>>t;
    while(t--){
        long n,m,a,b;
        cin>>n>>m>>a>>b;
        cout<<min_mod(n-1,a,b,m)<<endl;
    }
    return 0;
}
