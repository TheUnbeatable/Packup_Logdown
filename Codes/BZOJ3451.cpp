#include <cstdio>
#include <iostream>
#include <cmath>
#include <algorithm>
#define ri register int
#define travel(x, i) for (int i = fir[x]; i; i = e[i].nxt)
using namespace std;

typedef long long LL;

namespace Poly {
  const int N = 16;
  const double PI = acos(-1);
  struct cd {
    double x, y;
    cd() {}
    cd(double _x, double _y) : x(_x), y(_y) {}
    inline cd operator + (const cd &o) const { return cd(x + o.x, y + o.y); }
    inline cd operator - (const cd &o) const { return cd(x - o.x, y - o.y); }
    inline cd operator * (const cd &o) const {
      return cd(x * o.x - y * o.y, x * o.y + y * o.x);
    }
  } w[N + 1][(1 << N) + 1];
  int swp[N + 1][(1 << N) + 1];
  
  inline void pre() {
    for (ri i = 1, l = 2; i <= N; i ++, l <<= 1) {
      int *Swp = swp[i]; cd *W = w[i];
      for (ri j = 0; j < l; j ++) {
        if (j & 1) Swp[j] = Swp[j - 1] | (l >> 1);
        else Swp[j] = Swp[j >> 1] >> 1;
        W[j] = cd( cos(2 * PI * j / l), sin(2 * PI * j / l) );
      }
    }
  }
  inline void DFT(cd *a, int len, int h) {
    for (ri i = 0; i < len; i ++)
      if (swp[h][i] < i) swap(a[i], a[swp[h][i]]);
    cd x, y, *W = w[h];
    for (ri i = 2, ii = 1; i <= len; i <<= 1, ii <<= 1)
      for (ri j = 0; j < len; j += i)
        for (ri k = j; k < j + ii; k ++) {
          x = a[k]; y = W[(k - j) * (len / i)] * a[k + ii];
          a[k] = x + y; a[k + ii] = x - y;
        }
  }
}

const int N = 3e4 + 50;

struct edge {
  int nxt, to;
} e[N << 1];
bool vis[N];
int fir[N], sz[N];
int rt, best, tot;
int tmp[N], sum[N];
LL ans[N];

inline void calc(int *b, int n, int val) {
  if (n <= 300) {
    for (ri i = 1; i <= n; i ++)
      for (ri j = 1; j <= n; j ++)
        ans[i + j] += val * b[i] * b[j];
    return;
  }
  static Poly :: cd a[100000];
  int len = 1, t = 0;
  while (len <= n + 1) len <<= 1, t ++;
  len <<= 2; t += 2;
  for (ri i = 0; i <= n; i ++) a[i] = Poly :: cd((double)b[i], 0.0);
  for (ri i = n + 1; i < len; i ++) a[i] = Poly :: cd(0.0, 0.0);
  Poly :: DFT(a, len, t);
  for (ri i = 0; i < len; i ++) a[i] = a[i] * a[i];
  reverse(a + 1, a + len);
  Poly :: DFT(a, len, t);
  for (ri i = 1; i <= n + n; i ++)
    ans[i] += val * (LL)(a[i].x / len + 0.5);
}

inline void addedge(int x, int y) {
  static int cnt = 0;
  e[++ cnt] = (edge) { fir[x], y }; fir[x] = cnt;
}

inline void clr(int *a) {
  for (ri i = 1; a[i]; i ++) a[i] = 0;
}

inline void Dfs1(int x, int f) {
  sz[x] = 1;
  int Max = 0;
  travel(x, i)
    if (!vis[e[i].to] && e[i].to != f) {
      Dfs1(e[i].to, x);
      sz[x] += sz[e[i].to];
      Max = max(Max, sz[e[i].to]);
    }
  Max = max(Max, tot - sz[x]);
  if (Max < best) {
    rt = x;
    best = Max;
  }
}

inline int Dfs2(int x, int f, int dep) {
  tmp[dep] ++; sum[dep] ++; ans[dep] += 2;
  int r = dep;
  travel(x, i)
    if (e[i].to != f && !vis[e[i].to])
      r = max(r, Dfs2(e[i].to, x, dep + 1));
  return r;
}

inline void solve(int x, int totsz) {
  tot = totsz; best = 1e9;
  Dfs1(x, 0); vis[x = rt] = 1;
  clr(sum);
  int r = 0, c;
  travel(x, i)
    if (!vis[e[i].to]) {
      if (sz[e[i].to] > sz[x])
        sz[e[i].to] = totsz - sz[x];
      clr(tmp);
      r = max(r, c = Dfs2(e[i].to, 0, 1));
      calc(tmp, c, -1);
    }
  calc(sum, r, 1);
  travel(x, i)
    if (!vis[e[i].to])
      solve(e[i].to, sz[e[i].to]);  
}

int main() {
  int n;
  Poly :: pre();
  scanf("%d", &n);
  for (int i = 1, x, y; i < n; i ++) {
    scanf("%d%d", &x, &y);
    x ++; y ++;
    addedge(x, y); addedge(y, x);
  }
  solve(1, n);
  long double res = 0;
  for (int i = 1; i <= n; i ++)
    res += 1.0 * ans[i] / (i + 1);
  printf("%.4Lf\n", res + n);
  return 0;
}
