#include <bits/stdc++.h>
using namespace std;

struct interval_set{
    map<long,long> intervals;
    long total=0;

    void insert(long const l,long const r){
        long L=l;
        long R=r;
        if(L>R) return;
        auto it=intervals.lower_bound(l);
        if(it!=intervals.begin()){
            --it;
            if(it->second<l) ++it;
        }
        while(it!=intervals.end()&&it->first<=r){
            L=min(L,it->first);
            R=max(R,it->second);
            total-=(it->second-it->first+1);
            it=intervals.erase(it);
        }
        intervals[L]=R;
        total+=(R-L+1);
    }

    void erase(long const l,long const r){
        if(l>r) return;
        auto it=intervals.lower_bound(l);
        if(it!=intervals.begin()) --it;
        vector<pair<long,long>> toAdd;
        while(it!=intervals.end()&&it->first<=r){
            long L=it->first,R=it->second;
            if(R<l){
                ++it;
                continue;
            }
            total-=(R-L+1);
            if(L<l) toAdd.push_back({L,l-1});
            if(R>r) toAdd.push_back({r+1,R});
            it=intervals.erase(it);
        }
        for(auto [L,R]:toAdd){
            intervals[L]=R;
            total+=(R-L+1);
        }
    }

    void print() const{
        for(auto&p:intervals){
            cout<<p.first<<" "<<p.second<<endl;
        }
        cout<<endl;
    }

    long size() const{
        return total;
    }

    bool contains(long const x) const{
        auto it=intervals.upper_bound(x);
        if(it==intervals.begin()) return false;
        --it;
        return it->first<=x&&x<=it->second;
    }

    pair<long,long> section(long const x) const{
        auto it=intervals.upper_bound(x);
        if(it==intervals.begin()) return {-1,-1};
        --it;
        if(it->first<=x&&x<=it->second){
            return {it->first,it->second};
        }
        return {-1,-1};
    }

    size_t count() const{
        return intervals.size();
    }

    long max_len() const{
        long res=0;
        for(auto&p:intervals){
            res=max(res,p.second-p.first+1);
        }
        return res;
    }
};

//
// interval_set s;
// s.insert(2,9);
// s.erase(4,6);
// cout<<s.size()<<endl;
//
// if(s.contains(8))
// pair<long,long>p=s.section(8);
// cout<<s.max_len()<<endl;
//

int main(){
    // suit for ABC435_E
    interval_set s;
    long n,q;
    cin>>n>>q;
    s.insert(1,n);
    while(q--){
        long l,r;
        cin>>l>>r;
        s.erase(l,r);
        cout<<s.size()<<endl;
    }
}
