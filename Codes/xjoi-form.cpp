#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstring>
#include <algorithm>
using namespace std;

typedef long long LL;
const int M = 11;
const int N = 1 << M | 5;

namespace Poly {
  const double PI = acos(-1);
  struct cd {
    double x, y;
    cd(double _x = 0, double _y = 0) : x(_x), y(_y) {}
    inline cd operator + (const cd &o) const { return cd( x + o.x, y + o.y ); }
    inline cd operator - (const cd &o) const { return cd( x - o.x, y - o.y ); }
    inline cd operator * (const double o) const { return cd( x * o, y * o); }
    inline cd operator / (const double o) const { return cd( x / o, y / o); }
    inline cd operator * (const cd &o) const { return cd( x * o.x - y * o.y, x * o.y + y * o.x ); }
    inline cd operator / (const cd &o) const { return cd( x * o.x + y * o.y, y * o.x - x * o.y ) / ( o.x * o.x + o.y * o.y ); }
    inline void operator += (const cd &o) { x += o.x; y += o.y; }
    inline cd conj() { return cd( x, - y); }
  } W[N];
  inline cd conj(const cd &o) { return cd( o.x, - o.y ); }
  int rev[N];
  
  inline void FFT_Init() {
    for (int i = (1 << M) - 1; ~ i; i --)
      W[i] = cd( cos(2 * PI * i / (1 << M)), sin(2 * PI * i / (1 << M)) );
  }
  inline void Dft(cd *a, int n) {
    for (int i = 0; i < (1 << n); i ++) {
      if (i & 1) rev[i] = rev[i - 1] | ((1 << n) >> 1);
      else rev[i] = rev[i >> 1] >> 1;
      if (rev[i] < i) swap(a[rev[i]], a[i]);
    }
    register cd x, y;
    for (int i = 1, d = 1; d <= n; i <<= 1, d ++)
      for (int j = 0; j < (1 << n); j += 1 << d)
        for (int k = j; k < j + i; k ++) {
          x = a[k];
          y = a[k + i] * W[(k - j) << (M - d)];
          a[k] = x + y; a[k + i] = x - y;
        }
  }
  inline void IDft(cd *a, int n) {
    reverse(a + 1, a + (1 << n)); Dft(a, n);
    double inv = 1.0 / (1 << n);
    for (int i = (1 << n) - 1; ~ i; i --) a[i] = a[i] * inv;
  }
  inline void Mul(int *a, int *b, int *c, int n) {
    static cd d[N], e[N];
    for (int i = 0; i < (1 << n); i ++) d[i].x = a[i], d[i].y = b[i];
    Dft(d, n);
    cd p, q;
    for (int i = (1 << n) - 1; ~ i; i --) {
      p = d[i]; q = d[i ? (1 << n) - i : 0];
      e[i] = (p + conj(q)) * (p - conj(q)) / cd(0, 4);
    }
    IDft(e, n);
    for (int i = (1 << n) - 1; ~ i; i --) c[i] = (int) (e[i].x + 0.5);
  }
}

namespace Math {
  int fac[1050], ifac[1050], mo;
  inline int Pow(int x, int exp = mo - 2) {
    int res = 1;
    for (; exp; exp >>= 1, x = x * x % mo)
      if (exp & 1) res = res * x % mo;
    return res;
  }
  inline void Init() {
    for (int i = fac[0] = 1; i <= mo - 1; i ++) fac[i] = fac[i - 1] * i % mo;
    ifac[mo - 1] = Pow(fac[mo - 1]);
    for (int i = mo - 1; i; i --) ifac[i - 1] = ifac[i] * i % mo;
  }
  inline int C(int x, int y) {
    return fac[x] * ifac[y] * ifac[x - y] % mo;
  }
}
using Poly :: Mul;
using Math :: C;

inline void solve() {
  LL n1, n2, n3;
  int mod, p, m;
  scanf("%lld%lld%lld%d%d%d", &n1, &n2, &n3, &mod, &p, &m);
  Math :: mo = mod;
  Math :: Init();
  int bit = 0;
  while ((1 << bit) < p) bit ++;
  bit ++;
  int a[1 << bit | 5], b[1 << bit | 5];
  memset(a, 0, sizeof a);
  a[0] = 1;
  int base = 1;
  while (n1 || n3) {
    memset(b, 0, sizeof b);
    int m1 = n1 % mod, m2 = n2 % mod, m3 = n3 % mod;
    n1 /= mod; n2 /= mod; n3 /= mod;
    for (int i = min(m1, m3), k; ~ i; i --) {
      k = (m3 - i + mod) % mod;
      if (k > m2) continue;
      b[i * base % p] = (b[i * base % p] + C(m3, i) * C(m1, i) * C(m2, k)) % mod;
    }
    base = base * mod % p;
    Mul(a, b, a, bit);
    for (int i = p; i <= p + p; i ++) {
      a[i - p] += a[i];
      a[i] = 0;
      a[i - p] %= mod;
    }
  }
  int ans = 0;
  for (int i = 1, k; i <= m; i ++) {
    scanf("%d", &k);
    ans += a[k];
  }
  printf("%d\n", ans % mod);
}

int main() {
  Poly :: FFT_Init();
  int T;
  scanf("%d", &T);
  while (T --) solve();
  return 0;
}
