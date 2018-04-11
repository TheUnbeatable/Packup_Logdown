# Codeforces 553E Kyoya and Train 
## Description
 给定一张$n(n\le 50)$个车站$m(m\le 100)$条边的有向图,一个人从$1$号到$n$号点,如果到达终点的时间超过了$t(t \le 20000)$,就要付出额外的$x(x\le 10^6)$的钱.走第$i$条边要花费$c_i$的钱,且走完这条边花费$j(j \le t)$的时间的概率是$p_{i,j}$,求期望花费.
 
 ## Idea
 首先容易发现到达一个点的时间不同,接下来做的决策也不一定相同,而图中可能存在环,无法直接递推DP.
 考虑$dp_{u,k}$表示在点$u$,当前时间是$k$,走到终点的期望代价,
<!--stackedit_data:
eyJoaXN0b3J5IjpbLTYwNzA5MzIzNl19
-->