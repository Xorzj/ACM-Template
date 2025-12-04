namespace flow {
  const int N = 6e4 + 5, M = 9e6 + 5;
  struct edge {
    int next, to, w;
  } e[M << 1];
  int n, s, t;
  int head[N], cnt, cur[N], dis[N];
  void init(int n_, int s_, int t_)
  {
    s = s_, t = t_, n = n_, cnt = 0;
    for (int i = 0; i < n; i++) head[i] = -1;
  }
  void add_edge(int u, int v, int w)
  {
    e[cnt] = edge{ head[u], v, w };
    head[u] = cnt;
    cnt++;
    e[cnt] = edge{ head[v], u, 0 };
    head[v] = cnt;
    cnt++;
  }
  bool bfs()
  {
    for (int i = 0; i < n; i++) cur[i] = head[i], dis[i] = -1;
    queue<int> q;
    dis[s] = 0;
    q.push(s);
    while (!q.empty()) {
      int x = q.front();
      q.pop();
      for (int i = head[x]; ~i; i = e[i].next) {
        if (e[i].w) {
          int y = e[i].to;
          if (~dis[y]) continue;
          dis[y] = dis[x] + 1;
          q.push(y);
        }
      }
    }
    return ~dis[t];
  }
  int dfs(int x, int flow)
  {
    int ans = 0;
    if (x == t) return flow;
    for (int i = cur[x]; ~i; i = e[i].next, cur[x] = i) {
      int y = e[i].to, newflow = min(e[i].w, flow);
      if (dis[y] != dis[x] + 1 || !e[i].w) continue;
      newflow = dfs(y, newflow);
      e[i].w -= newflow;
      e[i ^ 1].w += newflow;
      flow -= newflow;
      ans += newflow;
      if (!flow) break;
    }
    return ans;
  }
  int dinic()
  {
    int ans = 0;
    while (bfs()) ans += dfs(s, numeric_limits<int>::max());
    return ans;
  }
  //返回S部点集，必须先跑完flow
  vector<bool> minCut()
  {
    vector<bool> c(n);
    for (int i = 0; i < n; i++) {
      c[i] = (dis[i] != -1);
    }
    return c;
  }
  //返回实际流量和上限流量
  struct Edge {
    int from;
    int to;
    int cap;
    int flow;
  };
  vector<Edge> edges()
  {
    vector<Edge> a;
    for (int i = 0; i < cnt; i += 2) {
      Edge x;
      x.from = e[i + 1].to;
      x.to = e[i].to;
      x.cap = e[i].w + e[i + 1].w;
      x.flow = e[i + 1].w;
      a.push_back(x);
    }
    return a;
  }
}  // namespace flow
using flow::add_edge;
using flow::dinic;
using flow::init;
