void kmp(const string &s, const string &t) {
  int n = s.length(), m = t.length();
  vector<int> pmt(m);
  // getpmt
  // 在 s 中匹配 t
  // pmt[i] 是 [0,i] 的最长公共前后缀长度
  for (int i = 1, j = 0; i < m; i++) {
    while (j && t[i] != t[j]) j = pmt[j - 1];
    if (t[i] == t[j]) j++;
    pmt[i] = j;
  }
  for (int i = 0, j = 0; i < n; i++) {
    while (j && s[i] != t[j]) j = pmt[j - 1];
    if (s[i] == t[j]) j++;
    if (j == m) {
      cout << i - j + 2 << "\n";
      j = pmt[j - 1];
    }
  }
}