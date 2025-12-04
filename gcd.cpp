inline int gcd(int a, int b) {
  register int az = __builtin_ctz(a), bz = __builtin_ctz(b),
               z = az > bz ? bz : az, diff;
  b >>= bz;
  while (a) {
    a >>= az;
    diff = b - a;
    az = __builtin_ctz(diff);
    if (a < b) b = a;
    a = diff < 0 ? -diff : diff;
  }
  return b << z;
}