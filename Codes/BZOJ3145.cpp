#include <cstdio>
#include <iostream>
#include <set>
#include <cstring>
#include <algorithm>
#define ri register int
using namespace std;

typedef set <int> Set;
const int N = 2e5 + 5;

struct edge {
  int nxt, to;
} e[N << 1];
char c[N];
int a[N << 1];
int l1, l2, pos[N];
Set A[N << 1], B[N << 1];
int tab[N][19], Log[N], ans = 0;
int rnk[N], id[N], buc1[N], buc2[N], _rnk[N], ord[N], hei[N];
int trans[N << 1][27], fa[N << 1], Max[N << 1], ID[N << 1], tot = 1, lst = 1;

inline void extend(int x, int I) {
  int p = lst, np = lst = ++ tot;
  pos[I] = np;
  Max[np] = Max[p] + 1;
  for (; !trans[p][x] && p; p = fa[p]) trans[p][x] = np;
  if (!p) fa[np] = 1;
  else {
    int q = trans[p][x];
    if (Max[p] + 1 == Max[q]) fa[np] = q;
    else {
      int nq = ++ tot;
      memcpy(trans[nq], trans[q], sizeof trans[q]);
      Max[nq] = Max[p] + 1;
      fa[nq] = fa[q];
      fa[np] = fa[q] = nq;
      for (; trans[p][x] == q; p = fa[p]) trans[p][x] = nq;
    }
  }
}

inline void GetRank(int n) {
  int g = max(n, 130);
  for (int i = 1; i <= n; i ++) rnk[i] = c[i];
  for (int h = 1; h <= n; h <<= 1) {
    memset(buc1, 0, sizeof(int) * (g + 2));
    memset(buc2, 0, sizeof(int) * (g + 2));
    int t = h >> 1;
    for (int i = 1; i <= n; i ++) {
      buc1[rnk[min(N - 1, i + t)]] ++;
      buc2[rnk[i]] ++;
    }
    for (int i = 1; i <= max(n, 130); i ++) {
      buc1[i] += buc1[i - 1];
      buc2[i] += buc2[i - 1];
    }
    for (int i = 1; i <= n; i ++) id[buc1[rnk[min(N - 1, i + t)]] --] = i;
    int pre = n;
    for (int i = n; i >= 1; i --) {
      if (i != n && rnk[min(N - 1, id[i] + t)] != rnk[min(N - 1, id[i + 1] + t)]) {
	for (int j = i + 1; j <= pre; j ++) buc2[rnk[id[j]]] --;
	pre = i;
      }
      _rnk[id[i]] = buc2[rnk[id[i]]];
    }
    swap(rnk, _rnk);
  }
  for (int i = 1; i <= n; i ++) ord[rnk[i]] = i;
  for (int i = 1, j, k = 0; i <= n; hei[rnk[i ++]] = k)
    for (k ? k -- : 0, j = ord[rnk[i] - 1]; c[i + k] == c[j + k]; k ++);
}

inline bool cmp(int a, int b) { return Max[a] > Max[b]; }

inline void ST(int n) {
  for (int i = 0; i <= 17; i ++) Log[1 << i] = i;
  for (int i = 1; i <= n; i ++) {
    if (!Log[i]) Log[i] = Log[i - 1];
    tab[i][0] = hei[i];
  }
  for (int h = 1, k = 1; h <= n; h <<= 1, k ++)
    for (int i = 1; i + h <= n; i ++) tab[i][k] = min(tab[i][k - 1], tab[i + h][k - 1]);
}

inline int LCP(int l, int r) {
  if (l == -1 || r == -1) return -1;
  if (l > r) swap(l, r); l ++;
  return min(tab[l][Log[r - l + 1]], tab[r - (1 << Log[r - l + 1]) + 1][Log[r - l + 1]]);
}

inline void Upd(const Set &s, const Set &b, int id) {
  if (b.size() == 0 || s.size() == 0) return;
  int res = 0, h;
  for (Set :: iterator it = s.begin(), d; it != s.end(); it ++) {
    h = (*it);
    if (h == -1) continue;
    d = b.lower_bound(h);
    if (d != b.end()) res = max(res, LCP(h, (*d)) + 1);
    if (d == b.begin()) continue;
    d --;
    res = max(res, LCP(h, (*d)) + 1);
  }
  ans = max(ans, res + Max[id]);
}

inline void Comb(Set &s, Set &e) {
  if (s.size() > e.size()) s.swap(e);
  for (Set :: iterator it = s.begin(); it != s.end(); it ++) e.insert(*it);
}

inline void Merge(int x, int f) {
  if (A[x].size() < B[f].size()) Upd(A[x], B[f], f);
  else Upd(B[f], A[x], f);
  if (A[f].size() < B[x].size()) Upd(A[f], B[x], f);
  else Upd(B[x], A[f], f);
  Comb(A[x], A[f]);
  Comb(B[x], B[f]);
}

int main() {
  scanf("%s", c + 1);
  l1 = strlen(c + 1);
  c[l1 + 1] = '%';
  scanf("%s", c + l1 + 2);
  l2 = strlen(c + l1 + 2);
  int n = l1 + l2 + 1, x;
  for (int i = 1; i <= n; i ++) extend(c[i] - 'a', i);
  GetRank(n); ST(n);
  
  for (int i = 1; i < l1; i ++)
    A[pos[i]].insert(rnk[i + 2]);
  A[pos[l1]].insert(-1);
  for (int i = l1 + 2; i < n; i ++)
    B[pos[i]].insert(rnk[i + 2]);
  B[pos[n - 1]].insert(-1);
  A[1].insert(rnk[2]);
  B[1].insert(rnk[l1 + 3]);
  Upd(A[1], B[1], 1);
  
  for (int i = 1; i <= tot; i ++) ID[i] = i;
  sort(ID + 1, ID + tot + 1, cmp);
  for (int i = 1; i <= tot; i ++) {
    x = ID[i];
    if (fa[x]) Merge(x, fa[x]);
  }
  ans = min(ans, l1);
  ans = min(ans, l2);
  printf("%d\n", ans);
  return 0;
}
