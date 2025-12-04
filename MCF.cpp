namespace MCF {
    const int N = 1e4 + 5, M = 1e6 + 5;
    struct edge {
        int next, to, w;
        ll c;
    } e[M << 1];
    int n, s, t;
    int head[N], cnt, pre[N];
    ll h[N], dis[N];
    void init(int n_, int s_, int t_)
    {
        s = s_, t = t_, n = n_, cnt = 0;
        for (int i = 0; i < n; i++) head[i] = -1;
    }
    void add_edge(int u, int v, int w, ll c)
    {
        assert(u < n && v < n);
        e[cnt] = edge{ head[u], v, w,c };
        head[u] = cnt;
        cnt++;
        e[cnt] = edge{ head[v], u, 0,-c };
        head[v] = cnt;
        cnt++;
        //有上下界最小费用可行流
        // if (c < 0) {
        //     assert(u < n && v < n);
        //     e[cnt] = edge{ head[u], v, 0,c };
        //     head[u] = cnt;
        //     cnt++;
        //     e[cnt] = edge{ head[v], u, w ,-c };
        //     head[v] = cnt;
        //     cnt++;
        // }
        // else {
        //     assert(u < n && v < n);
        //     e[cnt] = edge{ head[u], v, w,c };
        //     head[u] = cnt;
        //     cnt++;
        //     e[cnt] = edge{ head[v], u, 0,-c };
        //     head[v] = cnt;
        //     cnt++;
        // }
    }
    bool dijkstra()
    {
        for (int i = 0; i < n; i++)  dis[i] = numeric_limits<ll>::max(), pre[i] = -1;
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> q;
        dis[s] = 0;
        q.emplace(0, s);
        while (!q.empty()) {
            auto [d, x] = q.top();
            q.pop();
            if (dis[x] < d) continue;
            for (int i = head[x]; i != -1; i = e[i].next) {
                int y = e[i].to;
                ll c = e[i].c;
                int w = e[i].w;
                if (w > 0 && dis[y] > d + h[x] - h[y] + c) {
                    dis[y] = d + h[x] - h[y] + c;
                    pre[y] = i;
                    q.emplace(dis[y], y);
                }
            }
        }
        return dis[t] != numeric_limits<ll>::max();
        // 最小费用可行流
        // if (dis[t] == numeric_limits<ll>::max() || h[t] + dis[t] >= 0)return false;
        // else return true;

    }

    pair<int, ll> flow()
    {
        auto spfa = [&]() {
            queue<int> que;
            vector<bool> vis(n);
            for (int i = 0; i < n; i++)h[i] = numeric_limits<ll>::max() / 2;
            h[s] = 0, que.emplace(s);
            while (!que.empty()) {
                int x = que.front();
                que.pop();
                vis[x] = 0;
                for (int i = head[x]; ~i; i = e[i].next) {
                    int y = e[i].to, w = e[i].w;
                    ll c = e[i].c;
                    if (w > 0 && h[x] + c < h[y]) {
                        h[y] = h[x] + c;
                        if (!vis[y])que.emplace(y), vis[y] = 1;
                    }
                }
            }
        };
        int flow = 0;
        ll cost = 0;
        spfa();
        while (dijkstra()) {
            for (int i = 0; i < n; i++) h[i] += dis[i];
            int aug = numeric_limits<int>::max();
            for (int i = t; i != s; i = e[pre[i] ^ 1].to) aug = min(aug, e[pre[i]].w);
            for (int i = t; i != s; i = e[pre[i] ^ 1].to) {
                e[pre[i]].w -= aug;
                e[pre[i] ^ 1].w += aug;
            }
            flow += aug;
            cost += ll(aug) * h[t];
        }
        return make_pair(flow, cost);
    }
};  // namespace flow
using MCF::add_edge;
using MCF::flow;
using MCF::init;