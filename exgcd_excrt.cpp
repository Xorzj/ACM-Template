void exgcd(int a, int b, int &d, int &x, int &y) {
  if (!b)
    d = a, x = 1, y = 0;
  else
    exgcd(b, a % b, d, y, x), y -= x * (a / b);
}
pii excrt(pii l, pii r) {
  auto [r1, m1] = l;
  auto [r2, m2] = r;
  if (r1 == -1 || r2 == -1) return {-1, -1};
  int d, l1, l2;
  exgcd(m1, m2, d, l1, l2);
  if ((r2 - r1) % d) return {-1, -1};
  int L = m1 * m2 / d;
  int R = ((r1 + (r2 - r1) / d * l1 % L * m1) % L + L) % L;
  return {R, L};
}
