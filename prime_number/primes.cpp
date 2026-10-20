#include <bits/stdc++.h>
using namespace std;

struct prime{
    private:
        long limit;                 // n
        vector<long> prime_table;   // 0-n
        vector<long> spf_table;     // 0-n (smallest prime factor)
        vector<long> mobius_table;  // 1-n
        vector<long> totient_table; // 1-n
    public:
        explicit prime(long const n){
            limit=n;
            if(limit<2L)
                return;

            spf_table.assign(limit+1L,0);
            spf_table[1]=1;

            for(long i=2;i<=limit;i++){
                if(spf_table[i]==0){
                    spf_table[i]=i;
                    prime_table.push_back(i);
                }
                for(long const p:prime_table){
                    if(p>spf_table[i]||1L*p*i>limit)
                        break;
                    spf_table[p*i]=p;
                }
            }
            mobius_table.assign(limit+1,1);
            totient_table.assign(limit+1,0);
            for(long i=0;i<=limit;i++) totient_table[i]=i;
            for(long const p:prime_table){
                for(long x=p;x<=limit;x+=p){
                    mobius_table[x]=-mobius_table[x];
                    totient_table[x]-=totient_table[x]/p;
                }
                long const p2=p*p;
                if(p2<=limit){
                    for(long x=p2;x<=limit;x+=p2){
                        mobius_table[x]=0;
                    }
                }
            }
        }

        // true->prime
        bool judge(long const x) const{ // 0-n^2
            if(x<=1L)
                return false;
            if(x<=limit)
                return spf_table[x]==x;

            for(long const p:prime_table){
                if(1L*p*p>x)
                    break;
                if(x%p==0)
                    return false;
            }
            return true;
        }

        vector<bool> sieve_judge(long l,long r) const{
            if(l<2) l=2;
            if(r>limit*limit) r=limit;
            long const len=r-l+1;
            std::vector<bool> res(len,true);
            for(long const p:prime_table){
                long p2=1L*p*p;
                if(p2>r) break;
                long const start=max(p2,((l+p-1)/p)*p);
                for(long j=start;j<=r;j+=p){
                    res[j-l]=false;
                }
            }
            return res;
        }

        long sieve_count(long const l,long const r) const{
            long res=0;
            vector<bool> v=sieve_judge(l,r);
            for(bool const b:v)res+=b;
            return res;
        }

        vector<long> small_primes(long const bound) const{
            if(limit<bound) return prime_table;
            vector<long> res;
            for(long const p:prime_table){
                if(p>bound) break;
                res.push_back(static_cast<int>(p));
            }
            return res;
        }

};

prime prm(1e3);

int main(){
    return 0;
}
