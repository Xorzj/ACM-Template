vector<int> primes, minp, dp;
// dp is ans array
void sieve(int n)
{
    minp.assign(n + 1, 0);
    dp.assign(n + 1, 0);
    primes.clear();
    for (int i = 2; i <= n; i++) {
        if (minp[i] == 0) {
            minp[i] = i;
            dp[i] = 1;
            primes.push_back(i);
        }
        for (auto p : primes) {
            if (i * p > n) {
                break;
            }
            minp[i * p] = p;
            dp[i * p] = dp[i] + 1;
            if (p == minp[i]) {
                dp[i * p] = dp[i];
                break;
            }
        }
    }
}