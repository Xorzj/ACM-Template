int num[1000005];
int n, s, t, e, tol;
struct map {
  int a[500][500];
  map operator*(const map &x) const  // 重载运算符，一会儿要用
  {
    map c;
    memset(c.a, 0x3f3f3f3f, sizeof(c.a));  // 取min，显然置大数
    for (int k = 1; k <= tol; k++)
      for (int i = 1; i <= tol; i++)
        for (int j = 1; j <= tol; j++)
          c.a[i][j] = min(c.a[i][j], a[i][k] + x.a[k][j]);
    return c;
  }
} dis, ans;
void init() {
  memset(dis.a, 0x3f3f3f3f, sizeof(dis.a));
  int x, y, z;
  cin >> n >> t >> s >> e;
  for (int i = 1; i <= t; i++) {
    scanf("%d %d %d", &x, &y, &z);
    if (!num[y])  // 这里做一个离散化
      num[y] = ++tol;
    if (!num[z]) num[z] = ++tol;
    dis.a[num[y]][num[z]] = dis.a[num[z]][num[y]] = x;
  }
}
void doit()  // 快速幂
{
  n--;
  ans = dis;
  while (n) {
    if (n & 1) ans = ans * dis;
    dis = dis * dis;
    n >>= 1;
  }
}
int main() {
  init();
  doit();
  cout << ans.a[num[s]][num[e]];
}