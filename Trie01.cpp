struct Trie {
  static constexpr int N = 1e6;
  static constexpr int V = 19;
  array<array<int, 2>, N>next{};
  array<int, N>mx{};
  array<int, N>val{};
  int tot;
  void ins(int x, int pos = 0)
  {
    int cur = 0;
    for (int i = V; i >= 0; i--) {
      int u = (x >> i) & 1;
      if (!next[cur][u])
        next[cur][u] = ++tot;
      cur = next[cur][u];
      mx[cur] = max(mx[cur], pos);
    }
    val[cur]++;
  }
  int ask(int x)
  {
    int cur = 0;
    for (int i = V; i >= 0; i--) {
      int u = (x >> i & 1);
      if (!next[cur][u]) return 0;
      cur = next[cur][u];
    }
    return val[cur];
  }
  int Max_query(int x)  // 对于已经插入的二进制数,寻找最大的异或结果
  {
    int cur = 0;
    int s = 0;
    for (int i = V; i >= 0; i--) {
      int u = (x >> i & 1);
      if (next[cur][1 - u]) {
        cur = next[cur][1 - u];
        s |= 1 << i;
      }
      else
        cur = next[cur][u];
    }
    return s;
  }
  int query(int x, int mid)  // 查询前缀[1,i-1]中与a[i]异或值<=mid 的最大坐标。
  {
    int pos = 0;
    int cur = 0;
    for (int i = 29; i >= 0; i--) {
      int u = x >> i & 1, v = mid >> i & 1;
      if (v)pos = max(pos, mx[next[cur][u]]), cur = next[cur][u ^ 1];
      else cur = next[cur][u];
    }
    pos = max(pos, mx[cur]);
    return pos;
  }
  int query(int x, int mid)  // 查询前缀[1,i-1]中与a[i]异或值>=mid 的最大坐标。
  {
    int pos = 0;
    int cur = 0;
    for (int i = V; i >= 0; i--) {
      int u = x >> i & 1, v = mid >> i & 1;
      if (!v)pos = max(pos, mx[next[cur][u ^ 1]]), cur = next[cur][u];
      else cur = next[cur][u ^ 1];
    }
    pos = max(pos, mx[cur]);
    return pos;
  }
  void clear()
  {
    for (int i = 0; i <= tot; i++) {
      next[i][0] = next[i][1] = val[i] = 0;
    }
    tot = 1;
  }
};