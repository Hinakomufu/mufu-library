#include <bits/stdc++.h>
using namespace std;
using ld=long double;

ld inf=1L<<60;
ld eps=1L>>20;
ld pi=acosl(-1.0);
ld torad(long const deg){ return static_cast<ld>(deg)*pi/180; }
ld todeg(ld const ang){ return ang*180/pi; }

struct point{
    ld x,y;
    point(ld const x=0.0,ld const y=0.0) : x(x), y(y){}

    // clang-format off
    point operator +(const point&p) const{ return point(x+p.x,y+p.y); }
    point operator -(const point&p) const{ return point(x-p.x,y-p.y); }
    point operator *(ld const a) const{ return point(x*a,y*a); }
    point operator *(const point&p) const{ return point(x*p.x-y*p.y,x*p.y+y*p.x); }
    point operator /(ld const a) const{ return point(x/a,y/a); }
    static point conj(const point&p){ return point(p.x,-p.y); }
    static point rot(const point&p,ld const ang){ return point(cos(ang)*p.x-sin(ang)*p.y,sin(ang)*p.x+cos(ang)*p.y); }
    static point rot90(const point&p){ return point(-p.y,p.x); }
    static ld cross(const point&p,const point&q){ return p.x*q.y-p.y*q.x; }
    static ld dot(const point&p,const point&q){ return p.x*q.x+p.y*q.y; }
    static ld norm(const point&p){ return dot(p,p); }
    static ld abs(const point&p){ return sqrt(dot(p,p)); }
    static ld amp(const point&p){ld res=atan2(p.y,p.x);if(res<0) res+=pi*2;return res;}
    bool operator ==(const point&p) const{ return norm(*this-p)<eps*eps; }
    bool operator <(const point&p) const{ return (fabsl(x-p.x)>eps?x<p.x:y<p.y); }
    bool operator >(const point&p) const{ return (fabsl(x-p.x)>eps?x>p.x:y>p.y); }
    point unit() const { return *this / abs(*this); }
    point operator /(const point&p) const{ return *this*conj(p)/norm(p); }
    // clang-format on
};

struct line{
    point p,dir; // vec(u)=vec(p)+t*vec(dir)
    line(const point&p_,const point&q_) : p(p_), dir(q_-p_){}
    line(ld const px,ld const py,ld const qx,ld const qy): p(px,py), dir(point(qx,qy)-point(px,py)){}

    line(const pair<ld,ld>&a,const pair<ld,ld>&b): p(a.first,a.second),
                                                   dir(point(b.first,b.second)-point(a.first,a.second)){}

    bool is_parallel(const line&o) const{ return fabsl(point::cross(dir,o.dir))<eps; }
    bool is_perpendicular(const line&o) const{ return fabsl(point::dot(dir,o.dir))<eps; }
    bool operator==(const line&o) const{ return is_parallel(o)&&fabsl(point::cross(dir.unit(),o.p-p))<eps; }
    bool operator!=(const line&o) const{ return !(*this==o); }

    static line perpendicular_bisector(const point&a,const point&b){
        point const mid=(a+b)/2;
        point const n=point::rot90(b-a);
        return line(mid,mid+n);
    }

    static line perpendicular_bisector(ld const ax,ld const ay,ld const bx,ld const by){
        return perpendicular_bisector(point(ax,ay),point(bx,by));
    }

    bool has_intersection(const line&o) const{
        if(*this==o) return true;
        return !is_parallel(o);
    }

    point intersection(const line&o) const{
        ld const D=point::cross(o.dir,dir);
        if(fabsl(D)<eps) return point(inf,inf);
        return p+dir*point::cross(o.dir,o.p-p)/D;
    }

    ld distance(const point&r) const{
        return fabsl(point::cross(dir,r-p))/point::abs(dir);
    }

    tuple<ld,ld,ld> abc() const{ // ax+by=c
        return {-dir.y,dir.x,-dir.y*p.x+dir.x*p.y};
    }

    void debug() const{
        auto [a,b,c]=this->abc();
        cout<<a<<" "<<b<<" "<<c<<endl;
    }
};

struct segment{
    point p,q;
    segment(const point&p_,const point&q_) : p(p_), q(q_){}
    segment(ld const px,ld const py,ld const qx,ld const qy): p(px,py), q(qx,qy){}
    segment(const pair<ld,ld>&a,const pair<ld,ld>&b): p(a.first,a.second), q(b.first,b.second){}

    point dir() const{ return q-p; }
    point normal() const{ return point::rot90(dir()); }
    line to_line() const{ return line(p,q); }

    bool contains(const point&r) const{
        if(fabsl(point::cross(q-p,r-p))>eps) return false;
        return point::dot(r-p,r-q)<=eps;
    }

    bool overlaps(const segment&s) const{
        line const L1=this->to_line();
        line const L2=s.to_line();
        if(!(L1==L2)) return false;
        if(this->contains(s.p)) return true;
        if(this->contains(s.q)) return true;
        if(s.contains(this->p)) return true;
        if(s.contains(this->q)) return true;
        return false;
    }

    bool has_intersection(const segment&s) const{
        line const L1=this->to_line();
        line const L2=s.to_line();
        point const X=L1.intersection(L2);
        if(X.x==inf&&X.y==inf) return this->overlaps(s);
        return this->contains(X)&&s.contains(X);
    }

    point intersection(const line&L) const{
        point const X=L.intersection(this->to_line());
        if(X.x==inf&&X.y==inf) return point(inf,inf);

        return this->contains(X)?X:point(inf,inf);
    }

    ld distance(const point&r) const{
        point const v=q-p; // p→q
        point const w=r-p; // p→r
        ld const c1=point::dot(w,v);
        if(c1<=eps) return point::abs(r-p);
        ld const c2=point::dot(v,v);
        if(c2<=eps) return point::abs(r-p);
        if(c1>=c2-eps) return point::abs(r-q);
        ld const t=c1/c2;
        point const proj=p+v*t;
        return point::abs(r-proj);
    }
};

struct circle{
    point c;
    ld r;

    circle(const point&c_,ld const r_) : c(c_), r(r_){}
    circle(ld const cx,ld const cy,ld const r_) : c(cx,cy), r(r_){}

    circle(const point&A,const point&B){
        if(point::abs(A-B)<eps){
            c=point(inf,inf),r=-1;
            return;
        }
        c=(A+B)/2;
        r=point::abs(A-B)/2;
    }

    circle(const point&A,const point&B,const point&C){
        if(fabsl(point::cross(B-A,C-A))<eps){
            c=point(inf,inf),r=-1;
            return;
        }
        line const L1=line::perpendicular_bisector(A,B);
        line const L2=line::perpendicular_bisector(B,C);
        c=L1.intersection(L2);
        r=point::abs(A-c);
    }

    bool contains(const point&p) const{
        if(r<0)return false;
        return point::abs(p-c)<=r+eps;
    }

    ld distance(const point&p) const{
        if(r<0)return -inf;
        return fabsl(point::abs(p-c)-r);
    }

    bool intersects(const line&L) const{
        if(r<-eps)return false;
        return L.distance(c)<=r+eps;
    }

    vector<point> intersection(const line&L) const{
        point const v=L.dir;
        point const w=c-L.p;
        ld const t=point::dot(w,v)/point::dot(v,v);
        point proj=L.p+v*t;
        ld const d=point::abs(proj-c);
        if(d>r+eps) return {};
        if(fabsl(d-r)<=eps) return {proj};
        ld const h=sqrt(r*r-d*d);
        point const u=v.unit();
        return {proj+u*h,proj-u*h};
    }

    bool intersects(const segment&s) const{
        if(r<-eps)return false;
        line const L=s.to_line();
        auto const pts=this->intersection(L);
        for(auto&x:pts){
            if(s.contains(x)) return true;
        }
        return false;
    }

    vector<line> tangents_from_point(const point&p) const{
        point const v=p-c;
        ld const d2=point::norm(v);
        ld const r2=r*r;

        if(d2<r2-eps) return {};
        if(fabsl(d2-r2)<=eps){
            point const t=p;
            point const n=point::rot90(v);
            return {line(t,t+n)};
        }

        ld const d=sqrt(d2);
        point const u=v/d;
        point const u_per=point::rot90(u);
        ld const h=sqrt(d2-r2);

        point const t1=c+u*(r2/d2)+u_per*(r*h/d2);
        point const t2=c+u*(r2/d2)-u_per*(r*h/d2);

        return {line(t1,p),line(t2,p)};
    }
};

int main(){
    // ABC151F
    int n;
    cin>>n;
    vector<point> v(n);
    for(int i=0;i<n;i++)cin>>v[i].x>>v[i].y;
    circle C(point(0,0),-1);

    for(int i=0;i<n;i++){
        if(C.r<0||!C.contains(v[i])){
            C=circle(v[i],v[i]);
            for(int j=0;j<i;j++){
                if(!C.contains(v[j])){
                    C=circle(v[i],v[j]);
                    for(int k=0;k<j;k++){
                        if(!C.contains(v[k])){
                            C=circle(v[i],v[j],v[k]);
                        }
                    }
                }
            }
        }
    }

    cout<<fixed<<setprecision(12)<<C.r<<endl;
    return 0;
}
