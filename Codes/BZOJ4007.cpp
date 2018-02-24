#include <cstdio>
#include <iostream>
#include <vector>
using namespace std;

const int N = 10;

int n, m, t, p = -1;
int w[1 << (N - 1)][N], f[1 << (N - 1)][N];
vector <int> dp[1 << N][1 << N];

inline void mer(int x, int sz, int g) {
  for (int i = 0; i < g; i ++) {
    dp[x][i].resize(sz + 3);
    for (int t = 0; t <= sz; t ++)
      for (int j = max(0, t - (sz >> 1)); j <= (sz >> 1) && j <= t; j ++)
        dp[x][i][t] = max(dp[x][i][t], max(dp[x << 1][i][j] + dp[x << 1 | 1][i][t - j], dp[x << 1][i + g][j] + dp[x << 1 | 1][i + g][t - j]));
  }
}

inline void Dfs(int x, int sz, int g) {
  if (sz != 1) {
    Dfs(x << 1, sz >> 1, g << 1);
    Dfs(x << 1 | 1, sz >> 1, g << 1);
    mer(x, sz, g);
  }
  else {
    p ++;
    for (int j = 0; j < g; j ++) {
      dp[x][j].resize(2);
      for (int i = 0; i <= n - 2; i ++) {
        dp[x][j][0] += f[p][i] * (!((j >> i) & 1));
        dp[x][j][1] += w[p][i] * ((j >> i) & 1);
      }
    }
  }
}

int main() {
  scanf("%d%d", &n, &m);
  t = 1 << (n - 1);
  for (int i = 0; i < t; i ++)
    for (int j = n - 2; j >= 0; j --) scanf("%d", &w[i][j]);
  for (int i = 0; i < t; i ++)
    for (int j = n - 2; j >= 0; j --) scanf("%d", &f[i][j]);
  Dfs(1, t, 1);
  int ans = 0;
  for (int i = 0; i <= m; i ++) ans = max(ans, dp[1][0][i]);
  printf("%d\n", ans);
  return 0;
}
