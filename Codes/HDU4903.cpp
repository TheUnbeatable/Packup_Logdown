#include <cstdio>
#include <iostream>
#define ri register int
using namespace std;

typedef long long LL;
const int N = 15;
const LL mod = 1e9 + 7;

int n, k, l, dis[N];
LL ans, pw[400], C[N][N];

inline void pre() {
  for (int i = 0; i < N; i ++) {
    C[i][0] = 1;
    for (int j = 1; j <= i; j ++)
      C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % mod;
  }
}

inline void Dfs(int x, int tot, LL ways) {
  if (!ways) return;
  LL g = 1, h = 1;
  if (x == k) {
    LL tmp = l;
    for (ri i = 1; i <= tot; i ++) {
      h = h * (l - (x - dis[i])) % mod;
      g = g * (l - max(x - dis[i], 1) + 1) % mod;
      (tmp *= l - (k - dis[i]) + 1) %= mod;
    }
    (ways *= (g - h + mod)) %= mod;
    dis[++ tot] = k;
    for (ri i = tot + 1; i <= n; i ++) (ways *= tmp) %= mod;
    (ans += ways * pw[(n - tot) * (n - tot - 1) / 2]) %= mod;
    return;
  }
  for (ri i = 1; i <= tot; i ++) {
    h = h * (l - (x - dis[i])) % mod;
    g = g * (l - max(x - dis[i], 1) + 1) % mod;
  }
  g = (g - h + mod) % mod;
  int tmp = n - tot - 1;
  for (ri p = 0; tot < n; dis[++ tot] = x, p ++) {
    Dfs(x + 1, tot, ways * C[tmp][p] % mod * pw[p * (p - 1) / 2] % mod);
    ways = ways * g % mod;
  }
}

int main() {
  int T;
  pre();
  scanf("%d", &T);
  while (T --) {
    scanf("%d%d%d", &n, &k, &l);
    if (n == 1) {
      if (l == 0) puts("1");
      else puts("0");
      continue;
    }
    if (k > l) { puts("0"); continue; }
    for (ri i = pw[0] = 1; i <= 300; i ++)
      pw[i] = pw[i - 1] * l % mod;
    ans = 0;
    Dfs(1, 1, 1);
    printf("%lld\n", ans);
  }
  return 0;
}
