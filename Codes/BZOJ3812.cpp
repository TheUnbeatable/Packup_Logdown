#include <cstdio>
#include <iostream>
#define ri register int
using namespace std;

typedef long long LL;
const int N = 15;
const int M = (1 << N) + 1;
const LL mod = 1e9 + 7;

int n, m, G[N], tot;
int cnt[M], G1[33][M], G2[33][M], G3[33][M];
LL f[M], o[M], e[M], pw[400];

inline void pre() {
  for (ri i = 1; i < tot; i ++)
    cnt[i] = cnt[i - (i & (- i))] + 1;
  for (ri i = pw[0] = 1; i <= 300; i ++)
    pw[i] = (pw[i - 1] << 1) % mod;
  for (ri i = 1, k; i < 32; i ++) {
    for (k = 0; ((1 << k) & i) == 0; k ++);
    for (ri j = 0; j < tot; j ++)
      G1[i][j] = G1[i - (1 << k)][j] + cnt[G[k] & j];
  }
  for (ri i = 1, k; i < 32; i ++) {
    for (k = 0; ((1 << k) & i) == 0; k ++);
    for (ri j = 0; j < tot; j ++)
      G2[i][j] = G2[i - (1 << k)][j] + cnt[G[k + 5] & j];
  }
  for (ri i = 1, k; i < 32; i ++) {
    for (k = 0; ((1 << k) & i) == 0; k ++);
    for (ri j = 0; j < tot; j ++)
      G3[i][j] = G3[i - (1 << k)][j] + cnt[G[k + 10] & j];
  }
}

inline int count(int a, int b) {
  return G1[a & 31][b] + G2[(a >> 5) & 31][b] + G3[(a >> 10) & 31][b];
}

int main() {
  scanf("%d%d", &n, &m);
  tot = 1 << n;
  for (ri i = 1, x, y; i <= m; i ++) {
    scanf("%d%d", &x, &y);
    x --; y --;
    G[x] |= 1 << y;
  }
  pre();
  for (ri i = 1, t; i < tot; i ++) {
    t = i - (i & (- i));
    f[i] = pw[count(i, i)];
    for (ri j = i; j; j = (j - 1) & i)
      (f[i] -= (o[j] - e[j]) * pw[count(i ^ j, i)]) %= mod;
    if (f[i] < 0) f[i] += mod;
    for (ri j = t; j; j = (j - 1) & t) {
      (o[i] += e[j] * f[i ^ j]) %= mod;
      (e[i] += o[j] * f[i ^ j]) %= mod;
    }
    (f[i] -= (o[i] - e[i])) %= mod;
    (o[i] += f[i]) %= mod;
  }
  printf("%lld\n", f[tot - 1]);
  return 0;
}
