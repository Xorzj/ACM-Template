//求解的是最小权左部点完美匹配，如果不为完美匹配会死循环，即左部点必须完全匹配，可以添加虚边 如果求出的最小权值>INF 就说明不存在完美匹配
//初始邻接矩阵每条边应该为正无穷，给定的边权应该取反
//返回的是左部点的匹配
using T = long long;
const T KMMAX = 1e18;
pair<T, vector<int>>hungarian(const vector<vector<T>>& a)// 1 base 
{
    if (a.empty())return { 0,{} };
    int n = a.size() - 1, m = a[0].size() - 1;
    vector<T>u(n + 1), v(m + 1);
    vector<int>p(m + 1), ans(n + 1);
    for (int i = 1; i <= n; i++) {
        p[0] = i;
        int j0 = 0;// add "dummy" worker 0
        vector<T>dis(m + 1, KMMAX);
        vector<int>pre(m + 1, -1);
        vector<bool>vis(m + 1);
        do {
            vis[j0] = true;
            int i0 = p[j0], j1;
            T delta = KMMAX;
            for (int j = 1; j <= m; j++) {
                if (vis[j])continue;
                auto t = a[i0][j] - u[i0] - v[j];
                if (t < dis[j])dis[j] = t, pre[j] = j0;
                if (dis[j] < delta)delta = dis[j], j1 = j;
            }
            for (int j = 0; j <= m; j++) {
                if (vis[j])u[p[j]] += delta, v[j] -= delta;
                else dis[j] -= delta;
            }
            j0 = j1;
        } while (p[j0]);
        while (j0) {
            int j1 = pre[j0];
            p[j0] = p[j1];
            j0 = j1;
        }
    }
    for (int j = 1; j <= m; j++) {
        if (p[j]) {
            ans[p[j]] = j;
        }
    }
    return { -v[0],ans };// min cost
}