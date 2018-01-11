# Description
给定长为n(n<=1e5)的序列A和一个p(p <= 1e9),一共q(q<=1e5)个询问,每次求
$$ A_l^{\Big ( {A_{l+1}^{\big (  A_{l+2}^{ ( \dots A_r ) }  \big )}} \Big ) } mod \ p $$
# Idea
有一个叫欧拉定理的:
$$ (a,b)=1 \ \Rightarrow \ a^{\phi(p)} \equiv 1 \pmod p $$
$$ 所以(a,b)=1 \ \Rightarrow \ a^{b} \equiv a^{b \ mod \ \phi(p)} \pmod p $$
对于(a,b)!=1,有扩展定理:
$$ b \ge \phi(p) \ \Rightarrow \ a^{b} \equiv a^{b \ mod \ \phi(p)+\phi(p)} \pmod p $$
注意到对一个数不停求phi,最多经过log步,我们可以预处理出对k不断求phi的值.
对于每一个询问,从l开始不断递归求指数部分,复杂度O(Nlog^2N).
注意递归下去时,如果指数小于phi,要按照原定理而不是扩展定理.
因此快速幂的时候要对此加以处理
# Code
```cpp Core Part
inline int pow(int x, int exp, int mod) {
  int r = 1;
  for (; exp; exp >>= 1) {
    if (exp & 1) {
      if (1LL * r * x >= mod) r = 1LL * r * x % mod + mod;
      else r *= x;
    }
    if (1LL * x * x >= mod) x = 1LL * x * x % mod + mod;
    else x *= x;
  }
  return r;
}
inline int work(int l, int r, int p) {
  if (l == r) return a[l] >= step[p] ? (a[l] % step[p]) + step[p] : a[l];
  if (step[p] == 1) return 1;
  int res = pow(a[l], work(l + 1, r, p + 1), step[p]);
  if (res >= step[p]) res = res % step[p] + step[p];
  return res;
}
```
