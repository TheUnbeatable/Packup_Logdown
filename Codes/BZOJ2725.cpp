#include <cstdio>
#include <iostream>
#include <cstring>
#include <queue>
#define mp make_pair
using namespace std;

namespace fastIO {
#define buf_size 1000000
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
  template <typename T> inline void read(T &x) {
    char ch; x = 0;
    while (!isdigit(ch = gc()) && !error);
    if (error) return;
    do { x = (x << 1) + (x << 3) + ch - '0'; }
    while (isdigit(ch = gc()) && !error);
  }
  template <typename T> inline void writeln(T x) {
    if (!x) { puts("0"); return; }
    static int buf[23];
    register short len = 0;
    for (; x; x /= 10) buf[++ len] = x % 10;
    for (; len; len --) putchar(buf[len] + '0');
    puts("");
  }
#undef buf_size
}
using namespace fastIO;

typedef long long LL;
const int N = 2e5 + 5;
const LL INF = 1e18;

template <typename T> inline T abs(T x) { return x > 0 ? x : - x; }

namespace Heap {
  pair <LL, int> v[N << 1];
  int lc[N << 1], rc[N << 1], dep[N << 1], sz, rt, Size;
  int sta[N << 1], tp;
  
  inline void clear() { Size = tp = rt = sz = 0; }
  inline int mer(int x, int y) {
    if (!x || !y) return x + y;
    if (v[x] > v[y]) swap(x, y);
    rc[x] = mer(rc[x], y);
    if (dep[lc[x]] < dep[rc[x]]) swap(lc[x], rc[x]);
    dep[x] = dep[rc[x]] + 1;
    return x;
  }
  inline void push(pair <LL, int> val) {
    static int x;
    if (tp) x = sta[tp --];
    else x = ++ sz;
    v[x] = val;
    lc[x] = rc[x] = dep[x] = 0;
    rt = mer(rt, x);
    Size ++;
  }
  inline int top() { return v[rt].second; }
  inline void pop() { sta[++ tp] = rt; rt = mer(lc[rt], rc[rt]); Size --; }
}

namespace SegTree {
#define mid ((L + R) >> 1)
  int n;
  LL key[N << 2], val[N];
  
  inline void Init(int x = 1, int L = 1, int R = n) {
    key[x] = INF;
    if (L != R) {
      Init(x << 1, L, mid);
      Init(x << 1 | 1, mid + 1, R);
    }
  }
  inline void Upd(int l, int r, LL v, int x = 1, int L = 1, int R = n) {
    if (v > key[x]) return;
    if (l <= L && R <= r) {
      key[x] = v;
      return;
    }
    if (l <= mid) Upd(l, r, v, x << 1, L, mid);
    if (r > mid) Upd(l, r, v, x << 1 | 1, mid + 1, R);
  }
  inline void Dfs(int x = 1, int L = 1, int R = n) {
    if (L == R) {
      val[L] = key[x];
      return;
    }
    if (key[x]) {
      key[x << 1] = min(key[x << 1], key[x]);
      key[x << 1 | 1] = min(key[x << 1 | 1], key[x]);
    }
    Dfs(x << 1, L, mid);
    Dfs(x << 1 | 1, mid + 1, R);
  }
#undef mid
}

namespace Graph {
  struct edge {
    int nxt, to, len;
  } e[N << 1];
  bool vis[N];
  int fir[N], pos[N], path[N], top, le[N], ri[N];
  LL dis[N], _dis[N];
  
  inline void addedge(int x, int y, int l) {
    static int cnt = 1;
    e[++ cnt] = (edge) { fir[x], y, l };
    fir[x] = cnt;
  }
  inline void Dijkstra(int s, int t) {
    memset(dis, 63, sizeof dis);
    Heap :: clear();
    Heap :: push(mp(dis[s] = 0, s));
    int x;
    while (Heap :: Size) {
      x = Heap :: top(); Heap :: pop();
      if (vis[x]) continue;
      vis[x] = 1;
      for (int i = fir[x]; i; i = e[i].nxt)
        if (!vis[e[i].to] && dis[e[i].to] > dis[x] + e[i].len)
          Heap :: push(mp(dis[e[i].to] = dis[x] + e[i].len, e[i].to));
    }
    memset(_dis, 63, sizeof _dis);
    memset(vis, 0, sizeof vis);
    Heap :: clear();
    Heap :: push(mp(_dis[t] = 0, t));
    while (Heap :: Size) {
      x = Heap :: top(); Heap :: pop();
      if (vis[x]) continue;
      vis[x] = 1;
      for (int i = fir[x]; i; i = e[i].nxt)
        if (!vis[e[i].to] && _dis[e[i].to] > _dis[x] + e[i].len)
          Heap :: push(mp(_dis[e[i].to] = _dis[x] + e[i].len, e[i].to));
    }
  }
  inline void Find_path(int s, int t) {
    int x = s; top = 0;
    while (x != t) {
      path[++ top] = x;
      pos[x] = top;
      for (int i = fir[x]; i; i = e[i].nxt)
        if (dis[x] + e[i].len == dis[e[i].to]
            && dis[e[i].to] + _dis[e[i].to] == dis[t]) {
          x = e[i].to;
          break;
        }
    }
    path[++ top] = t;
    pos[t] = top;
  }
  inline void firstpos(int s, int *ps, LL *dis, int id) {
    queue <int> Q;
    Q.push(s); ps[s] = id;
    int x;
    while (!Q.empty()) {
      x = Q.front(); Q.pop();
      for (int i = fir[x]; i; i = e[i].nxt) {
        if (!pos[e[i].to] && !ps[e[i].to] && dis[x] + e[i].len == dis[e[i].to]) {
          ps[e[i].to] = id;
          Q.push(e[i].to);
        }
      }
    }
  }
  inline void solve(int m) {
    for (int i = 1; i <= top; i ++)
      firstpos(path[i], le, dis, i);
    for (int i = top; i >= 1; i --)
      firstpos(path[i], ri, _dis, i);
    m <<= 1;
    SegTree :: n = top;
    SegTree :: Init();
    for (int i = 2, u, v; i <= m; i += 2) {
      u = e[i].to;
      v = e[i + 1].to;
      if (pos[u] && pos[v] && abs(pos[u] - pos[v]) == 1) continue;
      if (le[u] < ri[v])
        SegTree :: Upd(le[u], ri[v] - 1, dis[u] + _dis[v] + e[i].len);
      swap(u, v);
      if (le[u] < ri[v])
        SegTree :: Upd(le[u], ri[v] - 1, dis[u] + _dis[v] + e[i].len);
    }
    SegTree :: Dfs();
  }
}
using namespace Graph;

int main() {
  int n, m, s, t, q;
  read(n); read(m);
  for (int i = 1, x, y, l; i <= m; i ++) {
    read(x); read(y); read(l);
    addedge(x, y, l);
    addedge(y, x, l);
  }
  read(s); read(t);
  Dijkstra(s, t);
  Find_path(s, t);
  solve(m);
  read(q);
  LL ans;
  for (int x, y; q; q --) {
    read(x); read(y); 
    if (dis[t] == dis[0]) puts("Infinity");
    else {
      if (!pos[x] || !pos[y] || abs(pos[x] - pos[y]) != 1) writeln(dis[t]);
      else {
        ans = SegTree :: val[min(pos[x], pos[y])];
        if (ans == INF) puts("Infinity");
        else writeln(ans);
      }
    }
  }
  return 0;
}
