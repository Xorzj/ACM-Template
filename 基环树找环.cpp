// 闹麻了，只能单一子图，多子图记得拿并查集维护下连通性，从任意点出发都可以，但找到环就跑路才不给你标记子图
//这代码也是唐诗，内向基环树，有jb vector遍历，有寄吧反向边(x==p)???
vector<vector<pii>>adj(n + 1);
for (int i = 0; i < n; i++) {
  int x, y;
  cin >> x >> y;
  adj[x].push_back({ y,i });
  adj[y].push_back({ x,i });
}
vector<int>par(n + 1);
vector<int>used(n + 1);
vector<int>cyc;
auto dfs = [&](int x, int f, int fid, auto&& self)->bool {
  used[x] = 1;
  par[x] = f;
  for (auto [y, id] : adj[x]) {
    if (used[y] == 2 || id == fid)continue;
    if (used[y]) {
      for (int v = x; v != y; v = par[v]) {
        cyc.push_back(v);
      }
      cyc.push_back(y);
      return true;
    }
    if (self(y, x, id, self)) {
      return true;
    }
  }
  used[x] = 2;
  return false;
};
dfs(1, 0, -1, dfs);