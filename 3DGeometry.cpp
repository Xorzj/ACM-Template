typedef double db;
constexpr db inf = 1e18;
constexpr db eps = 1e-10;
const db pi = acos(-1);
constexpr int sgn(db x) { return x < -eps ? -1 : x > eps; }
constexpr int cmp(db x, db y) { return sgn(x - y); }
//三维计算几何
mt19937_64 rnd(time(0));
struct Point3 {
    db x, y, z;
    constexpr Point3(db _x = 0, db _y = 0, db _z = 0) : x(_x), y(_y), z(_z) {}
    constexpr Point3 operator+() const noexcept { return *this; }
    constexpr Point3 operator-() const noexcept { return Point3(-x, -y, -z); }
    constexpr Point3 operator+(const Point3& p) const
    {
        return Point3(x + p.x, y + p.y, z + p.z);
    }
    constexpr Point3 operator-(const Point3& p) const
    {
        return Point3(x - p.x, y - p.y, z - p.z);
    }
    constexpr Point3 operator*(const db& k) { return Point3(x * k, y * k, z * k); }
    constexpr Point3 operator/(const db& k) { return Point3(x / k, y / k, z / k); }
    db operator / (const Point3& r) const { return x * r.x + y * r.y + z * r.z; }
    Point3 operator * (const Point3& r) const { return Point3(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x); }
    constexpr Point3& operator+=(const Point3& p)
    {
        return x += p.x, y += p.y, z += p.z, *this;
    }
    constexpr Point3& operator-=(const Point3& p)
    {
        return x -= p.x, y -= p.y, z -= p.z, *this;
    }
    constexpr Point3& operator*=(const db& k) { return x *= k, y *= k, z *= k, *this; }
    constexpr Point3& operator/=(const db& k) { return x /= k, y /= k, z /= k, *this; }
    constexpr bool operator==(const Point3& r) const noexcept
    {
        return cmp(x, r.x) == 0 and cmp(y, r.y) == 0 and cmp(z, r.z) == 0;
    }
    constexpr bool operator <(const Point3& r)const noexcept
    {
        if (sgn(x - r.x))return x < r.x;
        if (sgn(y - r.y))return y < r.y;
        return z < r.z;
    }
    friend istream& operator>>(istream& is, Point3& p) { return is >> p.x >> p.y >> p.z; }
    friend ostream& operator<<(ostream& os, Point3 p)
    {
        return os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
    }
    constexpr db dot(const Point3& r) const { return x * r.x + y * r.y + z * r.z; }
    constexpr Point3 cross(const Point3& r) const
    {
        return Point3(y * r.z - z * r.y, z * r.x - x * r.z, x * r.y - y * r.x);
    }
    void shake(double eps = 1e-12)//微小扰动，去掉四点共面
    {
        uniform_real_distribution<double> dist(-0.5, 0.5);
        x += dist(rnd) * eps;
        y += dist(rnd) * eps;
        z += dist(rnd) * eps;
    }
};
db dot(const Point3& a, const Point3& b) { return a.dot(b); }
Point3 cross(const Point3& a, const Point3& b) { return a.cross(b); }
db square(Point3 p) { return dot(p, p); }
double len(Point3 p) { return sqrt(square(p)); }
Point3 unit(Point3 p) { return p / len(p); }//db=double only
//volumn除以6为四面体组成的体积
db volumn0(Point3 a, Point3 b, Point3 c) { return dot(a, cross(b, c)); }
db volumn(Point3 a, Point3 b, Point3 c, Point3 d) { return dot(d - a, cross(b - a, c - a)); }
struct Line {
    Point3 a, b;
    Line(Point3 a_, Point3 b_) :a(a_), b(b_) {}
    Point3 normal() { return b - a; }
    Point3 pos(Point3 t) { return normal() * (t - a); }  // square == 0 在直线上
    Point3 pos(Line t) { return normal() * (t.normal()); } // square == 0 平行
    db dir(Line t) { return normal() / (t.normal()); }    // == 0 垂直，> 0 同向
    double dis_to_line(Point3 t) { return len(pos(t)) / len(normal()); } //点到直线距离
    bool equal(Line t) { return sgn(dir(t)) > 0 && sgn(square(pos(t.a))) == 0; }
    Point3 intersect(Line t)
    {  // 需要保证不平行，如果直线异面，返回的是俩直线的公垂线在 *this 直线上的垂足
        assert(sgn(square(pos(t))) != 0);
        return a + normal() * (t.pos(a) / pos(t)) / square(pos(t));
    }
};
struct face {
    Point3 a, b, c;//逆时针方向
    face(Point3 a_, Point3 b_, Point3 c_) :a(a_), b(b_), c(c_) {}
    Point3 normal() { return cross(b - a, c - a); }
    bool above(Point3 p) { return sgn(volumn(a, b, c, p)) > 0; }
    db pos(const Point3& t) { return dot(normal(), (t - a)); }   // > 0 严格在上方，== 0 在面上
    db pos(Line t) { return normal() / t.normal(); }   // == 0 平行
    Point3 pos(face t) { return normal() * t.normal(); } // square == 0 平行
    Point3 dir(Line t) { return normal() * t.normal(); }   // square == 0 垂直
    db dir(face t) { return normal() / t.normal(); }    // == 0 垂直, > 0 同向
    double dis_to_face(const Point3& t) { return pos(t) / len(normal()); }   // 在法向量上的投影
    bool equal(const face& t) { return sgn(dir(t)) > 0 && sgn(len(pos(t))) == 0 && sgn(pos(t.a)) == 0; }    // 面是否相等
};
typedef vector<Point3> vP;
typedef vector<vector<Point3>> vvP;
//真实表面积/2,真实体积/6
double area(vector<face>p)
{
    double res = 0;
    for (auto t : p)res += len(t.normal());
    return res;
}
db volumn(vector<face>p)
{
    db res = 0;
    for (auto t : p)res += dot(cross(t.a, t.b), t.c);
    return res;
}
vector<face> convex3d(vP p)
{
    auto q = p;
    // for (auto& x : p)x.shake();
    int n = p.size();
    for (int i = 0; i < n; i++) {
        if (p[i] < p[0])swap(p[0], p[i]), swap(q[0], q[i]);
    }
    for (int i = 2; i < n; i++) {
        if ((p[i].x - p[0].x) * (p[1].y - p[0].y) > (p[i].y - p[0].y) * (p[1].x - p[0].x)) {
            swap(p[1], p[i]), swap(q[1], q[i]);
        }
    }
    vector<face>res;
    set<pii>edge;
    //不允许出现四点共面
    if (n < 4) {
        return res;
    }
    function<void(int, int)>wrap = [&](int a, int b) {
        if (edge.count({ a,b }))return;
        int c = -1;
        for (int i = 0; i < n; i++) {
            if (i == a || i == b)continue;
            if (c == -1 || volumn(p[c], p[a], p[b], p[i]) > 0)c = i;
        }
        if (c == -1)return;
        res.emplace_back(q[a], q[b], q[c]);
        edge.insert({ a,b }), edge.insert({ b,c }), edge.insert({ c,a });
        wrap(c, b);
        wrap(a, c);
    };
    wrap(0, 1);
    return res;
}
vector<face>convex3d2(vP p)
{
    vector<face>res;
    // for (auto& x : p)x.shake();
    if (p.size() < 4) return res;   // 不允许四点共面
    shuffle(p.begin(), p.end(), rnd);
    res.push_back({ p[0], p[1], p[2] });
    res.push_back({ p[2], p[1], p[0] });
    for (int i = 3; i < p.size(); i++) {
        vector<face> tmp;
        set<pair<Point3, Point3>> edge;
        for (auto& x : res) {
            if (x.pos(p[i]) < 0) tmp.emplace_back(x);
            else edge.emplace(x.a, x.b), edge.emplace(x.b, x.c), edge.emplace(x.c, x.a);
        }
        for (auto& [x, y] : edge) {
            if (!edge.count({ y, x })) tmp.push_back({ x, y, p[i] });
        }
        swap(res, tmp);
    }
    return res;
}
// 凸包重心
Point3 centroid(vector<face>p, Point3 G = {}, db sum = 0)
{
    for (auto& x : p) {
        auto ng = x.a + x.b + x.c;
        db nv = dot(cross(x.a, x.b), x.c);
        sum -= nv;
        G -= ng * nv;
    }
    return G / 4 / sum;
}