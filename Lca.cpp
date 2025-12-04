struct LCA  // fa[j][i] i 2^j father
{
  int n, root, D;
  vector<int> dep, lg;
  vector<vector<int>> fa;
  vector<vector<int>> &adj;
  LCA(int n_, auto &g_, int r = 1) : n(n_), adj(g_), root(r) {
    dep.resize(n + 1);
    lg.resize(n + 1);
    for (int i = 1; i <= n; i++) lg[i] = lg[i - 1] + (1 << lg[i - 1] == i);
    D = lg[n];
    fa.assign(lg[n] + 1, vector<int>(n + 1, 0));
    auto dfs = [&](int x, int f, auto dfs) -> void {
      dep[x] = dep[f] + 1;
      fa[0][x] = f;
      for (int i = 1; i <= D; i++) fa[i][x] = fa[i - 1][fa[i - 1][x]];
      for (auto y : adj[x]) {
        if (y == f) continue;
        dfs(y, x, dfs);
      }
    };
    dfs(root, 0, dfs);
  }
  int lca(int x, int y) {
    if (dep[x] < dep[y]) swap(x, y);
    while (dep[x] > dep[y]) x = fa[lg[dep[x] - dep[y]] - 1][x];
    if (x == y) return y;
    for (int i = lg[dep[x]] - 1; ~i; i--) {
      if (fa[i][x] != fa[i][y]) {
        x = fa[i][x];
        y = fa[i][y];
      }
    }
    return fa[0][x];
  }
  int jump(int x, int d) {
    for (int i = lg[n]; i >= 0; i--) {
      if ((d >> i) & 1) {
        d ^= (1 << i);
        x = fa[i][x];
      }
    }
    return (!d) ? x : 0;
  }
};