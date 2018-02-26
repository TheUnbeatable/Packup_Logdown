#include <cstdio>
#include <iostream>
#include <cstring>
#define mid ((L + R) >> 1)
#define travel(x, i) for (int i = fir[x]; i; i = e[i].nxt)
using namespace std;

namespace fastIO {
#define buf_size 2000000
  bool error;
  inline char gc() {
    static char buf[buf_size + 1], *l = buf, *r = buf;
    if (l == r) {
      l = buf;
      r = buf + fread(buf, 1, buf_size, stdin);
      if (l == r) {error = 1; return -1;}
    }
    return *l ++;
  }
  inline void read(int &x) {
    char ch; x = 0;
    while (!isdigit(ch = gc()) && !error);
    if (error) return;
    do {x = (x << 1) + (x << 3) + ch - '0';} while (isdigit(ch = gc()) && !error);
  }
  inline void write(int x) {
    if (!x) { putchar('0'); return; }
    if (x < 0) { x = - x; putchar('-'); }
    static int buf[23];
    register short len = 0;
    for (; x; x /= 10) buf[++ len] = x % 10;
    for (; len; len --) putchar(buf[len] + '0');
  }
#undef buf_size
}
using namespace fastIO;

const int N = 1e5 + 50;
const int M = 1e7 + 5e6;

struct edge {
  int nxt, to;
} e[N];
int fir[N], cnt = 0, col[N], n;
int Mrt[N], Srt[N], dep[N], crt;
int lc[M], rc[M], key[M], Size;

inline void addedge(int x, int y) {
  e[++ cnt] = (edge) { fir[x], y }; fir[x] = cnt;
}

inline int newnode() {
  lc[Size] = rc[Size] = key[Size] = 0; return Size ++;
}

inline int newnode(int x) {
  lc[Size] = lc[x]; rc[Size] = rc[x]; key[Size] = key[x]; return Size ++;
}

inline int SAdd(int x, int p, int v, int L = 1, int R = n) {
  int t = newnode();
  if (L == R) {
    key[t] = key[x] + v; return t;
  }
  if (p <= mid) {
    rc[t] = rc[x]; lc[t] = SAdd(lc[x], p, v, L, mid);
  }
  else {
    lc[t] = lc[x]; rc[t] = SAdd(rc[x], p, v, mid + 1, R);
  }
  key[t] = key[lc[t]] + key[rc[t]];
  return t;
}

inline int MChg(int p, int v, int L = 1, int R = n) {
  int t = newnode();
  if (L == R) { key[t] = v; return t; }
  if (p <= mid) lc[t] = MChg(p, v, L, mid);
  else rc[t] = MChg(p, v, mid + 1, R);
  return t;
}

inline int SMerge(int x, int y, int L = 1, int R = n) {
  if (!x && !y) return 0;
  if (!x || !y) return newnode(x + y);
  if (L == R) {
    int t = newnode();
    key[t] = key[x] + key[y];
    return t;
  }
  int t = Size ++;
  lc[t] = SMerge(lc[x], lc[y], L, mid);
  rc[t] = SMerge(rc[x], rc[y], mid + 1, R);
  key[t] = key[lc[t]] + key[rc[t]];
  return t;
}

inline int MMerge(int x, int y, int L = 1, int R = n) {
  if (!x || !y) return x + y;
  if (L == R) {
    crt = SAdd(crt, max(key[x], key[y]), -1);
    key[x] = min(key[x], key[y]);
    return x;
  }
  lc[x] = MMerge(lc[x], lc[y], L, mid);
  rc[x] = MMerge(rc[x], rc[y], mid + 1, R);
  return x;
}

inline void Dfs(int x = 1, int d = 1) {
  dep[x] = d;
  travel(x, i) Dfs(e[i].to, d + 1);
  crt = SAdd(0, d, 1);
  Mrt[x] = MChg(col[x], d);
  travel(x, i) {
    crt = SMerge(crt, Srt[e[i].to]);
    Mrt[x] = MMerge(Mrt[x], Mrt[e[i].to]);
  }
  Srt[x] = crt;
}

inline int QS(int x, int d, int L = 1, int R = n) {
  if (R <= d) return key[x];
  if (d <= mid) return QS(lc[x], d, L, mid);
  else return QS(lc[x], d, L, mid) + QS(rc[x], d, mid + 1, R);
}

inline void solve() {
  int m;
  read(n); read(m);
  memset(fir, 0, sizeof (int) * (n + 2));
  cnt = Size = 1;
  for (int i = 1; i <= n; i ++) read(col[i]);
  for (int i = 2, f; i <= n; i ++) {
    read(f); addedge(f, i);
  }
  Dfs();
  for (int x, d, ans = 0; m; m --) {
    read(x); read(d);
    x ^= ans; d ^= ans;
    ans = QS(Srt[x], d + dep[x]);
    write(ans); putchar('\n');
  }
}

int main() {
  int T;
  for (scanf("%d", &T); T; T --) solve();
  return 0;
}
