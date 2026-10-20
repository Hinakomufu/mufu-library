#include <bits/stdc++.h>
using namespace std;

struct longint{
    private:
        __int128_t v; //value
        void from_string(const string&s){
            v=0;
            int i=0;
            bool neg=false;
            if(s[i]=='-'){
                neg=true;
                i++;
            } else if(s[i]=='+'){
                i++;
            }
            for(;i<static_cast<int>(s.size());i++){
                char const c=s[i];
                if('0'<=c&&c<='9'){
                    v=v*10+(c-'0');
                }
            }
            if(neg) v=-v;
        }

    public:
        longint() : v(0){}
        longint(int const x) : v(x){}
        longint(long const x) : v(x){}
        longint(unsigned long const x) : v(x){}
        longint(long long const x) : v(x){}
        longint(__int128_t const x) : v(x){}

        operator int() const{ return static_cast<int>(v); }
        operator long() const{ return static_cast<long>(v); }
        operator unsigned long() const{ return static_cast<unsigned long>(v); }
        operator long long() const{ return static_cast<long long>(v); }
        operator __int128_t() const{ return v; }
        operator double() const{ return static_cast<double>(v); }
        operator long double() const{ return static_cast<long double>(v); }

        string to_string() const{
            __int128 x=v;
            if(v==0) return "0";
            bool const neg=x<0;
            if(neg) x=-x;
            string s;
            while(x>0){
                s.push_back('0'+static_cast<int>(x%10));
                x/=10;
            }
            if(neg) s.push_back('-');
            reverse(s.begin(),s.end());
            return s;
        }

        static longint l_stol(const std::string&s){
            longint x;
            x.from_string(s);
            return x;
        }

        longint operator-() const{ return longint(-v); }
        bool operator<(const longint&y) const{ return v<y.v; }
        bool operator>(const longint&y) const{ return v>y.v; }
        bool operator<=(const longint&y) const{ return v<=y.v; }
        bool operator>=(const longint&y) const{ return v>=y.v; }
        bool operator==(const longint&y) const{ return v==y.v; }
        bool operator!=(const longint&y) const{ return v!=y.v; }

        longint operator+(const longint&y) const{ return longint(v+y.v); }
        longint operator-(const longint&y) const{ return longint(v-y.v); }
        longint operator*(const longint&y) const{ return longint(v*y.v); }
        longint operator/(const longint&y) const{ return longint(v/y.v); }
        longint operator%(const longint&y) const{ return longint(v%y.v); }

        longint operator&(const longint&y) const{ return longint(v&y.v); }
        longint operator|(const longint&y) const{ return longint(v|y.v); }
        longint operator^(const longint&y) const{ return longint(v^y.v); }

        longint operator<<(uint64_t const k) const{
            if(k>=127) return longint(0);
            return longint(v<<k);
        }

        longint operator>>(uint64_t const k) const{
            if(k>=127) return longint(v>=0?0:-1);
            return longint(v>>k);
        }

        longint&operator+=(const longint&y){
            *this=*this+y;
            return *this;
        }

        longint&operator-=(const longint&y){
            *this=*this-y;
            return *this;
        }

        longint&operator*=(const longint&y){
            *this=*this*y;
            return *this;
        }

        longint&operator/=(const longint&y){
            *this=*this/y;
            return *this;
        }

        longint&operator%=(const longint&y){
            *this=*this%y;
            return *this;
        }

        longint&operator&=(const longint&y){
            *this=*this&y;
            return *this;
        }

        longint&operator|=(const longint&y){
            *this=*this|y;
            return *this;
        }

        longint&operator^=(const longint&y){
            *this=*this^y;
            return *this;
        }

        longint&operator<<=(const longint&y){
            uint64_t const k=static_cast<uint64_t>(y.v);
            *this=*this<<k;
            return *this;
        }

        longint&operator>>=(const longint&y){
            uint64_t const k=static_cast<uint64_t>(y.v);
            *this=*this>>k;
            return *this;
        }

        longint&operator++(){
            *this+=longint(1);
            return *this;
        }

        longint operator++(int){
            longint const tmp=*this;
            *this+=longint(1);
            return tmp;
        }

        longint&operator--(){
            *this-=longint(1);
            return *this;
        }

        friend istream&operator>>(istream&is,longint&x){
            string s;
            is>>s;
            x.from_string(s);
            return is;
        }

        longint operator--(int){
            longint const tmp=*this;
            *this-=longint(1);
            return tmp;
        }

        friend ostream&operator<<(ostream&os,const longint&x){
            os<<x.to_string();
            return os;
        }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        longint operator+(Int const x) const{ return *this+longint(x); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        longint operator-(Int const x) const{ return *this-longint(x); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        longint operator*(Int const x) const{ return *this*longint(x); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        longint operator/(Int const x) const{ return *this/longint(x); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        longint operator<<(Int const k) const{
            uint64_t const s=static_cast<uint64_t>(k);
            if(s>=127)return longint(0);
            return longint(v<<s);
        }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        longint operator>>(Int const k) const{
            uint64_t const s=static_cast<uint64_t>(k);
            if(s>=127)return longint(v>=0?0:-1);
            return longint(v>>s);
        }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend longint operator+(Int const x,const longint&f){ return longint(x)+f; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend longint operator-(Int const x,const longint&f){ return longint(x)-f; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend longint operator*(Int const x,const longint&f){ return longint(x)*f; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend longint operator/(Int const x,const longint&f){ return longint(x)/f; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        bool operator==(Int const&y) const{ return v==static_cast<__int128>(y); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        bool operator!=(Int const&y) const{ return v!=static_cast<__int128>(y); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        bool operator<(Int const&y) const{ return v<static_cast<__int128>(y); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        bool operator>(Int const&y) const{ return v>static_cast<__int128>(y); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        bool operator<=(Int const&y) const{ return v<=static_cast<__int128>(y); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        bool operator>=(Int const&y) const{ return v>=static_cast<__int128>(y); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend bool operator==(Int const&x,const longint&y){ return static_cast<__int128>(x)==y.v; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend bool operator!=(Int const&x,const longint&y){ return static_cast<__int128>(x)!=y.v; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend bool operator<(Int const&x,const longint&y){ return static_cast<__int128>(x)<y.v; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend bool operator>(Int const&x,const longint&y){ return static_cast<__int128>(x)>y.v; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend bool operator<=(Int const&x,const longint&y){ return static_cast<__int128>(x)<=y.v; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend bool operator>=(Int const&x,const longint&y){ return static_cast<__int128>(x)>=y.v; }

        int l_popcount() const{
            uint64_t const lo=static_cast<uint64_t>(v);
            uint64_t const hi=static_cast<uint64_t>(v>>64);
            return __builtin_popcountll(lo)+__builtin_popcountll(hi);
        }

        longint l_sqrt() const{
            assert(v>=0);
            if(v==0) return longint(0);
            __uint128_t const x=static_cast<__uint128_t>(v);
            __uint128_t r=x;
            while(true){
                __uint128_t const nr=(r+x/r)>>1;
                if(nr>=r) break;
                r=nr;
            }
            return longint(static_cast<__int128_t>(r));
        }

        static longint l_floor_i_root(longint const n,int const i){
            auto ok=[&](longint const&x){
                longint v=1;
                for(int k=0;k<i;k++){
                    v*=x;
                    if(v>n) return false; // x^i > n -> false
                }
                return true; // x^i <= n -> true
            };

            longint l=1,r=n,ans=1;
            while(l<=r){
                longint m=(l+r)/2;
                if(ok(m)){
                    ans=m;
                    l=m+1;
                } else{
                    r=m-1;
                }
            }
            return ans;
        }

        static longint l_gcd(longint const a,longint const b){
            __int128_t x=a.v;
            __int128_t y=b.v;
            if(x<0) x=-x;
            if(y<0) y=-y;
            __uint128_t ux=static_cast<__uint128_t>(x);
            __uint128_t uy=static_cast<__uint128_t>(y);
            while(uy!=0){
                __uint128_t const r=ux%uy;
                ux=uy;
                uy=r;
            }
            return longint(static_cast<__int128_t>(ux));
        }
};

using lint=longint;
longint stol(string const&s){ return longint::l_stol(s); }
longint sqrt(longint const&a){ return a.l_sqrt(); }
longint floor_i_root(longint const&n,int const&i){ return longint::l_floor_i_root(n,i); }
longint gcd(longint const&a,longint const&b){ return longint::l_gcd(a,b); }

// sum in [0,x] of floor n^p((an+b)/m)^q (m!=0)
struct floor_sum{
    lint moment[3][3]{}; // moment[p][q],p+q<=2
    floor_sum() = default;

    floor_sum(lint const x,lint const a,lint const b,lint const m){ *this=calc(x,a,b,m); }

    lint&operator()(int const p,int const q){ return moment[p][q]; }
    lint const&operator()(int const p,int const q) const{ return moment[p][q]; }

    static lint div_floor(lint const p,lint const q){
        assert(q!=0);
        if(q<0)return div_floor(-p,-q);
        if(p>=0)return p/q;
        return -((-p+q-1)/q);
    }

    static lint mod_floor(lint const p,lint const q){
        assert(q != 0);
        if(q<0)return mod_floor(p,-q);
        lint r=p%q;
        if(r<0)r+=q;
        return r;
    }

    // sum in [0,x] of floor n^p((an+b)/m)^q (m!=0)
    static floor_sum calc(lint const x,lint a,lint b,lint const m){
        if(x<0)return {};
        if(m==0)return {};
        if(m<0)return calc(x,-a,-b,-m);
        if(a<0){
            auto const fs=calc(x,-a,a*x+b,m);
            floor_sum res=fs;
            res.moment[1][1]=x*fs.moment[0][1]-fs.moment[1][1];
            return res;
        }

        lint const qa=div_floor(a,m);
        lint const qb=div_floor(b,m);
        a=mod_floor(a,m);
        b=mod_floor(b,m);

        lint const N=x+1;
        lint const S0=N;
        lint const S1=x*(x+1)/2;
        lint const S2=x*(x+1)*(2*x+1)/6;

        floor_sum res;
        res.moment[0][0]=S0;
        res.moment[1][0]=S1;
        res.moment[2][0]=S2;
        if(a==0||a*x+b<m){
            res.moment[0][1]=qa*S1+qb*S0;
            res.moment[1][1]=qa*S2+qb*S1;
            res.moment[0][2]=qa*qa*S2+2*qa*qb*S1+qb*qb*S0;
            return res;
        }

        lint const y=div_floor(a*x+b,m);
        auto const fs=calc(y-1,m,m+a-b-1,a);
        lint const H01=fs.moment[0][1];
        lint const H11=fs.moment[1][1]+fs.moment[0][1];
        lint const G01=N*y-H01;
        lint const G11=y*S1-(fs.moment[0][2]-fs.moment[0][1])/2;
        lint const G02=N*y*y-2*H11+H01;
        res.moment[0][1]=qa*S1+qb*S0+G01;
        res.moment[1][1]=qa*S2+qb*S1+G11;
        res.moment[0][2]=qa*qa*S2+2*qa*qb*S1+qb*qb*S0+2*qa*G11+2*qb*G01+G02;
        return res;
    }
};

int main(){
    // ABC402G
    int t;
    cin>>t;
    while(t--){
        lint n,m,a,b,c;
        cin>>n>>m>>a>>b>>c;
        if(b<c)swap(b,c);
        floor_sum f(n-1,a,b,m),g(n-1,a,c,m);

        lint ans=a*a*f(2,0)+a*(b+c)*f(1,0)+b*c*f(0,0)
        -m*a*(f(1,1)+g(1,1))
        -m*(b*g(0,1)+c*f(0,1))
        +m*m*(f(0,2)-f(0,1)+g(0,2)+g(0,1))/2;

        cout<<ans<<endl;
    }
}
