#include <bits/stdc++.h>
using namespace std;

struct factor{
    private:
        struct pr{
            long prime;
            long exponent;
            pr(long const p,long const e) : prime(p), exponent(e){}

            auto operator<=>(pr const&) const = default;
        };

        vector<pr> pr_vec;
        bool normalized=false;

        void normalize(){
            sort(pr_vec.begin(),pr_vec.end(),[](auto const&a,auto const&b){
                return a.prime<b.prime;
            });

            vector<pr> nv;
            for(auto&p:pr_vec){
                if(!nv.empty()&&nv.back().prime==p.prime){
                    nv.back().exponent+=p.exponent;
                    if(nv.back().exponent==0){
                        nv.pop_back();
                    }
                } else{
                    if(p.exponent!=0)
                        nv.push_back(p);
                }
            }
            pr_vec.swap(nv);
            normalized=true;
        }

        factor square_free() const{
            factor r=*this;
            for(auto&t:r.pr_vec){
                t.exponent=1;
            }
            return r;
        }

    public:
        factor();

        factor(long x);

        vector<pr>::iterator begin(){ return pr_vec.begin(); }
        vector<pr>::iterator end(){ return pr_vec.end(); }
        vector<pr>::const_iterator begin() const{ return pr_vec.begin(); }
        vector<pr>::const_iterator end() const{ return pr_vec.end(); }

        void print(){
            if(!normalized)normalize();
            long const pr_cnt=pr_vec.size();
            for(long i=0;i<pr_cnt;i++){
                cout<<pr_vec[i].prime<<'^'<<pr_vec[i].exponent<<(i==pr_cnt-1?"\n":" ");
            }
        }

        long prime_count(){
            if(!normalized)normalize();
            return static_cast<long>(pr_vec.size());
        }

        long divisor_count(){
            if(!normalized)normalize();
            long res=1;
            for(auto const&t:pr_vec){
                if(t.exponent<0)return 0;
                res*=(t.exponent+1);
            }
            return res;
        }

        vector<long> primes(bool const unique_only=false){
            if(!normalized)normalize();
            factor tmp=*this;
            if(unique_only)tmp=tmp.square_free();
            vector<long> res;
            for(auto const&t:tmp.pr_vec){
                if(t.exponent<0)return {};
                for(long i=0;i<t.exponent;i++){
                    res.push_back(t.prime);
                }
            }
            return res;
        }

        vector<long> divisors(bool const unique_only=false){
            if(!normalized)normalize();
            for(auto const&t:pr_vec){
                if(t.exponent<0) return {};
            }
            factor tmp=*this;
            if(unique_only)tmp=tmp.square_free();
            vector<long> res;

            auto dfs=[&](auto&&self,size_t const idx,long const cur) ->void{
                if(idx==tmp.pr_vec.size()){
                    res.push_back(cur);
                    return;
                }
                auto [p, e]=tmp.pr_vec[idx];
                long v=1;
                for(long i=0;i<=e;i++){
                    self(self,idx+1,cur*v);
                    v*=p;
                }
            };

            dfs(dfs,0,1);

            sort(res.begin(),res.end());
            res.erase(unique(res.begin(),res.end()),res.end());

            return res;
        }

        factor lcm_with(factor&other){
            if(!normalized)normalize();
            if(!other.normalized)other.normalize();
            factor r;
            size_t i=0,j=0;
            while(i<pr_vec.size()||j<other.pr_vec.size()){
                if(j==other.pr_vec.size()||(i<pr_vec.size()&&pr_vec[i].prime<other.pr_vec[j].prime)){
                    r.pr_vec.push_back(pr_vec[i++]);
                } else if(i==pr_vec.size()||pr_vec[i].prime>other.pr_vec[j].prime){
                    r.pr_vec.push_back(other.pr_vec[j++]);
                } else{
                    r.pr_vec.emplace_back(pr_vec[i].prime,max(pr_vec[i].exponent,other.pr_vec[j].exponent));
                    i++,j++;
                }
            }
            return r;
        }

        factor gcd_with(factor&other){
            if(!normalized)normalize();
            if(!other.normalized)other.normalize();
            factor r;
            size_t i=0,j=0;
            while(i<pr_vec.size()&&j<other.pr_vec.size()){
                if(pr_vec[i].prime<other.pr_vec[j].prime){ i++; } else if(
                    pr_vec[i].prime>other.pr_vec[j].prime){ j++; } else{
                    r.pr_vec.emplace_back(pr_vec[i].prime,min(pr_vec[i].exponent,other.pr_vec[j].exponent));
                    i++,j++;
                }
            }
            return r;
        }

        template<class T=long>
        T val(){
            if(!normalized)normalize();
            T res=1;
            for(auto const&t:pr_vec){
                long p=t.prime;
                long e=t.exponent;
                if constexpr(!std::is_same_v<T,long>){
                    T base=T(p);
                    res*=base.pow(e);
                } else{ // T=long
                    if(e<0) return -1;
                    T pw=1;
                    long pp=p;
                    long ee=e;
                    while(ee>0){
                        if(ee&1){
                            if(pw>LLONG_MAX/pp) return -1;
                            pw*=pp;
                        }
                        if(ee>1){
                            if(pp>LLONG_MAX/pp) return -1;
                            pp*=pp;
                        }
                        ee>>=1;
                    }
                    if(res>LLONG_MAX/pw) return -1;
                    res*=pw;
                }
            }
            return res;
        }

        template<class T=long>
        T floor_i_root(int const i){
            if(!normalized)normalize();
            T n;

            if constexpr(std::is_same_v<T,long>){
                long tmp=this->val<long>();
                if(tmp<0) return -1;
                n=tmp;
            } else{
                n=this->val<T>();
            }

            auto ok=[&](T const&x){
                T v=1;
                for(int k=0;k<i;k++){
                    if constexpr(std::is_same_v<T,long>){
                        if(v>n/x) return false;
                    }
                    v*=x;
                }
                return v<=n;
            };

            T l=1,r=n,ans=1;
            while(l<=r){
                T m=l+(r-l)/2;
                if(ok(m)){
                    ans=m;
                    l=m+1;
                } else{
                    r=m-1;
                }
            }
            return ans;
        }

        bool is_nth_power(long const n){
            if(!normalized)normalize();
            if(n<=0) return false;
            for(auto const&t:pr_vec){
                if(t.exponent%n!=0)
                    return false;
            }
            return true;
        }

        factor operator*(factor const&other) const{
            factor r=*this;
            r.normalized=false;
            r.pr_vec.reserve(pr_vec.size()+other.pr_vec.size());
            r.pr_vec.insert(r.pr_vec.end(),other.pr_vec.begin(),other.pr_vec.end());
            if(pr_vec.size()>64)r.normalize();
            return r;
        }

        factor&operator*=(const factor&other){
            return *this=(*this)*other;
        }

        factor operator/(factor const&other) const{
            factor r=*this;
            r.normalized=false;
            r.pr_vec.reserve(pr_vec.size()+other.pr_vec.size());
            for(auto&p:other.pr_vec){
                r.pr_vec.emplace_back(p.prime,-p.exponent);
            }
            if(pr_vec.size()>64)r.normalize();
            return r;
        }

        factor&operator/=(factor const&other){
            return *this=(*this)/other;
        }

        factor pow(long const n) const{
            factor r=*this;
            r.normalized=false;
            for(auto&t:r.pr_vec){
                t.exponent*=n;
            }
            return r;
        }

        friend struct prime;
};

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
            vector<long> res;
            for(long const p:prime_table){
                if(p>bound) break;
                res.push_back(static_cast<int>(p));
            }
            return res;
        }

        factor factorize(long x) const{
            factor res;
            while(x>1){
                long p=(x<=limit?spf_table[x]:0);
                if(p==0){
                    for(long const q:prime_table){
                        if(1L*q*q>x) break;
                        if(x%q==0){
                            p=q;
                            break;
                        }
                    }
                    if(p==0){
                        res.pr_vec.emplace_back(x,1);
                        break;
                    }
                }
                long e=0;
                while(x%p==0){
                    x/=p;
                    e++;
                }
                res.pr_vec.emplace_back(p,e);
            }
            res.normalize();
            return res;
        }

        factor factorize(long const x,long const n) const{
            return factorize(x).pow(n);
        }

        factor lcm_factor(long const a,long const b) const{
            factor fa=factorize(a);
            factor fb=factorize(b);
            return fa.lcm_with(fb);
        }

        factor gcd_factor(long const a,long const b) const{
            factor fa=factorize(a);
            factor fb=factorize(b);
            return fa.gcd_with(fb);
        }

        long mobius(long const d) const{
            if(d<=0) return 0;
            if(d<=limit) return mobius_table[d];
            factor f=factorize(d);
            long cnt=0;
            for(auto const&p:f){
                if(p.exponent>1) return 0;
                cnt++;
            }
            return (cnt%2?-1:1);
        }

        long totient(long const n) const{
            if(n<=0) return 0;
            if(n<=limit) return totient_table[n];
            factor f=factorize(n);
            long res=n;
            for(auto const&p:f){
                res-=res/p.prime;
            }
            return res;
        }
};

prime prm(1e6);

factor::factor() = default;

factor::factor(long const x){ if(x>1){ *this=prm.factorize(x); } }
factor fc(long const x,long const n){ return prm.factorize(x,n); }

/*  template

    // prime judge
    if(prm.judge(13));

    // (1260) -> 2^2 3^2 5^1 7^1
    factor f=1260;
    cout<<f.floor_i_root(3)<<endl;
    f.print();
    vector u=f.vec(true); // {2,3,5,7}
    vector v=f.vec();     // {2,2,3,3,5,7}
    cout<<f.floor_i_root(3)<<endl; // 3rd root of 1260

    // lcm of (120,18) -> 2^3 3^2 5^1(360)
    factor f=prm.lcm_factor(x,y);
    f.print();
    cout<<f.val()<<endl;

    // others
    factor f=fc(1260,3); // 1260^3
    cout<<f.is_nth_power(3)<<endl; //true

    // mint
    cout<<f.val<mint>()<<endl;
*/

int main(){
    factor f=fc(1260,5);
    f/=35;
    f.print();
    cout<<f.val()<<endl;
    cout<<f.is_nth_power(2)<<endl;
    return 0;
}
