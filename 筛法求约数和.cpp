vector<int> primes, minp, dsum, h;
// dsum is ans array
void sieve(int n)
{
  minp.assign(n + 1, 0);
  dsum.assign(n + 1, 0);
  h.assign(n + 1, 0);
  dsum[1] = 1;
  primes.clear();
  for (int i = 2; i <= n; i++) {
    if (minp[i] == 0) {
      minp[i] = i;
      dsum[i] = 1 + i;
      h[i] = 1 + i;
      primes.push_back(i);
    }
    for (auto p : primes) {
      if (i * p > n) {
        break;
      }
      minp[i * p] = p;
      dsum[i * p] = dsum[i] * dsum[p];
      h[i * p] = 1 + p;
      if (p == minp[i]) {
        dsum[i * p] = dsum[i] / h[i] * (p * h[i] + 1);
        h[i * p] = p * h[i] + 1;
        break;
      }
    }
  }
}
