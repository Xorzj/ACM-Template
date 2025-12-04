vector<int> prime;
vector<int> vis;
void prime_sieve(int n) {
  vis.resize(n + 1);
  for (int i = 2; i <= n; i++) {
    if (!vis[i]) prime.push_back(i);
    for (auto p : prime) {
      if (p * i > n) break;
      vis[i * p] = 1;
      if (i % p == 0) break;
    }
  }
}
