#### [Contest Link](http://codeforces.com/gym/101630)
## Archery Tournament
### Description
给N(n <= 2e5)个靶子,每个靶子恰好与地面相切,没有靶子重叠,询问向某点射箭命中的靶子编号并将该靶子删除,若没有输出-1.
坐标范围(-1e9~1e9).
### Idea
注意到由于靶子不相重叠,每一个横坐标上方覆盖的靶子数量是log级别的,只需要维护动态开点线段树,每个节点上维护覆盖该段的靶子编号,每次狂扫出解即可.
空间O(NlogN),时间O(Nlog^2N).
### Code
```cpp Archery Tournament
#include <cstdio>
#include <vector>
#define INF 1000000000
#define mid (L + ((R - L) >> 1))
#define sqr(x) (1LL * (x) * (x))
using namespace std;

typedef long long LL;
const int N = 2e5 + 5;
const int M = 5e6 + 5;

int x[N], y[N];
vector <int> vec[M];
int lc[M], rc[M], sz = 1;

inline bool Judge(int id, int xx, int yy) {
  return sqr(xx - x[id]) + sqr(yy - y[id]) < sqr(y[id]);
}

inline int Query(int xx, int yy, int x = 1, int L = - INF, int R = INF) {
  if (!x) return -1;
  for (vector <int> :: iterator it = vec[x].begin(); it != vec[x].end(); it ++)
    if (Judge(*it, xx, yy)) return *it;
  if (L == R) return -1;
  if (xx <= mid) return Query(xx, yy, lc[x], L, mid);
  else return Query(xx, yy, rc[x], mid + 1, R);
}

inline void Add(int id, int X = 1, int L = - INF, int R = INF) {
  if (x[id] - y[id] <= L && R <= x[id] + y[id]) {
    vec[X].push_back(id);
    return;
  }
  if (x[id] - y[id] <= mid) {
    if (!lc[X]) lc[X] = ++ sz;
    Add(id, lc[X], L, mid);
  }
  if (x[id] + y[id] > mid) {
    if (!rc[X]) rc[X] = ++ sz;
    Add(id, rc[X], mid + 1, R);
  }
}

inline void Del(int id, int X = 1, int L = - INF, int R = INF) {
  if (x[id] - y[id] <= L && R <= x[id] + y[id]) {
    for (vector <int> :: iterator it = vec[X].begin(); it != vec[X].end(); it ++)
      if ((*it) == id) {
        vec[X].erase(it);
        return;
      }
    return;
  }
  if (x[id] - y[id] <= mid) {
    if (!lc[X]) lc[X] = ++ sz;
    Del(id, lc[X], L, mid);
  }
  if (x[id] + y[id] > mid) {
    if (!rc[X]) rc[X] = ++ sz;
    Del(id, rc[X], mid + 1, R);
  }
}

int main() {
  int n, op, X, Y, t;
  scanf("%d", &n);
  for (int i = 1; i <= n; i ++) {
    scanf("%d", &op);
    if (op == 1) {
      scanf("%d%d", &x[i], &y[i]);
      Add(i);
    }
    else {
      scanf("%d%d", &X, &Y);
      printf("%d\n", t = Query(X, Y));
      if (t != -1) Del(t);
    }
  }
  return 0;
}
```

---
## Connections
### Description
给定一张n点m边的强连通图(m>=2n),让你选出其中的2n条边,使得所有点仍然在一个强连通分量内.
### Idea
任意从一个点开始Dfs,形成Dfs树,显然树边都是要选的.此外,每个点都选择它能到达的Dfn最小的点,将这条边保存下来.
由于预先给定的强连通图的性质,若只有返祖边,显然是Dfn最小的最优;
如果有横叉边,由于终点必然能够回到根节点,所以实际上任取一条就可以了.
这样一共选择了至多2n-1条边,所以是符合题意的构造.
### Code
```cpp Connections
#include <cstdio>
#include <iostream>
#include <cstring>
using namespace std;

typedef long long LL;
const int N = 5e4 + 5;
const int M = 1e5 + 5;

int fir[N], nxt[M], fro[M], to[M], dfn[N], clo, tot;
bool use[M];

inline void Dfs(int x) {
  dfn[x] = ++ clo;
  int co = 0;
  for (int i = fir[x]; i; i = nxt[i]) {
    if (!dfn[to[i]]) use[i] = 1, tot ++, Dfs(to[i]);
    else if (dfn[to[co]] > dfn[to[i]]) co = i;
  }
  if (co) use[co] = 1, tot ++;
}

inline void solve() {
  int n, m;
  scanf("%d%d", &n, &m);
  memset(fir, 0, sizeof (int) * (n + 2));
  memset(dfn, 0, sizeof (int) * (n + 2));
  memset(use, 0, sizeof (bool) * (m + 2));
  clo = tot = 0;
  dfn[0] = 100000000;
  for (int i = 1, x; i <= m; i ++) {
    scanf("%d%d", &x, &to[i]);
    nxt[i] = fir[x];
    fir[x] = i;
    fro[i] = x;
  }
  Dfs(1);
  for (int i = 1; i <= m; i ++)
    if (!use[i]) {
      if (tot < (n << 1)) tot ++;
      else printf("%d %d\n", fro[i], to[i]);
    }
}

int main() {
  int T;
  scanf("%d", &T);
  while (T --) solve();
}
```

---
## The Great Wall
### Description
有一个长度为n的区间,你可以选择两个长度为r(r < n)的区间.第i个区间被覆盖0次,贡献为Ai,2次为Bi,3次为Ci.(Ai < Bi < Ci)
求第k小的总贡献.
### Idea
http://theunbeatable.logdown.com/posts/4695220/learn-notes-find-some-methods-of-kth-optimal-solution
根据定理1,我们可以用二分答案的方法将其转化为计数问题.
我们可以分两种情况计数:
Case 1:两个区间不相交
维护一个数据结构,维护选择某区间后比不选择会增加多少.每次插入,查询.
Case 2:两个区间相交
这个麻烦一些,因为贡献同时与两个区间的选择有关.
假设我们选择(x < y), [x,x+r-1] 和 [y,y+r-1]
那么我们有5个区间:
[1,x-1] : A
[x,y-1] : B
[y,x+r-1] : C
[x+r,y+r-1] : B
[y+r,n] : A
虽然两个区间是不确定的,但是我们可以对于某一个x计算出与y无关的式子
```mathjax
f_x = \sum_{i=1}^{i \le x - 1}a+\sum_{i=x}^{i \le x+r-1}b
```
```mathjax
g_y = \sum_{i=1}^{i \le y - 1}c+\sum_{i=y}^{i \le y+r-1}d
```
```mathjax
\Rightarrow
a+c=0,b+c=B,b+d=C,d=B
```
```mathjax
\Rightarrow
a=C-2B,b=C-B,c=2B-C,d=B
```
用数据结构维护f,要支持插入,删除,查询.
我选择的是树状数组(离散化).
![Result](http://user-image.logdown.io/user/42873/blog/39448/post/4698478/p37JJj2S06eTIjDK7hvd_2017-12-30%2012-01-23%E5%B1%8F%E5%B9%95%E6%88%AA%E5%9B%BE.png)
### Code
```cpp The Great Wall
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstring>
#define lb(x) (x & (- x))
#define rep(i, x, y) for (int i = (x); i <= (y); i ++)
using namespace std;

typedef long long LL;
const int N = 3e4 + 5;

LL A[N], B[N], C[N], h[N], f[N], g[N];
int bit[N], idh[N], idf[N], n, r;
LL th[N], tf[N], k;

inline void Add(int p, int v) {
  for (; p < N; p += lb(p)) bit[p] += v;
}

inline int Sum(int p) {
  int r = 0;
  for (; p > 0; p -= lb(p)) r += bit[p];
  return r;
}

inline LL check(LL bd) {
  bd --;
  LL res = 0;
  int p;
  memset(bit, 0, sizeof bit);
  rep(i, 1, n - r + 1) {
    if (i > r) Add(idh[i - r], 1);
    p = upper_bound(th + 1, th + n - r + 2, bd - A[n] - h[i]) - th - 1;
    res += Sum(p);
  }
  memset(bit, 0, sizeof bit);
  rep(i, 1, n - r + 1) {
    if (i > r) Add(idf[i - r], -1);
    p = upper_bound(tf + 1, tf + n - r + 2, bd - g[i]) - tf - 1;
    res += Sum(p);
    Add(idf[i], 1);
  }
  return res;
}

int main() {
  scanf("%d%d%lld", &n, &r, &k);
  rep(i, 1, n) scanf("%lld", &A[i]), A[i] += A[i - 1];
  rep(i, 1, n) scanf("%lld", &B[i]), B[i] += B[i - 1];
  rep(i, 1, n) scanf("%lld", &C[i]), C[i] += C[i - 1];
  rep(i, 1, n - r + 1) {
    h[i] = (B[i + r - 1] - B[i - 1]) - (A[i + r - 1] - A[i - 1]);
    f[i] = C[i - 1] - (B[i - 1] << 1) + (C[i + r - 1] - C[i - 1]) - (B[i + r - 1] - B[i - 1]) + A[i - 1];
    g[i] = (B[i - 1] << 1) - C[i - 1] + B[i + r - 1] - B[i - 1] + A[n] - A[i + r - 1];
    th[i] = h[i];
    tf[i] = f[i];
  }
  sort(th + 1, th + n - r + 2);
  sort(tf + 1, tf + n - r + 2);
  rep(i, 1, n - r + 1) {
    idh[i] = lower_bound(th + 1, th + n - r + 2, h[i]) - th;
    idf[i] = lower_bound(tf + 1, tf + n - r + 2, f[i]) - tf;
  }
  LL L = 1, R = 3e10, ans = 0, mid;
  while (L <= R) {
    mid = (L + R) >> 1;
    if (check(mid) < k) ans = mid, L = mid + 1;
    else R = mid - 1;
  }
  printf("%lld\n", ans);
  return 0;
}
```

---
## Interactive Sort
### Description
有一个长为n(n <= 10000)的序列,现在系统将它随机打乱了,将奇数与偶数分开,你每次可以询问i,j,系统将返回第i个偶数与第j个奇数的关系,你可以询问300000次,让你复原出打乱后的原序列(序列保证用java内置随机程序打乱).
### Idea
Ghostly.
首先我们发现要得到一个奇数的值只需要暴力枚举所有偶数和它比较,统计比它小的个数.
然后我们在暴枚时,又可以判断出被枚举的偶数之间的大小关系,将枚举的偶数分成小于/大于该奇数两个部分.
我们可以一个个奇数处理,每次二分找出一个存在比它小的数的偶数的集合,这样就不需要浪费比较次数在更小的偶数上了.
然后把这个集合和它的后一个集合中的元素和这个奇数一个个比过去,顺便将这些偶数以奇数为基准再分成两个块了.
找集合的第K个么...在数据结构中标出首位置,用线段树树上二分O(Nlog^2N).
(树状数组更快:O(Nlog^3N)).
### Code
```cpp Interactive Sort
#include <cstdio>
#include <iostream>
#define ri register int
#define lb(x) (x & (- x))
using namespace std;

const int N = 10005;

int id[N], O[N], E[N];
int bit[N], arr[N], vis[N];
int n, t, tot;

inline void Add(int x) {
  arr[x] = 1; tot ++;
  for (; x <= t; x += lb(x)) bit[x] ++;
}

inline int Sum(int x) {
  int r = 0;
  for (; x > 0; x -= lb(x)) r += bit[x];
  return r;
}

inline int GetKth(int k) {
  int p = 0;
  for (ri i = 14; i >= 0; i --)
    if ((1 << i) + p <= t && Sum(p + (1 << i)) < k) p += 1 << i;
  return p + 1;
}

inline char Ask(int e, int o) {
  static char ch[4];
  printf("? %d %d\n", e, o);
  fflush(stdout);
  scanf("%s", ch);
  return ch[0];
}

inline void Split(int l, int r, int I) {
  static int tmp[N];
  int pl = l, pr = r;
  for (ri i = l; i <= r; i ++) {
    if (vis[i] == I) tmp[pl ++] = id[i];
    else tmp[pr --] = id[i];
  }
  for (ri i = l; i <= r; i ++) id[i] = tmp[i];
  if (pl != l && pr != r) Add(pr + 1);
}

int main() {
  scanf("%d", &n);
  t = n >> 1;
  for (int i = 1; i <= t; i ++) id[i] = i;
  Add(1);
  int x, u, v, tt, s;
  for (ri i = 1; i <= n - t; i ++) {
    x = 1;
    for (ri j = 14; j >= 0; j --)
      if ((1 << j) + x <= tot && Ask(id[GetKth((1 << j) + x)], i) == '<') x += 1 << j;
    s = v = u = GetKth(x); v --; tt = 0;
    arr[s] = 0;
    for (; u <= t && !arr[u]; u ++) {
      if (Ask(id[u], i) == '<') vis[u] = i, v ++;
      else tt ++;
    }
    arr[s] = 1;
    if (tt) Split(s, u - 1, i);
    else if (u <= t) {
      arr[s = u] = 0;
      for (; u <= t && !arr[u]; u ++)
        if (Ask(id[u], i) == '<') vis[u] = i, v ++;
      arr[s] = 1;
      Split(s, u - 1, i);
    }
    O[i] = v << 1 | 1;
  }
  for (int i = 1; i <= t; i ++) E[id[i]] = i << 1;
  printf("!");
  for (int i = 1; i <= t; i ++) printf(" %d", E[i]);
  for (int i = 1; i <= n - t; i ++) printf(" %d", O[i]);
  puts("");
  return 0;
}
```

---
## Journey from Petersburg to Moscow
### Description
有一张点数n边数m的无向带权图,一条路径的长度定义为路径上所有边中前k大的边的和,求最短路(n,m<=3000) .
### Idea
可以枚举第k大边的长度L,将每条边权重置为 max(0,wei-L) ,则值为(最短路+k*L).
只有恰好是最小边的时候才是最优解.
![proof.PNG](http://user-image.logdown.io/user/42873/blog/39448/post/4698478/btDkYKzrSze7N4LHDSMb_proof.PNG)
复杂度O(N^2logN).

```cpp Journey from Petersburg to Moscow
#include <cstdio>
#include <iostream>
#include <cstring>
#define mp make_pair
#define travel(x, i) for (int i = fir[x]; i; i = e[i].nxt)
using namespace std;

typedef long long LL;
typedef pair <LL, int> Pair;
const int N = 3005;

namespace Heap {
#define lc (x << 1)
#define rc (x << 1 | 1)
#define fa (x >> 1)
  Pair key[N << 1];
  int sz;
  inline int illegal(int x) {
    if (lc <= sz && key[lc] < key[x]) {
      if (rc <= sz && key[rc] < key[lc]) return 1;
      return 0;
    }
    else if (rc <= sz && key[rc] < key[x]) return 1;
    return -1;
  }
  inline void down(int x) {
    int to;
    while (~ (to = illegal(x))) {
      to |= lc;
      swap(key[x], key[to]);
      x = to;
    }
  }
  inline void up(int x) {
    while (fa && (~ illegal(fa))) {
      swap(key[x], key[fa]);
      x = fa;
    }
  }
  inline Pair top() {
    swap(key[1], key[sz --]);
    down(1);
    return key[sz + 1];
  }
  inline void push(Pair x) {key[++ sz] = x; up(sz);}
  inline void clear() {sz = 0;}
#undef lc
#undef rc
#undef fa
}

struct edge {
  int nxt, to, len;
} e[N << 1];
int fir[N], cnt = 0;

inline void addedge(int x, int y, int l) {
  e[++ cnt] = (edge){fir[x], y, l};
  fir[x] = cnt;
}

int n, m, k;
int vis[N], C = 0;
LL dis[N];

inline LL Dijkstra(int de) {
  Heap :: clear();
  C ++;
  Pair cur; int x;
  Heap :: push(mp(0LL, 1));
  memset(dis, 0x3f, sizeof dis);
  while (Heap :: sz) {
    cur = Heap :: top();
    x = cur.second;
    if (vis[x] == C) continue;
    vis[x] = C;
    if (x == n) return 1LL * de * k + cur.first;
    travel(x, i)
      if (dis[e[i].to] > max(0, e[i].len - de) + cur.first) {
       dis[e[i].to] = max(0, e[i].len - de) + cur.first;
       Heap :: push(mp(dis[e[i].to], e[i].to));
      }
  }
  return (LL)1e18;
}

int main() {
  scanf("%d%d%d", &n, &m, &k);
  for (int i = 1, x, y, l; i <= m; i ++) {
    scanf("%d%d%d", &x, &y, &l);
    addedge(x, y, l); addedge(y, x, l);
  }
  LL ans = 1e18;
  for (int i = 0; i <= m; i ++)
    ans = min(ans, Dijkstra(e[i << 1].len));
  printf("%lld\n", ans);
  return 0;
}
```

---
## Laminar Family
### Description
定义一个集族,属于它的任意两个集合要么交为空集要么其中一个是另一个的子集.
现在给定一个集族与一棵树(n <= 1e5),集族中的集合是树上简单路径经过的点,问是否符合要求.
### Idea
显然元素较少的集合只能被包含或者独立.
我们可以考虑从大到小处理集合,用树剖维护.
每次判断一条路径上所有的点是否颜色相同,如果有不同,则代表该集合与某个集合有交集,但是两个集合之间没有子集关系.
然后将该路径覆盖上新的颜色.
注意线段树的标记合并.
!!!要在计算top的时候再打dfn标记!!!
### Code
```cpp Laminar Family
#include <cstdio>
#include <iostream>
#define ri register int
#define mid ((L + R) >> 1)
#define travel(x, i) for (int i = fir[x]; i; i = e[i].nxt)
using namespace std;

const int N = 1e5 + 5;

struct edge {
  int nxt, to;
} e[N << 1];
int fir[N], cnt = 0;
int dep[N], top[N], dfn[N], sz[N], clo = 0;
int fa[N][18], a[N], b[N], lca[N], len[N];
int key[N << 2], buc[N], id[N], tmp;

inline void addedge(int x, int y) {
  e[++ cnt] = (edge){fir[x], y};
  fir[x] = cnt;
}

inline void dfs1(int x, int f) {
  dep[x] = dep[f] + 1;
  sz[x] = 1;
  fa[x][0] = f;
  for (ri i = 1; i < 18 && fa[x][i - 1]; i ++)
    fa[x][i] = fa[fa[x][i - 1]][i - 1];
  travel(x, i)
    if (e[i].to != f) dfs1(e[i].to, x), sz[x] += sz[e[i].to];
}

inline void dfs2(int x, int f) {
  dfn[x] = ++ clo;
  int best = 0;
  travel(x, i)
    if (e[i].to != f && sz[e[i].to] > sz[best]) best = e[i].to;
  if (!best) return;
  top[best] = top[x];
  dfs2(best, x);
  travel(x, i)
    if (e[i].to != f && e[i].to != best) {
      top[e[i].to] = e[i].to;
      dfs2(e[i].to, x);
    }
}

inline int LCA(int x, int y) {
  if (dep[x] < dep[y]) swap(x, y);
  for (ri i = 17; dep[x] != dep[y]; i --)
    if (dep[fa[x][i]] >= dep[y]) x = fa[x][i];
  if (x == y) return x;
  for (ri i = 17; fa[x][0] != fa[y][0]; i --)
    if (fa[x][i] != fa[y][i]) {
      x = fa[x][i];
      y = fa[y][i];
    }
  return fa[x][0];
}

inline int Query(int l, int r, int c, int x = 1, int L = 1, int R = clo) {
  if (L == l && R == r) {
    tmp = key[x];
    key[x] = c;
    return tmp;
  }
  int res = 0;
  if (key[x] != -1) {
    key[x << 1] = key[x << 1 | 1] = key[x];
    key[x] = -1;
  }
  if (r <= mid) res = Query(l, r, c, x << 1, L, mid);
  else if (l > mid) res = Query(l, r, c, x << 1 | 1, mid + 1, R);
  else {
    res = Query(l, mid, c, x << 1, L, mid);
    if (Query(mid + 1, r, c, x << 1 | 1, mid + 1, R) != res) res = -1;
  }
  if (key[x << 1] == key[x << 1 | 1]) key[x] = key[x << 1];
  return res;
}

inline bool Check(int id) {
  int u = a[id], f = lca[id], c = -2, d = 0;
  while (top[u] != top[f]) {
    d = Query(dfn[top[u]], dfn[u], id);
    if (c != -2 && c != d) return 0;
    c = d;
    u = fa[top[u]][0];
  }
  if (u != f) {
    d = Query(dfn[f] + 1, dfn[u], id);
    if (c != -2 && c != d) return 0;
    c = d;
  }
  u = b[id];
  while (top[u] != top[f]) {
    d = Query(dfn[top[u]], dfn[u], id);
    if (c != -2 && c != d) return 0;
    c = d;
    u = fa[top[u]][0];
  }
  d = Query(dfn[f], dfn[u], id);
  if (c != -2 && c != d) return 0;
  c = d;
  return c != -1;
}

int main() {
  int n, m, x, y;
  scanf("%d%d", &n, &m);
  for (int i = 1; i < n; i ++) {
    scanf("%d%d", &x, &y);
    addedge(x, y); addedge(y, x);
  }
  dfs1(1, 0);
  top[1] = 1;
  dfs2(1, 0);
  for (int i = 1; i <= m; i ++) {
    scanf("%d%d", &a[i], &b[i]);
    lca[i] = LCA(a[i], b[i]);
    len[i] = dep[a[i]] + dep[b[i]] - (dep[lca[i]] << 1);
    buc[len[i]] ++;
  }
  for (int i = 1; i <= n; i ++) buc[i] += buc[i - 1];
  for (int i = 1; i <= m; i ++) id[buc[len[i]] --] = i;
  for (int i = m; i >= 1; i --)
    if (!Check(id[i])) return puts("No"), 0;
  puts("Yes");
  return 0;
}
```
