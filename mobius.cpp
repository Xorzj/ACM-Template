vector<int> prime;
vector<int> vis;
vector<int> mu;
vector<int> sum;
void prime_sieve(int n) {
  sum.resize(n + 1);
  vis.resize(n + 1);
  mu.resize(n + 1);
  mu[1] = 1;
  sum[1] = 1;
  for (int i = 2; i <= n; i++) {
    if (!vis[i]) prime.push_back(i), mu[i] = -1;
    for (auto p : prime) {
      if (p * i > n) break;
      vis[i * p] = 1;
      if (i % p == 0) break;
      mu[p * i] = -mu[i];
    }
    sum[i] = sum[i - 1] + mu[i];
  }
}
while (cin >> n >> m) {
  if (n == m && n == 0) break;
  if (n > m) swap(n, m);
  double ans = 0;
  for (int l = 1, r; l <= n; l = r + 1) {
    r = min(n / (n / l), m / (m / l));
    ans += (sum[r] - sum[l - 1]) * (n / l) * (m / l);
  }