struct Tag {
  ll add = 0;
  void apply(const Tag &v) { add += v.add; }
};
struct Info {
  ll mn = 0, len = 1;
  void apply(const Tag &v) { mn += v.add; }
};
Info operator+(const Info &a, const Info &b) {
  if (a.mn == b.mn) {
    return {a.mn, a.len + b.len};
  } else if (a.mn < b.mn) {
    return a;
  } else {
    return b;
  }
}
void solve() {
  int n;
  cin >> n;
  vector<array<int, 4>> a(n);
  vector<int> X, Y;
  for (auto &[x, y, u, v] : a) {
    cin >> x >> y >> u >> v;
    X.push_back(x);
    X.push_back(u);
    Y.push_back(y);
    Y.push_back(v);
  }
  sort(ALL(X));
  X.erase(unique(ALL(X)), X.end());
  sort(ALL(Y));
  Y.erase(unique(ALL(Y)), Y.end());
  int Nx = X.size(), Ny = Y.size();
  ll ans = 0;
  vector<vector<array<int, 3>>> seg(Ny);
  vector<vector<array<int, 3>>> seg2(Nx);
  LazySegmentTree<Info, Tag> tree(Nx - 1);
  LazySegmentTree<Info, Tag> tree2(Ny - 1);
  for (auto [x, y, u, v] : a) {
    int L = lower_bound(ALL(X), x) - X.begin() + 1;
    int R = lower_bound(ALL(X), u) - X.begin() + 1;
    int U = lower_bound(ALL(Y), y) - Y.begin();
    int V = lower_bound(ALL(Y), v) - Y.begin();
    if (L > R) swap(L, R);
    if (U < V) swap(U, V);
    seg[V].push_back({L, R, 1});
    seg[U].push_back({L, R, -1});
    seg2[L - 1].push_back({V + 1, U + 1, 1});
    seg2[R - 1].push_back({V + 1, U + 1, -1});
  }
  for (int i = 0; i < Ny; i++)
    sort(ALL(seg[i]), [&](auto x, auto y) { return x[2] > y[2]; });
  for (int i = 0; i < Nx; i++)
    sort(ALL(seg2[i]), [&](auto x, auto y) { return x[2] > y[2]; });
  for (int i = 1; i < Nx; i++) tree.update(i, {0, X[i] - X[i - 1]});
  for (int i = 1; i < Ny; i++) tree2.update(i, {0, Y[i] - Y[i - 1]});
  int lst = 0;
  for (int i = 0; i < Ny; i++) {
    for (auto [l, r, v] : seg[i]) {
      tree.rangeupdate(l, r - 1, {v});
      auto [mn, cnt] = tree.query(1, Nx - 1);
      int V = X.back() - X[0] - (mn == 0) * cnt;
      ans += abs(V - lst);
      lst = V;
    }
  }
  lst = 0;
  for (int i = 0; i < Nx; i++) {
    for (auto [l, r, v] : seg2[i]) {
      tree2.rangeupdate(l, r - 1, {v});
      auto [mn, cnt] = tree2.query(1, Ny - 1);
      int V = (Y.back() - Y[0] - (mn == 0) * cnt);
      ans += abs(V - lst);
      lst = V;
    }
  }
  cout << ans << "\n";
}