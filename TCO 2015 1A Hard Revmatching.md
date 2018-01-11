# Description
给定二分图,求删除最少的边使它不存在完美匹配(n <= 20).

# Idea
Hall's Thereom
如果一个二分图有完美匹配,那么任意取X的一个子集S,与S联通的Y的点集为C(S),|C(S)| >= |S|.
那么我们枚举每一个X的子集S,计算出删去任意与S相连的点与S的所有边的权值和,然后贪心的取最小的前几个使得|C(S)| < |S|,更新最优解.
如果本身就没有完美匹配就返回0.

# Code
```cpp Revmatching
#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#define lb(x) (x & (- x))
using namespace std;

const int N = 20;

class Revmatching {
private :
  int disg[N][1 << 10], dish[N][1 << 10], lkn[N];
  int n, t, h, g, tab[N + 10];
  map <int, int> mp;
public :
  inline int smallest(vector <string> A) {
    n = A.size();
    for (int i = 0; i < n; i ++) mp[1 << i] = i;
    t = 1 << n;
    g = 1 << (n >> 1);
    h = t / g;
    for (int i = 0; i < n; i ++)
      for (int j = 1; j < g; j ++)
        disg[i][j] = disg[i][j - lb(j)] + A[mp[lb(j)]][i] - '0';
    for (int i = 0; i < n; i ++)
      for (int j = 1; j < h; j ++)
        dish[i][j] = dish[i][j - lb(j)] + A[mp[lb(j)] + (n >> 1)][i] - '0';
    for (int i = 0; i < n; i ++)
      for (int j = 0; j < n; j ++)
        if (A[i][j] != '0') lkn[i] |= 1 << j;
    int ans = 1e9;
    g --; h --;
    for (int i = 1, s, c, u; i < t; i ++) {
      s = c = u = 0;
      for (int j = 0; j < n; j ++)
        if ((i >> j) & 1) s |= lkn[j], c --;
      for (int j = 0; j < n; j ++)
        if ((s >> j) & 1) {
          c ++;
          tab[u ++] = disg[j][i & g] + dish[j][(i >> (n >> 1)) & h];
        }
      if (c < 0) return 0;
      sort(tab, tab + u);
      u = 0;
      for (int j = 0; j <= c; j ++) u += tab[j];
      if (ans > u) ans = u;
    }
    return ans;
  }
};
```
