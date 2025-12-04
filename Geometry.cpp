//注意添加const & 优化 
typedef double db;
constexpr db inf = 1e18;
constexpr db eps = 1e-8;
const db pi = acos(-1);
constexpr int sgn(db x) { return x < -eps ? -1 : x > eps; }
constexpr int cmp(db x, db y) { return sgn(x - y); }
struct Point {
  db x, y;
  constexpr Point(db _x = 0, db _y = 0) : x(_x), y(_y) {}
  constexpr Point operator+() const noexcept { return *this; }
  constexpr Point operator-() const noexcept { return Point(-x, -y); }
  constexpr Point operator+(const Point& p) const
  {
    return Point(x + p.x, y + p.y);
  }
  constexpr Point operator-(const Point& p) const
  {
    return Point(x - p.x, y - p.y);
  }
  constexpr Point operator*(const db& k) { return Point(x * k, y * k); }
  constexpr Point operator/(const db& k) { return Point(x / k, y / k); }
  constexpr Point& operator+=(const Point& p)
  {
    return x += p.x, y += p.y, *this;
  }
  constexpr Point& operator-=(const Point& p)
  {
    return x -= p.x, y -= p.y, *this;
  }
  constexpr Point& operator*=(const db& k) { return x *= k, y *= k, *this; }
  constexpr Point& operator/=(const db& k) { return x /= k, y /= k, *this; }
  constexpr bool operator==(const Point& r) const noexcept
  {
    return cmp(x, r.x) == 0 and cmp(y, r.y) == 0;
  }
  constexpr bool operator<(const Point& r) const noexcept
  {
    return sgn(x - r.x) == 0 ? sgn(y - r.y) < 0 : x < r.x;
  }
  friend istream& operator>>(istream& is, Point& p) { return is >> p.x >> p.y; }
  friend ostream& operator<<(ostream& os, Point p)
  {
    return os << "(" << p.x << ", " << p.y << ")";
  }
  constexpr db dot(const Point& r) const { return x * r.x + y * r.y; }
  constexpr db cross(const Point& r) const { return x * r.y - y * r.x; }
  constexpr int quad() const
  {
    return sgn(y) > 0 || (sgn(y) == 0 && sgn(x) > 0) ? 1 : -1;
  }
  constexpr bool arg_cmp(const Point& r) const
  {
    int L = (*this).quad(), R = r.quad();
    if (L != R) return L < R;
    db X = x * r.y, Y = r.x * y;
    if (X != Y) return X > Y;
    return x < r.x;
  }
};
db dot(Point a, Point b) { return a.x * b.x + a.y * b.y; }
//a 逆时针转到 b
db cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
db square(Point p) { return dot(p, p); }
double len(Point p) { return sqrt(square(p)); }
Point unit(Point p) { return p / len(p); }
db arg(Point p) { return atan2(p.y, p.x); }
Point polar(db r, db theta) { return Point(cos(theta) * r, sin(theta) * r); }
Point rotleft(Point p) { return Point(-p.y, p.x); }
Point rotright(Point p) { return Point(p.y, -p.x); }
Point rotate(Point p, Point r, db angle)
{
  Point v = p - r;
  db c = cos(angle), s = sin(angle);
  return Point(p.x + v.x * c - v.y * s, p.y + v.x * s + v.y * c);
}
// 1 : left, 0 : same, -1 : right
int toleft(Point a, Point b, Point c) { return sgn(cross(b - a, c - a)); }
int toleft(Point a, Point b) { return sgn(cross(a, b)); }
//(0,0) in negative plane
int quad(Point p)
{
  return sgn(p.y) > 0 || (sgn(p.y) == 0 && sgn(p.x) > 0) ? 1 : -1;
}
bool arg_cmp(const Point& l, const Point& r) { return l.arg_cmp(r); }
double distance(const Point& a, const Point& b) { return len(a - b); }
struct Line {
  Point a, b;
  Line() = default;
  Line(Point a, Point b) : a(a), b(b) {}
  Line(Point p, db angle)
  {
    a = p;
    if (cmp(angle, pi / 2) == 0)
      b = (a + Point(0, 1));
    else
      b = (a + Point(1, tan(angle)));
  }
  // ax + by + c = 0
  Line(db A, db B, db C)
  {
    if (sgn(A) == 0)
      a = Point(0, -C / B), b = Point(1, -C / B);
    else if (sgn(B) == 0)
      a = Point(-C / A, 0), b = Point(-C / A, 1);
    else
      a = Point(0, -C / B), b = Point(-C / A, 0);
  }
};
struct Segment : Line {
  Segment() = default;
  Segment(Point a, Point b) : Line(a, b) {}
};
double len(const Line& l) { return len(l.b - l.a); }
int toleft(Point p, Line l) { return sgn(cross(l.b - l.a, p - l.a)); }
bool pointOnLineLeft(Point p, Line l) { return cross(l.b - l.a, p - l.a) > 0; }
bool parallel(Line l1, Line l2)
{
  return sgn(cross(l1.b - l1.a, l2.b - l2.a)) == 0;
}
bool orthogonal(const Line& l1, const Line& l2)
{
  return sgn(dot(l1.a - l1.b, l2.a - l2.b)) == 0;
}
#define crossOp(a, b) sgn(cross(a, b))
#define dotOp(a, b) sgn(dot(a, b))
// 两直线弧度
db rad(const Line& l1, const Line& l2)
{
  return atan2(fabs(cross(l1.b - l1.a, l2.b - l2.a)),
    dot(l1.b - l1.a, l2.b - l2.a));
}
// 返回直线倾斜角 0<=angle<π
db angle(const Line& l)
{
  db k = atan2(l.b.y - l.a.y, l.b.x - l.a.x);
  if (sgn(k) < 0) k += pi;
  if (sgn(k - pi) == 0) k -= pi;
  return k;
}
db distancePL(const Point& p, const Line& l)
{
  return fabs(cross(l.b - l.a, p - l.a)) / len(l);
}
db distancePS(const Point& p, const Segment& l)
{
  if (sgn(dot(p - l.a, l.b - l.a)) < 0) return distance(p, l.a);
  if (sgn(dot(p - l.b, l.a - l.b)) < 0) return distance(p, l.b);
  return distancePL(p, l);
}
//c in segment(a,b) 
//注意线段退化成点问题
int pointonsegment(Point a, Point b, Point c)
{
  b = b - a, c = c - a;
  if (cross(b, c) > 0) return +1;        // "COUNTER_CLOCKWISE"
  if (cross(b, c) < 0) return -1;        // "CLOCKWISE"
  if (dot(b, c) < 0) return +2;          // "ONLINE_BACK"
  if (square(b) < square(c)) return -2;  // "ONLINE_FRONT"
  return 0;                              // "ON_SEGMENT"
}
bool intersect(const Point& p, const Line& l)
{
  return abs(pointonsegment(l.a, l.b, p)) != 1;
}
bool intersect(const Point& p, const Segment& s)
{
  return pointonsegment(s.a, s.b, p) == 0;
}
bool intersect(const Line& l, const Line& m) { return !parallel(l, m); }
bool intersect(const Line& l, const Segment& s)
{
  return crossOp(l.b - l.a, s.a - l.a) * crossOp(l.b - l.a, s.b - l.a) <= 0;
}
bool intersect(const Segment& s, const Segment& t)
{
  return pointonsegment(s.a, s.b, t.a) * pointonsegment(s.a, s.b, t.b) <= 0 &&
    pointonsegment(t.a, t.b, s.a) * pointonsegment(t.a, t.b, s.b) <= 0;
}
db distanceSS(const Segment& l, const Segment& m)
{
  if (intersect(l, m)) return 0.0;
  return min({ distancePS(l.a, m), distancePS(l.b, m), distancePS(m.a, l),
              distancePS(m.b, l) });
}
// 2 规范相交 1 非规范相交 0 不相交
int crossSS(Segment l, Segment m)
{
  int d1 = toleft(m.a, l);
  int d2 = toleft(m.b, l);
  int d3 = toleft(l.a, m);
  int d4 = toleft(l.b, m);
  if ((d1 ^ d2) == -2 && (d3 ^ d4) == -2) return 2;
  return (d1 == 0 && intersect(m.a, l)) || (d2 == 0 && intersect(m.b, l)) ||
    (d3 == 0 && intersect(l.a, m)) || (d4 == 0 && intersect(l.b, m));
}
// 2 规范相交 1 非规范相交 0 不相交
int crossLS(Line l, Segment m)
{
  int d1 = toleft(m.a, l);
  int d2 = toleft(m.b, l);
  if ((d1 ^ d2) == -2) return 2;
  return (d1 == 0 || d2 == 0);
}
// 0 平行 1 重合 2 相交
int crossLL(Line l, Line m)
{
  if (parallel(l, m)) return intersect(l.a, m);
  return 2;
}
Point crosspointLL(const Line& l, const Line& m)
{
  db A = cross(l.b - l.a, m.b - m.a);
  db B = cross(l.b - l.a, l.b - m.a);
  if (sgn(A) == 0 and sgn(B) == 0) return m.a;
  return m.a + (m.b - m.a) * (B / A);
}
// 点到直线投影点
Point projection(const Point& p, const Line& l)
{
  db t = dot(p - l.a, l.a - l.b) / square(l.a - l.b);
  return l.a + (l.a - l.b) * t;
}
// 直线对称点
Point symmetrypoint(const Point& p, const Line& l)
{
  Point q = projection(p, l);
  return Point(2 * q.x - p.x, 2 * q.y - p.y);
}
using P = Point;
using Points = vector<P>;
using Polygon = Points;
// 周长
double perimeter(const Polygon& p)
{
  double res = 0;
  int n = p.size();
  for (int i = 0; i < n; i++) res += len(p[i] - p[(i + 1) % n]);
  return res;
}
// 面积两倍
db area2(const Polygon& p)
{
  db res = 0;
  int n = p.size();
  for (int i = 0; i < n; i++) res += cross(p[i], p[(i + 1) % n]);
  return res;
}

enum { OUT, ON, IN };
// 2 contains 1 on segment 0 out
int pointInPolygon(const Point& a, const Polygon& p)
{
  int n = p.size();
  for (int i = 0; i < n; i++) {
    if (intersect(a, Segment(p[i], p[(i + 1) % n]))) return ON;
  }
  int t = 0;
  for (int i = 0; i < n; i++) {
    auto u = p[i];
    auto v = p[(i + 1) % n];
    if (cmp(u.x, a.x) == -1 && cmp(v.x, a.x) >= 0 &&
      toleft(a, Line(v, u)) > 0) {
      t ^= 1;
    }
    if (cmp(u.x, a.x) >= 0 && cmp(v.x, a.x) == -1 &&
      toleft(a, Line(u, v)) > 0) {
      t ^= 1;
    }
  }
  return t == 1 ? IN : OUT;
}
// 凸多边形log求包含点问题
int pointInconvexPolygonlog(const Point& p, const Polygon& Q)
{
  int N = (int) Q.size();
  Point g = (Q[0] + Q[N / 3] + Q[N * 2 / 3]) / 3.0;
  if (g == p) return IN;
  Point gp = p - g;
  int l = 0, r = N;
  while (r - l > 1) {
    int mid = (l + r) / 2;
    Point gl = Q[l] - g;
    Point gm = Q[mid] - g;
    if (cross(gl, gm) > 0) {
      if (cross(gl, gp) >= 0 && cross(gm, gp) <= 0)
        r = mid;
      else
        l = mid;
    }
    else {
      if (cross(gl, gp) <= 0 && cross(gm, gp) >= 0)
        l = mid;
      else
        r = mid;
    }
  }
  r %= N;
  db v = cross(Q[l] - p, Q[r] - p);
  return sgn(v) == 0 ? ON : sgn(v) == -1 ? OUT : IN;
}
bool segmentInPolygon(const Segment& s, const Polygon& p)
{
  auto [a, b] = s;
  if (pointInPolygon(a, p) == 0 || pointInPolygon(b, p) == 0) return false;
  int n = p.size();
  Polygon q;
  for (int i = 0; i < n; i++) {
    auto l = Segment(p[i], p[(i + 1) % n]);
    if (crossSS(s, l) == 2) return false;
    if (intersect(a, l))
      q.push_back(a);
    else if (intersect(b, l))
      q.push_back(b);
    else if (intersect(p[i], l))
      q.push_back(p[i]);
  }
  sort(q.begin(), q.end());
  for (int i = 0; i + 1 < q.size(); i++) {
    if (pointInPolygon((q[i] + q[i + 1]) / 2, p) == 0) return false;
  }
  return true;
}
bool intersect(const Polygon& ps, const Polygon& qs)
{
  int pl = ps.size(), ql = qs.size(), i = 0, j = 0;
  while ((i < pl or j < ql) and (i < 2 * pl) and (j < 2 * ql)) {
    auto ps0 = ps[(i + pl - 1) % pl], ps1 = ps[i % pl];
    auto qs0 = qs[(j + ql - 1) % ql], qs1 = qs[j % ql];
    if (intersect(Segment(ps0, ps1), Segment(qs0, qs1))) return true;
    Point a = ps1 - ps0;
    Point b = qs1 - qs0;
    db v = cross(a, b);
    db va = cross(qs1 - qs0, ps1 - qs0);
    db vb = cross(ps1 - ps0, qs1 - ps0);
    if (!v and va < 0 and vb < 0) return false;
    if (!v and !va and !vb) {
      i += 1;
    }
    else if (v >= 0) {
      if (vb > 0)
        i += 1;
      else
        j += 1;
    }
    else {
      if (va > 0)
        j += 1;
      else
        i += 1;
    }
  }
  return false;
}
// 凸包
Polygon getHull(Polygon p)
{
  Polygon h, l;
  sort(p.begin(), p.end());
  p.erase(unique(p.begin(), p.end()), p.end());
  if (p.size() <= 1) return p;
  for (auto a : p) {
    //<= >= 弹出共线点 < >不弹出共线点
    while (h.size() > 1 && cross(a - h.back(), a - h[h.size() - 2]) <= 0)
      h.pop_back();
    while (l.size() > 1 && cross(a - l.back(), a - l[l.size() - 2]) >= 0)
      l.pop_back();
    l.push_back(a);
    h.push_back(a);
  }
  l.pop_back();
  reverse(h.begin(), h.end());
  h.pop_back();
  l.insert(l.end(), h.begin(), h.end());
  return l;
}
// 凸性判定 逆时针给出点
bool is_convex(Polygon p)
{
  int n = (int) p.size();
  if (n <= 2)return false;
  // Polygon 逆时针给出
  // for (int i = 0; i < n; i++) {
  //     if (pointonsegment(p[(i + n - 1) % n], p[i], p[(i + 1) % n]) == -1)
  //         return false;
  // return true;
  Polygon hull = getHull(p);
  if (hull.size() < n)return false;
  // for (int i = 0; i < n; i++)cerr << hull[i] << " \n"[i == n - 1];
  rotate(p.begin(), find(p.begin(), p.end(), hull[0]), p.end());
  if (p == hull)return true;
  reverse(p.begin(), p.end());
  rotate(p.begin(), find(p.begin(), p.end(), hull[0]), p.end());
  if (p == hull)return true;
  return false;
}

// 平面割 返回l左侧的多边形，凸多边形
Polygon convex_cut(const Polygon& a, const Line& l)
{
  Polygon ret;
  int n = a.size();
  for (int i = 0; i < n; i++) {
    const Point& now = a[i];
    const Point& nxt = a[(i + 1) % n];
    auto cf = toleft(now, l);
    auto cs = toleft(nxt, l);
    if (cf >= 0) {
      ret.emplace_back(now);
    }
    if (cf * cs < 0) {
      ret.emplace_back(crosspointLL(Line(now, nxt), l));
    }
  }
  return ret;
}
// 最远点对(旋转卡壳)凸多边形
db rotatingCalipers(Polygon a)
{
  int n = a.size();
  if (n <= 2) return distance(a[0], a[1]);
  int i = 0, j = 0;
  for (int k = 0; k < n; k++) {
    if (a[k] < a[i]) i = k;
    if (a[j] < a[k]) j = k;
  }
  db res = 0;
  int si = i, sj = j;
  while (i != sj || j != si) {
    res = max(res, distance(a[i], a[j]));
    if (crossOp(a[(i + 1) % n] - a[i], a[(j + 1) % n] - a[j]) < 0)
      i = (i + 1) % n;
    else
      j = (j + 1) % n;
  }
  return res;
}
// 分治法最近点对
db closestPair(Polygon s)
{
  sort(s.begin(), s.end());
  int n = s.size();
  vector<int>stk(n);
  db res = inf;
  auto cmpy = [&](const Point& a, const Point& b) {
    return a.y < b.y;
  };
  auto rec = [&](int l, int r, auto self)->void {
    if (r - l <= 3) {
      for (int i = l; i <= r; i++) {
        for (int j = i + 1; j <= r; j++) {
          res = min(res, distance(s[i], s[j]));
        }
      }
      sort(s.begin() + l, s.begin() + r + 1, cmpy);
      return;
    }
    int mid = (l + r) >> 1;
    db midx = s[mid].x;
    self(l, mid, self);
    self(mid + 1, r, self);
    inplace_merge(s.begin() + l, s.begin() + mid + 1, s.begin() + r + 1, cmpy);
    int top = 0;
    for (int i = l; i <= r; i++) {
      if (cmp(abs(s[i].x - midx), res) >= 0)continue;
      for (int j = top - 1; j >= 0 && s[i].y - s[stk[j]].y < res; j--) {
        res = min(res, distance(s[i], s[stk[j]]));
      }
      stk[top++] = i;
    }
  };
  rec(0, n - 1, rec);
  return res;
}
struct Circle {
  Point o;
  db r;
  Circle() {}
  Circle(Point o_, db r_) { o = o_, r = r_; }
  friend istream& operator>>(istream& is, Circle& c)
  {
    return is >> c.o >> c.r;
  }
};
// 4 相离 3 外切 2 相交 1 内切 0 包含
int crossCC(Circle c1, Circle c2)
{
  if (c1.r < c2.r) swap(c1, c2);
  db d = len(c1.o - c2.o);
  if (sgn(c1.r + c2.r - d) == -1) return 4;
  if (sgn(c1.r + c2.r - d) == 0) return 3;
  if (sgn(c1.r - c2.r - d) == -1) return 2;
  if (sgn(c1.r - c2.r - d) == 0) return 1;
  return 0;
}
int pointInCircle(const Point& p, const Circle& c)
{
  db d = distance(c.o, p);
  int cp = cmp(d, c.r);
  if (cp > 0) return OUT;
  if (cp < 0) return IN;
  return ON;
}
Circle incircle_triangle(Point a, Point b, Point c)
{
  db A = len(b - c), B = len(c - a), C = len(a - b);
  Point o = a * A + b * B + c * C;
  o /= A + B + C;
  db r = len(cross(o - a, b - a) / len(b - a));
  return { o, r };
}
//外接圆
Circle circumcircle_triangle(Point a, Point b, Point c)
{
  db x1 = a.x, x2 = b.x, x3 = c.x;
  db y1 = a.y, y2 = b.y, y3 = c.y;
  db A = x1 * x1 + y1 * y1, B = x2 * x2 + y2 * y2, C = x3 * x3 + y3 * y3;
  db u1 = x1 - x2, u2 = x1 - x3, u3 = x2 - x3;
  db v1 = y1 - y2, v2 = y1 - y3, v3 = y2 - y3;
  Point o;
  o.y = ((C - A) * u1 - (B - A) * u2) / (2 * v1 * u2 - 2 * v2 * u1);
  o.x = ((C - A) * v1 - (B - A) * v2) / (2 * u1 * v2 - 2 * u2 * v1);
  return { o,distance(o,a) };
}
bool intersect(const Circle& c, const Line& l)
{
  return cmp(c.r, distancePL(c.o, l)) >= 0;
}
//  0 圆的外部，内部包括圆上包含线段 1 严格穿过圆
//  2圆心在s的投影点在线段s上（不包括端点）
int intersect(const Circle& c, const Segment& s)
{
  Point h = projection(c.o, s);
  if (cmp(distance(c.o, h), c.r) > 0) return 0;
  db d1 = len(c.o - s.a), d2 = len(c.o - s.b);
  if (cmp(c.r, d1) >= 0 && cmp(c.r, d2) >= 0) return 0;
  if (cmp(c.r, d1) * cmp(c.r, d2) < 0) return 1;
  if (sgn(dot(s.a - h, s.b - h)) < 0) return 2;
  return 0;
}
//最小圆覆盖
Circle Min_Cover_Circle(Polygon P)
{
  mt19937_64 rnd(chrono::steady_clock::now().time_since_epoch().count());
  shuffle(P.begin(), P.end(), rnd);
  int n = P.size();
  if (P.empty()) {
    return Circle();
  }
  Circle C(P[0], 0);
  for (int i = 1; i < n; i++) {
    if (pointInCircle(P[i], C) == OUT) {
      C = Circle(P[i], 0);
      for (int j = 0; j < i; j++) {
        if (pointInCircle(P[j], C) == OUT) {
          C = Circle((P[j] + P[i]) / 2, len(P[j] - P[i]) / 2);
          for (int k = 0; k < j; k++) {
            if (pointInCircle(P[k], C) == OUT) {
              C = circumcircle_triangle(P[i], P[j], P[k]);
            }
          }
        }
      }
    }
  }
  return C;
}
// l1 l2 的对称轴
vector<Line> corner(Line l1, Line l2)
{
  vector<Line> res;
  if (parallel(l1, l2)) {
    db d = distancePL(l2.a, l1) / 2.0;
    Point v1 = l1.b - l1.a;
    v1 = unit(v1) * d;
    Point p = l2.a + rotleft(v1);
    db d1 = distancePL(p, l1);
    db d2 = distancePL(p, l2);
    if (abs(d1 - d2) > d) {
      p = l2.a + rotright(v1);
    }
    res.push_back(Line(p, p + v1));
  }
  else {
    Point p = crosspointLL(l1, l2);
    Point v1 = l1.b - l1.a, v2 = l2.b - l2.a;
    v1 = unit(v1);
    v2 = unit(v2);
    res.push_back(Line(p, p + (v1 + v2)));
    res.push_back(Line(p, p + rotleft(v1 + v2)));
  }
  return res;
}
vector<Point> crosspointCL(Circle c, Line l)
{
  Point h = projection(c.o, l);
  db d = c.r * c.r - square(c.o - h);
  if (sgn(d) == -1) return {};
  if (sgn(d) == 0) return { h };
  Point x = unit(l.a - l.b) * sqrt(d);
  return { h - x, h + x };
}
Polygon crosspointCS(const Circle& c, const Segment& s)
{
  int num = intersect(c, s);
  if (num == 0) return {};
  auto res = crosspointCL(c, Line(s.a, s.b));
  if (num == 2) return res;
  if (sgn(dot(s.a - res[0], s.b - res[0])) > 0) swap(res[0], res[1]);
  return { res[0] };
}
Polygon crosspointCC(Circle c1, Circle c2)
{
  db r1 = c1.r, r2 = c2.r;
  if (r1 < r2) return crosspointCC(c2, c1);
  db d = len(c2.o - c1.o);
  int op = crossCC(c1, c2);
  if (op == 4 || op == 0) return {};
  Point v = c2.o - c1.o;
  if (op == 3 || op == 1) return { c1.o + unit(v) * r1 };
  db p = ((r1 * r1 - r2 * r2) / d + d) / 2, q = sqrt(r1 * r1 - p * p);
  Point h = c1.o + unit(v) * p;
  Point i = unit(rotleft(v));
  return { h + i * q, h - i * q };
}
// p1,p2 的角平分线 在p1p2逆时针方向
Line bisector(Point p1, Point p2)
{
  Circle c1 = Circle(p1, len(p1 - p2)), c2 = Circle(p2, len(p1 - p2));
  auto p = crosspointCC(c1, c2);
  if (cross(p2 - p1, p[0] - p1) > 0) swap(p[0], p[1]);
  return Line(p[0], p[1]);
}
// 点到圆的切点
Polygon tangent_to_circle(const Circle& c, const Point& p)
{
  return crosspointCC(c, Circle(p, sqrt(square(c.o - p) - c.r * c.r)));
}
// 两圆公切线
vector<Line> common_tangent(const Circle& c1, const Circle& c2)
{
  if (c1.r < c2.r) return common_tangent(c2, c1);
  vector<Line> res;
  db g = distance(c1.o, c2.o);
  if (sgn(g) == 0) return res;
  Point u = (c2.o - c1.o) / g, v = rotleft(u);
  // -1 外公切线 1 内公切线
  for (int s : {-1, 1}) {
    db h = (c1.r + c2.r * s) / g;
    if (cmp(1, h * h) == 0)
      res.emplace_back(c1.o + u * c1.r, c1.o + (u + v) * c1.r);
    else if (cmp(1, h * h) > 0) {
      Point U = u * h, V = v * sqrt(1 - h * h);
      res.emplace_back(c1.o + (U + V) * c1.r, c2.o - (U + V) * c2.r * s);
      res.emplace_back(c1.o + (U - V) * c1.r, c2.o - (U - V) * c2.r * s);
    }
  }
  return res;
}
// 三角剖分 一个端点在圆心的三角形和圆的2倍面积并
db commonarea_impl(const Circle& c, const Point& a, const Point& b)
{
  auto va = c.o - a, vb = c.o - b;
  db f = cross(va, vb), res = 0;
  if (sgn(f) == 0) return res;
  if (cmp(max(len(va), len(vb)), c.r) <= 0) return f;
  if (cmp(distancePS(c.o, Segment(a, b)), c.r) >= 0)
    return c.r * c.r * arg(Point(dot(va, vb), cross(va, vb)));
  auto cand = crosspointCS(c, Segment(a, b));
  if (cand.empty()) return res;
  if (cand.size() > 1u && dot(cand[1] - cand[0], a - cand[0]) > 0)
    swap(cand[0], cand[1]);
  cand.emplace(cand.begin(), a);
  cand.emplace_back(b);
  for (int i = 0; i + 1 < cand.size(); i++)
    res += commonarea_impl(c, cand[i], cand[i + 1]);
  return res;
}
db commonarea(const Circle& c, const Polygon& P)
{
  if (P.size() < 3) return 0;
  db res = 0;
  int n = P.size();
  for (int i = 0; i < n; i++) res += commonarea_impl(c, P[i], P[(i + 1) % n]);
  return res / 2;
}
db commonarea(Circle c1, Circle c2)
{
  db r1 = c1.r, r2 = c2.r;
  db d = len(c1.o - c2.o);
  if (cmp(r1 + r2, d) <= 0) return 0;
  if (cmp(fabs(r1 - r2), d) >= 0) return pi * min(r1, r2) * min(r1, r2);
  db res = 0;
  for (int _ = 0; _ < 2; _++) {
    r1 = c1.r, r2 = c2.r;
    db cosine = (d * d + r1 * r1 - r2 * r2) / (2 * d * r1);
    db theta = acos(cosine) * 2;
    res += (theta - sin(theta)) * r1 * r1 / 2;
    swap(c1, c2);
  }
  return res;
}
// 已知两点半径，求圆心位置
vector<Point> center_given_radius(const Point& a, const Point& b, const db& r)
{
  Point m = (b - a) * 0.5;
  db d1 = len(m);
  if (sgn(d1) == 0 || d1 > r) return {};
  db d2 = sqrt(r * r - d1 * d1);
  Point n = rotleft(m) * d2 / d1;
  Point p = a + m - n, q = a + m + n;
  if (p == q) return { p };
  return { p, q };
}
//将平面上的A,B集合的点对相互连边，然后线段不交 随机化写法
vector<int> divide_no_cross_match(const Polygon& a, const Polygon& b)
{
  ll seed = chrono::steady_clock::now().time_since_epoch().count();
  mt19937_64 rnd(seed);
  int n = a.size();
  vector<int>match(n);
  auto solve = [&](vector<int>v1, vector<int>v2, auto self)->void {
    assert(v1.size() == v2.size());
    if (v1.empty())return;
    shuffle(ALL(v1), rnd);
    for (auto i : v1) {
      for (auto j : v2) {
        vector<int>nl1, nr1, nl2, nr2;
        for (auto x : v1) {
          if (i == x)continue;
          if (toleft(a[i], b[j], a[x]) > 0)nl1.push_back(x);
          else nr1.push_back(x);
        }
        for (auto x : v2) {
          if (j == x)continue;
          if (toleft(a[i], b[j], b[x]) > 0)nl2.push_back(x);
          else nr2.push_back(x);
        }
        if (nl1.size() != nl2.size())continue;
        match[i] = j;
        self(nl1, nl2, self);
        self(nr1, nr2, self);
        return;
      }
    }
    assert(0);
  };
  vector<int>v(n);
  iota(v.begin(), v.end(), 0);
  solve(v, v, solve);
  return match;
}
// n^2logn 通过极角排序
vector<int> divide_No_Cross_Match(const Polygon& a, const Polygon& b)
{
  int n = a.size();
  vector<pair<Point, int>>P(2 * n);
  vector<int>Id(2 * n);
  iota(ALL(Id), 0);
  for (int i = 0; i < n; i++)P[i] = { a[i],-1 }, P[i + n] = { b[i],1 };
  vector<int>match(n);
  vector<pair<vector<pair<Point, int>>, vector<int>>>stk;
  stk.emplace_back(P, Id);
  while (stk.size()) {
    auto [p, I] = stk.back();
    stk.pop_back();
    if (p.empty())continue;
    int m = p.size();
    for (int i = 1; i < m; i++) {
      if (p[i].first < p[0].first) {
        swap(p[i], p[0]);
        swap(I[i], I[0]);
      }
    }
    vector<int>id(m);
    iota(ALL(id), 0);
    sort(ALL(id), [&](int i, int j) {
      return (p[i].first - p[0].first).arg_cmp(p[j].first - p[0].first);
    });
    auto t = p;
    auto tI = I;
    for (int i = 0; i < m; i++) {
      p[i] = t[id[i]];
      I[i] = tI[id[i]];
    }
    int sum = 0;
    for (int i = 1; i < m; i++) {
      if (p[i].second != p[0].second && sum == 0) {
        if (p[0].second == -1)match[I[0]] = I[i];
        else match[I[i]] = I[0];
        stk.emplace_back(vector<pair<Point, int>>(p.begin() + 1, p.begin() + i), vector<int>(I.begin() + 1, I.begin() + i));
        stk.emplace_back(vector<pair<Point, int>>(p.begin() + i + 1, p.begin() + m), vector<int>(I.begin() + i + 1, I.begin() + m));
        break;
      }
      sum += p[i].second;
    }
  }
  for (int i = 0; i < n; i++)match[i] -= n;
  return match;
}
//半平面交
vector<Point> hp(vector<Line> lines)
{
  sort(lines.begin(), lines.end(), [&](auto l1, auto l2) {
    auto d1 = l1.b - l1.a;
    auto d2 = l2.b - l2.a;
    return (d1).arg_cmp(d2);
  });
  deque<Line> ls;
  deque<Point> ps;
  for (auto l : lines) {
    if (ls.empty()) {
      ls.push_back(l);
      continue;
    }
    while (!ps.empty() && !pointOnLineLeft(ps.back(), l)) {
      ps.pop_back();
      ls.pop_back();
    }
    while (!ps.empty() && !pointOnLineLeft(ps[0], l)) {
      ps.pop_front();
      ls.pop_front();
    }
    if (cross(l.b - l.a, ls.back().b - ls.back().a) == 0) {
      if (dot(l.b - l.a, ls.back().b - ls.back().a) > 0) {
        if (!pointOnLineLeft(ls.back().a, l)) {
          assert(ls.size() == 1);
          ls[0] = l;
        }
        continue;
      }
      return {};
    }
    ps.push_back(crosspointLL(ls.back(), l));
    ls.push_back(l);
  }
  while (!ps.empty() && !pointOnLineLeft(ps.back(), ls[0])) {
    ps.pop_back();
    ls.pop_back();
  }
  if (ls.size() <= 2) {
    return {};
  }
  ps.push_back(crosspointLL(ls[0], ls.back()));
  return vector(ps.begin(), ps.end());
}
Polygon Minkowski(const Polygon &P, const Polygon &Q) {
    P = getHull(P);
    Q = getHull(Q);
    int n = P.size(), m = Q.size();
    Polygon vP(n), vQ(m);
    for (int i = 0; i < n; i++) vP[i] = (P[(i + 1) % n] - P[i]);
    for (int i = 0; i < m; i++) vQ[i] = (Q[(i + 1) % m] - Q[i]);
    Polygon T;
    int i = 0, j = 0;
    T.push_back(vP[0] + vQ[0]);
    while (i < n && j < m) {
        if (cross(vP[i], vQ[j]) > 0) {
            T.push_back(T.back() + vP[i++]);
        } else {
            T.push_back(T.back() + vQ[j++]);
        }
    }
    while (i < n) T.push_back(T.back() + vP[i++]);
    while (j < m) T.push_back(T.back() + vQ[j++]);
    return getHull(T);
}