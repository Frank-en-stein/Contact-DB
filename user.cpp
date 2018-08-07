//#include <iostream>
//using namespace std;

#define MOD 100007
#define BASE 257

typedef unsigned long long ull;

typedef enum
{
	NAME,
	NUMBER,
	BIRTHDAY,
	EMAIL,
	MEMO,
	NONE
} FIELD;

typedef struct
{
	int count;
	char str[20];
} RESULT;

////////////////////////////////////////////Utility///////////////////////////////////////////////////

void cpy(char a[], char *b) {
	int i;
	for (i = 0; b[i] != 0; i++) a[i] = b[i];
	a[i] = 0;
}

bool cmp(char* a, char* b) {
	for (int i = 0;; i++)
		if (a[i] == 0 && b[i] == 0) return true;
		else if (a[i] != b[i]) return false;
}

int hash(char* str) {
	ull hh = 0;
	for (int i = 0; str[i] != 0; i++) {
		char c = str[i];
		hh = (hh*BASE + c) % MOD;
	}
	return (int)hh;
}

////////////////////////////////////////////Definitions///////////////////////////////////////////////////
class lnode;
class hnode;
class record {
public:
	char data[5][31];

	lnode* lanchors[5];
	hnode* hanchors[5];

	record(char* name, char* number, char* birthday, char* email, char* memo) {
		cpy(data[NAME], name);
		cpy(data[NUMBER], number);
		cpy(data[BIRTHDAY], birthday);
		cpy(data[EMAIL], email);
		cpy(data[MEMO], memo);

		for (int i = 0; i < 5; i++) {
			lanchors[i] = nullptr; 
			hanchors[i] = nullptr;
		}
	}
};

class lnode {
public:
	lnode *prev, *next;
	record* data;
};

class list {
public:
	lnode* head;
	int count;
	list() {
		count = 0;
		head = nullptr;
	}
	~list() {
		lnode* curr = head;
		while (curr) {
			lnode* tmp = curr;
			curr = curr->next;
			//delete tmp;
		}
	}
	void insert(record* r) {
		lnode* tmp = new lnode();
		tmp->data = r;
		tmp->next = head;
		if (head) head->prev = tmp;
		head = tmp;
		tmp->prev = nullptr;
		this->count++;
	}
	void del() {
		count = 0;
		lnode* curr = head;
		while (curr) {
			lnode* tmp = curr;
			curr = curr->next;
			//delete tmp->data;
			//delete tmp;
		}
	}
	void soft_del() {
		count = 0;
		lnode* curr = head;
		while (curr) {
			lnode* tmp = curr;
			curr = curr->next;
			//delete tmp;
		}
	}
};

class hnode {
public:
	char key[31];
	hnode* next;
	hnode* prev;
	list* records;

	hnode(char* key, record* r, hnode* nxt, hnode* prv) {
		cpy(this->key, key);
		records = new list();
		records->insert(r);
		next = nxt;
		prev = prv;
		if (next) next->prev = this;
	}

	void del(hnode* &arr) {
		if (this->next) next->prev = this->prev;
		if (this->prev) prev->next = this->next;
		else arr = this->next;
	}
};

hnode *hashes[5][MOD];

void delh(hnode* hn) {
	if (hn) {
		delh(hn->next);
		return;
	}
	//delete hn;
}

hnode* findh(hnode* arr[], char* str, int hh) {
	hnode* curr = arr[hh];
	while (curr) {
		if (cmp(str, curr->key)) break;
		else curr = curr->next;
	}
	return curr;
}

hnode* inserth(hnode* arr[], record* r, char* str) {
	int hh = hash(str);
	hnode* tmp = findh(arr, str, hh);
	if (!tmp) {
		tmp = new hnode(str, r, arr[hh], nullptr);
		arr[hh] = tmp;
	}
	else {
		tmp->records->insert(r);
	}
	return tmp;
}

void remove_refs(lnode* curr, FIELD excpt) {
	while (curr) {
		record* r = curr->data;
		for (int i = 0; i < 5; i++) if (i != excpt) if (r->hanchors[i]) if (r->lanchors[i]) {
			if (r->lanchors[i]->next) r->lanchors[i]->next->prev = r->lanchors[i]->prev;
			if (!r->lanchors[i]->prev) if (r->hanchors[i]->records->head != r->lanchors[i]) {
				int u = 0;
			}
			if (r->lanchors[i]->prev) r->lanchors[i]->prev->next = r->lanchors[i]->next;
			else r->hanchors[i]->records->head = r->lanchors[i]->next; //to check
			r->hanchors[i]->records->count--;
		}
		curr = curr->next;
	}
}

void InitDB()
{
	for (int i = 0; i < MOD; i++) for(int j=0; j<5; j++) {
		delh(hashes[j][i]);
		hashes[j][i] = nullptr;
	}
}

void Add(char* name, char* number, char* birthday, char* email, char* memo)
{
	record* r = new record(name, number, birthday, email, memo);
	for (int j = 0; j < 5; j++) {
		r->hanchors[j] = inserth(hashes[j], r, r->data[j]);
		r->lanchors[j] = r->hanchors[j]->records->head;
	}
}

int Delete(FIELD field, char* str)
{
	int res = 0;
	int hh = hash(str);
	hnode* hn = findh(hashes[field], str, hh);
	if (hn) if (hn->records->count > 0) {
		res = hn->records->count;
		remove_refs(hn->records->head, field);
		hn->records->del();
		hn->del(hashes[field][hh]);
		hn = nullptr;
	}
	return res;
}

int Change(FIELD field, char* str, FIELD changefield, char* changestr)
{
	int res = 0;
	int hh = hash(str);
	hnode* hn = findh(hashes[field], str, hh);
	if (hn) if (hn->records->count > 0) {
		res = hn->records->count;
		lnode* curr = hn->records->head;
		int nhh = hash(changestr);
		while (curr) {
			cpy(curr->data->data[changefield], changestr);
			lnode* currln = curr->data->lanchors[changefield];
			hnode* currhn = curr->data->hanchors[changefield];

			if (currln->next) currln->next->prev = currln->prev;
			if (currln->prev) currln->prev->next = currln->next;
			else currhn->records->head = curr->next;
			currhn->records->count--;

			curr->data->hanchors[changefield] = inserth(hashes[changefield], curr->data, changestr);
			curr->data->lanchors[changefield] = curr->data->hanchors[changefield]->records->head;

			curr = curr->next;
		}
	}
	return res;
}

RESULT Search(FIELD field, char* str, FIELD returnfield)
{
	RESULT r;
	r.count = 0;
	r.str[0] = 0;

	int hh = hash(str);
	hnode* hn = findh(hashes[field], str, hh);
	if (hn) if (hn->records->count > 0) {
		r.count = hn->records->count;
		if (r.count == 1) {
			cpy(r.str, hn->records->head->data->data[returnfield]);
		}
	}

	return r;
}

//int main() {
//	int hh = hash("AsdADSsadsadas");
//	return 0;
//}
