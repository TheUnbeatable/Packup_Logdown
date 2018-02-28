#include <cstdio>
#include <iostream>
#include <cstring>
#define ri register int
#define mid ((L + R) >> 1)
using namespace std;

typedef long long LL;
const int N = 305;

LL ans = 0;
int n, G[N][N];

inline void solve(int L, int R) {
  if (L == R) {
    for (ri i = 1; i <= n; i ++)
      for (ri j = 1; j <= n; j ++)
        if (i != j && i != L && j != L) ans += G[i][j];
    return;
  }
  int H[N][N];
  memcpy(H, G, sizeof G);
  for (ri k = mid + 1; k <= R; k ++)
    for (ri i = 1; i <= n; i ++)
      for (ri j = 1; j <= n; j ++)
        if (G[i][k] != -1 && G[k][j] != -1) {
          if (G[i][j] == -1) G[i][j] = G[i][k] + G[k][j];
          else G[i][j] = min(G[i][j], G[i][k] + G[k][j]);
        }
  solve(L, mid);
  memcpy(G, H, sizeof H);
  for (ri k = L; k <= mid; k ++)
    for (ri i = 1; i <= n; i ++)
      for (ri j = 1; j <= n; j ++)
        if (G[i][k] != -1 && G[k][j] != -1) {
          if (G[i][j] == -1) G[i][j] = G[i][k] + G[k][j];
          else G[i][j] = min(G[i][j], G[i][k] + G[k][j]);
        }
  solve(mid + 1, R);
}

int main() {
  scanf("%d", &n);
  for (int i = 1; i <= n; i ++)
    for (int j = 1; j <= n; j ++) scanf("%d", &G[i][j]);
  solve(1, n);
  printf("%lld\n", ans);
  return 0;
}
