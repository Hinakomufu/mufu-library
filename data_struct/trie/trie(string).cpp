#include <bits/stdc++.h>
using namespace std;

// trie info
static constexpr char max_val='z';
static constexpr char min_val='a';
static constexpr int node_count_max=1000000;
//

static constexpr int num=max_val-min_val+1;

struct trie_string{
    private:
        struct node{
            int nxt[num];
            int end;
            int cnt;
            int distinct;

            node() : end(0), cnt(0), distinct(0){
                fill_n(nxt,num,-1);
            }
        };

        vector<node> nodes;

        int find_node(const string&s) const{
            int idx=0;
            for(char const c:s){
                assert(min_val<=c&&c<=max_val);
                int const x=c-min_val;
                if(nodes[idx].nxt[x]==-1) return -1;
                idx=nodes[idx].nxt[x];
            }
            return idx;
        }

        void debug_dfs(int const idx,string&s) const{
            if(nodes[idx].end>0)cout<<s<<' '<<nodes[idx].end<<'\n';
            for(int x=0;x<num;x++){
                int const u=nodes[idx].nxt[x];
                if(u==-1) continue;
                s.push_back(min_val+x);
                debug_dfs(u,s);
                s.pop_back();
            }
        }

    public:
        trie_string(){
            nodes.reserve(node_count_max);
            nodes.push_back(node());
        }

        void insert(const string&s,int const count=1){
            if(s.empty()||count<=0) return;
            int const old_idx=find_node(s);
            bool const is_exist=(old_idx!=-1&&nodes[old_idx].end>0);
            nodes[0].cnt+=count;
            if(!is_exist) nodes[0].distinct++;
            int idx=0;
            for(char const c:s){
                assert(min_val<=c&&c<=max_val);
                int const x=c-min_val;
                if(nodes[idx].nxt[x]==-1){
                    nodes[idx].nxt[x]=nodes.size();
                    nodes.push_back(node());
                }
                idx=nodes[idx].nxt[x];
                nodes[idx].cnt+=count;
                if(!is_exist)nodes[idx].distinct++;
            }
            nodes[idx].end+=count;
        }

        void erase(const string&s,int const count=1){
            if(s.empty()||count<=0) return;
            int idx=find_node(s);
            if(idx==-1||nodes[idx].end==0) return;
            int const erase_count=min(count,nodes[idx].end);
            bool const is_erased=(erase_count==nodes[idx].end);
            nodes[0].cnt-=erase_count;
            if(is_erased)nodes[0].distinct--;
            idx=0;
            for(char const c:s){
                int const x=c-min_val;
                idx=nodes[idx].nxt[x];
                nodes[idx].cnt-=erase_count;
                if(is_erased)nodes[idx].distinct--;
            }
            nodes[idx].end-=erase_count;
        }

        void erase_all(const string&s){
            if(s.empty()) return;
            int idx=find_node(s);
            if(idx==-1||nodes[idx].end==0) return;
            int const count=nodes[idx].end;
            nodes[0].cnt-=count;
            nodes[0].distinct--;
            idx=0;
            for(char const c:s){
                int const x=c-min_val;
                idx=nodes[idx].nxt[x];
                nodes[idx].cnt-=count;
                nodes[idx].distinct--;
            }
            nodes[idx].end=0;
        }

        int total_count() const{ return nodes[0].cnt; }
        int total_distinct() const{ return nodes[0].distinct; }

        int count_exact(const string&s,bool const include_duplicate=true) const{
            if(s.empty()) return 0;
            int const idx=find_node(s);
            if(idx==-1) return 0;
            return include_duplicate?nodes[idx].end:(nodes[idx].end>0);
        }

        int prefix_count(const string&s,bool const include_duplicate=true) const{
            if(s.empty()) return 0;
            int const idx=find_node(s);
            if(idx==-1) return 0;
            return include_duplicate?nodes[idx].cnt:nodes[idx].distinct;
        }

        int count_less(const string&s,bool const include_duplicate=true,bool const exact=false) const{
            if(s.empty()) return 0;
            int idx=0;
            int ans=0;
            for(char const c:s){
                assert(min_val<=c&&c<=max_val);
                int const x=c-min_val;
                for(int y=0;y<x;y++){
                    int const u=nodes[idx].nxt[y];
                    if(u!=-1)ans+=include_duplicate?nodes[u].cnt:nodes[u].distinct;
                }
                ans+=include_duplicate?nodes[idx].end:(nodes[idx].end>0);
                if(nodes[idx].nxt[x]==-1)return ans;
                idx=nodes[idx].nxt[x];
            }
            if(exact)ans+=count_exact(s,include_duplicate);
            return ans;
        }

        int count_greater(const string&s,bool const include_duplicate=true,bool const exact=false) const{
            if(s.empty()) return 0;
            int idx=0;
            int ans=0;
            for(char const c:s){
                assert(min_val<=c&&c<=max_val);
                int const x=c-min_val;
                for(int y=x+1;y<num;y++){
                    int const u=nodes[idx].nxt[y];
                    if(u!=-1) ans+=include_duplicate?nodes[u].cnt:nodes[u].distinct;
                }
                if(nodes[idx].nxt[x]==-1) return ans;
                idx=nodes[idx].nxt[x];
            }
            if(include_duplicate)ans+=nodes[idx].cnt-nodes[idx].end;
            else ans+=nodes[idx].distinct-(nodes[idx].end>0);
            if(exact) ans+=count_exact(s,include_duplicate);
            return ans;
        }

        string kth_rank(int k,bool const include_duplicate=true) const{
            if(k<=0) return "";
            if(include_duplicate&&total_count()<k) return "";
            if(!include_duplicate&&total_distinct()<k) return "";

            int idx=0;
            string ans;
            while(true){
                int const self_count=include_duplicate?nodes[idx].end:(nodes[idx].end>0);
                if(self_count>=k) return ans;
                k-=self_count;
                for(int x=0;x<num;x++){
                    int const u=nodes[idx].nxt[x];
                    if(u==-1) continue;
                    int const subtree_count=include_duplicate?nodes[u].cnt:nodes[u].distinct;
                    if(subtree_count>=k){
                        ans.push_back(min_val+x);
                        idx=u;
                        break;
                    }
                    k-=subtree_count;
                }
            }
        }

        void debug() const{
            string s;
            debug_dfs(0,s);
        }
};

int main(){
    trie_string tr;
    tr.insert("abc",3);
    tr.insert("def",3);
    tr.insert("ghi",3);
    tr.insert("jkl",3);

    cout<<tr.kth_rank(4,true)<<endl;
    cout<<tr.kth_rank(4,false)<<endl;
    return 0;
}
