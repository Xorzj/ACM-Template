ll seed = chrono::steady_clock::now().time_since_epoch().count();
mt19937_64 rnd(seed);
int Rnd(int l, int r)
{
    return rnd() % (r - l + 1) + l;
}