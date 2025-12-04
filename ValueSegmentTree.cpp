template <class Info>
struct ValueSegmentTree {
  int n;
  vector<Info> info;
  ValueSegmentTree() : n(0) {}
  ValueSegmentTree(int n_, Info v_ = Info()) { init(vector<Info>(n_ + 1, v_)); }
  ValueSegmentTree(vector<Info> t_) { init(t_); }
  void init(vector<Info> a)  //[1,n]
  {
    n = a.size() - 1;
    info.assign((n << 2) + 1, Info());
    function<void(int, int, int)> build = [&](int x, int l, int r) -> void {
      if (l == r) {
        info[x] = a[l];
        return;
      }
      int mid = l + r >> 1;
      build(x << 1, l, mid);
      build(x << 1 | 1, mid + 1, r);
      pull(x);
    };
    build(1, 1, n);
  }
  void pull(int x) { info[x] = info[x << 1] + info[x << 1 | 1]; }
  void update(int x, int l, int r, int p, const Info& v)
  {
    if (l == r) {
      info[x] = info[x] + v;
      return;
    }
    int mid = l + r >> 1;
    if (p <= mid)
      update(x << 1, l, mid, p, v);
    else
      update(x << 1 | 1, mid + 1, r, p, v);
    pull(x);
  }
  void update(int p, const Info& v) { update(1, 1, n, p, v); }
  int kth(int x, int l, int r, int k)
  {
    if (l == r) {
      return l;
    }
    int mid = (l + r) / 2;
    if (k <= info[x << 1].sum)
      return kth(x << 1, l, mid, k);
    else
      return kth(x << 1 | 1, mid + 1, r, k - info[x << 1].sum);
  }
  //满足sum>v的前缀升序序列的长度
  int query(int x, int l, int r, int v)
  {
    if (info[x].sum <= v)return -1;
    if (l == r) {
      return v / b[l - 1];
    }
    int mid = (l + r) / 2;
    if (v < info[x << 1].sum)
      return query(x << 1, l, mid, v);
    else
      return info[x << 1].cnt + query(x << 1 | 1, mid + 1, r, v - info[x << 1].sum);
  }
  int kth(int k) { return kth(1, 1, n, k); }
};
struct Info {
  int sum = 0;
};
Info operator+(const Info& a, const Info& b)
{
  Info c;
  c.sum = a.sum + b.sum;
  return c;
}