struct TwoSat {
  int n;
  vector<vector<int>> e;
  vector<int> ans;
  TwoSat(int n) : n(n), e(2 * n), ans(n) {}

  // u or v
  void add(int u, bool f, int v, bool g) {
    // 偶数是u取反的变量，奇数是正变量
    // 一般来说需要反变量向正变量连边。
    e[2 * u + !f].push_back(2 * v + g);
    if (u == v) return;  // 对于单变量指定特判
    e[2 * v + !g].push_back(2 * u + f);
  }
  // u -> v
  void addarrow(int u, bool f, int v, bool g) { add(u, !f, v, g); }
  void set(int a, bool f) {
    if (f == 0)
      addarrow(a, 1, a, 0);
    else
      addarrow(a, 0, a, 1);
  }
  void addor(int a, bool f, int b, bool g, bool c) {
    if (c == 0) {
      set(a, !f);
      set(b, !g);
    } else {
      add(a, f, b, g);
    }
  }
  void addand(int a, bool f, int b, bool g, bool c) {
    if (c == 0) {
      addarrow(a, 1 ^ (!f), b, 0 ^ (!g));
      addarrow(b, 1 ^ (!f), a, 0 ^ (!g));
    } else {
      addarrow(a, 0 ^ (!f), a, 1 ^ (!g));
      addarrow(b, 0 ^ (!f), b, 1 ^ (!g));
    }
  }
  void addxor(int a, bool f, int b, bool g, bool c) {
    if (c == 0) {
      addarrow(a, 0 ^ (!f), b, 0 ^ (!g));
      addarrow(a, 1 ^ (!f), b, 1 ^ (!g));
    } else {
      addarrow(a, 0 ^ (!f), b, 1 ^ (!g));
      addarrow(a, 1 ^ (!f), b, 0 ^ (!g));
    }
  }
  bool judge() {
    vector<int> id(2 * n, -1), dfn(2 * n, -1), low(2 * n, -1);
    vector<int> stk;
    int now = 0, cnt = 0;
    function<void(int)> tarjan = [&](int u) {
      stk.push_back(u);
      dfn[u] = low[u] = now++;
      for (auto v : e[u]) {
        if (dfn[v] == -1) {
          tarjan(v);
          low[u] = min(low[u], low[v]);
        } else if (id[v] == -1) {
          low[u] = min(low[u], dfn[v]);
        }
      }
      if (dfn[u] == low[u]) {
        int v;
        do {
          v = stk.back();
          stk.pop_back();
          id[v] = cnt;
        } while (v != u);
        ++cnt;
      }
    };
    for (int i = 0; i < 2 * n; ++i)
      if (dfn[i] == -1) tarjan(i);
    for (int i = 0; i < n; ++i) {
      if (id[2 * i] == id[2 * i + 1]) return false;
      ans[i] = id[2 * i] > id[2 * i + 1];
    }
    return true;
  }
  vector<int> answer() { return ans; }
};