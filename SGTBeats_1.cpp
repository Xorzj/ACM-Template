//区间加，区间取min，查询区间和，区间最大值，区间历史最大值
struct SGTBeats {
#define inf 1e18
#define mid ((l + r) >> 1)
#define ls (x << 1)
#define rs (x << 1 | 1)

    void chmax(ll& x, ll y) { x = max(x, y); }

    struct Info {
        ll mxa = -inf, se = -inf, cnt = 0, mxb = -1e18, sum = 0;
        // 下面四个都是加法懒标记，只是对象不同
        ll add_a = 0;  // 区间最大值
        ll add_b = 0;  // 区间历史最大值
        ll add_a1 = 0;  // 区间非最大值
        ll add_b1 = 0;  // 区间历史非最大值
        int len = 0;
    };
    friend Info operator + (const Info& a, const Info& b)
    {
        Info c = Info();
        c.sum = a.sum + b.sum;
        c.mxa = max(a.mxa, b.mxa);
        c.mxb = max(a.mxb, b.mxb);
        c.len = a.len + b.len;
        if (a.mxa == b.mxa) {
            c.cnt = a.cnt + b.cnt;
            c.se = max(a.se, b.se);
        }
        else {
            c.cnt = (a.mxa > b.mxa ? a.cnt : b.cnt);
            c.se = (a.mxa > b.mxa ? max(b.mxa, a.se) : max(a.mxa, b.se));
        }
        return c;
    }
    int n;
    vector<Info> info;
    SGTBeats() : n(0) {}
    SGTBeats(vector<ll> t_) { init(t_); }
    void pull(int x)
    {
        info[x] = info[ls] + info[rs];
    }
    void init(vector<ll> a)  //[1,n]
    {
        n = a.size() - 1;
        info.assign((n << 2) + 1, Info());
        function<void(int, int, int)> build = [&](int x, int l, int r) -> void {
            if (l == r) {
                info[x].mxa = info[x].mxb = info[x].sum = a[l];
                info[x].se = -inf;
                info[x].cnt = 1;
                info[x].len = 1;
                return;
            }
            build(x << 1, l, mid);
            build(x << 1 | 1, mid + 1, r);
            pull(x);
        };
        build(1, 1, n);
    }
    void modify(int x, ll add_a, ll add_b, ll add_a1, ll add_b1)
    {
        Info t = info[x];
        t.sum += add_a * info[x].cnt + add_a1 * (info[x].len - info[x].cnt);
        t.mxa += add_a;
        chmax(t.mxb, info[x].mxa + add_b);
        t.add_a += add_a;
        t.add_a1 += add_a1;
        chmax(t.add_b, info[x].add_a + add_b);
        chmax(t.add_b1, info[x].add_a1 + add_b1);
        if (t.se != -inf) t.se += add_a1;
        info[x] = t;
    }
    void push(int x)
    {
        int mxn = max(info[ls].mxa, info[rs].mxa);
        if (info[ls].mxa == mxn)
            modify(ls, info[x].add_a, info[x].add_b, info[x].add_a1, info[x].add_b1);
        else
            modify(ls, info[x].add_a1, info[x].add_b1, info[x].add_a1, info[x].add_b1);
        if (info[rs].mxa == mxn)
            modify(rs, info[x].add_a, info[x].add_b, info[x].add_a1, info[x].add_b1);
        else
            modify(rs, info[x].add_a1, info[x].add_b1, info[x].add_a1, info[x].add_b1);
        info[x].add_a = info[x].add_a1 = info[x].add_b = info[x].add_b1 = 0;
    }
    void modify_add(int x, int l, int r, int ql, int qr, int v)
    {
        if (ql <= l && r <= qr) {
            modify(x, v, v, v, v);
            return;
        }
        push(x);
        if (ql <= mid) modify_add(ls, l, mid, ql, qr, v);
        if (qr > mid) modify_add(rs, mid + 1, r, ql, qr, v);
        pull(x);
    }
    void modify_add(int ql, int qr, int v)
    {
        modify_add(1, 1, n, ql, qr, v);
    }
    void modify_min(int x, int l, int r, int ql, int qr, int v)
    {
        if (ql > r || qr < l || v >= info[x].mxa) return;
        if (ql <= l && r <= qr && v > info[x].se) {
            modify(x, v - info[x].mxa, v - info[x].mxa, 0, 0);
            return;
        }
        push(x);
        modify_min(ls, l, mid, ql, qr, v);
        modify_min(rs, mid + 1, r, ql, qr, v);
        pull(x);
    }
    void modify_min(int ql, int qr, int v)
    {
        modify_min(1, 1, n, ql, qr, v);
    }
    Info query(int x, int l, int r, int ql, int qr)
    {
        if (ql > r || qr < l) return Info();
        if (ql <= l && r <= qr) return info[x];
        push(x);
        return query(ls, l, mid, ql, qr) + query(rs, mid + 1, r, ql, qr);
    }
    Info query(int ql, int qr)
    {
        return query(1, 1, n, ql, qr);
    }
};