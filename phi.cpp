vector<int> primes, minp, phi;
void sieve(int n) {
  minp.assign(n + 1, 0);
  phi.assign(n + 1, 0);
  primes.clear();
  phi[1] = 1;
  for (int i = 2; i <= n; i++) {
    if (minp[i] == 0) {
      minp[i] = i;
      primes.push_back(i);
      phi[i] = i - 1;
    }
    for (auto p : primes) {
      if (i * p > n) {
        break;
      }
      minp[i * p] = p;
      phi[i * p] = phi[i] * phi[p];
      if (p == minp[i]) {
        phi[i * p] = phi[i] * p;
        break;
      }
    }
  }
}