#include <bits/stdc++.h>
using namespace std;

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

using fr=fraction<long>;
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

template<class T>
struct Min{
    using Fr=fraction<T>;
    using Line=line_fraction<T>;

    static bool is_better(Line const&L1,Line const&L2,Fr const&x){
        T const xn=x.numerator();
        T const xd=x.denominator();
        T const n1=L1.c*xd-L1.a*xn;
        T const n2=L2.c*xd-L2.a*xn;
        return n1*L2.b<n2*L1.b;
    }

    static Fr id(){return Fr(T(1),T(0));}
};

template<class T>
struct Max{
    using Fr=fraction<T>;
    using Line=line_fraction<T>;
    static bool is_better(Line const&L1,Line const&L2,Fr const&x){
        T const xn=x.numerator();
        T const xd=x.denominator();
        T const n1=L1.c*xd-L1.a*xn;
        T const n2=L2.c*xd-L2.a*xn;
        return n1*L2.b>n2*L1.b;
    }

    static Fr id(){return Fr(T(-1),T(0));}
};

template<class T,typename M>
struct li_chao_tree_integer{
    private:
        using Line = line_fraction<T>;
        using Fr = fraction<T>;
        static constexpr T x_min=static_cast<T>(-1e9);
        static constexpr T x_max=static_cast<T>(1e9);

        struct Node{
            Line line;
            int left=0;
            int right=0;
        };

        vector<Node> tree;
        int root=0;

        int new_node(){
            tree.emplace_back();
            return static_cast<int>(tree.size())-1;
        }

        int add_line_impl(Line L,int k,T const l,T const r){
            if(k==0){
                k=new_node();
                tree[k].line=L;
                return k;
            }
            T const m=l+(r-l)/T(2);
            if(M::is_better(L,tree[k].line,m)){
                swap(L,tree[k].line);
            }
            if(l==r) return k;
            if(M::is_better(L,tree[k].line,l)){
                tree[k].left=add_line_impl(L,tree[k].left,l,m);
            } else if(M::is_better(L,tree[k].line,r)){
                tree[k].right=add_line_impl(L,tree[k].right,m+T(1),r);
            }
            return k;
        }

        int calc_impl(T const x,int const k,T const l,T const r) const{
            if(k==0) return 0;
            int best=k;
            if(l==r) return best;
            T const m=l+(r-l)/T(2);
            int child;
            if(x<=m)child=calc_impl(x,tree[k].left,l,m);
            else child=calc_impl(x,tree[k].right,m+T(1),r);
            if(child!=0&&M::is_better(tree[child].line,tree[best].line,x)){ best=child; }
            return best;
        }

    public:
        li_chao_tree_integer(){
            tree.reserve(1<<20);
            tree.emplace_back();
        }

        void add_line(Line const&L){
            if(L.b==T(0)) return;
            root=add_line_impl(L,root,x_min,x_max);
        }

        void add_line_equation(T const a,T const b,T const c){ // ax+by=c
            if(b==T(0)) return;
            root=add_line_impl(Line(a,b,c),root,x_min,x_max);
        }

        void add_line_function(T const a,T const b){ // y=ax+b
            root=add_line_impl(Line(a,T(-1),-b),root,x_min,x_max);
        }

        void add_line_function(Fr const a,Fr const b){ // y=ax+b
            T const d=lcm(a.denominator(),b.denominator());
            T const p=a.numerator()*(d/a.denominator());
            T const q=b.numerator()*(d/b.denominator());
            root=add_line_impl(Line(p,-d,-q),root,x_min,x_max);
        }

        Fr calc(T const x) const{
            int const k=calc_impl(x,root,x_min,x_max);
            if(k==0) return M::id();
            return tree[k].line.y_val(x);
        }

        Fr operator()(T const x) const{ return calc(x); }
};

int main(){
    // EDPC Z
    int n;
    long c;
    cin>>n>>c;

    vector<long> h(n);
    for(int i=0;i<n;i++)cin>>h[i];
    li_chao_tree_integer<long,Min<long>> tr;
    vector<long> dp(n);
    dp[0]=0;

    tr.add_line_function(-2*h[0],h[0]*h[0]+dp[0]);

    for(int i=1;i<n;i++){
        long x=h[i];
        dp[i]=x*x+c+tr(x); // min_j(-2*h[j]*x+h[j]^2+dp[j])
        tr.add_line_function(-2*x,x*x+dp[i]);
    }

    cout<<dp[n-1]<<endl;
}

