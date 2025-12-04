const int MAXN = 5e5 + 5;
const int D = 5e5;
const int INF = 0x3f3f3f3f3f3f3f3fll;
int n, m, s;
int id[MAXN];
int dis[MAXN << 1];
vector<pair<int, int>> g[MAXN << 1];
void build(int x, int l, int r) {
  if (l == r) {
    id[l] = x;
    return;
  }
  int mid = l + r >> 1;
  g[x].push_back({x << 1, 0});
  g[x].push_back({x << 1 | 1, 0});
  g[(x << 1) + D].push_back({x + D, 0});
  g[(x << 1 | 1) + D].push_back({x + D, 0});
  build(x << 1, l, mid);
  build(x << 1 | 1, mid + 1, r);
}
void add(int x, int l, int r, int L, int R, int v, int w, int op) {
  if (L <= l && r <= R) {
    if (op)
      g[v].push_back({x, w});
    else
      g[x + D].push_back({v, w});
    return;
  }
  int mid = l + r >> 1;
  if (L <= mid) add(x << 1, l, mid, L, R, v, w, op);
  if (R > mid) add(x << 1 | 1, mid + 1, r, L, R, v, w, op);
}
void dijkstra(int s) {
  vector<int> vis(MAXN * 2, 0);
  priority_queue<pair<int, int>> q;
  memset(dis, INF, sizeof(dis));
  dis[s] = 0;
  q.push({0, s});
  while (!q.empty()) {
    int x = q.top().second;
    q.pop();
    if (vis[x]) continue;
    vis[x] = 1;
    for (auto [y, w] : g[x]) {
      dis[y] = min(dis[y], dis[x] + w);
      q.push({-dis[y], y});
    }
  }
}
// op 1 : v->u 单向边
// op 2 : v->[l,r]
// op 3 :[l,r]-> v
void solve() {
  cin >> n >> m >> s;
  build(1, 1, n);
  for (int i = 1; i <= n; i++) {
    int x = id[i];
    g[x].push_back({x + D, 0});
    g[x + D].push_back({x, 0});
  }
  while (m--) {
    int op;
    cin >> op;
    if (op == 1) {
      int v, u, w;
      cin >> v >> u >> w;
      g[id[v]].push_back({id[u], w});
    } else if (op == 2) {
      int v, l, r, w;
      cin >> v >> l >> r >> w;
      add(1, 1, n, l, r, id[v], w, 1);
    } else {
      int v, l, r, w;
      cin >> v >> l >> r >> w;
      add(1, 1, n, l, r, id[v], w, 0);
    }
  }
  dijkstra(id[s]);
  for (int i = 1; i <= n; i++) {
    if (dis[id[i]] == INF)
      cout << "-1 ";
    else
      cout << dis[id[i]] << ' ';
  }
}