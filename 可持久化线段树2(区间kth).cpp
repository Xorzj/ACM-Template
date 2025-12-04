int n, m;
int a[MAXN], b[MAXN];
int tot;
int root[MAXN];
struct node {
  int l, r, v;
} tree[MAXN << 5];
int clone(int x) {
  tree[++tot] = tree[x];
  return tot;
}
int build(int x, int l, int r) {
  x = ++tot;
  if (l == r) return x;
  int mid = l + r >> 1;
  tree[x].l = build(tree[x].l, l, mid);
  tree[x].r = build(tree[x].r, mid + 1, r);
  return x;
}
int update(int x, int l, int r, int q) {
  x = clone(x);
  tree[x].v++;
  if (l == r) return x;
  int mid = l + r >> 1;
  if (q <= mid)
    tree[x].l = update(tree[x].l, l, mid, q);
  else
    tree[x].r = update(tree[x].r, mid + 1, r, q);
  return x;
}
int query(int x, int y, int l, int r, int k) {
  if (l == r) return l;
  int V = tree[tree[y].l].v - tree[tree[x].l].v;
  int mid = l + r >> 1;
  if (V >= k)
    return query(tree[x].l, tree[y].l, l, mid, k);
  else
    return query(tree[x].r, tree[y].r, mid + 1, r, k - V);
}
// [l,r] 区间第 k 小
int main() {
  n = read(), m = read();
  rep(i, 1, n) a[i] = read(), b[i] = a[i];
  sort(b + 1, b + 1 + n);
  int N = unique(b + 1, b + 1 + n) - (b + 1);
  root[0] = build(root[0], 1, N);
  rep(i, 1, n) {
    int p = lower_bound(b + 1, b + 1 + N, a[i]) - b;
    root[i] = update(root[i - 1], 1, N, p);
  }
  while (m--) {
    int l = read(), r = read(), k = read();
    int d = query(root[l - 1], root[r], 1, N, k);
    printf("%d\n", b[d]);
  }
  return 0;
}