typedef pair<double, double> pdd;
void solve() {
  int n;
  cin >> n;
  double ans = 1e18;
  vector<pdd> p(n);
  for (auto &[x, y] : p) cin >> x >> y;
  sort(ALL(p));
  auto cmp = [&](const pdd &a, const pdd &b) -> bool {
    return a.second < b.second;
  };
  multiset<pdd, decltype(cmp)> s(cmp);
  auto dis = [&](pdd a, pdd b) -> double {
    return sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
  };
  for (int i = 0, l = 0; i < n; i++) {
    while (l < i && p[i].first - p[l].first >= ans) s.erase(s.find(p[l++]));
    for (auto it = s.lower_bound({p[i].first, p[i].second - ans});
         it != s.end() && it->second - p[i].second < ans; it++)
      ans = min(ans, dis(*it, p[i]));
    s.insert(p[i]);
  }
  cout << fixed << setprecision(4) << ans << "\n";
}