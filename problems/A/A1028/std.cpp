#include <iostream>
#include <chrono>
using namespace std;
//==========================================
#include <cmath>
#include <tuple>
#include <vector>
#include <algorithm>
const int maxn = 1e4+5;
const int maxh = 1e5+5;
typedef tuple<int,int,int> tp3;
int a[maxn];
struct Node { int l,r,cnt,sum; } sgt[maxh<<2];
inline int ls(int k) { return k<<1; }
inline int rs(int k) { return k<<1|1; }
inline void pushup(int k)
{
    sgt[k].cnt = sgt[ls(k)].cnt+sgt[rs(k)].cnt;
    sgt[k].sum = sgt[ls(k)].sum+sgt[rs(k)].sum;
}
void build(int l,int r,int k=1)
{
    sgt[k].l=l, sgt[k].r=r;
    if(l==r) return;
    int m = (l+r)>>1;
    build(l,m,ls(k));
    build(m+1,r,rs(k));
    pushup(k);
}
void modify(int p,int z,int k=1)
{
    int l=sgt[k].l, r=sgt[k].r;
    if(l==r)
    {
        sgt[k].cnt += z;
        sgt[k].sum += z*l;
        return;
    }
    int m = (l+r)>>1;
    if(p<=m) modify(p,z,ls(k));
    else modify(p,z,rs(k));
    pushup(k);
}
pair<int,int> query(int p,int k=1)
{
    int l=sgt[k].l, r=sgt[k].r;
    if(l>=p) return {sgt[k].cnt, sgt[k].sum};
    int m = (l+r)>>1;
    if(p<=m)
    {
        auto r = query(p, ls(k));
        return {r.first+sgt[rs(k)].cnt, r.second+sgt[rs(k)].sum};
    }
    else return query(p, rs(k)); 
}
int pos[maxn];
vector<tp3> q;
int ans[maxn];
inline void add(int p) { modify(a[p], 1); }
inline void sub(int p) { modify(a[p], -1); }
signed main(signed argc, char const *argv[])
{
#ifdef LOCAL
    freopen("in.in", "r", stdin);
    freopen("out.out", "w", stdout);
    auto c1 = chrono::high_resolution_clock::now();
#endif
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    //======================================
    build(1,1e5);
    int n,m;
    cin>>n>>m;
    for(int i=1;i<=n;i++) cin>>a[i];
    int siz = sqrt(n);
    for(int i=1;i<=n;i++) pos[i] = i/siz;
    for(int i=0;i<m;i++)
    {
        int l,r;
        cin>>l>>r;
        q.emplace_back(l,r,i);
    }
    sort(q.begin(), q.end(), [](const tp3& a,const tp3& b) {
        return pos[get<0>(a)]==pos[get<0>(b)]?get<1>(a)<get<1>(b):pos[get<0>(a)]<pos[get<0>(b)];
    });
    int l=1, r=0;
    for(int i=0;i<m;i++)
    {
        auto [x,y,z] = q[i];
        while(l<x) sub(l++);
        while(l>x) add(--l);
        while(r<y) add(++r);
        while(r>y) sub(r--);
        auto p = query(ceil((r-l+1)/2.0));
        ans[z] = p.second-(p.first*(p.first-1)>>1);
    }
    for(int i=0;i<m;i++) cout<<ans[i]+42<<'\n';
    //======================================
#ifdef LOCAL
    auto c2 = chrono::high_resolution_clock::now();
    cerr<<"Time Used:"<<chrono::duration_cast<chrono::milliseconds>(c2-c1).count()<<"ms"<<endl;
#endif
    return 0;
}
