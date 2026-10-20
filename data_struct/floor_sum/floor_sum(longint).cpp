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

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        longint(Int x) : v(static_cast<__int128>(x)){}

        operator int() const{ return static_cast<int>(v); }
        operator long() const{ return static_cast<long>(v); }
        operator __int128() const{ return v; }
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

// sum in [0,x] of floor((an+b)/m) (m!=0)
lint floor_sum(lint const&x,lint a,lint b,lint const&m){
    function<lint(lint,lint)> div_floor=[&](lint const&p,lint const&q){
        if(q<0)return div_floor(-p,-q);
        if(p>=0) return p/q;
        return -((-p+q-1)/q);
    };
    function<lint(lint,lint)> mod_floor=[&](lint const&p,lint const&q){
        if(q<0) return mod_floor(p,-q);
        lint r=p%q;
        if(r<0) r+=q;
        return r;
    };

    if(m==0)return 0;
    if(m<0)return floor_sum(x,-a,-b,-m);
    if(a<0)return -floor_sum(x,-a,m-b-1,m);
    if(a==0)return (x+1)*div_floor(b,m);
    lint res=0;
    if(a>=m){
        res+=x*(x+1)*div_floor(a,m)/2;
        a=mod_floor(a,m);
    }
    if(b>=m||b<0){
        res+=(x+1)*div_floor(b,m);
        b=mod_floor(b,m);
    }
    lint const y=div_floor(a*x+b,m);
    lint const z=mod_floor(a*x+b,m);
    if(y!=0)res+=y+floor_sum(y-1,m,z,a);
    return res;
}

// sum in [l,r] of floor((an+b)/c)
lint range_floor_sum(lint const&l,lint const&r,lint const&a,lint const&b,lint const&m){
    lint val=0;
    if(l<=r)val+=floor_sum(r,a,b,m);
    if(l>0)val-=floor_sum(l-1,a,b,m);
    return val;
}

int main(){
    // ARC182E
    int n;
    lint m,c,k;
    cin>>n>>m>>c>>k;
    vector<lint>a(n);
    for(int i=0;i<n;i++)cin>>a[i];
    sort(a.begin(), a.end());
    a.erase(unique(a.begin(), a.end()),a.end());
    int s=a.size();

    vector<lint>f(s);
    for(int i=0;i<s;i++)f[i]=floor_sum(k-1,c,a[i],m);

    lint ans=c*k*(k-1)/2+k*a[0]-m*f[0];
    for(int i=1;i<s;i++)ans-=(a[0]+m-a[i])*(f[i]-f[i-1]);
    cout<<ans<<endl;
    return 0;
}
