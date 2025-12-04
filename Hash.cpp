ll seed = chrono::steady_clock::now().time_since_epoch().count();
mt19937_64 rnd(seed);
bool isprime(int n)
{
  if (n <= 1)return false;
  for (int i = 2; 1LL * i * i <= n; i++) {
    if (n % i == 0)return false;
  }
  return true;
}
int find(int n)
{
  while (!isprime(n))n++;
  return n;
}
const int mod = find(rnd() % 900000000 + 100000000);
struct Hash {
  int n;
  vector<int> has, poww, has2, poww2;
  int base = 131;
  int base2 = 117;
  void init(string s)
  {
    n = s.length();
    has.resize(n + 1);
    has2.resize(n + 1);
    poww.resize(n + 1);
    poww2.resize(n + 1);
    poww[0] = 1;
    poww2[0] = 1;
    for (int i = 0; i < n; i++) {
      poww[i + 1] = 1LL * poww[i] * base % mod;
      poww2[i + 1] = 1LL * poww2[i] * base2 % mod;
      has[i + 1] = (1LL * has[i] * base % mod + (s[i] - 'A' + 1)) % mod;
      has2[i + 1] = (1LL * has2[i] * base2 % mod + (s[i] - 'A' + 1)) % mod;
    }
  }
  Hash() {}
  Hash(string s) { init(s); }
  pii get(int l, int r)
  {
    assert(r < n);
    return { (has[r + 1] - 1LL * has[l] * poww[r - l + 1] % mod + mod) % mod,(has2[r + 1] - 1LL * has2[l] * poww2[r - l + 1] % mod + mod) % mod };
  }
};
bool same(Hash& a, Hash& b, int l, int r, int L, int R)
{
  return a.get(l, r) == b.get(L, R);
}
