pair<int, int> tree_diameter_pair(int n, vector<vector<int>> g) {
  auto bfs = [&](int s) {
    vector<int> dis(n + 1, INF);
    vector<int> vis(n + 1, 0);
    queue<int> q;
    q.push(s);
    dis[s] = 0, vis[s] = 1;
    int mx = -1, d = 0;
    while (!q.empty()) {
      int x = q.front();
      if (mx < dis[x]) {
        mx = dis[x];
        d = x;
      }
      q.pop();
      for (auto y : g[x]) {
        if (vis[y]) continue;
        dis[y] = dis[x] + 1;
        vis[y] = 1;
        q.push(y);
      }
    }
    return d;
  };
  int U = bfs(1);
  int V = bfs(U);
  return make_pair(U, V);
}
int tree_diameter_value(int n, vector<vector<int>> g) {
  int d = 0;
  vector<int> dis1(n + 1), dis2(n + 1);
  function<void(int, int)> dfs = [&](int x, int fa) -> void {
    dis1[x] = dis2[x] = 0;
    for (auto y : g[x]) {
      if (y == fa) continue;
      dfs(y, x);
      int t = dis1[y] + 1;
      if (t > dis1[x]) {
        dis2[x] = dis1[x];
        dis1[x] = t;
      } else if (t > dis2[x])
        dis2[x] = t;
    }
    d = max(d, dis1[x] + dis2[x]);
  };
  dfs(1, 0);
  return d;
}