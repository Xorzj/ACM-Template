vector<int> primes, minp;
void sieve(int n) {
  minp.assign(n + 1, 0);
  primes.clear();

  for (int i = 2; i <= n; i++) {
    if (minp[i] == 0) {
      minp[i] = i;
      primes.push_back(i);
    }

    for (auto p : primes) {
      if (i * p > n) {
        break;
      }
      minp[i * p] = p;
      if (p == minp[i]) {
        break;
      }
    }
  }
}
vector<pii> factor(int x) {
  assert(x != 0);
  vector<pii> t;
  while (x != 1) {
    int u = minp[x], v = 0;
    while (x % u == 0) {
      x /= u;
      v++;
    }
    if (v) t.push_back({u, v});
  }
  return t;
}