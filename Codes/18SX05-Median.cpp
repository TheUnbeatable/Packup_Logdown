#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#define Reg register
using namespace std;

typedef long long LL;
typedef vector <LL> poly;
const int N = 84;
const LL mod = 1e9 + 7;

template <typename T>
inline void Inc(T &x, T y) {
  if ((x += y) >= mod) x -= mod;
}

inline LL Pow(LL x, LL exp) {
  x %= mod;
  if (x < 0) x += mod;
  LL res = 1;
  for (; exp; exp >>= 1, x = x * x % mod)
    if (exp & 1) res = res * x % mod;
  return res;
}

inline void Add(const poly &A, const poly &B, poly &C) {
  poly D;
  if (A.size() > B.size()) {
    D = A;
    for (Reg size_t i = 0; i < B.size(); i ++) Inc(D[i], B[i]);
  }
  else {
    D = B;
    for (Reg size_t i = 0; i < A.size(); i ++) Inc(D[i], A[i]);
  }
  C = D;
}

inline void Mul(const poly &A, const poly &B, poly &C) {
  C.clear();
  C.resize(A.size() + B.size() - 1);
  for (Reg size_t i = 0; i < A.size(); i ++)
    for (Reg size_t j = 0; j < B.size(); j ++)
      Inc(C[i + j], A[i] * B[j] % mod);
}

int n, m, t;
LL l[N], r[N], tab[N << 1], pw[N << 1][N << 1];
poly sum[N << 1];

inline LL Calc(LL r, int exp) {
  if (!exp) return r;
  LL pow = 1, res = 0;
  for (size_t i = 0; i < sum[exp].size(); i ++) {
    Inc(res, pow * sum[exp][i] % mod);
    pow = pow * r % mod;
  }
  return res;
}

inline LL Arit(LL L, LL R) {
  return ((R - L + 1) * (L + R) % mod * ((mod + 1) / 2) % mod + mod) % mod;
}

inline LL DP(LL mid) {
  static int dp[N][N >> 1][2];
  memset(dp, 0, sizeof dp);
  dp[1][0][0] = 1; LL tmp;
  for (Reg short i = 1; i <= n; i ++)
    for (Reg short j = 0; j < m && j < i; j ++) {
      if (l[i] <= mid && mid <= r[i])
        Inc(dp[i + 1][j][1], dp[i][j][0]);
      if (l[i] < mid) {
        tmp = Arit(l[i] - mid, min(0LL, r[i] - mid));
        Inc(dp[i + 1][j + 1][0], (int)(tmp * dp[i][j][0] % mod));
        Inc(dp[i + 1][j + 1][1], (int)(tmp * dp[i][j][1] % mod));
      }
      if (r[i] > mid) {
        tmp = Arit(max(0LL, l[i] - mid), r[i] - mid);
        Inc(dp[i + 1][j][0], (int)(tmp * dp[i][j][0] % mod));
        Inc(dp[i + 1][j][1], (int)(tmp * dp[i][j][1] % mod));
      }
    }
  return dp[n + 1][m - 1][1];
}

inline poly Interpolation(const vector <pair <LL, LL> > &Point) {
  poly ret;
  static poly pre[N << 1], suf[N << 1], tmp;
  static LL x[N << 1];
  int n = Point.size();
  for (Reg short i = 0; i < n; i ++)
    x[i] = Point[i].first;
  pre[0] = poly { (mod - x[0]) % mod, 1LL };
  for (Reg short i = 1; i < n; i ++)
    Mul(pre[i - 1], poly { (mod - x[i]) % mod, 1LL }, pre[i]);
  suf[n - 1] = poly { (mod - x[n - 1]) % mod, 1LL };
  for (Reg short i = n - 2; ~ i; i --)
    Mul(suf[i + 1], poly { (mod - x[i]) % mod, 1LL }, suf[i]);
  for (Reg short i = 0; i < n; i ++) {
    LL Down = 1;
    for (Reg short j = 0; j < n; j ++)
      if (i != j) Down = Down * (x[i] - x[j]) % mod;
    Down = Pow(Down, mod - 2) * Point[i].second % mod;
    if (!i) tmp = suf[1];
    else if (i == n - 1) tmp = pre[n - 2];
    else Mul(pre[i - 1], suf[i + 1], tmp);
    for (auto &it : tmp) it = it * Down % mod;
    Add(ret, tmp, ret);
  }
  return ret;
}

inline bool contri(LL L, LL R) {
  int cntl = 0, mid = 0, cntr = 0;
  for (int i = 1; i <= n; i ++) {
    cntl += l[i] <= L;
    cntr += r[i] >= R;
    mid += l[i] <= L && R <= r[i];
  }
  return cntl >= m && cntr >= m && mid;
}

inline LL work(LL L, LL R) {
  if (!contri(L, R)) return 0;
  LL res = 0;
  if (R - L + 1 <= 2 * n) {
    for (LL i = L; i <= R; i ++) Inc(res, DP(i));
    return res;
  }
  LL bd = min(n + n, 130);
  vector <pair <LL, LL> > Point;
  for (LL i = L; i < L + bd; i ++)
    Point.emplace_back(i, DP(i));
  poly ans = Interpolation(Point);
  int nn = ans.size() - 1;
  while (nn && !ans[nn]) nn --;
  for (size_t i = 0; i < ans.size(); i ++)
    res += (Calc(R, i) - Calc(L - 1, i)) * ans[i] % mod;
  return (res % mod + mod) % mod;
}

inline void Init() {
  for (int i = 1; i <= (n << 1) + 2; i ++) {
    pw[i][0] = 1;
    for (int j = 1; j <= (n << 1); j ++)
      pw[i][j] = pw[i][j - 1] * i % mod;
  }
  vector <pair <LL, LL> > tmp;
  tmp.reserve(N << 1 | 1);
  for (int i = 1; i <= (n << 1); i ++) {
    tmp.clear();
    LL tem = 0;
    for (int j = 1; j <= i + 2; j ++) {
      Inc(tem, pw[j][i]);
      tmp.emplace_back(j, tem);
    }
    sum[i] = Interpolation(tmp);
  }
}

int main() {
  LL tot = 1;
  scanf("%d", &n);
  Init();
  m = (n + 1) >> 1;
  for (int i = 1; i <= n; i ++) {
    scanf("%lld%lld", &l[i], &r[i]);
    tab[++ t] = l[i];
    tab[++ t] = r[i] + 1;
    tot = tot * (r[i] - l[i] + 1) % mod;
  }
  sort(tab + 1, tab + t + 1);
  t = unique(tab + 1, tab + t + 1) - tab - 1;
  LL ans = 0;
  for (int i = 1; i < t; i ++)
    Inc(ans, work(tab[i], tab[i + 1] - 1));
  if ((m - 1) & 1) ans = (mod - ans) % mod;
  printf("%lld\n", ans);
  return 0;
}
