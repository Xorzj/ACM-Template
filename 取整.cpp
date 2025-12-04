int sq(int a, int b) {
  if (a > 0) return (a - 1) / b + 1;
  return a / b;
}
int xq(int a, int b) {
  if (a >= 0) return a / b;
  return (a + 1) / b - 1;
}