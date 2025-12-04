#include <bits/stdc++.h>
#define rep(a, b, c) for (int a = b; a <= c; a++)
#define ALL(x) (x).begin(), (x).end()
#define IOS cin.tie(0)->sync_with_stdio(false)
#ifdef LOCAL
#include "debug.h"
#else
#define deb(...) 42
#endif
#define OPENSTACK
using namespace std;
typedef long long ll;
typedef unsigned long long ull;
typedef pair<int, int> pii;
const int MAXN = 3e5 + 5;
const int INF = 0x3f3f3f3f;
struct DSU {
  std::vector<int> f, siz;
  vector<int> sta;
  int top;
  DSU() {}
  DSU(int n, int m) { init(n, m); }
  void init(int n, int m) {
    top = 0;
    f.resize(n + 1);
    sta.resize(m + 1);
    std::iota(f.begin(), f.end(), 0);
    siz.assign(n + 1, 1);
  }
  int find(int x) {
    if (x == f[x]) return x;
    return find(f[x]);
  }
  bool same(int x, int y) { return find(x) == find(y); }
  bool merge(int x, int y) {
    x = find(x);
    y = find(y);
    if (x == y) {
      return false;
    }
    if (siz[x] < siz[y]) swap(x, y);
    siz[x] += siz[y];
    f[y] = x;
    sta[++top] = y;
    return true;
  }
  void back() {
    int y = sta[top--];
    siz[f[y]] -= siz[y];
    f[y] = y;
  }
  void turnback(int t = 0) {
    while (top > t) back();
  }
};
void solve() {
  int n, m;
  cin >> n >> m;
  DSU dsu(n, m);
  vector<vector<int>> adj(m + 1);
  vector<int> op(m + 1);
  vector<int> a(m + 1);
  vector<int> b(m + 1);
  for (int i = 1; i <= m; i++) {
    cin >> op[i] >> a[i];
    if (op[i] != 2) {
      cin >> b[i];
      adj[i - 1].push_back(i);
    } else {
      adj[a[i]].push_back(i);
    }
  }
  vector<int> tim(m + 1);
  vector<int> ans(m + 1);
  auto dfs = [&](int x, auto dfs) -> void {
    tim[x] = dsu.top;
    if (op[x] == 3) ans[x] = dsu.same(a[x], b[x]);
    if (op[x] == 1) dsu.merge(a[x], b[x]);
    for (auto y : adj[x]) {
      dfs(y, dfs);
    }
    dsu.turnback(tim[x]);
  };
  dfs(0, dfs);
  for (int i = 1; i <= m; i++) {
    if (op[i] == 3) cout << ans[i] << "\n";
  }
}
int main() {
#ifdef LOCAL
#ifdef OPENSTACK
  int size = 128 << 20;  // 64MB
  char* p = (char*)malloc(size) + size;
#if (defined _WIN64) or (defined __unix)
  __asm__("movq %0, %%rsp\n" ::"r"(p));
#else
  __asm__("movl %0, %%esp\n" ::"r"(p));
#endif
#endif
#endif
  IOS;
  int _ = 1;
  while (_--) {
    solve();
  }
#ifdef LOCAL
#ifdef OPENSTACK
  exit(0);
#else
  return 0;
#endif
#endif
}