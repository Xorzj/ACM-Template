// 返回在循环同构下的最小字典序字符串
string min_rep(string s) {
  int n = s.length();
  int k = 0, i = 0, j = 1;
  while (k < n && i < n && j < n) {
    if (s[(i + k) % n] == s[(j + k) % n])
      k++;
    else {
      if (s[(i + k) % n] > s[(j + k) % n])
        i = i + k + 1;
      else
        j = j + k + 1;
      if (i == j) j++;
      k = 0;
    }
  }
  i = min(i, j);
  string ans = s.substr(i) + s.substr(0, i);
  return ans;
}