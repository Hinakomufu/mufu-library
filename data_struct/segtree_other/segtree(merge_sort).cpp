#include <bits/stdc++.h>
using namespace std;

template<typename T>
struct ms_tree{
    int len;
    int size;
    vector<vector<T>> val;
    vector<vector<T>> pref;

    static vector<T> f(vector<T> const&a,vector<T> const&b){
        vector<T> res(static_cast<int>(a.size())+static_cast<int>(b.size()));
        merge(a.begin(),a.end(),b.begin(),b.end(),res.begin());
        return res;
    }

    static vector<T> e(){ return {}; }

    explicit ms_tree(vector<T> const&arr){
        len=arr.size();
        size=1;
        while(size<len) size<<=1;
        val.assign(2*size,e());
        pref.assign(2*size,e());
        for(int i=0;i<len;i++){
            val[size+i]={arr[i]};
            pref[size+i]={arr[i]};
        }
        for(int i=len;i<size;i++){
            val[size+i]=e();
            pref[size+i]=e();
        }
        for(int j=size-1;j>0;j--){
            val[j]=f(val[2*j],val[2*j+1]);
            pref[j].resize(val[j].size());
            if(!val[j].empty()){
                pref[j][0]=val[j][0];
                for(int k=1;k<static_cast<int>(val[j].size());k++){
                    pref[j][k]=pref[j][k-1]+val[j][k];
                }
            }
        }
    }

    struct update_proxy{
        ms_tree&st;
        int pos;
        update_proxy(ms_tree&st_,int const p_) : st(st_), pos(p_){}

        update_proxy&operator=(T const&x){
            int p=pos+st.size;
            st.val[p]={x};
            st.pref[p]={x};
            while(p>1){
                p>>=1;
                st.val[p]=f(st.val[2*p],st.val[2*p+1]);
                st.pref[p].resize(st.val[p].size());
                if(!st.val[p].empty()){
                    st.pref[p][0]=st.val[p][0];
                    for(int k=1;k<static_cast<int>(st.val[p].size());k++){
                        st.pref[p][k]=st.pref[p][k-1]+st.val[p][k];
                    }
                }
            }
            return *this;
        }
    };

    update_proxy update(int pos){ return update_proxy(*this,pos); }
    vector<T> get(int const pos) const{ return val[size+pos]; }

    int count_less(int l,int r,T x,bool const inclusive) const{
        int res=0;
        l+=size;
        r+=size+1;
        while(l<r){
            if(l&1){
                if(inclusive){
                    res+=upper_bound(val[l].begin(),val[l].end(),x)-val[l].
                            begin();
                } else{
                    res+=lower_bound(val[l].begin(),val[l].end(),x)-val[l].
                            begin();
                }
                l++;
            }
            if(r&1){
                --r;
                if(inclusive){
                    res+=upper_bound(val[r].begin(),val[r].end(),x)-val[r].
                            begin();
                } else{
                    res+=lower_bound(val[r].begin(),val[r].end(),x)-val[r].
                            begin();
                }
            }
            l>>=1;
            r>>=1;
        }
        return res;
    }

    int count_more(int const l,int const r,T x,bool const inclusive) const{
        return r-l+1-count_less(l,r,x,!inclusive);
    }

    int count_between(int const l,int const r,T x,T y,bool const inclusive) const{
        if(x>y)swap(x,y);
        return count_less(l,r,y,inclusive)-count_less(l,r,x,!inclusive);
    }
//
//
//
//
//

    T sum_all(int l,int r) const{
        T res=0;
        l+=size;
        r+=size+1;
        while(l<r){
            if(l&1){
                if(!pref[l].empty()) res+=pref[l].back();
                l++;
            }
            if(r&1){
                --r;
                if(!pref[r].empty()) res+=pref[r].back();
            }
            l>>=1;
            r>>=1;
        }
        return res;
    }

    T sum_less(int l,int r,T x,bool const inclusive) const{
        T res=0;
        l+=size;
        r+=size+1;
        while(l<r){
            if(l&1){
                if(inclusive){
                    int const k=upper_bound(val[l].begin(),val[l].end(),x)-val[
                                    l].begin();
                    if(k>0) res+=pref[l][k-1];
                } else{
                    int const k=lower_bound(val[l].begin(),val[l].end(),x)-val[
                                    l].begin();
                    if(k>0) res+=pref[l][k-1];
                }
                l++;
            }
            if(r&1){
                --r;
                if(inclusive){
                    int const k=upper_bound(val[r].begin(),val[r].end(),x)-val[
                                    r].begin();
                    if(k>0) res+=pref[r][k-1];
                } else{
                    int const k=lower_bound(val[r].begin(),val[r].end(),x)-val[
                                    r].begin();
                    if(k>0) res+=pref[r][k-1];
                }
            }
            l>>=1;
            r>>=1;
        }
        return res;
    }

    T sum_more(int l,int r,T x,bool const inclusive) const{
        return sum_all(l,r)-sum_less(l,r,x,!inclusive);
    }

    T sum_between(int const l,int const r,T x,T y,bool const inclusive) const{
        if(x>y)swap(x,y);
        return sum_less(l,r,y,inclusive)-sum_less(l,r,x,!inclusive);
    }
};

/*

    segtree<long> s(v);
    ans=s.sum_less(l,r,x,true);

*/

int main(){
    // suit for ABC339G
    long n;
    cin>>n;
    vector<long>v(n);
    for(long i=0;i<n;i++){
        cin>>v[i];
    }

    ms_tree<long> s(v);
    long ans=0;
    long q;
    cin>>q;

    while(q--){
        long a,b,c;
        cin>>a>>b>>c;

        long l=(a^ans)-1;
        long r=(b^ans)-1;
        long x=c^ans;

        ans=s.sum_less(l,r,x,true);
        cout<<ans<<endl;
    }

    return 0;
}
