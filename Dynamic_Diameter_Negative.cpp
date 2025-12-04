//O(nlog^2n) 每次操作树状数组维护维护2logn区间
template <class Info, class Tag, class F>
struct LazySegmentTree {
    int n;
    vector<Info> info;
    vector<Tag> tag;
    F pred;
    LazySegmentTree() : n(0) {}
    LazySegmentTree(int n_, Info v_ = Info(), F pred_ = F()) :pred(pred_) { init(vector<Info>(n_ + 1, v_)); }
    LazySegmentTree(vector<Info> t_) { init(t_); }
    void init(vector<Info> a)  //[1,n]
    {
        n = a.size() - 1;
        info.assign((n << 2) + 1, Info());
        tag.assign((n << 2) + 1, Tag());
        function<void(int, int, int)> build = [&](int x, int l, int r) -> void {
            if (l == r) {
                info[x] = a[l];
                return;
            }
            int mid = l + r >> 1;
            build(x << 1, l, mid);
            build(x << 1 | 1, mid + 1, r);
            pull(x);
        };
        build(1, 1, n);
    }
    void pull(int x) { info[x] = combine(info[x << 1], info[x << 1 | 1], pred); }
    void update(int x, int l, int r, int p, const Info& v)
    {
        if (l == r) {
            info[x] = v;
            return;
        }
        int mid = l + r >> 1;
        if (p <= mid)
            update(x << 1, l, mid, p, v);
        else
            update(x << 1 | 1, mid + 1, r, p, v);
        pull(x);
    }
    void update(int p, const Info& v) { update(1, 1, n, p, v); }
    Info query(int x, int l, int r, int ql, int qr)
    {
        if (l > qr || r < ql) return Info();
        if (ql <= l && r <= qr) return info[x];
        int mid = l + r >> 1;
        return combine(query(x << 1, l, mid, ql, qr), query(x << 1 | 1, mid + 1, r, ql, qr), pred);
    }
    Info query(int ql, int qr) { return query(1, 1, n, ql, qr); }
    void rangeupdate(int x, int l, int r, int ql, int qr, const Tag& v)
    {
        if (r<ql || l>qr)return;
        if (ql <= l && r <= qr) {
            return;
        }
        int mid = l + r >> 1;
        rangeupdate(x << 1, l, mid, ql, qr, v);
        rangeupdate(x << 1 | 1, mid + 1, r, ql, qr, v);
        pull(x);
    }
    void rangeupdate(int ql, int qr, const Tag& v) { rangeupdate(1, 1, n, ql, qr, v); }
};
struct Tag {
    void apply(const Tag& v) {}
};
struct Info {
    array<int, 2>p{ -1,-1 };
    int dia = 0;
    void apply(const Tag& v) {}
};
template<class F>
Info combine(const Info& a, const Info& b, F dis)
{
    Info c = Info();
    if (a.dia < b.dia) {
        c.p = b.p;
        c.dia = b.dia;
    }
    else {
        c.p = a.p;
        c.dia = a.dia;
    }
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            int d = dis(a.p[i], b.p[j]);
            if (d > c.dia) {
                c.dia = d;
                c.p = { a.p[i],b.p[j] };
            }
        }
    }
    return c;
}
void solve()
{
    int n, q, W;
    cin >> n >> q >> W;
    vector<vector<array<int, 2>>>adj(n + 1);
    vector<array<int, 3>>Edge(n);
    vector<vector<int>>g(n + 1);
    for (int i = 1; i < n; i++) {
        int x, y, w;
        cin >> x >> y >> w;
        Edge[i] = { x,y,w };
        adj[x].push_back({ y,w });
        adj[y].push_back({ x,w });
    }
    vector<int>dfn, in(n + 1), out(n + 1);
    vector<int>dep(n + 1);
    auto pre_init = [&](int x, int fa, auto self)->void {
        dfn.push_back(x);
        in[x] = dfn.size();
        for (auto [y, w] : adj[x]) {
            if (y == fa)continue;
            dep[y] = dep[x] + 1;
            self(y, x, self);
        }
        out[x] = dfn.size();
    };
    pre_init(1, 0, pre_init);
    Fenwick<int>c(n);
    auto pred = [&](int x, int y)->int {
        if (x == -1 || y == -1)return -INF;
        return c.sum(x) + c.sum(y) - 2 * c.sum(in[L.lca(dfn[x - 1], dfn[y - 1])]);
    };
    LazySegmentTree<Info, Tag, decltype(pred) >tree(n, Info(), pred);
    for (int i = 1; i <= n; i++) {
        Info t;
        t.p = { i,i };
        t.dia = 0;
        tree.update(i, t);
    }
    auto update = [&](int l, int r, int w) {
        c.add(l, w);
        if (r + 1 <= n)c.add(r + 1, -w);
        tree.rangeupdate(l, r, {});
    };
    for (int i = 1; i < n; i++) {
        auto [x, y, w] = Edge[i];
        if (dep[x] < dep[y])swap(x, y);
        update(in[x], out[x], w);
    }
    int lst = 0;
    while (q--) {
        int D, E;
        cin >> D >> E;
        D = (lst + D) % (n - 1) + 1;
        E = (lst + E) % W;
        auto [x, y, w] = Edge[D];
        if (dep[x] < dep[y])swap(x, y);
        update(in[x], out[x], E - w);
        Edge[D][2] = E;
        lst = tree.query(1, n).dia;
        cout << lst << "\n";
    }
}