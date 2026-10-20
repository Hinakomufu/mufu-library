#include <bits/stdc++.h>
using namespace std;

int main() {
    long n;
    cin>>n;
    vector<long> divisors;
    for(long i=1;i*i<=n;++i) {
        if(n%i==0) {
            divisors.push_back(i);
            if(i!=n/i) divisors.push_back(n/i);
        }
    }

    sort(divisors.begin(), divisors.end());
    for(long x:divisors) {
        cout<<x<<" ";
    }
    cout<<endl;
    return 0;
}
