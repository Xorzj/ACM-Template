struct Matrix {
  int mat[MAXN][MAXN], n;
  Matrix(int _n) {
    n = _n;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        mat[i][j] = 0;
      }
    }
  }
  Matrix operator*(const Matrix &b) const {
    Matrix ret = Matrix(n);
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        for (int k = 0; k < n; k++) {
          int tmp = (long long)mat[i][k] * b.mat[k][j] % mod;
          ret.mat[i][j] = (ret.mat[i][j] + tmp) % mod;
        }
    return ret;
  }
};
Matrix qpow(Matrix a, int b) {
  int n = a.n;
  Matrix res(n);
  for (int i = 0; i < n; i++) res.mat[i][i] = 1;
  while (b) {
    if (b & 1) res = res * a;
    a = a * a;
    b >>= 1;
  }
  return res;
}