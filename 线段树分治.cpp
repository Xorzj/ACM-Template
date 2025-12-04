#include <bits/stdc++.h>
#define rep(a, b, c) for (int a = b; a <= c; a++)
#define ALL(x) (x).begin(), (x).end()
#define IOS cin.tie(0)->sync_with_stdio(false)
#ifdef LOCAL
#else
#define deb(...) 42
#endif
#define OPENSTACK
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
const int MAXN = 2e5 + 5;
const int INF = 0x3f3f3f3f;
struct DSU {
    std::vector<int> f, siz;
    vector<int>sta;
    int top;
    DSU() {}
    DSU(int n) { init(n); }
    void init(int n)
    {
        top = 0;
        f.resize(n + 1);
        sta.resize(2 * n + 1);
        std::iota(f.begin(), f.end(), 0);
        siz.assign(n + 1, 1);
    }
    int find(int x)
    {
        if (x == f[x])return x;
        return find(f[x]);
    }
    bool same(int x, int y) { return find(x) == find(y); }
    bool merge(int x, int y)
    {
        x = find(x);
        y = find(y);
        if (x == y) {
            return false;
        }
        if (siz[x] < siz[y])swap(x, y);
        siz[x] += siz[y];
        f[y] = x;
        sta[++top] = y;
        return true;
    }
    void back()
    {
        int y = sta[top--];
        siz[f[y]] -= siz[y];
        f[y] = y;
    }
    void turnback(int t = 0)
    {
        while (top > t)back();
    }
    int size(int x)
    {
        return siz[find(x)];
    }
}dsu;
ll ans = 0;
vector<vector<pii>>col;
template<class Info>
struct seg_tree {
    vector<vector<Info>>info;
    int n;
    seg_tree() {}
    seg_tree(int n_) { init(n_); }
    void init(int n_)
    {
        n = n_;
        dsu.init(n);
        info.resize(4 * (n + 1) + 5);
    }

    void add(int x, int l, int r, int ql, int qr, Info v)
    {
        if (ql <= l && r <= qr) {
            info[x].push_back(v);
            return;
        }
        int mid = l + r >> 1;
        if (ql <= mid)add(x << 1, l, mid, ql, qr, v);
        if (qr > mid)add(x << 1 | 1, mid + 1, r, ql, qr, v);
    }
    void add(int ql, int qr, Info v)
    {
        add(1, 1, n, ql, qr, v);
    }
    void dfs(int x, int l, int r)
    {
        int lst = dsu.top;
        for (auto [u, v] : info[x]) {
            dsu.merge(u, v);
        }
        if (l == r) {
            for (auto [u, v] : col[l]) {
                ans += 1LL * dsu.size(u) * dsu.size(v);
            }
            dsu.turnback(lst);
            return;
        }
        int mid = l + r >> 1;
        dfs(x << 1, l, mid);
        dfs(x << 1 | 1, mid + 1, r);
        dsu.turnback(lst);
    }
};
void solve()
{
    int n;
    cin >> n;
    seg_tree<pii>tree(n);
    dsu.init(n);
    col.resize(n + 1);
    for (int i = 1; i < n; i++) {
        int x, y, w;
        cin >> x >> y >> w;
        col[w].emplace_back(x, y);
        if (w - 1 >= 1)tree.add(1, w - 1, { x,y });
        if (w + 1 <= n)tree.add(w + 1, n, { x,y });
    }
    tree.dfs(1, 1, n);
    cout << ans << "\n";
}
int main()
{
#ifdef LOCAL
#ifdef OPENSTACK
    int size = 128 << 20; // 64MB
    char* p = (char*) malloc(size) + size;
#if (defined _WIN64) or (defined __unix)
    __asm__("movq %0, %%rsp\n" ::"r"(p));
#else
    __asm__("movl %0, %%esp\n" ::"r"(p));
#endif
#endif
#endif
    IOS;
    int _ = 1;
    while (_--) {
        solve();
    }
#ifdef LOCAL
#ifdef OPENSTACK
    exit(0);
#else
    return 0;
#endif
#endif
}