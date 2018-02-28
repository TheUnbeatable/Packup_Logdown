#include <cstdio>
#include <iostream>
#include <queue>
#include <cstring>
#define travel(x, i) for (int i = fir[x]; i; i = e[i].nxt)
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
  inline void read(int &x) {
    char ch; x = 0;
    while (!isdigit(ch = gc()) && !error);
    if (error) return;
    do {x = (x << 1) + (x << 3) + ch - '0';} while (isdigit(ch = gc()) && !error);
  }
  inline void writeln(int x) {
    if (!x) { puts("0"); return; }
    if (x < 0) { x = -x; putchar('-'); }
    static int buf[23];
    register short len = 0;
    for (; x; x /= 10) buf[++ len] = x % 10;
    for (; len; len --) putchar(buf[len] + '0');
    puts("");
  }
#undef buf_size
}
using namespace fastIO;

typedef pair <int, int> pii;
const int N = 1e5 + 5;
const int M = 2e5 + 5;
const int K = M * 3;

struct edge {
  int nxt, to, len;
} e[M << 1];
int fir[N], cnt = 0;
int n, m, ans[N];
int dep[N], dis[N], fa[N][18];
int add[N], sub[N], lc[K], rc[K], key[K], tag[K], d[K], Sz = 0;
bool vis[N];

inline void addedge(int x, int y, int l) {
  e[++ cnt] = (edge) { fir[x], y, l };
  fir[x] = cnt;
}

inline void Dijkstra() {
  priority_queue <pii, vector <pii>, greater <pii> > Q;
  memset(dis, 127, sizeof dis);
  Q.push(make_pair(dis[1] = 0, 1));
  int x;
  while (!Q.empty()) {
    x = Q.top().second; Q.pop();
    if (vis[x]) continue;
    vis[x] = 1;
    travel(x, i)
      if (!vis[e[i].to] && dis[x] + e[i].len < dis[e[i].to])
        Q.push(make_pair(dis[e[i].to] = dis[x] + e[i].len, e[i].to));
  }
}

inline void Dfs(int x, int f) {
  dep[x] = dep[f] + 1;
  fa[x][0] = f;
  for (int i = 1; i < 18; i ++)
    fa[x][i] = fa[fa[x][i - 1]][i - 1];
  travel(x, i)
    if (dis[x] + e[i].len == dis[e[i].to]) Dfs(e[i].to, x);
}

inline int __lca(int x, int y) {
  if (dep[x] < dep[y]) swap(x, y);
  for (int i = 17; dep[x] != dep[y]; i --)
    if (dep[fa[x][i]] >= dep[y]) x = fa[x][i];
  if (x == y) return x;
  for (int i = 17; fa[x][0] != fa[y][0]; i --)
    if (fa[x][i] != fa[y][i]) { x = fa[x][i]; y = fa[y][i]; }
  return fa[x][0];
}

inline void Down(int x) {
  if (tag[x]) {
    key[lc[x]] += tag[x];
    tag[lc[x]] += tag[x];
    key[rc[x]] += tag[x];
    tag[rc[x]] += tag[x];
    tag[x] = 0;
  }
}

inline int mer(int x, int y) {
  if (!x || !y) return x + y;
  if (key[x] > key[y]) swap(x, y);
  Down(x);
  rc[x] = mer(rc[x], y);
  if (d[rc[x]] > d[lc[x]]) swap(lc[x], rc[x]);
  d[x] = d[rc[x]] + 1;
  return x;
}

inline int node(int v) { key[++ Sz] = v; return Sz; }

inline void Solve(int x) {
  travel(x, i)
    if (dis[x] + e[i].len == dis[e[i].to]) {
      Solve(e[i].to);
      if (sub[e[i].to]) {
        tag[sub[e[i].to]] += e[i].len;
        key[sub[e[i].to]] += e[i].len;
        sub[x] = mer(sub[x], sub[e[i].to]);
      }
      if (add[e[i].to]) {
        tag[add[e[i].to]] += e[i].len;
        key[add[e[i].to]] += e[i].len;
        add[x] = mer(add[x], add[e[i].to]);
      }
    }
  while (add[x] && sub[x] && key[add[x]] == key[sub[x]]) {
    Down(add[x]); Down(sub[x]);
    add[x] = mer(lc[add[x]], rc[add[x]]);
    sub[x] = mer(lc[sub[x]], rc[sub[x]]);
  }
  if (add[x]) ans[x] = key[add[x]];
  else ans[x] = -1;
}

int main() {
  read(n); read(m);
  for (int i = 1, x, y, l; i <= m; i ++) {
    read(x); read(y); read(l);
    addedge(x, y, l);
    addedge(y, x, l);
  }
  Dijkstra();
  Dfs(1, 0);
  for (int i = 1, u, v, l, lca; i <= cnt; i += 2) {
    u = e[i].to; v = e[i + 1].to; l = e[i].len;
    if (dis[u] + l == dis[v] || dis[v] + l == dis[u]) continue;
    lca = __lca(u, v);
    if (u == lca || v == lca) {
      if (u == lca) swap(u, v);
      add[u] = mer(add[u], node(dis[v] + l));
      sub[v] = mer(sub[v], node(l + dis[u]));
    }
    else {
      add[u] = mer(add[u], node(dis[v] + l));
      add[v] = mer(add[v], node(dis[u] + l));
      sub[lca] = mer(sub[lca], node(dis[v] + dis[u] - dis[lca] + l));
      sub[lca] = mer(sub[lca], node(dis[v] + dis[u] - dis[lca] + l));
    }
  }
  travel(1, i)
    if (dis[1] + e[i].len == dis[e[i].to]) Solve(e[i].to);
  for (int i = 2; i <= n; i ++) writeln(ans[i]);
  return 0;
}
