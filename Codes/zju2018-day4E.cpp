#pragma GCC optimize(3)
#include <bits/stdc++.h>
using namespace std;

typedef long long LL;
typedef pair <int, int> pii;

namespace S {
#define mid ((L + R) >> 1)
  const int N = 1e7 + 2e6;
  int lc[N], rc[N], Sz;
  int sz[N];
  LL sum[N];
  inline int nnd() {
    Sz ++;
    lc[Sz] = rc[Sz] = sz[Sz] = sum[Sz] = 0;
    return Sz;
  }
  inline void up(int x) {
    sz[x] = sz[lc[x]] + sz[rc[x]];
    sum[x] = sum[lc[x]] + sum[rc[x]];
  }
  inline pii split(int x, int v, int L = 0, int R = 32767) {
    if (!v || !x) return pii(0, x);
    if (v >= sz[x]) return pii(x, 0);
    int y = nnd();
    if (L == R) {
      sz[y] = sz[x] - v;
      sum[y] = 1ll * L * sz[y];
      sz[x] = v;
      sum[x] = 1ll * L * sz[x];
      return pii(x, y);
    }
    if (v <= sz[lc[x]]) {
      pii p = split(lc[x], v, L, mid);
      lc[x] = p.first;
      lc[y] = p.second;
      rc[y] = rc[x];
      rc[x] = 0;
    } else {
      pii p = split(rc[x], v - sz[lc[x]], mid + 1, R);
      rc[x] = p.first;
      rc[y] = p.second;
    }
    up(x); up(y);
    return pii(x, y);
  }
  inline int mer(int x, int y) {
    if (!x || !y) return x + y;
    sz[x] += sz[y];
    sum[x] += sum[y];
    lc[x] = mer(lc[x], lc[y]);
    rc[x] = mer(rc[x], rc[y]);
    return x;
  }
  inline int mk(int v, int L = 0, int R = 32767) {
    int x = nnd();
    sz[x] = 1;
    sum[x] = v;
    if (L == R) return x;
    if (v <= mid) lc[x] = mk(v, L, mid);
    else rc[x] = mk(v, mid + 1, R);
    return x;
  }
#undef mid
}

namespace T {
  const int N = 5e5 + 50;
  LL val[N], sum[N];
  int sz[N], wei[N];
  int rnd[N], lc[N], rc[N], id[N], Sz;

  inline int nnd(int i) {
    if (!i) return 0;
    Sz ++;
    lc[Sz] = rc[Sz] = 0;
    val[Sz] = sum[Sz] = S :: sum[i];
    sz[Sz] = wei[Sz] = S :: sz[i];
    id[Sz] = i;
    return Sz;
  }
  inline void up(int x) {
    sz[x] = sz[lc[x]] + sz[rc[x]] + wei[x];
    sum[x] = val[x] + sum[lc[x]] + sum[rc[x]];
  }
  inline int mer(int x, int y) {
    if (!x || !y) return x + y;
    if (rnd[x] > rnd[y]) {
      rc[x] = mer(rc[x], y);
      up(x); return x;
    } else {
      lc[y] = mer(x, lc[y]);
      up(y); return y;
    }
  }
  inline pii split(int x, int v) {
    if (!v || !x) return pii(0, x);
    if (v >= sz[x]) return pii(x, 0);
    if (sz[lc[x]] >= v) {
      pii tmp = split(lc[x], v);
      lc[x] = tmp.second;
      up(x); return pii(tmp.first, x);
    }
    if (sz[lc[x]] + wei[x] >= v) {
      int t = rc[x]; rc[x] = 0;
      up(x); return pii(x, t);
    }
    pii tmp = split(rc[x], v - sz[lc[x]] - wei[x]);
    rc[x] = tmp.first;
    up(x); return pii(x, tmp.second);
  }
  inline int Max(int p) {
    while (rc[p]) p = rc[p];
    return p;
  }
  inline pii accurate(int x, int v) {
    if (!v) return pii(0, x);
    if (v >= sz[x]) return pii(x, 0);
    pii p = split(x, v);
    int u = Max(p.first);
    pii q = split(p.first, sz[p.first] - wei[u]);
    pii r = S :: split(id[u], v - sz[q.first]);
    int c = mer( q.first, nnd( r.first ) );
    int d = mer( nnd( r.second ), p.second );
    return pii(c, d);
  }
  int Rt;
  inline void Dfs(int x) {
    if (!x) return;
    Rt = S :: mer(Rt, id[x]);
    Dfs(lc[x]); Dfs(rc[x]);
  }
  inline int condense(int x) {
    Rt = 0; Dfs(x); Sz = 0; return nnd(Rt);
  }
}

int n;

using S :: mk;
using T :: nnd;
using T :: mer;
using T :: accurate;

inline void solve() {
  T :: Sz = 0;
  S :: Sz = 0;
  int rt = 0;
  for (int i = 1, v; i <= n; i ++) {
    scanf("%d", &v);
    rt = mer( rt, nnd(mk(v)) );
  }
  int q, a, b, t;
  scanf("%d", &q);
  while (q --) {
    scanf("%d", &t);
    if (t < 4) scanf("%d%d", &a, &b);
    if (t == 1) {
      pii p = accurate(rt, a - 1);
      rt = mer( p.first, nnd(mk(b)) );
      rt = mer( rt, p.second );
    } else if (t == 2) {
      pii q = accurate(rt, b);
      pii p = accurate(q.first, a - 1);
      printf("%lld\n", T :: sum[p.second]);
      rt = mer(p.first, p.second);
      rt = mer(rt, q.second);
    } else if (t == 3) {
      pii q = accurate(rt, a);
      pii p = accurate(q.first, a - 1);
      rt = mer( p.first, nnd(mk(b)) );
      rt = mer( rt, q.second );
    } else rt = T :: condense(rt);
  }
}

int main() {
  srand(20011227);
  for (int i = 0; i < T :: N; i ++)
    T :: rnd[i] = rand();
  while (scanf("%d", &n) != EOF) solve();
  return 0;
}
