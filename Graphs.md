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
把一条边的信息转化成一个点的信息储存,由这个点向该边原来的两个端点连边.这样转化就完全没有分类讨论和复杂的维护了(复杂度应该没问题).

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
给定n个点,可以在任意两点之间连边,使得1到n的最短路经过k条边,求方案数(n <= 100).
### Idea
由于边权为1,考虑BFS分层,层内可以任意连边,下一层的每一个点至少和上一层的一个点有边相连.
dp[i][j][k]表示在第i层有j个点,前i层有k个点.O(n)转移,总复杂度`$\mathcal{O(N^4)}$`.
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
所以线段树区间更新一下,总复杂度`$\mathcal{O(NlogN)}$`.
### [Code](https://github.com/TheUnbeatable/Packup_Logdown/blob/master/Codes/BZOJ2725%5BViolet%206%5D%E6%95%85%E4%B9%A1%E7%9A%84%E6%A2%A6.cpp)

## 计蒜之道2016复赛 百度地图的实时路况
### Description
给定一张图和它的邻接矩阵,定义`$d(x,y,z)$`表示从x到y不经过z的最短路,求`$P = \sum_{1 \leq x,y,z \leq n , x \neq y , y \neq z}{d(x,y,z)}$`.(n <= 300,2s).
### Idea
既然任意两点件都要计算最短路,那么Floyd就好了.但是考虑到不能支持快速删除操作和点集的合并,只能动态加入,所以对于每一个z到要把整个图重新计算一次,就是O(n^4).
考虑分治,处理`$z \in [L,R]$`的答案,先保存原图,用[mid+1,R]更新最短路,处理右边,然后恢复成原图,用[L,mid]更新最短路,递归处理右边.
这样每一层中总共加入n个点,复杂度`$\mathcal{O(N^3logN)}$`
### [Code](https://github.com/TheUnbeatable/Packup_Logdown/blob/master/Codes/%E8%AE%A1%E8%92%9C%E4%B9%8B%E9%81%932016%E5%A4%8D%E8%B5%9B%20%E7%99%BE%E5%BA%A6%E5%9C%B0%E5%9B%BE%E7%9A%84%E5%AE%9E%E6%97%B6%E8%B7%AF%E5%86%B5.cpp)

## BZOJ 1576 [Usaco2009 Jan]安全路经Travel
### Description
给定一张N点M边的无向图(N <= 1e5, M <= 2e5)，保证从1号点到每个点的最短路唯一。设1号点到i号点的最短路最后一条边为Ei，对每一个i求出1号点不经过Ei到达i的最短路。
### Idea
Sol 1
由于题目保证最短路唯一，所以最短路DAG即为最短路树.考虑每一条非树边(u,v,l)，且u与v在树上不是父子关系，设u和v在最短路树上的lca为x。
那么从1号点先走到u再到v再到[x,v]间的一个点y不会经过Ey，用dis[u] + l + dis[v] - dis[y]更新ans[y].用左偏树维护一下，支持一下整棵树加的tag，在LCA的地方减两次。
**记得pop的时候要把根的标记先下放!!!**
Sol 2
考虑到一定是长度小的非树边贡献更大，所以可以把非树边排序后并查集卷一下。
复杂度`$\mathcal{O(NlogN)}$`，后者常数更小.
### [Code](https://github.com/TheUnbeatable/Packup_Logdown/blob/master/Codes/LuoguP2934%20%5BUSACO09JAN%5D%E5%AE%89%E5%85%A8%E5%87%BA%E8%A1%8CSafeTravel.cpp)
