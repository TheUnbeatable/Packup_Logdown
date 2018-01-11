# Definition
> ### Linear Programming is a problem like this:
Maximize `$ \sum_{i=1}^{i\le n}c_i*x_i $`
`$ \forall i \ x_i \ge 0 $`
While satisfying m constraints like this:
`$ \forall \ Jth \ constraint : \sum_{i=1}^{i\le n}b_{ji}*x_i \le b_j $`
> #### By which means 
`$ Maxmize c^T*x $`
`$ While \  A*x \le b $`
`$ x \ge 0 $`

# Simplex

> ### We can define m more variables, called the Slack Variables
`$ x_{i+n}=b_i-\sum_{i=1}^{i\le n}b_{ji}*x_i $`
And it's the Slack Form of MP(c,A,b)
Then we have m+n variables, we call the variables on the left Basic Variables, and the others Free Variables.
### Now its time to introduce the first operation : pivot
void pivot(int j, int k) {
  `$ \ \  replace \ x_k \ with \ -(b_j + \sum_{i=1}^{i\le n \cap i \neq k}x_i*A_{ji} - x_{j+n}) / A_{jk} $`
}
IF we already have B > 0 then we can begin
### simplex
```cpp
void simplex() {
  for (;;) {
    find a free variable Xe and Ce > 0; 
    (so that we can increase Xe a little bit and reach a better solution)
    if (not found) exit();
    find a basic variable Xn+l which satisfies A[l][e] > 0 and minimizes b[l]/a[l][e]; 
    (so that b > 0 is guarenteed after this round)
    (if there are more than one basic variable, find the one with the smallest ID[Bland's Algorithm])
    if (not found) return INF;
    pivot(l,e);
  }
}
```
We can see in every round we tries to increase the target value a little to come to the best solution.

# Init_Simplex()
> This function deals with the situation when exists b[i] < 0
First we find the minimum b[l]
Then we add one variable X0 to each constraint
and we change the target value to ans = -X0
pivot(l,0) so b > 0
simplex() and we have the solution to the auxiliary MP
if the maximized answer is not less than zero, we have a basic valid solution in which X = 0
else this problem is Infeasible.

# Complexity
> O(nm*2^n)
But it turns out to be a very sufficient algorithm as long as you stick to the Bland Algorithm


Template Problem Link : http://uoj.ac/problem/179
Submission ID(the code below) : http://uoj.ac/submission/212916
# Template

```cpp Template of Linear Programming
#include <cstdio>
#include <iostream>
#define sgn(x) ((x) < - eps ? - 1 : ((x) > eps))
#define rep(i, x, y) for (register int i = (x); i <= (y); i ++)
using namespace std;

typedef long double LD;
const int N = 50;
const LD eps = 1e-8;

int n, m, t;
LD a[N][N], val[N];
int idB[N], idN[N];

inline void pivot(int x, int y) {
  swap(idB[x], idN[y]);
  LD c = - a[x][y];
  a[x][y] = -1;
  rep(i, 0, n) a[x][i] /= c;
  rep(i, 0, m) {
    if (!sgn(a[i][y]) || i == x) continue;
    c = a[i][y]; a[i][y] = 0;
    rep(j, 0, n) a[i][j] += c * a[x][j];
  }
}

inline bool simplex() {
  while (1) {
    int x = 0, y = 0;
    rep(i, 1, n)
      if (sgn(a[0][i]) == 1 && (!y || idN[i] < idN[y])) y = i;
    if (!y) break;
    LD bound = 1e19, t;
    rep(i, 1, m)
      if (sgn(a[i][y]) == -1) bound = min(bound, - a[i][0] / a[i][y]);
    rep(i, 1, m)
      if (sgn(a[i][y]) == -1 && sgn((t = - a[i][0] / a[i][y]) - bound) == 0 && (!x || idB[i] < idB[x])) x = i;
    if (!x) return 0;
    pivot(x, y);
  }
  return 1;
}

inline bool init_simplex() {
  int x = 0;
  rep(i, 1, m)
    if (a[i][0] < a[x][0]) x = i;
  if (sgn(a[x][0]) != -1) return 1;
  LD arr[N];
  rep(i, 1, n) arr[i] = a[0][i], a[0][i] = 0;
  n ++;
  a[0][n] = -1;
  rep(i, 1, m) a[i][n] = 1;
  pivot(x, n);
  simplex();
  if (sgn(a[0][0]) == -1) return 0;
  int pos = 0;
  rep(i, 1, m)
    if (!idB[i]) {
      rep(j, 1, n)
        if (sgn(a[i][j])) {pivot(i, j); break;}
    }
  rep(i, 1, n)
    if (!idN[i]) {pos = i; break;}
  if (pos != n) {
    idN[pos] = idN[n];
    rep(i, 1, m) a[i][pos] = a[i][n];
  }
  n --;
  rep(i, 1, n) a[0][i]=0;
  rep(i, 1, m)
    if(idB[i] <= n) {
      rep(j, 0, n) a[0][j] += a[i][j] * arr[idB[i]];
    }
  rep(i, 1, n)
    if(idN[i] <= n) a[0][i] += arr[idN[i]];
  return 1;
}

int main() {
  scanf("%d%d%d", &n, &m, &t);
  rep(i, 1, n) scanf("%Lf", &a[0][i]);
  rep(i, 1, m) {
    rep(j, 1, n) scanf("%Lf", &a[i][j]), a[i][j] *= -1;
    scanf("%Lf", &a[i][0]);
  }
  rep(i, 1, n) idN[i] = i;
  rep(i, 1, m) idB[i] = i + n;
  if (!init_simplex()) return puts("Infeasible"), 0;
  if (!simplex()) return puts("Unbounded"), 0;
  printf("%.9Lf\n", a[0][0]);
  if (!t) return 0;
  rep(i, 1, m) val[idB[i]] = a[i][0];
  rep(i, 1, n) printf("%.9Lf ", val[i]);
  puts("");
  return 0;
}
```
