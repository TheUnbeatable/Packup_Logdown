#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <queue>
using namespace std;

const int N = 51;
const int M = 151;
const int K = 21;

struct E {
  int u, v, l;
  inline bool operator < (const E &o) const { return l < o.l; }
} a[M];
struct edge {
  int nxt, to, len;
} e[M << 1];
struct node {
  int a, b, c, d;
  node() {};
  node(int _a, int _b, int _c, int _d) : a(_a), b(_b), c(_c), d(_d) {}
  inline bool operator < (const node &o) const { return d > o.d; }
};
int fir[N], cnt = 1;
int n, m, k;
int dis[N][M][K], vis[N][M][K], C, ans = 1e9;

inline void addedge(int x, int y, int l) {
  e[++ cnt] = (edge) { fir[x], y, l }; fir[x] = cnt;
}

inline void Dijkstra(int R, int sum) {
  C ++;
  priority_queue <node> Q;
  memset(dis, 63, sizeof dis);
  dis[1][0][0] = 0;
  Q.push(node( 1, 0, 0, 0 ));
  node c, d;
  while (!Q.empty()) {
    c = Q.top(); Q.pop();
    if (vis[c.a][c.b][c.c] == C) continue;
    vis[c.a][c.b][c.c] = C;
    if (c.d + sum >= ans) continue;
    if (c.a == n) {
      ans = min(ans, c.d + sum);
      continue;
    }
    for (int i = fir[c.a], v; i; i = e[i].nxt) {
      v = e[i].to;
      if (c.b < R && c.c < k && dis[v][c.b + 1][c.c + 1] > c.d) {
        dis[v][c.b + 1][c.c + 1] = c.d;
        Q.push(node( v, c.b + 1, c.c + 1, c.d ));
      }
      if (c.b + ((i >> 1) <= R) > R) continue;
      d = node( v, c.b + ((i >> 1) <= R), c.c, c.d + ((i >> 1) <= R ? 0 : e[i].len) );
      if (dis[d.a][d.b][d.c] > d.d) {
        dis[d.a][d.b][d.c] = d.d; Q.push(d);
      }
    }
  }
}

int main() {
  scanf("%d%d%d", &n, &m, &k);
  for (int i = 1; i <= m; i ++)
    scanf("%d%d%d", &a[i].u, &a[i].v, &a[i].l);
  sort(a + 1, a + m + 1);
  for (int i = 1; i <= m; i ++) {
    addedge(a[i].u, a[i].v, a[i].l);
    addedge(a[i].v, a[i].u, a[i].l);
  }
  for (int i = 1, s = 0; i <= m; i ++) {
    s += a[i].l;
    Dijkstra(i, s);
  }
  printf("%d\n", ans);
  return 0;
}
