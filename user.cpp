#define BUCKET_SIZE 1
#define MAX_PRICE  16000

struct node {
	node* next = nullptr;
	int val;
	int w;
};

class list {
public:
	node* head;
	node* tail;
	int count = 0;
	list() { head = new node(); tail = head; }
	void push(int val, int w) {
		node* tmp = new node();
		tmp->val = val;
		tmp->w = w;
		tail->next = tmp;
		tail = tmp;
		count++;
	}
	void insertAfter(node* nd, int val, int w) {
		node* tmp = new node();
		tmp->val = val;
		tmp->w = w;
		tmp->next = nd->next;
		nd->next = tmp;
		if (nd == tail) tail = tmp;
		count++;
	}
	void pop() {
		node* tmp = head->next;
		head->next = head->next->next;
		delete tmp;
		if (head->next == nullptr) tail = head;
		count--;
	}
	node* top() {
		return tail;
	}
	bool isEmpty() {
		return head->next == nullptr;
	}
	int countUntil(int price, int p1, int p2, int w1, int w2) {
		int cc = 0;
		node* curr = head->next;
		node* prev = head;
		while (curr) {
			if (curr->val < price) {
				if (curr->w >= w1 && curr->w <= w2) {
					cc++;
					if (curr->val >= p1 && curr->val <= p2) prev->next = curr->next, count--;
				}
				prev = curr;
				curr = curr->next;
			}
			else break;
		}
		return cc;
	}
	int countInRange(int w1, int w2, int p1, int p2) {
		int cc = 0;
		node* curr = head->next;
		node* prev = head;
		while (curr) {
			if (curr->w >= w1 && curr->w <= w2) {
				cc++;
				if (curr->val >= p1 && curr->val <= p2) prev->next = curr->next, count--;
			}
			prev = curr;
			curr = curr->next;
		}
		return cc;
	}
};

list* skiplist[MAX_PRICE / BUCKET_SIZE + 1];

void init()
{
	for (int i = 0; i < MAX_PRICE / BUCKET_SIZE + 1; i++) skiplist[i] = new list();
}

void add(int price, int weight)
{
	int idx = price / BUCKET_SIZE;
	node* curr = skiplist[idx]->head->next;
	node* prev = skiplist[idx]->head;
	while (1) {
		if (curr) {
			if (curr->val < price) {
				prev = curr;
				curr = curr->next;
			}
			else break;
		}
		else break;
	}
	skiplist[idx]->insertAfter(prev, price, weight);
}

int search(int price1, int price2, int weight1, int weight2)
{
	int lidx = price1 / BUCKET_SIZE;
	int ridx = price2 / BUCKET_SIZE;

	int count = 0;
	for (int i = lidx; i < ridx; i++) count += skiplist[i]->countInRange(weight1, weight2, price1, price2);
	count -= skiplist[lidx]->countUntil(price1, price1, price2, weight1, weight2);
	count += skiplist[ridx]->countUntil(price2 + 1, price1, price2, weight1, weight2);
	return count;
}
