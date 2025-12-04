template <int P>
constexpr MInt<P> findPrimitiveRoot()
{
  MInt<P> i = 2;
  int k = __builtin_ctz(P - 1);
  while (true) {
    if (qpow(i, (P - 1) / 2) != 1) {
      break;
    }
    i += 1;
  }
  return qpow(i, (P - 1) >> k);
}
template <int P>
constexpr MInt<P> primitiveRoot = findPrimitiveRoot<P>();
template <>
constexpr MInt<998244353> primitiveRoot<998244353>{31};
vector<int> rev;
vector<Z> roots{ 0, 1 };
void dft(vector<Z>& a)
{
  int n = a.size();

  if (int(rev.size()) != n) {
    int k = __builtin_ctz(n) - 1;
    rev.resize(n);
    for (int i = 0; i < n; i++) {
      rev[i] = rev[i >> 1] >> 1 | (i & 1) << k;
    }
  }

  for (int i = 0; i < n; i++) {
    if (rev[i] < i) {
      swap(a[i], a[rev[i]]);
    }
  }
  if (int(roots.size()) < n) {
    int k = __builtin_ctz(roots.size());
    roots.resize(n);
    while ((1 << k) < n) {
      Z e = qpow(primitiveRoot<P>, (P - 1) >> (k + 1));
      for (int i = 1 << (k - 1); i < (1 << k); i++) {
        roots[2 * i] = roots[i];
        roots[2 * i + 1] = roots[i] * e;
      }
      k++;
    }
  }
  for (int k = 1; k < n; k *= 2) {
    for (int i = 0; i < n; i += 2 * k) {
      for (int j = 0; j < k; j++) {
        Z u = a[i + j];
        Z v = a[i + j + k] * roots[k + j];
        a[i + j] = u + v;
        a[i + j + k] = u - v;
      }
    }
  }
}
void idft(vector<Z>& a)
{
  int n = a.size();
  reverse(a.begin() + 1, a.end());
  dft(a);
  Z inv = (1 - P) / n;
  for (int i = 0; i < n; i++) {
    a[i] *= inv;
  }
}
struct Poly {
  vector<Z> a;
  Poly() {}
  explicit Poly(
    int size, function<Z(int)> f = [](int) { return 0; })
    : a(size)
  {
    for (int i = 0; i < size; i++) {
      a[i] = f(i);
    }
  }
  Poly(const vector<Z>& a) : a(a) {}
  Poly(const initializer_list<Z>& a) : a(a) {}
  int size() const { return a.size(); }
  void resize(int n) { a.resize(n); }
  Z operator[](int idx) const
  {
    if (idx < size()) {
      return a[idx];
    }
    else {
      return 0;
    }
  }
  Z& operator[](int idx) { return a[idx]; }
  Poly mulxk(int k) const
  {
    auto b = a;
    b.insert(b.begin(), k, 0);
    return Poly(b);
  }
  Poly modxk(int k) const
  {
    k = min(k, size());
    return Poly(vector<Z>(a.begin(), a.begin() + k));
  }
  Poly divxk(int k) const
  {
    if (size() <= k) {
      return Poly();
    }
    return Poly(vector<Z>(a.begin() + k, a.end()));
  }
  friend Poly operator+(const Poly& a, const Poly& b)
  {
    vector<Z> res(max(a.size(), b.size()));
    for (int i = 0; i < int(res.size()); i++) {
      res[i] = a[i] + b[i];
    }
    return Poly(res);
  }
  friend Poly operator-(const Poly& a, const Poly& b)
  {
    vector<Z> res(max(a.size(), b.size()));
    for (int i = 0; i < int(res.size()); i++) {
      res[i] = a[i] - b[i];
    }
    return Poly(res);
  }
  friend Poly operator-(const Poly& a)
  {
    vector<Z> res(a.size());
    for (int i = 0; i < int(res.size()); i++) {
      res[i] = -a[i];
    }
    return Poly(res);
  }
  friend Poly operator*(Poly a, Poly b)
  {
    if (a.size() == 0 || b.size() == 0) {
      return Poly();
    }
    if (a.size() < b.size()) {
      swap(a, b);
    }
    if (b.size() < 128) {
      Poly c(a.size() + b.size() - 1);
      for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < b.size(); j++) {
          c[i + j] += a[i] * b[j];
        }
      }
      return c;
    }
    int sz = 1, tot = a.size() + b.size() - 1;
    while (sz < tot) {
      sz *= 2;
    }
    a.a.resize(sz);
    b.a.resize(sz);
    dft(a.a);
    dft(b.a);
    for (int i = 0; i < sz; ++i) {
      a.a[i] = a[i] * b[i];
    }
    idft(a.a);
    a.resize(tot);
    return a;
  }
  friend Poly operator*(Z a, Poly b)
  {
    for (int i = 0; i < int(b.size()); i++) {
      b[i] *= a;
    }
    return b;
  }
  friend Poly operator*(Poly a, Z b)
  {
    for (int i = 0; i < int(a.size()); i++) {
      a[i] *= b;
    }
    return a;
  }
  Poly& operator+=(Poly b) { return (*this) = (*this) + b; }
  Poly& operator-=(Poly b) { return (*this) = (*this) - b; }
  Poly& operator*=(Poly b) { return (*this) = (*this) * b; }
  Poly& operator*=(Z b) { return (*this) = (*this) * b; }
  Poly deriv() const
  {
    if (a.empty()) {
      return Poly();
    }
    vector<Z> res(size() - 1);
    for (int i = 0; i < size() - 1; ++i) {
      res[i] = (i + 1) * a[i + 1];
    }
    return Poly(res);
  }
  Poly integr() const
  {
    vector<Z> res(size() + 1);
    for (int i = 0; i < size(); ++i) {
      res[i + 1] = a[i] / (i + 1);
    }
    return Poly(res);
  }
  Poly inv(int m) const
  {
    Poly x{ a[0].inv() };
    int k = 1;
    while (k < m) {
      k *= 2;
      x = (x * (Poly{ 2 } - modxk(k) * x)).modxk(k);
    }
    return x.modxk(m);
  }
  Poly log(int m) const { return (deriv() * inv(m)).integr().modxk(m); }
  Poly exp(int m) const
  {
    Poly x{ 1 };
    int k = 1;
    while (k < m) {
      k *= 2;
      x = (x * (Poly{ 1 } - x.log(k) + modxk(k))).modxk(k);
    }
    return x.modxk(m);
  }
  Poly pow(int k, int m) const
  {
    int i = 0;
    while (i < size() && a[i].val() == 0) {
      i++;
    }
    if (i == size() || 1LL * i * k >= m) {
      return Poly(vector<Z>(m));
    }
    Z v = a[i];
    auto f = divxk(i) * v.inv();
    return (f.log(m - i * k) * k).exp(m - i * k).mulxk(i * k) * qpow(v, k);
  }
  Poly sqrt(int m) const
  {
    Poly x{ 1 };
    int k = 1;
    while (k < m) {
      k *= 2;
      x = (x + (modxk(k) * x.inv(k)).modxk(k)) * ((P + 1) / 2);
    }
    return x.modxk(m);
  }
  friend Poly quotient(Poly a, Poly b)
  {
    int n = a.size();
    int m = b.size();
    reverse(ALL(a.a));
    reverse(ALL(b.a));
    Poly Q = (a * (b.inv(n - m + 1))).modxk(n - m + 1);
    reverse(Q.a.begin(), Q.a.end());
    return Q;
  }
  friend pair<Poly, Poly> Div(Poly a, Poly b)
  {
    Poly Q = quotient(a, b);
    Poly R = a - b * Q;
    return { Q, R };
  }
  Poly mulT(Poly b) const
  {
    if (b.size() == 0) {
      return Poly();
    }
    int n = b.size();
    reverse(b.a.begin(), b.a.end());
    return ((*this) * b).divxk(n - 1);
  }
  vector<Z> eval(vector<Z> x) const
  {
    if (size() == 0) {
      return vector<Z>(x.size(), 0);
    }
    const int n = max(int(x.size()), size());
    vector<Poly> q(4 * n);
    vector<Z> ans(x.size());
    x.resize(n);
    function<void(int, int, int)> build = [&](int p, int l, int r) {
      if (r - l == 1) {
        q[p] = Poly{ 1, -x[l] };
      }
      else {
        int m = (l + r) / 2;
        build(2 * p, l, m);
        build(2 * p + 1, m, r);
        q[p] = q[2 * p] * q[2 * p + 1];
      }
    };
    build(1, 0, n);
    function<void(int, int, int, const Poly&)> work = [&](int p, int l, int r,
      const Poly& num) {
      if (r - l == 1) {
        if (l < int(ans.size())) {
          ans[l] = num[0];
        }
      }
      else {
        int m = (l + r) / 2;
        work(2 * p, l, m, num.mulT(q[2 * p + 1]).modxk(m - l));
        work(2 * p + 1, m, r, num.mulT(q[2 * p]).modxk(r - m));
      }
    };
    work(1, 0, n, mulT(q[1].inv(n)));
    return ans;
  }
};
