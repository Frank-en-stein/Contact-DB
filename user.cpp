#include <malloc.h>

inline int kabs(int n) {
	return (n < 0 ? -n : n);
}

const int MAX = 100000;
int count = 1000000009;

typedef struct node {
	int val, prior, size;
	struct node *l, *r;
}node;
typedef node* pnode;
int sz(pnode t) {
	return t ? t->size : 0;
}
void upd_sz(pnode t) {
	if (t) t->size = sz(t->l) + 1 + sz(t->r);
}
void split(pnode t, pnode &l, pnode &r, int key) {
	if (!t) l = r = nullptr;
	else if (t->val <= key) split(t->r, t->r, r, key), l = t; //elem=key comes in l
	else split(t->l, l, t->l, key), r = t;
	upd_sz(t);
}
void merge(pnode &t, pnode l, pnode r) {
	if (!l || !r) t = l ? l : r;
	else if (l->prior > r->prior)merge(l->r, l->r, r), t = l;
	else merge(r->l, l, r->l), t = r;
	upd_sz(t);
}

void insert(pnode &t, pnode it) {
	if (!t) t = it;
	else if (it->prior>t->prior) split(t, it->l, it->r, it->val), t = it;
	else insert(t->val <= it->val ? t->r : t->l, it);
	upd_sz(t);
}
void erase(pnode &t, int key) {
	if (!t)return;
	else if (t->val == key) { pnode temp = t; merge(t, t->l, t->r); free(temp); }
	else erase(t->val<key ? t->r : t->l, key);
	upd_sz(t);
}
pnode makenode(int val) {
	pnode ret = (pnode)malloc(sizeof(node));
	ret->val = val; ret->size = 1; ret->prior = count--; ret->l = ret->r = NULL;
	return ret;
}

pnode root;
bool lookingLeft;
int lasthead;

int remove_closest(int tr) {
	pnode curr = root;
	int d = MAX + 10;
	pnode r = nullptr;
	while (curr) {
		if (kabs(tr - curr->val) < d || (kabs(tr - curr->val) == d && curr->val < r->val)) {
			d = kabs(tr - curr->val);
			r = curr;
		}
		if (tr < curr->val) curr = curr->l;
		else curr = curr->r;
	}
	if (r) {
		int res = r->val;
		erase(root, res);
		return res;
	}
	else {
		return MAX+10;
	}
}
int remove_smaller(int tr) {
	pnode curr = root;
	int d = MAX + 10;
	pnode r = nullptr;
	while (curr) {
		if (tr >= curr->val && (tr - curr->val) < d) {
			d = kabs(tr - curr->val);
			r = curr;
		}
		if (tr < curr->val) curr = curr->l;
		else curr = curr->r;
	}
	if (r) {
		int res = r->val;
		erase(root, res);
		return res;
	}
	else {
		return MAX + 10;
	}
}
int remove_greater(int tr) {
	if (tr == 101) {
		int uu = 0;
	}
	pnode curr = root;
	int d = MAX + 10;
	pnode r = nullptr;
	while (curr) {
		if (tr <= curr->val && (tr - curr->val) < d) {
			d = kabs(tr - curr->val);
			r = curr;
		}
		if (tr < curr->val) curr = curr->l;
		else curr = curr->r;
	}
	if (r) {
		int res = r->val;
		erase(root, res);
		return res;
	}
	else {
		return MAX + 10;
	}
}

void init(int track_size, int head) {
	count = 1000000009;
	root = nullptr;
	lookingLeft = true;
	lasthead = head;
}

void request(int track) {
	pnode tmp = makenode(track);
	insert(root, tmp);
}

int fcfs() {
	int r = root->val;
	erase(root, r);
	return lasthead = r;
}

int sstf() {
	int r = remove_closest(lasthead);
	return lasthead = r;
}

int look() {
	int r = (lookingLeft ? remove_smaller(lasthead) : remove_greater(lasthead));
	if (r == MAX + 10) {
		lookingLeft = !lookingLeft;
		r = (lookingLeft ? remove_smaller(lasthead) : remove_greater(lasthead));
	}
	return lasthead = r;
}

int clook() {
	int r = remove_smaller(lasthead);
	if (r == MAX + 10) {
		r = remove_smaller(MAX+10);
	}
	return lasthead = r;
}

