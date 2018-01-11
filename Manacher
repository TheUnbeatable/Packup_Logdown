# Algorithm
设r为最当前回文串靠右的右端点,pos为该回文串的对称中心.
![Manacher算法_1.png](http://user-image.logdown.io/user/42873/blog/39448/post/4729819/ac7WZoxcT32sPVb4UUnA_Manacher%E7%AE%97%E6%B3%95_1.png)
如果i < r,那么可以通过对称性a[i] = min(a[pos * 2 - i], r - i),否则a[i] = 0.
显然根据对称性,如果此时i+a[i] < r,那么就没有再向右扩展的可能了.
否则暴力向右扩展,同时更新r和pos.
由于每次暴力扩展r都在不断增加,所以复杂度时O(n)的.
这也揭示了回文串的一个性质:本质不同的回文串只有n个,每次r向右增加时产生回文串.
```cpp Manacher
inline void manacher() {l = strlen(c + 1);
    for (int i = 1, j = 0; i <= l; i ++) s[++ j] = '%', s[++ j] = c[i];
    l = l * 2 + 1;
    s[l] = '%';
    pos = r = 1;
    a[1] = b[1] = 0;
    for (int i = 2; i <= l; i ++) {
      a[i] = b[i] = 0;
      if (r >= i) a[i] = min(a[pos * 2 - i], r - i);
      else a[i] = 0;
      if (a[i] + i < r) continue;
      while (i + a[i] + 1 <= l && s - a[i] - 1 > 0 && s[i + a[i] + 1] == s[i - a[i] - 1]) a[i] ++;
      if (i + a[i] > r) {
       r = i + a[i];
       pos = i;
      }
    }
}
```
# Example [BZOJ3790:神奇项链](http://www.lydsy.com/JudgeOnline/problem.php?id=3790)
## Description
每次可以生成回文串并且拼到已有串后面,假如已有后缀和新串的前缀是完全相同的，那么可以将这个重复部分重叠.
问至少拼合多少次.
## Idea
先manacher,然后贪心向右扩展.
## Code
```cpp BZOJ3790
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cassert>
using namespace std;

const int N = 100010;

char c[N], s[N << 1];
int a[N << 1], b[N << 1];

int main() {
  int n, pos, r, l, tot;
  while (scanf("%s", c + 1) != EOF) {
    l = strlen(c + 1);
    for (int i = 1, j = 0; i <= l; i ++) s[++ j] = '%', s[++ j] = c[i];
    l = l * 2 + 1;
    s[l] = '%';
    pos = r = 1;
    a[1] = b[1] = 0;
    for (int i = 2; i <= l; i ++) {
      a[i] = b[i] = 0;
      if (r >= i) a[i] = min(a[pos * 2 - i], r - i);
      else a[i] = 0;
      if (a[i] + i < r) continue;
      while (i + a[i] + 1 <= l && s - a[i] - 1 > 0 && s[i + a[i] + 1] == s[i - a[i] - 1]) a[i] ++;
      if (i + a[i] > r) {
       r = i + a[i];
       pos = i;
      }
    }
    for (int i = 1; i <= l; i ++) {
      b[i - a[i]] = max(b[i - a[i]], i + a[i]);
    }
    r = 1; tot = 0;
    for (int i = 1, j; r <= l;) {
      tot ++; j = r;
      for (; i <= j; i ++) r = max(r, b[i]);
      r ++;
    }
    printf("%d\n", tot - 1);
  }
  return 0;
}
```
