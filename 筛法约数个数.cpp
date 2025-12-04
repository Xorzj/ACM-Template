vector<int> primes, minp, d, h;
// d is ans array
void sieve(int n)
{
  minp.assign(n + 1, 0);
  d.assign(n + 1, 0);
  h.assign(n + 1, 0);
  primes.clear();
  d[1] = 1;
  for (int i = 2; i <= n; i++) {
    if (minp[i] == 0) {
      minp[i] = i;
      d[i] = 2;
      h[i] = 1;
      primes.push_back(i);
    }
    for (auto p : primes) {
      if (i * p > n) {
        break;
      }
      minp[i * p] = p;
      d[i * p] = d[i] * d[p];
      h[i * p] = 1;
      if (p == minp[i]) {
        d[i * p] = d[i] / (h[i] + 1) * (h[i] + 2);
        h[i * p] = h[i] + 1;
        break;
      }
    }
  }
}