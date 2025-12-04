struct DynamicValueSegmentTree {
  static const int MAXN = 8e6 + 5;
  static const int V = 1e7 + 5;
  struct Info {
    int ls = 0, rs = 0, val = 0;
    int tag = 0;
  } tree[MAXN];
  int tot = 1;
#define ls(x) tree[x].ls
#define rs(x) tree[x].rs
#define val(x) tree[x].val

  void push(int x)
  {
    if (!ls(x)) ls(x) = ++tot;
    if (!rs(x)) rs(x) = ++tot;
  }
  void pull(int x) { val(x) = val(ls(x)) + val(rs(x)); }
  void update(int x, int l, int r, int p, int v)
  {
    if (l == r) {
      val(x) += v;
      return;
    }
    int mid = (l + r - 1) / 2;
    push(x);
    if (p <= mid)
      update(ls(x), l, mid, p, v);
    else
      update(rs(x), mid + 1, r, p, v);
    pull(x);
  }
  int query(int x, int l, int r, int ql, int qr)
  {
    if (ql <= l && r <= qr) {
      return val(x);
    }
    if (l > qr || r < ql) return 0;
    push(x);
    int mid = (l + r - 1) / 2;
    return query(ls(x), l, mid, ql, qr) + query(rs(x), mid + 1, r, ql, qr);
  }
  void update(int p, int v) { update(1, -V, V, p, v); }
  int query(int ql, int qr) { return query(1, -V, V, ql, qr); }
  void add(int p) { update(p, 1); }
  void del(int p) { update(p, -1); }
  int rnk(int x)
  {
    int v = V;
    return query(-v, x - 1) + 1;
  }
  int kth(int x, int l, int r, int k)
  {
    if (l == r) {
      return l;
    }
    int mid = (l + r - 1) / 2;
    if (k <= val(ls(x)))
      return kth(ls(x), l, mid, k);
    else
      return kth(rs(x), mid + 1, r, k - val(ls(x)));
  }
  int kth(int k)
  {
    if (k > val(1)) return -1;
    return kth(1, -V, V, k);
  }
  int pre(int x) { return kth(rnk(x) - 1); }
  int nxt(int x) { return kth(rnk(x + 1)); }
};
