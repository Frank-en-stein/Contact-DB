const int MAX = 100000;

inline int abs(int k) {
	return (k < 0 ? -k : k);
}

struct lnode;
struct bstnode;

struct datanode {
	int val;
	lnode* listnode;
	bstnode* diffnode;
	bstnode* valnode;
};

struct bstnode {
	int val;
	datanode* data;
	bstnode* left;
	bstnode* right;
	bstnode* par;
	bstnode(datanode* d, int n, bstnode* l, bstnode* r) { data = d, val = n, left = l, right = r; }
};

class bst {
public:
	bstnode* root;
	bst() { root = nullptr; }

	bstnode* push(int n, datanode* d, bstnode* curr, bstnode* &res) {
		if (!curr) return res = new bstnode(d, n, nullptr, nullptr);
		if (n <= curr->val) { curr->left = push(n, d, curr->left, res); curr->left->par = curr; }
		else { curr->right = push(n, d, curr->right, res); curr->right->par = curr; }
		return curr;
	}
	datanode* remove_closest(int tr) {
		bstnode* curr = root;
		int d = MAX + 10;
		bstnode* r = root;
		while (curr) {
			if (abs(tr - curr->val) < d) {
				d = abs(tr - curr->val);
				r = curr;
			}
			if (tr < curr->val) curr = curr->left;
			else curr = curr->right;
		}
		if(r) return remove(r);
		else {
			int uu = 0;
			return nullptr;
		}
	}
	datanode* remove_smaller(int tr) {
		bstnode* curr = root;
		int d = MAX + 10;
		bstnode* r = nullptr;
		while (curr) {
			if (tr >= curr->val && (tr - curr->val) < d) {
				d = abs(tr - curr->val);
				r = curr;
			}
			if (tr < curr->val) curr = curr->left;
			else curr = curr->right;
		}
		if (r) return remove(r);
		else return nullptr;
	}
	datanode* remove_greater(int tr) {
		if (tr == 101) {
			int uu = 0;
		}
		bstnode* curr = root;
		int d = MAX + 10;
		bstnode* r = nullptr;
		while (curr) {
			if (tr <= curr->val && (tr - curr->val) < d) {
				d = abs(tr - curr->val);
				r = curr;
			}
			if (tr < curr->val) curr = curr->left;
			else curr = curr->right;
		}
		return remove(r);
	}
	datanode* remove(bstnode* tmp) {
		if (!tmp->left && !tmp->right) {
			if (tmp->par) {
				if (tmp->par->left == tmp) tmp->par->left = nullptr;
				else tmp->par->right = nullptr;
			}
			else root = nullptr;
			return tmp->data;
		}
		bstnode* curr = tmp;
		bstnode* pred = nullptr;
		if (curr->right) {
			pred = curr->right;
			while (pred->left) pred = pred->left;
			if (pred == curr->right) {
				pred->left = curr->left;
				if (curr->left) curr->left->par = pred;
			}
			else {
				pred->par->left = pred->right;
				if (pred->right) pred->right->par = pred->par;

				pred->right = curr->right;
				curr->right->par = pred;
				pred->left = curr->left;
				if (curr->left) curr->left->par = pred;
			}
		}
		else {
			pred = curr->left;
			if (curr->left) curr->left->par = curr->par;
		}
		if (curr->par) if (curr->par->left == curr) curr->par->left = pred; else curr->par->right = pred;
		pred->par = curr->par;
		if (curr == root) root = pred;

		return curr->data;
	}
};

struct lnode {
	int val;
	datanode* data;
	lnode* prev;
	lnode* next;
	lnode(datanode* d, int n, lnode* prv, lnode* nxt) {
		data = d;
		val = n;
		prev = prv;
		next = nxt;
	}
};

class list {
public:
	lnode *head, *tail;
	int count;
	list() { head = new lnode(nullptr, -1, nullptr, nullptr); tail = head; count = 0; }
	lnode* push(int n, datanode* d) {
		lnode* tmp = new lnode(d, n, tail, nullptr);
		tail->next = tmp;
		tail = tmp;
		return tmp;
	}
	datanode* pop() {
		lnode* tmp = head->next;
		datanode* r = tmp->data;
		remove(tmp);
		return r;
	}
	void remove(lnode* node) {
		if (node == tail) tail = node->prev;
		node->prev->next = node->next;
		if (node->next) node->next->prev = node->prev;
	}
};

list* fifo;
//bst* dsorted;
bst* vsorted;
int lasthead;
bool islookingleft;

void init(int track_size, int head) {
	fifo = new list();
	//dsorted = new bst();
	vsorted = new bst();
	lasthead = head;
	islookingleft = true;
}

void request(int track) {
	datanode* d = new datanode();
	d->val = track;
	d->listnode = fifo->push(track, d);
	//d->diffnode = dsorted->push(abs(lasthead - track), d);
	vsorted->root = vsorted->push(track, d, vsorted->root, d->valnode);
	vsorted->root->par = nullptr;
}

int fcfs() {
	datanode* r = fifo->pop();
	//dsorted->remove(r->diffnode);
	vsorted->remove(r->valnode);
	return lasthead = r->val;
}

int sstf() {
	datanode* r = vsorted->remove_closest(lasthead);
	fifo->remove(r->listnode);
	return lasthead = r->val;
}

int look() {
	datanode* r = (islookingleft ? vsorted->remove_smaller(lasthead) : vsorted->remove_greater(lasthead));
	if (!r) islookingleft = !islookingleft, r = (islookingleft ? vsorted->remove_smaller(lasthead) : vsorted->remove_greater(lasthead));
	fifo->remove(r->listnode);
	return lasthead = r->val;
}

int clook() {
	datanode* r = vsorted->remove_smaller(lasthead);
	if (!r) r = vsorted->remove_smaller(MAX + 100);
	fifo->remove(r->listnode);
	return lasthead = r->val;
}

