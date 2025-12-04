// 固定根和非固定根的树哈希
struct Tree {
  int A = 0, B = 0;
  bool operator==(const Tree& o) const { return ((A == o.A) && (B == o.B)); }
};
map<vector<int>, int> id;
int idx;
struct AHU {
  int n;
  Tree v;
  vector<int> has;
  vector<vector<int>>& g;
  AHU(int n_, auto& g_) : g(g_), n(n_) { has.assign(n + 1, 0); }
  void dfs(int x, int fa) {
    vector<int> p;
    for (auto y : g[x]) {
      if (y == fa) continue;
      dfs(y, x);
      p.push_back(has[y]);
    }
    sort(p.begin(), p.end());
    int& q = id[p];
    if (!q) q = ++idx;
    has[x] = q;
  }
  int rooted(int root) {
    dfs(root, 0);
    return v.B = has[root];
  }
  void unrooted() {
    vector<int> siz(n + 1);
    vector<int> mx(n + 1);
    int mn = 1e9;
    auto dfs = [&](int x, int fa, auto dfs) -> void {
      siz[x] = 1;
      int M = 0;
      for (auto y : g[x]) {
        if (y == fa) continue;
        dfs(y, x, dfs);
        siz[x] += siz[y];
        M = max(M, siz[y]);
      }
      mx[x] = max(M, n - siz[x]);
      mn = min(mn, mx[x]);
    };
    dfs(1, 0, dfs);
    vector<int> vv;
    for (int i = 1; i <= n; i++) {
      if (mx[i] == mn) vv.push_back(rooted(i));
    }
    if (vv.size() == 2) {
      v.A = vv[0];
      v.B = vv[1];
    } else
      v.A = vv[0];
    if (v.A > v.B) swap(v.A, v.B);
  }
};
