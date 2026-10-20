#include <bits/stdc++.h>
using namespace std;

struct rle_unit{
    char ch;
    long len;

    rle_unit(char const c,long const l) : ch(c), len(l){}
    rle_unit() : ch(0), len(0){}

    bool operator==(const rle_unit&other) const{
        return ch==other.ch&&len==other.len;
    }
};

struct rle{
    vector<rle_unit> v;
    long raw_len=0;

    rle(){}

    // "aaabbcccc" -> {(a,3),(b,2),(c,4)}
    explicit rle(const string&s){
        raw_len=s.size();
        if(s.empty())
            return;
        v.emplace_back(s[0],1L);
        int const l=s.size();
        for(int i=1;i<l;i++){
            if(v.back().ch==s[i])
                v.back().len++;
            else
                v.emplace_back(s[i],1L);
        }
    }

    // "a3b2c4" -> {(a,3),(b,2),(c,4)}
    explicit rle(const string&s,bool fromEncoded){
        long s_len=0;
        int const l=s.size();
        for(int i=0;i<l;){
            char ch=s[i++];
            long num=0;
            while(i<l&&isdigit(s[i])){
                num=num*10+(s[i]-'0');
                i++;
            }
            v.emplace_back(ch,num);
            s_len+=num;
        }
        raw_len=s_len;
    }

    struct len_ref{
        rle*parent;
        long index;

        operator long() const{ return parent->v[index].len; }

        len_ref&operator=(long const new_len){
            if(new_len<=0){
                parent->delete_unit(index);
                return *this;
            }
            parent->change_unit(index,parent->v[index].ch,new_len);
            return *this;
        }


        len_ref&operator+=(long const x){
            long const new_len=parent->v[index].len+x;
            parent->change_unit(index,parent->v[index].ch,new_len);
            return *this;
        }

        len_ref&operator-=(long const x){
            long const new_len=parent->v[index].len-x;
            parent->change_unit(index,parent->v[index].ch,new_len);
            return *this;
        }
    };

    struct ch_ref{
        rle*parent;
        long index;

        operator char() const{ return parent->v[index].ch; }

        ch_ref&operator=(char const new_ch){
            parent->change_unit(index,new_ch,parent->v[index].len);
            return *this;
        }
    };

    struct rle_unit_proxy{
        rle*parent;
        long index;

        explicit operator const rle_unit&() const{
            return parent->v[index];
        }

        len_ref len(){ return len_ref{parent,index}; }
        ch_ref ch(){ return ch_ref{parent,index}; }
        long len() const{ return parent->v[index].len; }
        char ch() const{ return parent->v[index].ch; }

        rle_unit_proxy&operator=(const rle_unit&u){
            parent->change_unit(index,u.ch,u.len);
            return *this;
        }
    };

    rle_unit_proxy operator[](long const i){
        assert(0<=i&&i<static_cast<long>(v.size()));
        return rle_unit_proxy{this,i};
    }

    const rle_unit&operator[](long const i) const{
        assert(0<=i&&i<static_cast<long>(v.size()));
        return v[i];
    }

    // {(a,3),(b,2),(c,4)} -> "abc"
    string chars() const{
        string res;
        for(auto&p:v){
            res+=p.ch;
        }
        return res;
    }

    // {(a,3),(b,2),(c,4)} -> (true)  "a3b2c4"
    // {(a,3),(b,2),(c,4)} -> (false) "aaabbcccc"
    string print(bool const encoded=true) const{
        string res;
        if(encoded){
            for(auto&p:v)
                res+=string(1,p.ch)+to_string(p.len);
        } else{
            for(auto&p:v)
                res.append(p.len,p.ch);
        }
        return res;
    }

    void debug(bool const encoded=true) const{
        string res;
        if(encoded){
            for(auto&p:v)
                res+=string(1,p.ch)+to_string(p.len);
        } else{
            for(auto&p:v)
                res.append(p.len,p.ch);
        }
        cout<<res<<endl;
    }

    // {(a,3),(b,2),(c,4)} -> 3
    long rle_size() const{
        return static_cast<long>(v.size());
    }

    // {(a,3),(b,2),(c,4)} -> 9
    long raw_size() const{
        return raw_len;
    }

    void normalize(long i){
        if(i>0&&v[i-1].ch==v[i].ch){
            v[i-1].len+=v[i].len;
            v.erase(v.begin()+i);
            i--;
        }
        if(i+1<static_cast<long>(v.size())&&v[i].ch==v[i+1].ch){
            v[i].len+=v[i+1].len;
            v.erase(v.begin()+(i+1));
        }
    }

    void rle_push_back(char const c,long const l){
        raw_len+=l;
        if(!v.empty()&&v.back().ch==c){
            v.back().len+=l;
        } else{
            v.emplace_back(c,l);
        }
    }

    void raw_push_back(const string&s){
        if(s.empty()) return;
        int const n=s.size();
        int i=0;
        while(i<n){
            char const ch=s[i];
            long cnt=1;
            i++;
            while(i<n&&s[i]==ch){
                cnt++;
                i++;
            }
            rle_push_back(ch,cnt);
        }
    }

    void rle_pop_back(long cnt=1){
        while(cnt>0&&!v.empty()){
            raw_len-=v.back().len;
            v.pop_back();
            cnt--;
        }
    }

    void raw_pop_back(long cnt=1){
        raw_len-=cnt;
        while(cnt>0&&!v.empty()){
            auto&last=v.back();
            if(last.len>cnt){
                last.len-=cnt;
                cnt=0;
            } else{
                cnt-=last.len;
                v.pop_back();
            }
        }
    }

    void delete_unit(long const i){
        if(i<0||i>=static_cast<long>(v.size())) return;
        raw_len-=v[i].len;
        v.erase(v.begin()+i);
        if(i>0&&i<static_cast<long>(v.size())&&v[i-1].ch==v[i].ch){
            v[i-1].len+=v[i].len;
            v.erase(v.begin()+i);
        }
    }

    void change_unit(long const i,char const new_ch,long const new_len){
        if(i<0||i>=static_cast<long>(v.size())) return;
        if(new_len<=0){
            delete_unit(i);
            return;
        }
        raw_len-=v[i].len;
        raw_len+=new_len;
        v[i].ch=new_ch;
        v[i].len=new_len;
        normalize(i);
    }

    void swap_unit(long i,long j){
        if(i<0||j<0||i>=static_cast<long>(v.size())||j>=static_cast<long>(v.size())) return;
        if(i==j) return;
        std::swap(v[i],v[j]);
        if(i>j) std::swap(i,j);
        normalize(i);
        normalize(j);
    }

    rle&repeat(long k){
        if(k<=0){
            v.clear();
            raw_len=0;
            return *this;
        }
        if(k==1) return *this;
        rle const original=*this;
        for(long i=1;i<k;i++){
            *this+=original;
        }
        return *this;
    }

    rle repeated(long const k) const{
        rle res=*this;
        res.repeat(k);
        return res;
    }

    // rle + rle
    rle&operator+=(const rle&other){
        for(const auto&p:other.v){
            rle_push_back(p.ch,p.len);
        }
        return *this;
    }

    rle operator+(const rle&other) const{
        rle res=*this;
        res+=other;
        return res;
    }

    // rle + rle
    rle operator+(const rle_unit&u) const{
        rle res=*this;
        res.rle_push_back(u.ch,u.len);
        return res;
    }

    rle&operator+=(const rle_unit&u){
        rle_push_back(u.ch,u.len);
        return *this;
    }

    // rle_unit + rle
    friend rle operator+(const rle_unit&u,const rle&p){
        rle res;
        res.rle_push_back(u.ch,u.len);
        res+=p;
        return res;
    }

    // rle_pair + string
    rle operator+(const string&s) const{
        rle res=*this;
        res.raw_push_back(s);
        return res;
    }

    rle&operator+=(const string&s){
        raw_push_back(s);
        return *this;
    }

    // string + rle
    friend rle operator+(const string&s,const rle&p){
        rle res(s);
        res+=p;
        return res;
    }

    rle&operator-=(long const cnt){
        raw_pop_back(cnt);
        return *this;
    }

    rle operator-(long const cnt) const{
        rle res=*this;
        res.raw_pop_back(cnt);
        return res;
    }

    rle&operator*=(long const k){
        return repeat(k);
    }

    rle operator*(long const k) const{
        rle res=*this;
        res.repeat(k);
        return res;
    }

    bool operator==(const rle&other) const{
        if(raw_len!=other.raw_len) return false;
        if(v.size()!=other.v.size()) return false;

        return v==other.v;
    }

    bool is_same_shape(const rle&other) const{
        if(rle_size()!=other.rle_size())
            return false;
        for(long i=0;i<rle_size();i++){
            if(v[i].ch!=other.v[i].ch)
                return false;
        }
        return true;
    }

};

//  {1,3,3,2,4,5} -> "<=><<"
string cmp_str(const vector<int>&v){
    int const n=v.size();
    string s;
    for(int i=1;i<n;i++){
        if(v[i-1]>v[i]) s+='>';
        else if(v[i-1]==v[i]) s+='=';
        else s+='<';
    }
    return s;
}

/* template

string s="aaabb";
rle r(s);
r+="bbcc";
r+=rle_unit('a',3);
r-=2;
r=r*3-2;
r[4].ch()='b';
r[1].len()=0;
r.swap_unit(2,4);
cout<<r.print(true)<<endl;
cout<<r[2].len()*r[3].len()<<endl;
cout<<r.raw_size()<<endl;
cout<<r.rle_size()<<endl;

rle x("a3c3b30a3b3c3",true);
cout<<(x.is_same_shape(r)?"Yes":"No")<<endl;

*/

int main(){
    // ABC406C
    int n;
    cin>>n;
    vector<int> v(n);
    for(int i=0;i<n;i++){
        cin>>v[i];
    }

    string s=cmp_str(v);
    rle p(s);
    int l=p.rle_size();
    long ans=0;
    for(int i=1;i<l-1;i++){
        if(p[i].ch()=='<')continue;
        ans+=p[i-1].len()*p[i+1].len();
    }

    cout<<ans<<endl;
    return 0;
}
