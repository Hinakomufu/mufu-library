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
        fraction operator+(Int const x) const{return *this+fraction(T(x));}

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        fraction operator-(Int const x) const{return *this-fraction(T(x));}

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        fraction operator*(Int const x) const{return *this*fraction(T(x));}

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        fraction operator/(Int const x) const{return *this/fraction(T(x));}

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend fraction operator+(Int const x,const fraction&f){return fraction(T(x))+f;}

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend fraction operator-(Int const x,const fraction&f){return fraction(T(x))-f;}

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend fraction operator*(Int const x,const fraction&f){return fraction(T(x))*f;}

        template<class Int,enable_if_t<is_integral_v<Int>,int> =0>
        friend fraction operator/(Int const x,const fraction&f){return fraction(T(x))/f;}

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

using fr=fraction<long>;
fr inf=fr(1,0);

int main(){
    fr a(5,3);
    a+=fr(3,4);
    --a;
    cout<<a<<endl;

    fr b=fr(-1,2);
    b/=0;
    cout<<b<<endl;
    return 0;
}
