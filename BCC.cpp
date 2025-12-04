struct BCC {
    int n;
    vector<vector<int>>& adj;
    vector<int> stk;
    vector<int> dfn, low, bel, cut;
    vector<vector<int>> bcc;
    int cur, cutnum;

    BCC(int n_, auto& adj_) : adj(adj_), n(n_) { init(n); }

    void init(int n)
    {
        this->n = n;
        dfn.resize(n + 1);
        low.resize(n + 1);
        bel.assign(n + 1, -1);
        cut.resize(n + 1);
        stk.clear();
        cur = 0;
        cutnum = 0;
        work();
    }
    void dfs(int x, int fa)
    {
        dfn[x] = low[x] = ++cur;
        stk.push_back(x);
        int child = 0;
        for (auto y : adj[x]) {
            if (dfn[y] == 0) {
                dfs(y, x);
                child++;
                low[x] = min(low[x], low[y]);
                if (low[y] >= dfn[x]) {
                    cut[x] = 1;
                    int cnt = bcc.size();
                    bcc.push_back({});
                    int z;
                    do {
                        z = stk.back();
                        bel[z] = cnt;
                        deb(x, z, cnt);
                        stk.pop_back();
                        bcc.back().push_back(z);
                    } while (y != z);
                    bcc.back().push_back(x);
                    bel[x] = cnt;
                }
            }
            else if (y != fa) {
                low[x] = min(low[x], dfn[y]);
            }
        }
        if (fa == 0 && child <= 1)cut[x] = 0;
        cutnum += cut[x];
    }
    vector<int> cutvertex()
    {
        vector<int>ans;
        for (int i = 1; i <= n; i++) {
            if (cut[i])ans.push_back(i);
        }
        return ans;
    }
    void work()
    {
        for (int i = 1; i <= n; i++) {
            if (dfn[i] == 0) {
                dfs(i, 0);
            }
        }
    }
    vector<vector<int>> rebuild()
    {
        vector<vector<int>>cut_from(n + 1);
        for (int i = 0; i < bcc.size(); i++) {
            for (int x : bcc[i]) {
                if (cut[x]) cut_from[x].push_back(i);
            }
        }
        vector<vector<int>>g(bcc.size() + cutnum);
        for (int u = 1; u <= n; u++) {
            if (cut[u]) {
                int cnt = bcc.size();
                int fu = bel[u] = cnt;
                bcc.push_back({ u });
                for (int fv : cut_from[u]) {
                    g[fu].push_back(fv);
                    g[fv].push_back(fu);
                }
            }
        }
        return g;
    }
};