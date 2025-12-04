struct Info {
  int mn = 0;
  void apply(const Tag& v) {}
};
Info operator+(const Info& a, const Info& b)
{
  Info c = Info();
  c.mn = min(a.mn, b.mn);
  return c;
}
// 离线区间mex
void solve()
{
  int n, m;
  cin >> n >> m;
  vector<int> a(n + 1);
  for (int i = 1; i <= n; i++) cin >> a[i], a[i]++;
  vector<array<int, 3>> q(m);
  for (int i = 0; i < m; i++) {
    int l, r;
    cin >> l >> r;
    q[i] = { l, r, i };
  }
  sort(ALL(q), [&](const array<int, 3>& fi, const array<int, 3>& se) {
    return fi[1] < se[1];
  });
  const int N = 2e5 + 1;
  LazySegmentTree<Info, Tag> tree(N);
  int j = 0;
  vector<int> ans(m);
  for (auto [l, r, id] : q) {
    while (j + 1 <= r) {
      j++;
      tree.update(a[j], Info{ j });
    }
    ans[id] =
      tree.findFirst(1, N, [&](const Info& v) { return v.mn <= l - 1; });
    //N 必须至少为值域+1 否则可能查找不到mex 
  }
  for (int i = 0; i < m; i++) cout << ans[i] - 1 << "\n";
}
