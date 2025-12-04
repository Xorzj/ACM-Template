template <typename T>
struct Fenwick {
  int n;
  vector<T> tree;
  Fenwick(int n = 0) { init(n); }
  void init(int n) {
    this->n = n;
    tree.assign(n + 1, T());
  }
  void add(int x, T v) {
    for (int i = x; i <= n; i += i & -i) {
      tree[i] += v;
    }
  }
  T sum(int x) {
    auto ans = T();
    for (int i = x; i > 0; i -= i & -i) {
      ans += tree[i];
    }
    return ans;
  }
  T rangeSum(int l, int r) { return sum(r) - sum(l - 1); }
  int kth(T k) {
    assert(k > 0);
    int x = 1;
    for (int i = 1 << __lg(n); i; i /= 2) {
      if (x + i <= n && k > tree[x + i - 1]) {
        x += i;
        k -= tree[x - 1];
      }
    }
    return x;
  }
  void clear() {
    for (int i = 1; i <= n; i++) tree[i] = T();
  }
};