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
        longint(long long const x) : v(x){}
        longint(__int128_t const x) : v(x){}

        operator int() const{ return static_cast<int>(v); }
        operator long() const{ return static_cast<long>(v); }
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

        static longint l_lcm(longint const a,longint const b){
            return a/l_gcd(a,b)*b;
        }
};

using lint=longint;
longint stol(string const&s){ return longint::l_stol(s); }
longint sqrt(longint const&a){ return a.l_sqrt(); }
longint floor_i_root(longint const&n,int const&i){ return longint::l_floor_i_root(n,i); }
longint gcd(longint const&a,longint const&b){ return longint::l_gcd(a,b); }
longint lcm(longint const&a,longint const&b){ return longint::l_lcm(a,b); }

template<class T>
struct fraction{
    private:
        T num;
        T den;

        void normalize(){
            if(den==T(0)){
                num=(num>=T(0)?T(1):T(-1));
                return;
            }
            if(num==T(0)){
                den=1;
                return;
            }
            if(den<T(0)){
                num=-num;
                den=-den;
            }
            T const g=gcd(num,den);
            num/=g;
            den/=g;
        }

    public:
        fraction(T const n=T(0),T const d=T(1)): num(n), den(d){
            assert(!(n==T(0)&&d==T(0)));
            normalize();
        }

        T numerator() const{ return num; }
        T denominator() const{ return den; }

        // fraction / T
        fraction(const fraction&f,T const d): num(f.num), den(f.den*d){
            assert(!(f.den==T(0)&&d==T(0))); // inf/0
            assert(!(f.num==T(0)&&d==T(0))); // 0/0
            normalize();
        }

        // T / fraction
        fraction(T const n,const fraction&f): num(n*f.den), den(f.num){
            assert(!(f.den==T(0)));          // n/inf
            assert(!(n==T(0)&&f.num==T(0))); // 0/0
            normalize();
        }

        // fraction / fraction
        fraction(const fraction&a,const fraction&b): num(a.num*b.den), den(a.den*b.num){
            assert(!(a.den==T(0)&&b.den==T(0))); // inf/inf
            assert(!(a.num==T(0)&&b.num==T(0))); // 0/0
            normalize();
        }

        operator int() const{
            assert(den!=T(0));
            return num/den;
        }

        operator long() const{
            assert(den!=T(0));
            return num/den;
        }

        operator double() const{
            if(den==T(0)) return (num>=T(0)?INFINITY:-INFINITY);
            return static_cast<double>(num)/static_cast<double>(den);
        }

        operator long double() const{
            if(den==T(0)) return (num>=T(0)?INFINITY:-INFINITY);
            return static_cast<long double>(num)/static_cast<long double>(den);
        }

        friend ostream&operator<<(ostream&os,const fraction&f){
            if(f.den==T(0)) return os<<(f.num>=T(0)?"inf":"-inf");
            if(f.den==T(1)) return os<<f.num;
            return os<<f.num<<"/"<<f.den;
        }

        friend istream&operator>>(istream&is,fraction&f){
            T n,d;
            if(!(is>>n)) return is;
            if(is.peek()=='/'){
                char c;
                is>>c>>d;
                if(d==T(0)){
                    assert(n!=T(0));
                    f=fraction(n>=T(0)?T(1):T(-1),T(0));
                } else f=fraction(n,d);
            } else{
                is>>d;
                if(d==T(0)){
                    assert(n!=T(0));
                    f=fraction(n>=T(0)?T(1):T(-1),T(0));
                } else f=fraction(n,d);
            }
            return is;
        }

        fraction inv() const{ return fraction(den,num); }

        T floor(bool const open=false) const{
            assert(den!=T(0));
            T p=num,q=den;
            if(q<T(0))p=-p,q=-q;
            T val;
            if(p>=T(0)) val=p/q;
            else val=-((-p+q-T(1))/q);
            if(open&&p%q==T(0)) --val;
            return val;
        }

        T ceil(bool const open=false) const{
            assert(den!=T(0));
            T p=num,q=den;
            if(q<T(0))p=-p,q=-q;
            T val;
            if(p>=T(0)) val=(p+q-T(1))/q;
            else val=-((-p)/q);
            if(open&&p%q==T(0)) ++val;
            return val;
        }

        T mod_floor() const{
            assert(den!=T(0));
            T const p=num;
            T q=den;
            if(q<T(0))q=-q;
            T val=p%q;
            if(val<T(0))val+=q;
            return val;
        }

        fraction operator+(const fraction&other) const{
            if(den==T(0)||other.den==T(0)){ // inf
                assert(!(den==T(0)&&other.den==T(0)&&num!=other.num));
                return (den==T(0)?*this:other);
            }
            return fraction(num*other.den+other.num*den,den*other.den);
        }

        fraction operator-(const fraction&other) const{
            if(den==T(0)||other.den==T(0)){ // inf
                assert(!(den==T(0)&&other.den==T(0)&&num!=other.num));
                return (den==T(0)?*this:other);
            }
            return fraction(num*other.den-other.num*den,den*other.den);
        }

        fraction operator*(const fraction&other) const{
            if(den==T(0)||other.den==T(0)){ // inf
                assert(!(num==T(0)||other.num==T(0)));
                return fraction(num*other.num,T(0));
            }
            return fraction(num*other.num,den*other.den);
        }

        fraction operator/(const fraction&other) const{
            if(other.den==T(0))
                assert(false);
            if(other.num==T(0))return fraction(num>=T(0)?T(1):T(-1),T(0)); // inf
            if(den==T(0)){
                T const n=num*other.den*(other.num>=T(0)?T(1):T(-1)); // sign only
                return fraction(n,T(0));
            }

            return fraction(num*other.den,den*other.num);
        }

        bool operator<(const fraction&other) const{
            if(den==T(0)&&other.den==T(0)) return num<other.num; // inf
            if(den==T(0)) return num<T(0);
            if(other.den==T(0)) return other.num>T(0);
            return static_cast<__int128>(num)*other.den<static_cast<__int128>(other.num)*den;
        }

        bool operator>(const fraction&other) const{
            if(den==T(0)&&other.den==T(0)) return num>other.num; // inf
            if(den==T(0)) return num>T(0);
            if(other.den==T(0)) return other.num<T(0);
            return static_cast<__int128>(num)*other.den>static_cast<__int128>(other.num)*den;
        }

        bool operator<=(const fraction&other) const{ return !(other<*this); }
        bool operator>=(const fraction&other) const{ return !(*this<other); }

        bool operator==(const fraction&other) const{
            if(den==T(0)||other.den==T(0))
                return den==other.den&&num==other.num;
            return static_cast<__int128>(num)*other.den==static_cast<__int128>(other.num)*den;
        }

        bool operator!=(const fraction&other) const{
            return !(*this==other);
        }

        fraction operator-() const{
            return fraction(-num,den);
        }

        fraction&operator+=(const fraction&other){
            *this=*this+other;
            return *this;
        }

        fraction&operator-=(const fraction&other){
            *this=*this-other;
            return *this;
        }

        fraction&operator*=(const fraction&other){
            *this=*this*other;
            return *this;
        }

        fraction&operator/=(const fraction&other){
            *this=*this/other;
            return *this;
        }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        fraction operator+(Int const x) const{ return *this+fraction(T(x)); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        fraction operator-(Int const x) const{ return *this-fraction(T(x)); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        fraction operator*(Int const x) const{ return *this*fraction(T(x)); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        fraction operator/(Int const x) const{ return *this/fraction(T(x)); }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend fraction operator+(Int const x,const fraction&f){ return fraction(T(x))+f; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend fraction operator-(Int const x,const fraction&f){ return fraction(T(x))-f; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend fraction operator*(Int const x,const fraction&f){ return fraction(T(x))*f; }

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend fraction operator/(Int const x,const fraction&f){ return fraction(T(x))/f; }

        fraction operator+(T const x) const{ return *this+fraction(T(x)); }
        fraction operator-(T const x) const{ return *this-fraction(T(x)); }
        fraction operator*(T const x) const{ return *this*fraction(T(x)); }
        fraction operator/(T const x) const{ return *this/fraction(T(x)); }
        friend fraction operator+(T const x,const fraction&f){ return fraction(T(x))+f; }
        friend fraction operator-(T const x,const fraction&f){ return fraction(T(x))-f; }
        friend fraction operator*(T const x,const fraction&f){ return fraction(T(x))*f; }
        friend fraction operator/(T const x,const fraction&f){ return fraction(T(x))/f; }

        fraction&operator++(){
            num+=den;
            return *this;
        }

        fraction operator++(int){
            fraction const f=*this;
            num+=den;
            return f;
        }

        fraction&operator--(){
            num-=den;
            return *this;
        }

        fraction operator--(int){
            fraction const f=*this;
            num-=den;
            return f;
        }
};

using fr=fraction<lint>;
fr inf=fr(1,0);

template<class T>
struct line_fraction{
    public:
        T a,b,c; // ax+by=c , y =(-a/b)x+(c/b)
        using Fr=fraction<T>;

        line_fraction() : a(T(0)), b(T(1)), c(T(0)){}

        line_fraction(T const a_,T const b_,T const c_) : a(a_), b(b_), c(c_){
            if(b<T(0)){
                a=-a;
                b=-b;
                c=-c;
            }
        }

        bool operator==(const line_fraction&other) const{
            if(!is_parallel(other)) return false;
            if(b!=T(0)||other.b!=T(0)){
                return static_cast<__int128>(c)*other.b==static_cast<__int128>(other.c)*b;
            }
            return static_cast<__int128>(c)*other.a==static_cast<__int128>(other.c)*a;
        }

        bool operator!=(const line_fraction&other) const{
            return !(*this==other);
        }

        bool operator<(const line_fraction&other) const{
            if((this)->slope()!=other.slope())return (this)->slope()<other.slope();
            return (this)->intercept_y()>other.intercept_y();
        }

        Fr slope() const{
            if(b==T(0))return inf;
            return Fr(-a,b);
        }

        Fr intercept_y() const{ // x=0
            if(b==T(0))return inf;
            return Fr(c,b);
        }

        Fr intercept_x() const{ // y=0
            if(b==T(0))return inf;
            return Fr(c,a);
        }

        Fr y_val(fr const&x) const{
            if(b==T(0))return inf;
            return Fr(c-a*x,b);
        }

        Fr intersect_x(const line_fraction&other) const{
            return Fr(other.intercept_y()-intercept_y(),slope()-other.slope());
        }

        Fr intersect_y(const line_fraction&other) const{ return y_val(intersect_x(other)); }

        static line_fraction through_line(T const&x1,T const&y1,T const&x2,T const&y2){
            T const a=y1-y2;
            T const b=x2-x1;
            T const c=a*x1+b*y1;
            return line_fraction(a,b,c);
        }

        static line_fraction perpendicular_bisector(T const&x1,T const&y1,T const&x2,T const&y2){
            T const dx=x2-x1;
            T const dy=y2-y1;

            T const a=T(2)*dx;
            T const b=T(2)*dy;
            T const c=dx*(x1+x2)+dy*(y1+y2);

            return line_fraction(a,b,c);
        }

        bool is_parallel(const line_fraction&other) const{
            return a*other.b==b*other.a;
        }

        bool is_perpendicular(const line_fraction&other) const{
            return a*other.a+b*other.b==0;
        }

        bool has_intersection(const line_fraction&other) const{
            if(*this==other) return true;
            return !is_parallel(other);
        }

        Fr area(Fr const&fr_l,Fr const&fr_r) const{
            if(fr_l>fr_r)return Fr(0);
            return slope()*(fr_r*fr_r-fr_l*fr_l)/2+intercept_y()*(fr_r-fr_l);
        }

        T count_lattice_on_line(fr const&fr_l,fr const&fr_r,
                                bool const l_open=false,bool const r_open=false) const{
            T const l=fr_l.ceil(l_open);
            T const r=fr_r.floor(r_open);
            if(l>r)return 0;

            T const g=gcd(a,b);
            if(c%g!=T(0)) return 0;
            T const a2=a/g;
            T const b2=b/g;
            T const c2=c/g;

            auto modinv=[&](T const A,T const M){
                auto eg=[&](auto&&self,T const a,T b)->pair<T,T>{
                    if(b==T(0)) return {T(1),T(0)};
                    auto [x1, y1]=self(self,b,a%b);
                    return {y1,x1-(a/b)*y1};
                };
                auto [x0, y0]=eg(eg,A,M);
                x0%=M;
                if(x0<T(0)) x0+=M;
                return x0;
            };

            T const inv=modinv(a2,b2);
            T x0=(c2*inv)%b2;
            if(x0<T(0)) x0+=b2;
            T const d=b2;
            T const k_min=(l<=x0?T(0):(l-x0+d-1)/d);
            T const k_max=(r<x0?T(-1):(r-x0)/d);
            if(k_min>k_max) return 0;
            return k_max-k_min+T(1);
        }

        T count_lattice_under_line(fr fr_l,fr fr_r,
                                   bool l_open=false,bool r_open=false,
                                   bool const line_open=false) const{ // floor_sum

            T const l=fr_l.ceil(l_open);
            T const r=fr_r.floor(r_open);
            if(l>r) return T(0);
            assert(b!=T(0));
            function<T(T,T)> div_floor=[&](T const p,T const q){
                if(q<T(0))return div_floor(-p,-q);
                if(p>=T(0)) return p/q;
                return -((-p+q-T(1))/q);
            };
            function<T(T,T)> mod_floor=[&](T const p,T const q){
                if(q<T(0)) return mod_floor(p,-q);
                T m=p%q;
                if(m<T(0)) m+=q;
                return m;
            };

            function<T(T,T,T,T)> floor_sum=
                    [&](T const x,T A,T B,T C)->T{
                if(B==T(0)) return T(0);
                if(B<T(0)) return floor_sum(x,-A,-B,-C);
                if(A<T(0)) return -floor_sum(x,-A,B,B-T(1)-C);
                if(A==T(0)) return (x+T(1))*div_floor(C,B);

                T res=T(0);
                if(A>=B){
                    res+=x*(x+T(1))*div_floor(A,B)/2;
                    A=mod_floor(A,B);
                }
                if(C>=B||C<T(0)){
                    res+=(x+T(1))*div_floor(C,B);
                    C=mod_floor(C,B);
                }
                T const y=div_floor(A*x+C,B);
                T const z=mod_floor(A*x+C,B);
                if(y!=T(0)) res+=y+floor_sum(y-T(1),B,A,z);
                return res;
            };
            T val=T(0);
            if(l<=r) val+=floor_sum(r,-a,b,c);
            if(l>T(0)) val-=floor_sum(l-T(1),-a,b,c);
            if(line_open){
                if(b>T(0)){
                    if(intercept_x()<=fr_r){
                        fr_r=intercept_x();
                        r_open=true;
                    }
                } else{
                    if(fr_l<=intercept_x()){
                        fr_l=intercept_x();
                        l_open=true;
                    }
                } // y>0 only
                if(fr_l<=fr_r)val-=count_lattice_on_line(fr_l,fr_r,l_open,r_open);
            }
            return val;
        }

        T count_lattice_under_line(T l,T r) const{
            return count_lattice_under_line(fr(l),fr(r),false,false,true);
        }

        void debug() const{ cout<<a<<" "<<b<<" "<<c<<endl; }
};

using line_fr=line_fraction<lint>;

struct convex_hull_line{
    private:
        vector<line_fr> lines;
        vector<fr> starts;

    public:
        void add_line(const line_fr&L){
            fr s=-inf;
            if(!lines.empty()&&lines.back().slope()==L.slope())return;
            while(!lines.empty()){
                fr x=lines.back().intersect_x(L);
                if(x==inf||x==-inf)return;
                if(x<=starts.back()){
                    lines.pop_back();
                    starts.pop_back();
                } else{
                    s=x;
                    break;
                }
            }
            lines.push_back(L);
            starts.push_back(s);
        }

        line_fr get_hull(fr const&x) const{
            int const idx=upper_bound(starts.begin(),starts.end(),x)-starts.begin()-1;
            return lines[idx];
        }

        int size() const{ return static_cast<int>(lines.size()); }
        const line_fr&hull_line(int const i) const{ return lines[i]; }
        fr hull_start(int const i) const{ return starts[i]; }
};

int main(){
    // ABC372G
    int t;
    cin>>t;
    while(t--){
        int n;
        cin>>n;
        vector<line_fr> v(n);
        fr r_max=inf;
        for(int i=0;i<n;i++){
            cin>>v[i].a>>v[i].b>>v[i].c;
            r_max=min(r_max,v[i].intercept_x());
        }
        sort(v.rbegin(),v.rend());
        //for(int i=0;i<n;i++)v[i].debug();
        //cout<<r_max.floor()<<endl;

        lint ans=0;
        convex_hull_line hull;
        for(int i=0;i<n;i++)hull.add_line(v[i]);
        int const s=hull.size();
        int idx=0;
        lint l=1;
        while(l<=r_max.floor()){
            line_fr cur=hull.hull_line(idx);
            lint r=cur.intercept_x().floor();
            if(idx+1<s) r=min(r,hull.hull_start(idx+1).floor());
            if(r>=r_max.floor())r=r_max.floor();
            if(l<=r){
                lint const val=cur.count_lattice_under_line(l,r);
                //cout<<l<<" "<<r<<" "<<val<<endl;
                ans+=val;
                l=r+1;
            }
            idx++;
        }
        cout<<ans<<endl;
    }
    return 0;
}
