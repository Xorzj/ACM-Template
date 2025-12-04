istream &operator>>(istream &is, __int128 &T) {
  char c;
  int f = 1;
  T = 0;
  c = is.get();
  while (c != '-' && !isdigit(c)) c = is.get();
  if (c == '-') f = -1, c = is.get();
  while (isdigit(c)) {
    T = T * 10 + (c - '0');
    c = is.get();
  }
  T = f * T;
  return is;
}
ostream &operator<<(ostream &os, __int128 T) {
  if (T < 0) T = -T, os << "-";
  if (T > 9) os << T / 10;
  return os << int(T % 10);
}