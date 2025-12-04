struct Basis {
  vector<ull> B;
  vector<int> rnk;
  int n = 0;
  void insert(ull x) {
    n++;
    for (auto y : B) x = min(x, y ^ x);
    for (auto &y : B) y = min(y, y ^ x);
    if (x) B.push_back(x);
  }
  bool query(ull x) {
    for (auto y : B) x = min(x, x ^ y);
    return x == 0;
  }
  void get_sort()  // before kth and query_rnk
  {
    sort(ALL(B));
  }
  int kth(int k) {
    ull ans = 0;
    if (size() < n) k--;
    for (auto y : B) {
      if (k & 1) ans ^= y;
      k >>= 1;
    }
    if (k == 0)
      return ans;
    else
      return -1;
  }
  int query_rnk(ull x)  // the first position x in all subset xor value
                        // （在所有的子集异或中第一次出现的位置）
  {
    ull ans = 0;
    int cnt = size();
    for (int i = 0; i < cnt; i++) {
      int v = 63 - __builtin_clzll(B[i]);
      if (x >> v & 1) ans |= 1ll << i;
    }
    return ans * (1ll << (n - cnt)) + 1;
  }
  ull query_max() {
    ull ans = 0;
    for (auto x : B) ans ^= x;
    return ans;
  }
  ull query_min() {
    ull ans = B[0];
    for (auto x : B) ans = min(ans, x);
    return ans;
  }
  ull xormin(ull x) {
    for (auto &y : B) x = min(x, x ^ y);
    return x;
  }
  void merge(Basis &o) {
    for (auto &x : o.B) (*this).insert(x);
  }
  int size() { return B.size(); }
};
