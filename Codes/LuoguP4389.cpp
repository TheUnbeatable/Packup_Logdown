// luogu-judger-enable-o2
#include <cstdio>
#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <cstring>
#include <algorithm>
using namespace std;

namespace Poly {
#define ri register int
#define Int(x) (((LL)(x + 0.5)) % Mod)
  typedef long long LL;
  typedef vector <LL> poly;
  const int S = 18, N = 1 << S | 5;
  const LL mod = 998244353, G = 3;
  const double PI = acos(-1);

  struct cd {
    double x, y;
    cd(LL _x, LL _y) : x(_x), y(_y) {}
    cd(int _x, int _y) : x(_x), y(_y) {}
    cd(double _x = 0, double _y = 0) : x(_x), y(_y) {}
    inline cd operator + (const cd &o) const { return cd( x + o.x, y + o.y ); }
    inline cd operator - (const cd &o) const { return cd( x - o.x, y - o.y ); }
    inline cd operator * (const double o) const { return cd( x * o, y * o); }
    inline cd operator / (const double o) const { return cd( x / o, y / o); }
    inline cd operator * (const cd &o) const { return cd( x * o.x - y * o.y, x * o.y + y * o.x ); }
    inline cd operator / (const cd &o) const { return cd( x * o.x + y * o.y, y * o.x - x * o.y ) / ( o.x * o.x + o.y * o.y ); }
    inline void operator += (const cd &o) { x += o.x; y += o.y; }
    inline cd conj() { return cd( x, - y); }
    friend ostream& operator << (ostream &o, cd c) {
      o << fixed << setprecision(5) << "(" << c.x << "," << c.y << ")";
      return o;
    }
  };
  inline cd conj(const cd &o) { return cd( o.x, - o.y ); }
  typedef vector <cd> fpoly;

  int rev[N];
  cd W[N];
  LL w[N], Inv[N];
  poly Swap1, Swap2, Swap3;
  
  inline int Up(int x) {
    int d = 0;
    while ((1 << d) < x) d ++;
    return d;
  }
  inline LL Pow(LL x, LL exp) {
    LL r = 1;
    for (; exp; exp >>= 1, x = x * x % mod)
      if (exp & 1) r = r * x % mod;
    return r;
  }
  inline void NTT_Init() {
    Swap1.reserve(N);
    Swap2.reserve(N);
    Swap3.reserve(N);
    LL tmp = Pow(G, (mod - 1) >> S);
    Inv[1] = w[0] = 1;
    for (int i = 1; i < N; i ++)
      w[i] = w[i - 1] * tmp % mod;
    for (int i = 2; i < N; i ++)
      Inv[i] = Inv[mod % i] * (mod - mod / i) % mod;
  }
  inline void FFT_Init() {
    for (int i = (1 << S) - 1; ~ i; i --)
      W[i] = cd( cos(2 * PI * i / (1 << S)), sin(2 * PI * i / (1 << S)) );
  }
  inline void Dft(fpoly &a, int n) {
    int E = 1 << n; a.resize(E);
    for (int i = 0; i < E; i ++) {
      if (i & 1) rev[i] = rev[i - 1] | (E >> 1);
      else rev[i] = rev[i >> 1] >> 1;
      if (rev[i] < i) swap(a[rev[i]], a[i]);
    }
    register cd x, y;
    for (int i = 1, d = 1; d <= n; i <<= 1, d ++)
      for (int j = 0; j < E; j += 1 << d)
        for (int k = j; k < j + i; k ++) {
          x = a[k];
          y = a[k + i] * W[(k - j) << (S - d)];
          a[k] = x + y; a[k + i] = x - y;
        }
  }
  inline void IDft(fpoly &a, int n) {
    a.resize(1 << n);
    reverse(a.begin() + 1, a.end());
    Dft(a, n);
    double inv = 1.0 / (1 << n);
    for (int i = (1 << n) - 1; ~ i; i --) a[i] = a[i] * inv;
  }
  inline void Dft(poly &a, int n) {
    int E = 1 << n; a.resize(E);
    for (int i = 0; i < E; i ++) {
      if (i & 1) rev[i] = rev[i - 1] | (E >> 1);
      else rev[i] = rev[i >> 1] >> 1;
      if (rev[i] < i) swap(a[rev[i]], a[i]);
    }
    register LL x, y;
    for (ri i = 1, d = 1; d <= n; i <<= 1, d ++)
      for (ri j = 0; j < E; j += 1 << d)
        for (ri k = j; k < j + i; k ++) {
          x = a[k];
          y = a[k + i] * w[(k - j) << (S - d)] % mod;
          a[k] = x + y; a[k + i] = x - y;
          if (a[k] >= mod) a[k] -= mod;
          if (a[k + i] < 0) a[k + i] += mod;
        }
  }
  inline void IDft(poly &a, int n) {
    a.resize(1 << n);
    reverse(a.begin() + 1, a.end());
    Dft(a, n);
    LL inv = Inv[1 << n];
    for (int i = (1 << n) - 1; ~ i; i --) a[i] = a[i] * inv % mod;
  }
  inline void Mul(const fpoly &a, const fpoly &b, fpoly &c) { // WARNING : may cause relative large precision loss
    int len = a.size() + b.size() - 1;
    if (len <= 100) {
      static fpoly d;
      d.clear(); d.resize(len);
      for (size_t i = 0; i < a.size(); i ++)
        for (size_t j = 0; j < b.size(); j ++)
          d[i + j] += a[i] * b[j];
      c = d; return;
    }
    int n = Up(len);
    fpoly d;
    c.resize(1 << n);
    d.resize(1 << n);
    for (size_t i = 0; i < a.size(); i ++) d[i] = a[i];
    for (size_t i = 0; i < b.size(); i ++) d[i] += b[i] * cd(0, 1);
    Dft(d, n);
    cd p, q;
    for (int i = (1 << n) - 1; ~ i; i --) {
      p = d[i]; q = d[i ? (1 << n) - i : 0];
      c[i] = (p + conj(q)) * (p - conj(q)) / cd(0, 4);
    }
    IDft(c, n); c.resize(len);
  }
  inline void Mul(const poly &a, const poly &b, poly &c) {
    int len = a.size() + b.size() - 1;
    if (len <= 100) {
      static poly d;
      d.clear(); d.resize(len);
      for (size_t i = 0; i < a.size(); i ++)
        for (size_t j = 0; j < b.size(); j ++)
          d[i + j] = (d[i + j] + a[i] * b[j]) % mod;
      c = d; return;
    }
    poly A = a, B = b;
    int n = Up(len);
    Dft(A, n); Dft(B, n);
    c.resize(1 << n);
    for (int i = (1 << n) - 1; ~ i; i --) c[i] = A[i] * B[i] % mod;
    IDft(c, n); c.resize(len);
  }
  inline void SuperMul(poly &a, poly &b, poly &c, LL Mod) {
    static fpoly s0, s1, t0, t1;
    int len = a.size() + b.size() - 1, n = Up(len);
    LL base = sqrt(Mod) + 1;
    t0.resize(1 << n); t1.resize(1 << n);
    s0.resize(1 << n); s1.resize(1 << n);
    for (size_t i = 0; i < a.size(); i ++) {
      a[i] %= Mod;
      t0[i] = cd( a[i] / base, a[i] % base );
    }
    for (size_t i = 0; i < b.size(); i ++) {
      b[i] %= Mod;
      t1[i] = cd( b[i] / base, b[i] % base );
    }
    Dft(t0, n); Dft(t1, n);
    cd p, q, xx, yy;
    cd a0, a1, b0, b1;
    for (int i = (1 << n) - 1; ~ i; i --) {
      p = t0[i]; q = t0[i ? (1 << n) - i : 0];
      a0 = (p + conj(q)) / 2;
      a1 = (p - conj(q)) / cd(0, 2);
      p = t1[i]; q = t1[i ? (1 << n) - i : 0];
      b0 = (p + conj(q)) / 2;
      b1 = (p - conj(q)) / cd(0, 2);
      s0[i] = a0 * b0 + a0 * b1 * cd(0, 1);
      s1[i] = a1 * b0 + a1 * b1 * cd(0, 1);
    }
    IDft(s0, n); IDft(s1, n);
    c.resize(len);
    LL base2 = base * base % Mod, a0b0, a0b1, a1b0, a1b1;
    for (int i = len - 1; ~ i; i --) {
      a0b0 = Int(s0[i].x); a0b1 = Int(s0[i].y);
      a1b0 = Int(s1[i].x); a1b1 = Int(s1[i].y);
      c[i] = base2 * a0b0 % Mod + base * (a0b1 + a1b0) % Mod + a1b1;
      c[i] %= Mod;
    }
  }
  inline void Lim(poly &a) {
    if (!a.size()) return;
    for (size_t i = 0; i < a.size() - 1; i ++)
      a[i] = a[i + 1] * (i + 1) % mod;
    a.pop_back();
  }
  inline void Sum(poly &a) {
    a.push_back(0);
    for (int i = a.size() - 1; i >= 0; i --)
      a[i + 1] = a[i] * Inv[i + 1] % mod;
    a[0] = 0;
  }
  inline void Inverse(const poly &A, poly &E) { //WARING : make sure A(0) != 0
    if (!A.size() || !A[0]) throw;
    static poly B, C, D;
    int n = Up((int)A.size());
    B.clear();
    B.reserve(2 << n | 5);
    C.reserve(2 << n | 5);
    B.push_back(Pow(A[0], mod - 2));
    for (int d = 0; d < n; d ++) {
      C.resize(min((int)A.size(), (2 << d)));
      for (int i = C.size() - 1; ~ i; i --) C[i] = A[i];
      D = B;
      Dft(D, d + 2); Dft(C, d + 2);
      for (int i = C.size() - 1; ~ i; i --)
        C[i] = C[i] * D[i] % mod * D[i] % mod;
      IDft(C, d + 2);
      B.resize(2 << d);
      for (int i = B.size() - 1; i >= (1 << d); i --)
        if (C[i]) B[i] = mod - C[i];
    }
    B.resize(A.size());
    E = B;
  }
  inline void Division(const poly &A, const poly &B, poly &D) {
    poly Ar, Br;
    int n = A.size() - 1, m = B.size() - 1;
    Ar.resize(n - m + 1);
    for (int i = 0; i <= n - m; i ++)
      Ar[i] = A[n - i];
    Br = B;
    reverse(Br.begin(), Br.end());
    Inverse(Br, Br);
    Br.resize(n - m + 1);
    Mul(Br, Ar, D);
    D.resize(n - m + 1);
    reverse(D.begin(), D.end());
  }
  inline void Modulo(const poly &A, const poly &B, poly &E) {
    poly D;
    Division(A, B, D);
    Mul(D, B, D);
    E.resize(B.size() - 1);
    for (size_t i = 0; i < E.size(); i ++) {
      E[i] = A[i] - D[i];
      if (E[i] < 0) E[i] += mod;
    }
  }
  inline void solve(int L, int R, const poly &a, poly &b) {
    if (R - L + 1 <= 100) {
      for (int i = L; i <= R; i ++) {
        if (!i) {
          b[i] = 1; continue;
        }
        b[i] %= mod;
        for (int j = L; j < i; j ++)
          (b[i] += b[j] * a[i - j - 1]) %= mod;
        (b[i] *= Inv[i]) %= mod;
      }
      return;
    }
    int mid = (L + R) >> 1;
    solve(L, mid, a, b);
    int n = Up(R - L + 1);
    int len = 1 << n;
    Swap1.clear(); Swap1.resize(len);
    Swap2.clear(); Swap2.resize(len);
    for (int i = L; i <= mid; i ++)
      Swap1[i - L] = b[i], Swap2[i - L] = a[i - L];
    for (int i = mid + 1; i <= R; i ++) Swap2[i - L] = a[i - L];
    Dft(Swap1, n); Dft(Swap2, n);
    for (int i = 0; i < len; i ++)
      Swap2[i] = Swap2[i] * Swap1[i] % mod;
    IDft(Swap2, n);
    for (int i = mid + 1; i <= R; i ++) b[i] += Swap2[i - L - 1];
    solve(mid + 1, R, a, b);
  }
  inline void Exponent(const poly &a, poly &b) {
    poly c = a, d;
    Lim(c);
    d.resize(a.size());
    solve(0, a.size() - 1, c, d);
    swap(b, d);
  }
#undef Int
#undef ri
}
using namespace Poly;

int tab[N];

int main() {
  NTT_Init();
  poly A;
  int n, m;
  scanf("%d%d", &n, &m);
  for (int i = 1, x; i <= n; i ++)
    scanf("%d", &x), tab[x] ++;
  A.resize(m + 1);
  for (int i = 1; i <= m; i ++)
    if (tab[i]) {
      LL tmp = i * tab[i] % mod;
      for (int j = i; j <= m; j += i) {
        A[j] += tmp;
        if (A[j] >= mod) A[j] -= mod;
      }
    }
  for (int i = 0; i <= m; i ++)
    A[i] = Inv[i] * A[i] % mod;
  Exponent(A, A);
  for (int i = 1; i <= m; i ++) printf("%lld\n", A[i]);
  return 0;
}
