struct LazyDynamicValueSegmentTree {
  static const int MAXN = 8e6 + 5;
  static const int V = 1e9;
  struct Info {
    int ls = 0, rs = 0;
    int val = 0;
    int tag = 0;
  } tree[MAXN];
  int tot = 1;
#define ls(x) tree[x].ls
#define rs(x) tree[x].rs
#define val(x) tree[x].val
#define tag(x) tree[x].tag
  void push(int x, int len) {
    if (len <= 1) return;
    if (!ls(x)) ls(x) = ++tot;
    if (!rs(x)) rs(x) = ++tot;
    val(ls(x)) += 1ll * tag(x) * (len / 2);
    tag(ls(x)) += 1ll * tag(x);
    val(rs(x)) += 1ll * tag(x) * (len - len / 2);
    tag(rs(x)) += 1ll * tag(x);
    tag(x) = 0;
  }
  void pull(int x) { val(x) = val(ls(x)) + val(rs(x)); }
  void rangeupdate(int x, int l, int r, int ql, int qr, int v) {
    if (r < ql || l > qr) return;
    if (ql <= l && r <= qr) {
      val(x) += (r - l + 1) * v;
      tag(x) += v;
      return;
    }
    push(x, r - l + 1);
    int mid = (l + r - 1) / 2;
    if (mid >= ql) rangeupdate(ls(x), l, mid, ql, qr, v);
    if (mid < qr) rangeupdate(rs(x), mid + 1, r, ql, qr, v);
    pull(x);
  }
  void rangeupdate(int ql, int qr, int v) {
    return rangeupdate(1, 1, V, ql, qr, v);
  }
  int query(int x, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) return val(x);
    push(x, r - l + 1);
    int mid = (l + r - 1) / 2;
    int ans = 0;
    if (mid >= ql) ans += query(ls(x), l, mid, ql, qr);
    if (mid < qr) ans += query(rs(x), mid + 1, r, ql, qr);
    return ans;
  }
  int query(int ql, int qr) { return query(1, 1, V, ql, qr); }
  void update(int x, int l, int r, int p, int v) {
    if (l == r) {
      val(x) += v;
      return;
    }
    int mid = (l + r - 1) / 2;
    push(x, r - l + 1);
    if (p <= mid)
      update(ls(x), l, mid, p, v);
    else
      update(rs(x), mid + 1, r, p, v);
    pull(x);
  }
  void update(int p, int v) { update(1, 1, V, p, v); }
  void clear() {
    for (int i = 0; i <= tot; i++) tree[i] = {0, 0, 0, 0};
    tot = 1;
  }
};