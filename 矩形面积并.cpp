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
// 矩形面积并
void solve() {
  int n;
  cin >> n;
  vector<int> X, Y;
  vector<array<int, 4>> p(n);
  for (int i = 0; i < n; i++) {
    cin >> p[i][0] >> p[i][1] >> p[i][2] >> p[i][3];
    X.push_back(p[i][0]);
    X.push_back(p[i][2]);
    Y.push_back(p[i][1]);
    Y.push_back(p[i][3]);
  }
  sort(ALL(X));
  X.erase(unique(ALL(X)), X.end());
  sort(ALL(Y));
  Y.erase(unique(ALL(Y)), Y.end());
  int Nx = X.size(), Ny = Y.size();
  vector<vector<array<int, 3>>> seg(Ny + 1);
  deb(X, Y);
  for (auto [x, y, a, b] : p) {
    int L = lower_bound(ALL(X), x) - X.begin() + 1,
        R = lower_bound(ALL(X), a) - X.begin() + 1;
    int U = lower_bound(ALL(Y), y) - Y.begin(),
        V = lower_bound(ALL(Y), b) - Y.begin();
    if (L > R) swap(L, R);
    if (U < V) swap(U, V);
    seg[V].push_back({L, R, 1});
    seg[U].push_back({L, R, -1});
  }
  LazySegmentTree<Info, Tag> tree(Nx - 1);
  for (int i = 0; i + 1 < Nx; i++) {
    tree.update(i + 1, {0, X[i + 1] - X[i]});
  }
  int ans = 0;
  for (int i = 0; i + 1 < Ny; i++) {
    for (auto [l, r, v] : seg[i]) {
      tree.rangeupdate(l, r - 1, {v});
    }
    auto [mn, cnt] = tree.query(1, Nx - 1);
    ans += (X.back() - X[0] - (mn == 0) * cnt) * (Y[i + 1] - Y[i]);
  }
  cout << ans << "\n";
}
