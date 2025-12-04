struct node {
  int l, r, v;
} tree[MAXN * 20];
int a[MAXN];
int root[MAXN * 20];
int tot;
int n, m;
int clone(int x) {
  tot++;
  tree[tot] = tree[x];
  return tot;
}
int build(int x, int l, int r) {
  x = ++tot;
  if (l == r) {
    tree[x].v = a[l];
    return x;
  }
  int mid = l + r >> 1;
  tree[x].l = build(tree[x].l, l, mid);
  tree[x].r = build(tree[x].r, mid + 1, r);
  return x;
}
int update(int x, int l, int r, int q, int v) {
  x = clone(x);
  if (l == r)
    tree[x].v = v;
  else {
    int mid = l + r >> 1;
    if (q <= mid) tree[x].l = update(tree[x].l, l, mid, q, v);
    if (q > mid) tree[x].r = update(tree[x].r, mid + 1, r, q, v);
  }
  return x;
}
int query(int x, int l, int r, int q) {
  if (l == r) return tree[x].v;
  int mid = l + r >> 1;
  if (q <= mid)
    return query(tree[x].l, l, mid, q);
  else
    return query(tree[x].r, mid + 1, r, q);
}
// op 1: 在rt版本上 ,修改 q 位置为 x
// op 2: 在rt版本上 ,访问 q 位置值
int main() {
  n = read(), m = read();
  rep(i, 1, n) a[i] = read();
  root[0] = build(root[0], 1, n);
  rep(i, 1, m) {
    int rt = read(), op = read(), q = read();
    if (op == 1) {
      int x = read();
      root[i] = update(root[rt], 1, n, q, x);
    } else {
      printf("%d\n", query(root[rt], 1, n, q));
      root[i] = root[rt];
    }
  }
}