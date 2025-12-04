//存在负权边无法使用，因为区间dep最小值不一定是离根节点最近的lca
struct Tag {
    int add = 0;
    void apply(const Tag& v) { add += v.add; }
};
struct Info {
    int mind = INF, maxd = -INF;
    int lm = -INF, rm = -INF, lmr = -INF;
    void apply(const Tag& v)
    {
        mind += v.add;
        maxd += v.add;
        lm -= v.add;
        rm -= v.add;
    }
};
Info operator+(const Info& a, const Info& b)
{
    Info c = Info();
    c.mind = min(a.mind, b.mind);
    c.maxd = max(a.maxd, b.maxd);
    c.lm = max({ a.lm,b.lm,a.maxd - 2 * b.mind });
    c.rm = max({ a.rm,b.rm,b.maxd - 2 * a.mind });
    c.lmr = max({ a.lmr,b.lmr,a.lm + b.maxd,a.maxd + b.rm });
    return c;
}
void solve()
{
    int n, q, W;
    cin >> n >> q >> W;
    vector<vector<array<int, 2>>>adj(n + 1);
    vector<array<int, 3>>Edge(n);
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
            dfn.push_back(x);
        }
        out[x] = dfn.size();
    };
    pre_init(1, 0, pre_init);
    int N = dfn.size();
    LazySegmentTree<Info, Tag> tree(N, Info{ 0,0,0,0,0 });
    for (int i = 1; i < n; i++) {
        auto [x, y, w] = Edge[i];
        if (dep[x] < dep[y])swap(x, y);
        tree.rangeupdate(in[x], out[x], Tag{ w });
    }
    int lst = 0;
    while (q--) {
        int D, E;
        cin >> D >> E;
        D = (lst + D) % (n - 1) + 1;
        E = (lst + E) % W;
        auto [x, y, w] = Edge[D];
        if (dep[x] < dep[y])swap(x, y);
        tree.rangeupdate(in[x], out[x], Tag{ E - w });
        Edge[D][2] = E;
        lst = tree.query(1, N).lmr;
        cout << lst << "\n";
    }
}