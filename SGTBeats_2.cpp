//区间加，区间取min,区间取max,查询区间和，区间最大值，区间最小值
struct SGTBeats {
#define inf 1e18
#define mid ((l + r) >> 1)
#define ls (x << 1)
#define rs (x << 1 | 1)

    struct Info {
        ll min1 = inf, min2 = inf, mincnt = 0;
        ll max1 = -inf, max2 = -inf, maxcnt = 0;
        ll tag = 0;
        ll sum = 0;
        int len = 0;
    };
    friend Info operator + (const Info& a, const Info& b)
    {
        Info c;
        c.sum = a.sum + b.sum;
        c.len = a.len + b.len;
        if (a.max1 == b.max1) {
            c.max1 = a.max1;
            c.maxcnt = a.maxcnt + b.maxcnt;
            c.max2 = max(a.max2, b.max2);
        }
        else if (a.max1 > b.max1) {
            c.max1 = a.max1;
            c.maxcnt = a.maxcnt;
            c.max2 = max(a.max2, b.max1);
        }
        else {
            c.max1 = b.max1;
            c.maxcnt = b.maxcnt;
            c.max2 = max(a.max1, b.max2);
        }
        if (a.min1 == b.min1) {
            c.min1 = a.min1;
            c.mincnt = a.mincnt + b.mincnt;
            c.min2 = min(a.min2, b.min2);
        }
        else if (a.min1 < b.min1) {
            c.min1 = a.min1;
            c.mincnt = a.mincnt;
            c.min2 = min(a.min2, b.min1);
        }
        else {
            c.min1 = b.min1;
            c.mincnt = b.mincnt;
            c.min2 = min(a.min1, b.min2);
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
                info[x].max1 = info[x].min1 = info[x].sum = a[l];
                info[x].maxcnt = info[x].mincnt = 1;
                info[x].max2 = -inf;
                info[x].min2 = inf;
                info[x].len = 1;
                return;
            }
            build(x << 1, l, mid);
            build(x << 1 | 1, mid + 1, r);
            pull(x);
        };
        build(1, 1, n);
    }
    void push_add(int x, ll v)
    {
        info[x].tag += v;
        info[x].sum += v * info[x].len;
        info[x].max1 += v;
        info[x].max2 += v;
        info[x].min1 += v;
        info[x].min2 += v;
    }
    void push_max(int x, ll v)
    {
        info[x].sum += (v - info[x].min1) * info[x].mincnt;
        if (info[x].min1 == info[x].max1)info[x].min1 = info[x].max1 = v;
        else if (info[x].min1 == info[x].max2)info[x].min1 = info[x].max2 = v;
        else info[x].min1 = v;
    }
    void push_min(int x, ll v)
    {
        info[x].sum -= (info[x].max1 - v) * info[x].maxcnt;
        if (info[x].max1 == info[x].min1)info[x].max1 = info[x].min1 = v;
        else if (info[x].max1 == info[x].min2)info[x].max1 = info[x].min2 = v;
        else info[x].max1 = v;
    }
    void push(int x)
    {
        if (info[x].tag) {
            push_add(ls, info[x].tag);
            push_add(rs, info[x].tag);
            info[x].tag = 0;
        }
        if (info[x].max1 < info[ls].max1) push_min(ls, info[x].max1);
        if (info[x].max1 < info[rs].max1) push_min(rs, info[x].max1);
        if (info[x].min1 > info[ls].min1) push_max(ls, info[x].min1);
        if (info[x].min1 > info[rs].min1) push_max(rs, info[x].min1);
    }
    void update_add(int x, int l, int r, int ql, int qr, ll v)
    {
        if (ql <= l and r <= qr) return push_add(x, v);
        push(x);
        if (ql <= mid) update_add(ls, l, mid, ql, qr, v);
        if (mid < qr) update_add(rs, mid + 1, r, ql, qr, v);
        pull(x);
    }
    void update_add(int ql, int qr, ll v)
    {
        if (ql > qr) return;
        update_add(1, 1, n, ql, qr, v);
    }

    void update_max(int x, int l, int r, int ql, int qr, ll v)
    {
        if (v <= info[x].min1) return;
        if (ql <= l and r <= qr and v < info[x].min2) return push_max(x, v);
        push(x);
        if (ql <= mid) update_max(ls, l, mid, ql, qr, v);
        if (mid < qr) update_max(rs, mid + 1, r, ql, qr, v);
        pull(x);
    }
    void update_max(int ql, int qr, ll v)
    {
        if (ql > qr) return;
        update_max(1, 1, n, ql, qr, v);
    }

    void update_min(int x, int l, int r, int ql, int qr, ll v)
    {
        if (v >= info[x].max1) return;
        if (ql <= l and r <= qr and v > info[x].max2) return push_min(x, v);
        push(x);
        if (ql <= mid) update_min(ls, l, mid, ql, qr, v);
        if (mid < qr) update_min(rs, mid + 1, r, ql, qr, v);
        pull(x);
    }
    void update_min(int ql, int qr, ll v)
    {
        if (ql > qr) return;
        update_min(1, 1, n, ql, qr, v);
    }
    Info query(int x, int l, int r, int ql, int qr)
    {
        if (ql > r || qr < l) return Info();
        if (ql <= l && r <= qr) {
            return info[x];
        }
        push(x);
        return query(ls, l, mid, ql, qr) + query(rs, mid + 1, r, ql, qr);
    }
    Info query(int ql, int qr)
    {
        return query(1, 1, n, ql, qr);
    }
};