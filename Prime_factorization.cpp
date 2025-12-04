vector<pair<int, int>> prime_fac(auto n) {
  vector<pair<int, int>> ans;
  for (int i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      int cnt = 0;
      while (n % i == 0) {
        n /= i;
        cnt++;
      }
      ans.push_back({i, cnt});
    }
  }
  if (n > 1) ans.push_back({n, 1});
  return ans;
}