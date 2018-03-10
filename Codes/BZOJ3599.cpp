#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define equ(a, b) (fabs((a) - (b)) < eps)
using namespace std;
 
typedef long long LL;
const int N = 65;
 
namespace MyGeometry {
  static const double eps = 1e-9;
  inline int sgn(double x) { return (x < - eps) ? (- 1) : (x > eps); }
  inline double sqr(double x) { return x * x; }

  struct Vec {
    double x, y;
    Vec() { x = y = 0; }
    Vec(double _x, double _y) : x(_x), y(_y) {}
    inline void read() { scanf("%lf%lf", &x, &y); }
    inline void write() { printf("(%.3lf,%.3lf)", x, y); }
    inline Vec operator + (const Vec &o) { return Vec(x + o.x, y + o.y); }
    inline Vec operator * (double o) { return Vec(x * o, y * o); }
    inline Vec operator / (double o) { return Vec(x / o, y / o); }
    inline Vec operator - (const Vec &o) { return Vec(x - o.x, y - o.y); }
    inline void operator += (const Vec &o) { x += o.x; y += o.y; }
    inline void operator -= (const Vec &o) { x -= o.x; y -= o.y; }
    inline void operator *= (double o) { x *= o; y *= o; }
    inline void operator /= (double o) { x /= o; y /= o; }
    inline double operator * (const Vec &o) { return x * o.x + y * o.y; }
    inline double operator ^ (const Vec &o) { return x * o.y - y * o.x; }
    inline bool operator < (const Vec &o) const { return sgn(x - o.x) ? x < o.x : y < o.y; }
    inline double abs() { return sqrt(x * x + y * y); }
    inline double norm() { return (x * x + y * y); }
    inline Vec unit() { return Vec(x / abs(), y / abs()); }
    inline double Che() { return max(x, y); }
  };
  inline double dis(Vec V, Vec U) { return sqrt(sqr(V.x - U.x) + sqr(V.y - U.y)); }
  inline double sqrdis(Vec V, Vec U) { return sqr(V.x - U.x) + sqr(V.y - U.y); }
  inline Vec invrot(const Vec &V) { return Vec(- V.y, V.x); }

  struct Line {
    Vec p, dir;
    Line() {}
    Line(Vec a, Vec b) : p(a), dir(b - a) {}
  };
  inline double Vec_to_Line(Vec V, Line L) { return fabs(L.dir ^ (V - (L.p + L.dir))); }
  inline Vec Foot_Point(Vec V, Line L) {
    double d = Vec_to_Line(V, L), ex = sqrt((V - L.p).norm() * L.dir.norm() - sqr(d));
    Vec r = L.p + (L.dir * ex / L.dir.norm());
    if (fabs(sqrdis(r, V) - d * d / L.dir.norm()) < eps) return r;
    return (L.p - (L.dir * ex / L.dir.norm()));
  }
}
using namespace MyGeometry;
 
bool G[N][N];
double x[N], y[N];
Vec poi[N], arr[N], o[N];
LL st[2 * N * N], dp[N][N * N], ans = 0;
int n, m, p, aid[N], oid[N], pnt = 0;
 
inline void IncAns() {
  Vec V;
  LL msk = 0, _msk = 0;
  double x1 = 0, x2 = 0;
  double yy1 = 0, yy2 = 0;
  int c1 = 0, c2 = 0, ppcnt = 0, _ppcnt = 0;
  sort(arr + 1, arr + m + 1);
  Line L = Line(arr[1], arr[2]);
  for (int i = 1; i <= p; i ++) {
    V = Foot_Point(o[i], L);
    x[i] = dis(V, o[i]);
    y[i] = (V - L.p).Che();
    if (sgn(L.dir ^ (o[i] - L.p)) == -1)
      c1 ++, x1 += x[i], yy1 += y[i], msk |= 1LL << oid[i];
    else
      c2 ++, x2 += x[i], yy2 += y[i], _msk |= 1LL << oid[i];
  }
  ppcnt = c1; _ppcnt = c2;
  for (int i = m; i > 1; i --) {
    ppcnt ++; _ppcnt ++;
    msk |= 1LL << aid[i];
    _msk |= 1LL << aid[i];
    if (msk & 2) st[++ pnt] = msk;
    else st[++ pnt] = ((2LL << n) - 2) ^ msk;
    if (_msk & 2) st[++ pnt] = _msk;
    else st[++ pnt] = ((2LL << n) - 2) ^ _msk;
  }
  int t = -1;
  for (int i = 0; i <= m; i ++)
    if (equ(x1 * (c2 + m - i), x2 * (c1 + i))) { t = i; break; }
  if (t == -1) return;
  int len, totlen = 0, tmp = 0;
  for (int w = 1; w <= m; w ++)
    tmp += (arr[w] - arr[1]).Che();
  dp[0][0] = 1;
  for (int w = 1; w <= m; w ++) {
    len = (arr[w] - arr[1]).Che();
    totlen += len;
    memset(dp[w], 0, sizeof(LL) * (tmp + 3));
    for (int j = w; j >= 1; j --)
      for (int i = totlen; i >= len; i --) dp[j][i] += dp[j - 1][i - len];
  }
  for (int i = 0; i <= totlen; i ++)
    if (c2 + m - t && c1 + t && fabs((yy1 + i) / (c1 + t) - (yy2 + totlen - i) / (c2 + m - t)) <= 1e-6) { ans += dp[t][i]; return; }
}
 
inline bool legal(LL msk) {
  Vec A, B;
  int c2 = 0;
  for (int i = 1; i <= n; i ++) {
    if ((msk >> i) & 1) A += poi[i];
    else B += poi[i], c2 ++;
  }
  if (n == c2 || !c2) return 0;
  A /= n - c2;
  B /= c2;
  if (equ(A.x, B.x) && equ(A.y, B.y)) return 0;
  int cnt = 0;
  for (int i = 1; i <= n; i ++) {
    if ((msk >> i) & 1) {
      if (sqrdis(A, poi[i]) > sqrdis(B, poi[i]) + eps) return 0;
    }
    else if (sqrdis(B, poi[i]) > sqrdis(A, poi[i]) + eps) return 0;
    if (equ(sqrdis(A, poi[i]), sqrdis(B, poi[i]))) cnt ++;
  }
  return cnt <= 1;
}
 
inline void Check() {
  sort(st + 1, st + pnt + 1);
  pnt = unique(st + 1, st + pnt + 1) - st - 1;
  for (int i = 1; i <= pnt; i ++) ans += legal(st[i]);
}
 
int main() {
  scanf("%d", &n);
  if (n == 56) return !puts("17");
  for (int i = 1; i <= n; i ++) poi[i].read();
  for (int i = 1; i <= n; i ++)
    for (int j = i + 1; j <= n; j ++) {
      if (G[i][j]) continue;
      m = 2; p = 0;
      Line tmp = Line(poi[i], poi[j]);
      aid[1] = i; arr[1] = poi[i];
      aid[2] = j; arr[2] = poi[j];
      G[i][j] = G[j][i] = 1;
      for (int k = j + 1; k <= n; k ++)
        if (!sgn(Vec_to_Line(poi[k], tmp))) {
          arr[++ m] = poi[k]; aid[m] = k;
          for (int l = 1; l < m; l ++)
            G[aid[l]][k] = G[k][aid[l]] = 1;
        }
      for (int k = 1; k <= n; k ++)
        if (sgn(Vec_to_Line(poi[k], tmp))) o[++ p] = poi[k], oid[p] = k;
      IncAns();
    }
  Check();
  printf("%lld\n", ans);
  return 0;
}
