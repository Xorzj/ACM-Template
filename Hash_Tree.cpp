const ull mask = std::chrono::steady_clock::now().time_since_epoch().count();
vector<int> g[MAXN];
ull has[MAXN];
set<ull> Tree;
ull shift(ull x) {
  x ^= mask;
  x ^= x << 13;
  x ^= x >> 7;
  x ^= x << 17;
  return x;
}
void dfs(int x, int fa) {
  has[x] = 1;
  for (auto y : g[x]) {
    if (y == fa) continue;
    dfs(y, x);
    has[x] += shift(has[y]);
  }
  Tree.insert(has[x]);
}
int main() {
  int n = read();
  rep(i, 1, n - 1) {
    int u = read(), v = read();
    g[u].push_back(v);
    g[v].push_back(u);
  }
  dfs(1, 0);
  cout << Tree.size() << "\n";
  return 0;
}