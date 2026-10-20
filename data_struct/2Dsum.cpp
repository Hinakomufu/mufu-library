#include <bits/stdc++.h>
using namespace std;

vector<vector<long>> imos(int h,int w,const array<vector<long>,4>&rects){
    vector<vector<long>> v(h+1,vector<long>(w+1,0));
    // rects:(a,b)~(c,d) 0_index
    int const n=rects[0].size();
    for(int i=0;i<n;i++){
        int a=rects[0][i];
        int b=rects[1][i];
        int c=rects[2][i];
        int d=rects[3][i];
        if(a>c)swap(a,c);
        if(b>d)swap(b,d);
        v[a][b]++;
        v[a][d+1]--;
        v[c+1][b]--;
        v[c+1][d+1]++;
    }

    for(int i=0;i<=h;i++){
        for(int j=1;j<=w;j++){
            v[i][j]+=v[i][j-1];
        }
    }
    for(int j=0;j<=w;j++){
        for(int i=1;i<=h;i++){
            v[i][j]+=v[i-1][j];
        }
    }

    v.pop_back();
    for(auto&row:v) row.pop_back();

    return v;
}

vector<vector<long>> prefix_sum(const vector<vector<long>>&p){
    int const H=p.size()+1;
    int const W=p[0].size()+1;
    vector<vector<long>> s(H,vector<long>(W,0));
    // 0_index
    for(int i=1;i<H;i++){
        for(int j=1;j<W;j++){
            s[i][j]=p[i-1][j-1]+s[i-1][j]+s[i][j-1]-s[i-1][j-1];
        }
    }

    return s;
}

//
//
//
//
//

struct array2d{
    int H,W;
    vector<vector<long>> p;

    explicit array2d(const vector<vector<long>>&input) : p(input){
        H=input.size();
        W=input[0].size();
    }

    void print() const{
        for(int i=0;i<H;i++){
            for(int j=0;j<W;j++){
                cout<<p[i][j]<<(j==W-1?"\n":" ");
            }
        }
    }

    vector<vector<long>>prefix() const{return prefix_sum(p);}

    vector<vector<long>>single_prefix() const{
        vector<vector<long>> q(H,vector<long>(W,0));
        for(int i=0;i<H;i++){
            for(int j=0;j<W;j++){
                q[i][j]=(p[i][j]>0?1:0);
            }
        }
        return prefix_sum(q);
    }

};

struct sum2d{
    const vector<vector<long>> v;
    explicit sum2d(const vector<vector<long>>&input) : v(input){}

    long query(int a,int b,int c,int d) const{
        // (a,b)~(c,d) 0-index
        if(a>c)swap(a,c);
        if(b>d)swap(b,d);
        return v[c+1][d+1]-v[a][d+1]-v[c+1][b]+v[a][b];
    }


    void print() const{
        int const H=v.size();
        int const W=v[0].size();
        for(int i=1;i<H;i++){
            for(int j=1;j<W;j++){
                cout<<v[i][j]<<(j==W-1?"\n":" ");
            }
        }
    }

};

/*
    sum2d const s(prefix_sum(p));
    cout<<s.query(a,b,c,d)<<endl;

    array<vector<long>,4> rects;
    for(int k=0;k<4;k++)rects[k].resize(n);
    for(int i=0;i<n;i++){
        cin>>rects[0][i]>>rects[1][i]>>rects[2][i]>>rects[3][i];
        rects[0][i]--,rects[1][i]--,rects[2][i]--,rects[3][i]--;
    }
    array2d const p(imos(h,w,rects));
    sum2d const s(p.prefix);
*/

void solve_A08(){
    // suit for tessoku A08
    int h,w;
    cin>>h>>w;
    vector<vector<long>> p(h,vector<long>(w));
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            cin>>p[i][j];
        }
    }

    sum2d const s(prefix_sum(p));
    //s.print();

    int q;
    cin>>q;
    while(q--){
        int a,b,c,d;
        cin>>a>>b>>c>>d;
        a--,b--,c--,d--;
        cout<<s.query(a,b,c,d)<<endl;
    }

}

void solve_A09(){
    // suit for tessoku A09
    int h,w,n;
    cin>>h>>w>>n;

    array<vector<long>,4> rects;
    for(int k=0;k<4;k++)rects[k].resize(n);

    for(int i=0;i<n;i++){
        cin>>rects[0][i]>>rects[1][i]>>rects[2][i]>>rects[3][i];
        rects[0][i]--,rects[1][i]--,rects[2][i]--,rects[3][i]--;
    }

    array2d const p(imos(h,w,rects));
    p.print();

}

int main(){
    solve_A08();
    return 0;
}
