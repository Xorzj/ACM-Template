void FMT_or(vector<Z>& f, int n, int op)
{
    for (int i = 1; i < 1 << n; i *= 2) {
        for (int j = 0; j < (1 << n); j += 2 * i) {
            for (int k = 0; k < i; k++) {
                f[i + j + k] += f[j + k] * op;
            }
        }
    }
}
void FMT_and(vector<Z>& f, int n, int op)
{
    for (int i = 1; i < 1 << n; i *= 2) {
        for (int j = 0; j < (1 << n); j += 2 * i) {
            for (int k = 0; k < i; k++) {
                f[j + k] += f[i + j + k] * op;
            }
        }
    }
}
void FWT_xor(vector<Z>& f, int n, int op)
{
    Z inv2 = CInv<2, P>;
    for (int i = 1; i < 1 << n; i *= 2) {
        for (int j = 0; j < (1 << n); j += 2 * i) {
            for (int k = 0; k < i; k++) {
                auto x = f[j + k], y = f[i + j + k];
                f[j + k] = x + y;
                f[i + j + k] = x - y;
                if (op == -1) {
                    f[j + k] *= inv2;
                    f[i + j + k] *= inv2;
                }
            }
        }
    }
}