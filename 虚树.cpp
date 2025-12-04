/*
图计数
S 导出 T 的子图
G 满足以下所有条件：
G 是一棵树。度数为 1 的所有顶点都具有相同的颜色。
*/
// O(1) Lca
// pos 是唯一虚树有用的 其他都是求o1 lca的
void solve()
{
  int n;
  cin >> n;
  vector<int> col(n + 1);
  vector<vector<int>> g(n + 1);
  vector<vector<int>> sc(n + 1);
  for (int i = 1; i <= n; i++) {
    cin >> col[i];
    sc[col[i]].push_back(i);
  }
  for (int i = 1; i < n; i++) {
    int x, y;
    cin >> x >> y;
    g[x].push_back(y);
    g[y].push_back(x);
  }
  vector<int> dfn(2 * n);
  vector<int> idfn(n + 1);
  vector<int> pos(n + 1);
  int cnt = 0, tot = 0;
  auto dfs1 = [&](int x, int fa, auto self) -> void {
    tot++;
    int t = tot;
    dfn[++cnt] = tot;
    idfn[tot] = x;
    pos[x] = cnt;
    for (auto y : g[x]) {
      if (y == fa) continue;
      self(y, x, self);
      dfn[++cnt] = t;
    }
  };
  Z ans = 0;
  vector<vector<int>> son(n + 1);
  dfs1(1, 0, dfs1);
  ST S(dfn);
  vector<array<Z, 2>> f(n + 1);
  auto Lca = [&](int x, int y) -> int {
    if (x == y) return x;
    x = pos[x];
    y = pos[y];
    if (y < x) swap(x, y);
    return idfn[S.query(x, y)];
  };
  auto calc = [&](int c) -> void {
    if (sc[c].size() == 1) {
      ans += 1;
      return;
    }
    sort(ALL(sc[c]), [&](int x, int y) { return pos[x] < pos[y]; });
    for (int i = sc[c].size() - 1; i > 0; i--) {
      sc[c].push_back(Lca(sc[c][i], sc[c][i - 1]));
    }
    sort(ALL(sc[c]), [&](int x, int y) { return pos[x] < pos[y]; });
    sc[c].erase(unique(ALL(sc[c])), sc[c].end());
    for (int i = 1; i < sc[c].size(); i++) {
      son[Lca(sc[c][i], sc[c][i - 1])].push_back(sc[c][i]);
    }
    auto dp = [&](int x, auto dp) -> void {
      f[x][0] = 1, f[x][1] = 0;
      for (auto y : son[x]) {
        dp(y, dp);
        f[x][0] *= (f[y][0] + 1);
        f[x][1] += f[y][0];
      }
      if (col[x] == c) {
        ans += f[x][0];
      }
      else {
        f[x][0] -= 1;
        ans += f[x][0] - f[x][1];
      }
    };
    dp(sc[c].front(), dp);
    for (auto x : sc[c]) son[x].clear();
  };
  for (int i = 1; i <= n; i++) {
    if (sc[i].size()) calc(i);
  }
  cout << ans << "\n";
}
//对同一个颜色的所有有序点对，求他们之间距离之和
void solve()
{
  int n;
  cin >> n;
  vector<vector<int>>adj(n + 1);
  for (int i = 1; i < n; i++) {
    int x, y;
    cin >> x >> y;
    adj[x].push_back(y);
    adj[y].push_back(x);
  }
  Lca L(n, adj);
  vector<vector<int>>sc(n + 1);
  for (int i = 1; i <= n; i++) {
    int x;
    cin >> x;
    sc[x].push_back(i);
  }
  vector<int> pos(n + 1);
  int cnt = 0;
  auto dfs1 = [&](int x, int fa, auto self) -> void {
    pos[x] = ++cnt;
    for (auto y : adj[x]) {
      if (y == fa) continue;
      self(y, x, self);
    }
  };
  dfs1(1, 0, dfs1);
  ll ans = 0;
  vector<int>sum(n + 1);
  vector<vector<int>> son(n + 1);
  auto calc = [&](int c) -> void {
    for (auto x : sc[c])sum[x] = 1;
    int sz = sc[c].size();
    sort(ALL(sc[c]), [&](int x, int y) { return pos[x] < pos[y]; });
    for (int i = sc[c].size() - 1; i > 0; i--) {
      sc[c].push_back(L.lca(sc[c][i], sc[c][i - 1]));
    }
    sort(ALL(sc[c]), [&](int x, int y) { return pos[x] < pos[y]; });
    sc[c].erase(unique(ALL(sc[c])), sc[c].end());
    for (int i = 1; i < sc[c].size(); i++) {
      son[L.lca(sc[c][i], sc[c][i - 1])].push_back(sc[c][i]);
    }
    int root = sc[c].front();
    auto dfs_sum = [&](int x, auto self)->void {
      for (auto y : son[x]) {
        self(y, self);
        sum[x] += sum[y];
      }
    };
    dfs_sum(root, dfs_sum);
    auto dp = [&](int x, auto self) -> void {
      for (auto y : son[x]) {
        ans += 1LL * L.dis(x, y) * sum[y] * (sz - sum[y]);
        self(y, self);
      }
    };
    dp(root, dp);
    for (auto x : sc[c]) son[x].clear(), sum[x] = 0;
  };
  for (int i = 1; i <= n; i++) {
    if (sc[i].size() > 1) calc(i);
  }
  cout << ans << "\n";
}