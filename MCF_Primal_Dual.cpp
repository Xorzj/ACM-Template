namespace MCF_Primal_Dual {
    const int N = 1e3 + 5;
    int flow[N][N];
    int pre[N];
    bool vis[N];
    ll cost[N][N];
    ll h[N], dis[N];
    int n, s, t;
    void init(int n_, int s_, int t_)
    {
        s = s_, t = t_, n = n_;
    }
    void add_edge(int u, int v, int w, ll c)
    {
        assert(u < n && v < n);
        flow[u][v] = w;
        flow[v][u] = 0;
        cost[u][v] = c;
        cost[v][u] = -c;
        //有上下界最小费用可行流
        // if (c < 0) {
        //     flow[u][v] = 0;
        //     flow[v][u] = w;
        //     cost[u][v] = c;
        //     cost[v][u] = -c;
        // }
        // else {
        //     flow[u][v] = w;
        //     flow[v][u] = 0;
        //     cost[u][v] = c;
        //     cost[v][u] = -c;
        // }
    }
    bool dijkstra()
    {
        for (int i = 0; i < n; i++)  dis[i] = numeric_limits<ll>::max(), pre[i] = -1;
        for (int i = 0; i < n; i++)vis[i] = false;
        dis[s] = 0;
        while (true) {
            int x = -1;
            for (int i = 0; i < n; i++) {
                if (vis[i] == false) {
                    if (x == -1 || dis[i] < dis[x]) {
                        x = i;
                    }
                }
            }
            if (x == -1)break;
            vis[x] = true;
            for (int y = 0; y < n; y++) {
                int w = flow[x][y];
                ll c = cost[x][y];
                if (w > 0 && dis[y] > dis[x] + h[x] - h[y] + c) {
                    dis[y] = dis[x] + h[x] - h[y] + c;
                    pre[y] = x;
                }
            }
        }
        return dis[t] != numeric_limits<ll>::max();
    }
    pair<int, ll>EK()
    {
        auto spfa = [&]() {
            queue<int>que;
            memset(vis, 0, sizeof(vis));
            for (int i = 0; i < n; i++)h[i] = numeric_limits<ll>::max();
            h[s] = 0;
            que.emplace(s);
            while (!que.empty()) {
                int x = que.front();
                que.pop();
                vis[x] = 0;
                for (int y = 0; y < n; y++) {
                    int w = flow[x][y];
                    ll c = cost[x][y];
                    if (w && h[x] + c < h[y]) {
                        h[y] = h[x] + c;
                        if (!vis[y])que.emplace(y), vis[y] = 1;
                    }
                }
            }
        };
        int fl = 0;
        ll co = 0;
        spfa();
        while (dijkstra()) {
            for (int i = 0; i < n; i++)h[i] += dis[i];
            int aug = numeric_limits<int>::max();
            for (int i = t; i != s; i = pre[i])aug = min(aug, flow[pre[i]][i]);
            for (int i = t; i != s; i = pre[i])flow[pre[i]][i] -= aug, flow[i][pre[i]] += aug;
            fl += aug;
            co += ll(aug) * h[t];
        }
        return make_pair(fl, co);
    }
};
using MCF_Primal_Dual::add_edge;
using MCF_Primal_Dual::EK;
using MCF_Primal_Dual::init;