vector<pii>E;
// 如果原图都为边双分量，将其改为强联通分量时边的方向
struct EBCC {
    int n;
    vector<vector<pii>>& adj;
    //到达的点 边的编号
    vector<int> stk;
    vector<int> dfn, low, bel;
    vector<vector<int>> ebcc;
    int m;
    vector<int>bridge;
    int cur;

    EBCC(int n_, int m_, auto& adj_) : adj(adj_), n(n_), m(m_) { init(n, m); }

    void init(int n, int m)
    {
        this->n = n;
        this->m = m;
        dfn.resize(n + 1);
        low.resize(n + 1);
        bel.assign(n + 1, -1);
        bridge.resize(m * 2);
        stk.clear();
        cur = 0;
        work();
    }
    void dfs(int x, int pre)
    {
        dfn[x] = low[x] = ++cur;
        stk.push_back(x);
        for (auto [y, id] : adj[x]) {
            if (dfn[y] == 0) {
                E.emplace_back(x, y);
                dfs(y, id);
                low[x] = min(low[x], low[y]);
                if (low[y] > dfn[x]) {
                    bridge[id] = bridge[id ^ 1] = 1;
                }
            }
            else if (id != (pre ^ 1)) {
                if (dfn[y] < dfn[x])E.emplace_back(x, y);
                low[x] = min(low[x], dfn[y]);
            }
        }
        if (dfn[x] == low[x]) {
            int cnt = ebcc.size();
            ebcc.push_back({});
            int y;
            do {
                y = stk.back();
                bel[y] = cnt;
                stk.pop_back();
                ebcc.back().push_back(y);
            } while (y != x);
        }
    }
    void work()
    {
        for (int i = 1; i <= n; i++) {
            if (dfn[i] == 0) {
                dfs(i, -1);
            }
        }
    }
    vector<vector<int>> rebuild()
    {
        int cnt = ebcc.size();
        vector<vector<int>>g(cnt);
        for (int u = 1; u <= n; u++) {
            for (auto [v, _] : adj[u]) {
                if (bel[u] != bel[v]) {
                    g[bel[u]].push_back(bel[v]);
                }
            }
        }
        return g;
    }
};