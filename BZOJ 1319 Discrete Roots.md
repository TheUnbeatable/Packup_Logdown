# Description
给出三个整数p,k,a,其中p为质数，求出所有的x,满足:
![捕获.PNG](http://user-image.logdown.io/user/42873/blog/39448/post/4722628/tMaLzncQv6EClkTaRwuh_%E6%8D%95%E8%8E%B7.PNG)

# Idea
数论大集合
## 原根的正确使用方式
如果我们找到了一个原根g,由于p是质数,g的1到p-1次方模p取遍1到p-1.
![公式.PNG](http://user-image.logdown.io/user/42873/blog/39448/post/4722628/0g3l7zm0Q23QWV2BbUQN_%E5%85%AC%E5%BC%8F.PNG)
这样我们只需要用exgcd找出所有满足条件的 ind_X 就可以了.

## 原根的正确计算方式
原根的数量是phi(phi(p))=phi(p-1),数量还是非常多的,所以只需要暴力求前几个就可以了.
![捕获.PNG](http://user-image.logdown.io/user/42873/blog/39448/post/4722628/e7prL5PnQKafgOCzJUDH_%E6%8D%95%E8%8E%B7.PNG)
```cpp
inline LL primitive(LL p) {
  bool s;
  for (LL i = 2; i < p; i ++) {
    s = 0;
    for (int j = 1; j <= tp; j ++)
      if (pow(i, (p - 1) / pri[j], p) == 1) { s = 1; break; }
    if (!s) return i;
  }
  return 0;
}

inline void primes(LL p) {
  for (LL i = 2; i * i <= p; i ++)
    if (p % i == 0) {
      pri[++ tp] = i;
      while (p % i == 0) p /= i;
    }
  if (p != 1) pri[++ tp] = p;
}
```
## 求指标indx
A^x=B(mod p)
注意,这里p就是原题中的p而不是phi(p).
BSGS求解.
```cpp
inline LL bsgs(LL g, LL a, LL p) {
  map <LL, int> tab;
  LL s = (LL)sqrt(p) + 1, t = 1, v = 1;
  for (int i = 1; i <= s; i ++) {
    t = t * g % p;
    tab[t * a % p] = i;
  }
  for (int i = 0; i <= s; i ++, v = v * t % p)
    if (tab.count(v)) return (i * s - tab[v]);
  return 0;
}
```
### 最后我们再通过已经求出的ind求出原来的x就可以了.
```cpp
inline void ex_gcd(LL &x, LL &y, LL a, LL b) {
  if (b == 0) {
    x = 1; y = 0;
    return;
  }
  ex_gcd(x, y, b, a % b);
  LL t = x;
  x = y;
  y = t - (a / b) * x;
}

inline LL pow(LL x, LL e, LL p) {
  LL r = 1; x %= p; if (x < 0) x += p;
  for (; e; e >>= 1, x = x * x % p)
    if (e & 1) r = r * x % p;
  return r;
}

inline void solve(LL a, LL b, LL c, LL g) {
  ///ax % c == b
  ///ax - cy == b
  LL x, y, d = __gcd(a, - c);
  if (b % d) return (void)puts("0");
  a /= d; b /= d; c /= d;
  ex_gcd(x, y, a, -c);
  if (c < 0) c = - c;
  x *= b; x %= c;
  if (x < 0) x += c;
  printf("%lld\n", (p - 1 - x) / c + 1);
  int tot = 0;
  while (x < p) {
    ans[++ tot] = pow(g, x, p);
    x += c;
  }
  sort(ans + 1, ans + tot + 1);
  for (int i = 1; i <= tot; i ++) printf("%lld\n", ans[i]);
}
```
