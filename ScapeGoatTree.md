### 复杂度
>+ 树深
>$$  \alpha ^ {d(x)} · size(SGT) > size(x) $$
>$$  d(x) < log_{1 \over \alpha}size(SGT) $$
> 节点x的势能为 $\phi(x) = |size[lc] - size[rc]|$
> 整棵树的总势能$\Phi(T)$为
> $$ \Phi(T) = \sum_{x ∈ T} \phi(x) $$

```cpp Template of ScapeGoat Tree
#include <queue>
using namespace std;
class Scapegoat_Tree{
	private :
		int ch[101000][2], fa[101000], sz[101000], key[101000], rep[101000];
		int cnt, len, root, tmp[101000];
		double a;
		queue<int> Q;
	public :
		inline void init(double _a) {
			a = _a;
			cnt = root = 0;
			while (!Q.empty()) Q.pop();
		}
		inline int newnode() {
			int ret;
			if (Q.empty()) ret = ++ cnt;
			else ret = Q.front(), Q.pop();
			fa[ret] = ch[ret][0] = ch[ret][1] = rep[ret] = key[ret] = sz[ret] = 0;
			return ret;
		}
		inline bool balance(int x) {
			return ((double)max(sz[ch[x][0]], sz[ch[x][1]]) <= (double)sz[x] * a);
		}
		inline int pos_max_element() {
			int p = root;
			while (ch[p][1]) p = ch[p][1];
			return p;
		}
		inline int pos_min_element() {
			int p = root;
			while (ch[p][0]) p = ch[p][0];
			return p;
		}
		inline void output(int x) {
			if (ch[x][0]) output(ch[x][0]);
			tmp[++ len] = x;
			if (ch[x][1]) output(ch[x][1]);
		}
		inline int build(int l, int r) {
			if (l > r) return 0;
			int mid = (l + r) >> 1, p = tmp[mid];
			fa[ch[p][0] = build(l, mid - 1)] = p;
			fa[ch[p][1] = build(mid + 1, r)] = p;
			sz[p] = sz[ch[p][0]] + sz[ch[p][1]] + rep[p];
			return p;
		}
		inline void rebuild(int x) {
			len = 0; output(x);
			int f = fa[x], ws = (x == ch[f][1]), u;
			u = build(1, len);
			fa[u] = f;
			if (f) ch[f][ws] = u;
			if (x == root) root = u;
		}
		inline void insert(int x) {
			if (!root) {
				key[root = newnode()] = x;
				rep[root] = sz[root] = 1;
				return;
			}
			int p = root;
			while (x != key[p] && ch[p][key[p] < x]) {
				sz[p] ++;
				p = ch[p][key[p] < x];
			}
			sz[p] ++;
			if (x == key[p]) rep[p] ++;
			else {
				int tp = newnode();
				fa[tp] = p;
				ch[p][key[p] < x] = tp;
				rep[tp] = sz[tp] = 1;
				key[tp] = x;
				p = tp;
			}
			int rec = 0;
			for (int i = p; i; i = fa[i])
				if (!balance(i)) rec = i;
			if (rec) rebuild(rec);
		}
		inline int find(int x) {
			int p = root;
			while (ch[p][key[p] < x] && key[p] != x) p = ch[p][key[p] < x];
			return p;
		}
		inline void erase(int x) {
			int p = find(x), rec = 0;
			if (rep[p] > 1) {
				rep[p] --;
				sz[p] --;
				for (int i = fa[p]; i; i = fa[i]) {
					sz[i] --;
					if (!balance(i)) rec = i;
				}
				if (rec) rebuild(rec);
			}
			else if (ch[p][0] && ch[p][1]) {
				int tp = ch[p][0];
				while (ch[tp][1]) tp = ch[tp][1];
				Q.push(tp);
				key[p] = key[tp];
				rep[p] = rep[tp];
				int son = ch[tp][0], ws = (ch[fa[tp]][1] == tp);
				fa[ch[fa[tp]][ws] = son] = fa[tp];
				for (int i = fa[tp]; i != p; i = fa[i]) {
					sz[i] -= rep[tp];
					if (!balance(i)) rec = i;
				}
				for (int i = p; i; i = fa[i]) {
					sz[i] --;
					if (!balance(i)) rec = i;
				}
				if (rec) rebuild(rec);
			}
			else {
				int son = (ch[p][0] ? ch[p][0] : ch[p][1]), ws = (ch[fa[p]][1] == p);
				fa[ch[fa[p]][ws] = son] = fa[p];
				if (p == root) root = son;
				Q.push(p);
				for (int i = fa[p]; i; i = fa[i]) {
					sz[i] --;
					if (!balance(i)) rec = i;
				}
				if (rec) rebuild(rec);
			}
		}
		inline int size() {
			return sz[root];
		}
		inline int pred(int x) {
			int p = root, ret = (int)-2e9 - 1e8;
			while (ch[p][key[p] < x]) {
				if (key[p] < x) ret = max(ret, key[p]);
				p = ch[p][key[p] < x];
			}
			if (key[p] < x) ret = max(ret, key[p]);
			return ret;
		}
		inline int succ(int x) {
			int p = root, ret = (int)2e9 + 1e8;
			while (ch[p][key[p] <= x]) {
				if (key[p] > x) ret = min(ret, key[p]);
				p = ch[p][key[p] <= x];
			}
			if (key[p] > x) ret = min(ret, key[p]);
			return ret;
		}
		inline int rank_ord(int x) {
			int p = root, ret = 0;
			while (ch[p][key[p] < x] && key[p] != x) {
				if (key[p] < x) ret += rep[p] + sz[ch[p][0]];
				p = ch[p][key[p] < x];
			}
			if (key[p] == x) ret += sz[ch[p][0]];
			return ret + 1;
		}
		inline int find_kth(int k) {
			if (k > sz[root]) return key[pos_max_element()];
			int p = root;
			while (p) {
				if (sz[ch[p][0]] < k && k <= sz[ch[p][0]] + rep[p]) return key[p];
				if (k <= sz[ch[p][0]]) p = ch[p][0];
				else {
					k -= sz[ch[p][0]] + rep[p];
					p = ch[p][1];
				}
			}
			return key[p];
		}
};
```
