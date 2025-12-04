struct seg_tree {
  vector<int> sum, tag;
  seg_tree(int n) : sum((n << 2) + 1), tag((n << 2) + 1) {}
  void pushup(int x) { sum[x] = sum[x << 1] + sum[x << 1 | 1]; }
  void pushdown(int x, int l, int r) {
    if (tag[x]) {
      int mid = l + r >> 1;
      tag[x << 1] += tag[x];
      tag[x << 1 | 1] += tag[x];
      sum[x << 1] += (mid - l + 1) * (tag[x]);
      sum[x << 1 | 1] += (r - mid) * (tag[x]);
      tag[x] = 0;
    }
  }
  void build(int x, int l, int r, auto &a) {
    if (l == r) {
      sum[x] = a[l];
      return;
    }
    int mid = l + r >> 1;
    build(x << 1, l, mid, a);
    build(x << 1 | 1, mid + 1, r, a);
    pushup(x);
  }
  void modify(int x, int l, int r, int ql, int qr, int v) {
    if (ql <= l && r <= qr) {
      sum[x] += (r - l + 1) * v;
      tag[x] += v;
      return;
    }
    int mid = l + r >> 1;
    pushdown(x, l, r);
    if (ql <= mid) modify(x << 1, l, mid, ql, qr, v);
    if (qr > mid) modify(x << 1 | 1, mid + 1, r, ql, qr, v);
    pushup(x);
  }
  int ask(int x, int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) return sum[x];
    pushdown(x, l, r);
    int mid = l + r >> 1;
    int res = 0;
    if (ql <= mid) res += ask(x << 1, l, mid, ql, qr);
    if (qr > mid) res += ask(x << 1 | 1, mid + 1, r, ql, qr);
    return res;
  }
};
