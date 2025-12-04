struct SCC {
  // scc 数组逆序满足拓扑序
  int n;
  vector<vector<int>>& adj;
  vector<vector<int>> scc;
  vector<int> stk;
  vector<int> dfn, low, bel;
  int cur, cnt;

  SCC(int n_, vector<vector<int>>& adj_) : adj(adj_), n(n_) { init(n); }

  void init(int n)
  {
    this->n = n;
    dfn.assign(n + 1, 0);
    low.resize(n + 1);
    bel.assign(n + 1, -1);
    stk.clear();
    cur = 0;
    work();
  }

  void dfs(int x)
  {
    dfn[x] = low[x] = ++cur;
    stk.push_back(x);
    for (auto y : adj[x]) {
      if (dfn[y] == 0) {
        dfs(y);
        low[x] = min(low[x], low[y]);
      }
      else if (bel[y] == -1) {
        low[x] = min(low[x], dfn[y]);
      }
    }

    if (dfn[x] == low[x]) {
      int cnt = scc.size();
      scc.push_back({});
      int y;
      do {
        y = stk.back();
        bel[y] = cnt;
        stk.pop_back();
        scc.back().push_back(y);
      } while (y != x);
    }
  }
  vector<vector<int>>rebuild()
  {
    int cnt = scc.size();
    vector<vector<int>>g(cnt);
    for (int x = 1; x <= n; x++) {
      for (auto y : adj[x]) {
        if (bel[x] != bel[y]) {
          g[bel[x]].push_back(bel[y]);
        }
      }
    }
    return g;
  }
  void work()
  {
    for (int i = 1; i <= n; i++) {
      if (dfn[i] == 0) {
        dfs(i);
      }
    }
  }
};