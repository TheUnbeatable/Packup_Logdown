# Link-Cut-Tree
图论处理中的数据结构利器.
它是用Splay维护轻重链剖分,Splay的关键值为深度.
父亲不指向轻儿子只指向重儿子，儿子都指向父亲.(用这个判断是否到了链顶)
<!--more-->
### Splay(x)
使得x成为这条链的splay的根节点,它在splay中的父亲指向链顶在原树中的父亲.
**rotate的时候,如果father已经是当前链的splay的根节点,不要把grandfather的孩子指向x!**
### Access(x)
将一个点与原先的重儿子切断,并使这个原树上点到根路径上的边全都变为重边.
也就是提取出一个点到根的路径. **Access之后x不一定是Splay的根节点**
### Make_Root(x)
首先Access,然后链的两端是x和root,只需要将整棵树翻转就可以了.
### Link(x,y)
首先Make_root(x),然后Access(y),Splay(y),**不然维护的信息就会出错**.
然后把x->y设为一条虛边,更新信息.
### Cut(x,y)
首先Make_root(x),然后Access(y),Splay(y),x就在y的左儿子了,把这条边删掉,更新信息.
## 维护信息
### 1.子树信息
以维护子树大小为例(点权为1,运算为加法).([BJOI2014 大融合](https://loj.ac/problem/2230)).
一个点的子树大小为它重儿子和轻儿子的大小和.每个点维护一个val等于所有轻儿子的答案.
那么一个点的答案就是:它所在链中深度比它大的点的数量 + 它们的val值之和.
注意到一个点val改变只会因为轻儿子改变而改变.也就是只有Access,Link与Cut的时候要更新val.
```cpp
  inline void access(int x) {
    for (int y = 0; x; y = x, x = fat) {
      splay(x);
      val[x] += em[rc] + sz[rc] - em[y] - sz[y];
      rc = y; Up(x);
    }
  }
  inline void link(int x, int y) {
    make_root(x); access(y); splay(y);
    fat = y; val[y] += em[x] + sz[x];
  }
```
### 2.边信息
有一种干干净净的方法,把一条边的信息转化成一个点的信息储存,由这个点向该边原来的两个端点连边.这样转化就完全没有分类讨论和复杂的维护了.

---
# Practices
## UOJ #3 NOI2014 魔法森林
### Description
给定n点m边的图,每条边有两个权值a和b,求一条1到n的路径,最小化路径上所有边的MaxA+MaxB,输出这个值.(n <= 5e4, m <= 1e5)
### Idea
按照a从小到大依次加入边,维护以b为权值的最小生成树.用(当前a)+(1到n路径上的最大权值)更新答案.
一条边(u,v,a,b),若u,v不联通就直接加入到树中;若u到v路径上最大边权大于b,就把最大的边删掉,然后把这一条加进去.
### [Code](http://uoj.ac/submission/224027)

## HihoCoder 1147 时空阵
### Description
给定n个点,可以在任意两点之间连边,使得1到n的最短路经过k条边,求方案数.
### Idea
由于边权为1,考虑BFS分层,层内可以任意连边,下一层的每一个点至少和上一层的一个点有边相连.
dp[i][j][k]表示在第i层有j个点,前i层有k个点.O(n)转移.
### [Code](https://cn.vjudge.net/solution/12639894)

## BZOJ 2725 [Violet 6]故乡的梦
### Description
给定的n点m边的无向图(n, m <= 2e5)以及起/终点s和t,q次询问呢(q <= 2e5),每次求删除图中一条边后的最短路,若s,t不连通则输出Infinity.
### Idea
首先可以求出一条最短路path,如果删除的是该路径外的边,那么不影响答案.
由于每次只是删除一条边,所以只要预处理出删除该路径上的每一条边后的答案.
首先当然要求出s到每个点的距离dis数组和t的Dis数组,复杂度O(NlogN).
处理出每一个点x,从s到x最短路最早可以从path的哪一个点离开le[x],以及从t到x的最短路最早可以从path的哪一个点离开ri[x].
考虑每一条不在path上的边E(u,v,l),le[u]到ri[v]的边删除都可以用dis[u] + Dis[v] + l来替代.
可以发现,如果不在le[u]到ri[v]的范围内,E(u,v,l)所能带来的最短替代路径一定不是最优的.
所以线段树区间更新一下,总复杂度O(NlogN).
