#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#define ri register int
using namespace std;

struct edge {
  int nxt, to, len;
} e[50000];
int fir[1000], cnt = 0;
int n, m, p;

inline void addedge(int x, int y, int l) {
  e[++ cnt] = (edge) { fir[x], y, l };
  fir[x] = cnt;
}

inline void Calc(int F, int G) {
  static int g[10];
  int T = 0, P = F;
  g[0] = 0;
  for (ri i = 1; i < m; i ++, F /= 3)
    g[i] = g[i - 1] + F % 3 - 1;
  for (ri i = 0; i < m; i ++)
    g[i] += (G >> i) & 1;
  for (ri i = 1; i <= 30; i ++)
    for (ri j = 0; j < m; j ++) {
      if (j) g[j] = min(g[j], g[j - 1] + ((G >> j) & 1));
      if (j < m - 1) g[j] = min(g[j], g[j + 1] + ((G >> j) & 1));
    }
  for (ri i = m - 1; i; i --) T = T * 3 + (g[i] - g[i - 1] + 1);
  addedge(T, P, g[0]);
}

inline void mod(int &x, int y) { if ((x += y) >= p) x -= p; }

int dp[103][103][250], ans[110];

int main() {
  scanf("%d%d%d", &n, &m, &p);
  swap(n, m);
  int t = 1;
  for (ri i = 1; i < m; i ++) t *= 3;
  for (ri i = 0; i < t; i ++)
    for (ri j = (1 << m) - 1; j >= 0; j --) Calc(i, j);
  dp[0][0][t - 1] = 1;
  for (ri i = 1; i <= n; i ++)
    for (ri j = 0; j <= i; j ++)
      for (ri k = 0; k < t; k ++)
        for (ri x = fir[k]; x; x = e[x].nxt) {
          if (j >= e[x].len) mod(dp[i][j][k], dp[i - 1][j - e[x].len][e[x].to]);
        }
  for (ri j = 0, l, v; j <= n; j ++)
    for (ri k = 0; k < t; k ++)
      if (dp[n][j][k]) {
        l = k; v = j;
        for (ri i = 1; i < m; i ++, l /= 3) v += l % 3 - 1;
        if (v >= 0) mod(ans[v], dp[n][j][k]);
      }
  for (int i = 0; i < n + m; i ++) printf("%d\n", ans[i]);
  return 0;
}
