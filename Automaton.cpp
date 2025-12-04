struct AC {
  static const int MAXN = 5e5 + 5;
  static const int MAXC = 26;
  static const int MAXQ = 5e5 + 5;
  int tree[MAXN][MAXC], val[MAXN], fail[MAXN], end[MAXN], sum[MAXN], invend[MAXQ];
  int cnt, n;
  // cnt 为节点编号 n为询问次数
  vector<int> adj[MAXN];
  AC() {}
  AC(int _n)
  {
    init(_n);
  }
  void init(int _n)
  {
    n = _n;
    cnt = 0;
  }
  void ins(string s, int num = 0)
  {
    int u = 0;
    for (auto x : s) {
      int c = x - 'a';
      if (!tree[u][c]) tree[u][c] = ++cnt;
      u = tree[u][c];
    }
    val[u]++;
    end[u] = num;
    invend[num] = u;
  }
  void build()
  {
    queue<int> q;
    for (int i = 0; i < MAXC; i++)
      if (tree[0][i]) fail[tree[0][i]] = 0, q.push(tree[0][i]);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int i = 0; i < MAXC; i++) {
        if (tree[u][i]) {
          fail[tree[u][i]] = tree[fail[u]][i];
          q.push(tree[u][i]);
        }
        else
          tree[u][i] = tree[fail[u]][i];
      }
    }
  }
  void failtree()
  {
    for (int i = 1; i <= cnt; i++) {
      adj[fail[i]].push_back(i);
    }
  }
  int query_times(string s)  // 查询有多少个不同的模式串在文本串里出现过
  {
    int u = 0;
    int ans = 0;
    for (auto x : s) {
      int c = x - 'a';
      u = tree[u][c];
      for (int v = u; v && val[v] != -1; v = fail[v])
        ans += val[v], val[v] = -1;
    }
    return ans;
  }
  void query_appear_times(string s)  // 找出每一个模式串(模式串允许重复)在文本串 T 中出现的次数
  {
    int u = 0;
    for (auto x : s) {
      int c = x - 'a';
      u = tree[u][c];
      sum[u]++;
    }
    auto dfs = [&](int x, auto&& self)->void {
      for (auto y : adj[x]) {
        self(y, self);
        sum[x] += sum[y];
      }
    };
    dfs(0, dfs);
    for (int i = 1; i <= n; i++) {
      cout << sum[invend[i]] << "\n";
    }
  }
  void clear()
  {
    for (int i = 0; i <= cnt; i++) {
      fail[i] = end[i] = val[i] = sum[i] = 0;
      adj[i].clear();
      for (int j = 0; j < MAXC; j++) {
        tree[i][j] = 0;
      }
    }
    cnt = 0;
  }
};