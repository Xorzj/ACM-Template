struct ST {
  int n, T;
  vector<vector<int>> f;
  ST(int _n) {
    n = _n;
    T = __lg(n);
    f = vector<vector<int>>(n + 1, vector(T + 1, 0));
  }
  ST(const vector<int> &a) {
    n = a.size() - 1;
    T = __lg(n);
    f = vector<vector<int>>(n + 1, vector(T + 1, 0));
    for (int i = 1; i <= n; i++) f[i][0] = a[i];
    init();
  }
  void init() {
    for (int j = 1; j <= T; j++)
      for (int i = 1; i <= n; i++)
        if (i + (1 << (j - 1)) <= n)
          f[i][j] = max(f[i][j - 1], f[i + (1 << (j - 1))][j - 1]);
  }
  int query(int l, int r) {
    int t = __lg(r - l + 1);
    return max(f[l][t], f[r + 1 - (1 << t)][t]);
  }
};