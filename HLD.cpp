struct HLD {
  int n;
  vector<int> siz, top, dep, par, in, out, seq;
  vector<vector<int>> adj;
  int cur;

  HLD(int n_, vector<vector<int>> &adj_, int r = 1) : n(n_), adj(adj_) {
    init(n);
    top[r] = r;
    dep[r] = 1;
    dfs1(r);
    dfs2(r);
  }
  void init(int n) {
    this->n = n;
    siz.resize(n + 1);
    top.resize(n + 1);
    dep.resize(n + 1);
    par.resize(n + 1);
    in.resize(n + 1);
    out.resize(n + 1);
    seq.resize(n + 1);
    cur = 0;
  }
  void dfs1(int u) {
    if (par[u] != 0) {
      adj[u].erase(find(adj[u].begin(), adj[u].end(), par[u]));
    }

    siz[u] = 1;
    for (auto &v : adj[u]) {
      par[v] = u;
      dep[v] = dep[u] + 1;
      dfs1(v);
      siz[u] += siz[v];
      if (siz[v] > siz[adj[u][0]]) {
        swap(v, adj[u][0]);
      }
    }
  }
  void dfs2(int u) {
    in[u] = ++cur;
    seq[in[u]] = u;
    for (auto v : adj[u]) {
      top[v] = v == adj[u][0] ? top[u] : v;
      dfs2(v);
    }
    out[u] = cur;
  }
  int lca(int u, int v) {
    while (top[u] != top[v]) {
      if (dep[top[u]] > dep[top[v]]) {
        u = par[top[u]];
      } else {
        v = par[top[v]];
      }
    }
    return dep[u] < dep[v] ? u : v;
  }

  int dist(int u, int v) { return dep[u] + dep[v] - 2 * dep[lca(u, v)]; }

  int jump(int u, int k) {
    if (dep[u] < k) {
      return -1;
    }

    int d = dep[u] - k;

    while (dep[top[u]] > d) {
      u = par[top[u]];
    }

    return seq[in[u] - dep[u] + d];
  }

  // Is u is v par ?
  bool isAncester(int u, int v) { return in[u] <= in[v] && in[v] <= out[u]; }
  // make u to be root then get the parent of v
  int rootedParent(int u, int v) {
    if (u == v) {
      return u;
    }
    if (!isAncester(v, u)) {
      return par[v];
    }
    auto it = upper_bound(adj[v].begin(), adj[v].end(), u,
                          [&](int x, int y) { return in[x] < in[y]; });
    return *prev(it);
  }
  // make u to be root then get the size of subtree v
  int rootedSize(int u, int v) {
    if (u == v) {
      return n;
    }
    if (!isAncester(v, u)) {
      return siz[v];
    }
    return n - siz[rootedParent(u, v)];
  }
  // make a to be root the get the lca between b and c
  int rootedLca(int a, int b, int c) {
    return lca(a, b) ^ lca(b, c) ^ lca(c, a);
  }
  // hld.modify_path(x, y, [&](int x, int y) -> void { tree.rangeupdate(x,
  // y,{z}); });
  template <class Q>
  void modify_path(int u, int v, const Q &q, bool edge = false) {
    while (top[u] != top[v]) {
      if (dep[top[u]] < dep[top[v]]) swap(u, v);
      q(in[top[u]], in[u]);
      u = par[top[u]];
    }
    if (dep[u] > dep[v]) swap(u, v);
    q(in[u] + edge, in[v]);
  }

  template <class Q>
  void modify_subtree(int u, const Q &q) {
    q(in[u], out[u]);
  }
  // hld.query_subtree<Info>(x, [&](int x, int y) -> Info { return tree.query(x,
  // y); })
  template <class T, class Q>
  T query_path(int u, int v, const Q &q, bool edge = false) {
    T ret = T();
    while (top[u] != top[v]) {
      if (dep[top[u]] < dep[top[v]]) swap(u, v);
      ret = q(in[top[u]], in[u]) + ret;
      u = par[top[u]];
    }
    if (dep[u] > dep[v]) swap(u, v);
    return q(in[u] + edge, in[v]) + ret;
  }

  template <class T, class Q>
  T query_subtree(int u, const Q &q) {
    return q(in[u], out[u]);
  }
};