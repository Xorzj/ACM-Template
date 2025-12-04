struct trie {
  int next[MAXN][MAXC] = {0};
  int num[MAXN] = {0};
  int cnt = 0;
  int change(char c) {
    if ('A' <= c && c <= 'Z')
      return (c - 'A' + 27);
    else if ('a' <= c && c <= 'z')
      return (c - 'a' + 1);
    else
      return (c - '0' + 26 * 2 + 1);
  }
  void ins(string s) {
    int cur = 0;
    for (auto c : s) {
      int v = change(c);
      if (!next[cur][v]) next[cur][v] = ++cnt;
      cur = next[cur][v];
      num[cur]++;
    }
  }
  int ask(string s) {
    int cur = 0;
    for (auto c : s) {
      int v = change(c);
      if (!next[cur][v]) return 0;
      cur = next[cur][v];
    }
    return 1;
  }
  int ask_num(string s) {
    int cur = 0;
    for (auto c : s) {
      int v = change(c);
      if (!next[cur][v]) return 0;
      cur = next[cur][v];
    }
    return num[cur];
  }
  void clear() {
    for (int i = 0; i <= cnt; i++)
      for (int j = 0; j < MAXC) num[i] = next[i][j] = 0;
    cnt = 0;
  }
};