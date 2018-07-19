typedef enum
{
	CMD_INIT,
	CMD_ADD,
	CMD_DELETE,
	CMD_CHANGE,
	CMD_SEARCH
} COMMAND;

typedef enum
{
	NAME,
	NUMBER,
	BIRTHDAY,
	EMAIL,
	MEMO
} FIELD;

typedef struct
{
	int count;
	char str[20];
} RESULT;

char * strcpy(char* str) {
	char* tmp = new char[21];
	int i = 0;
	for (i = 0; str[i] != '\0' && i<20; i++) tmp[i] = str[i];
	tmp[i] = 0;
	return tmp;
}

class TRIE;
class LIST;
class listNode;
class trieNode;

struct record {
	char* NAME;
	char* NUMBER;
	char* BIRTHDAY;
	char* EMAIL;
	char* MEMO;

	trieNode* name_tnode;
	trieNode* number_tnode;
	trieNode* dob_tnode;
	trieNode* email_tnode;
	trieNode* memo_tnode;

	listNode* name_plist;
	listNode* number_plist;
	listNode* dob_plist;
	listNode* email_plist;
	listNode* memo_plist;
};

#define mapped_char 38
int map_len = 0, map[128];

////////////////////list stuff/////////////////////////
class listNode {
public:
	listNode * next;
	record * value;
};

class List {
public:
	listNode * root_next;
	List(){
		root_next = nullptr;
	}
	void insert(record * rec) {
		listNode * tmp = new listNode();
		tmp->value = rec;
		tmp->next = root_next;
		root_next = tmp;
	}
	void del() {
		listNode * curr = root_next;
		root_next = nullptr;
		while (curr != nullptr) {
			listNode * tmp = curr;
			curr = curr->next;
			delete tmp->value;
			tmp->value = nullptr;
			delete tmp;
		}
	}
	bool del(record * r) {
		listNode * curr = root_next;
		listNode * prev = root_next;
		while (curr != nullptr) {
			if (curr->value == r) {
				if (prev == curr) prev = curr->next;
				else prev->next = curr->next;
				return true;
			}
			prev = curr;
			curr = curr->next;
		}
		return false;
	}
	void update(FIELD changefield, char* changestr) {
		listNode * curr = root_next;
		while (curr != nullptr) {
			record * rr = curr->value;
			if (changefield == NAME) {
				char* tmp = new char[20];
				int i = 0;
				for (i = 0; changestr[i] != '\0'; i++) tmp[i] = changestr[i];
				tmp[i] = 0;
				rr->NAME = tmp;
			}
			else if (changefield == NUMBER) {
				char* tmp = new char[20];
				int i = 0;
				for (i = 0; changestr[i] != '\0'; i++) tmp[i] = changestr[i];
				tmp[i] = 0;
				rr->NUMBER = tmp;
			}
			else if (changefield == EMAIL) {
				char* tmp = new char[20];
				int i = 0;
				for (i = 0; changestr[i] != '\0'; i++) tmp[i] = changestr[i];
				tmp[i] = 0;
				rr->EMAIL = tmp;
			}
			else if (changefield == BIRTHDAY) {
				char* tmp = new char[20];
				int i = 0;
				for (i = 0; changestr[i] != '\0'; i++) tmp[i] = changestr[i];
				tmp[i] = 0;
				rr->BIRTHDAY = tmp;
			}
			else if (changefield == MEMO) {
				char* tmp = new char[20];
				int i = 0;
				for (i = 0; changestr[i] != '\0'; i++) tmp[i] = changestr[i];
				tmp[i] = 0;
				rr->MEMO = tmp;
			}
			curr = curr->next;
		}
	}
};

////////////////////list stuff ends/////////////////////////

////////////////////trie stuff/////////////////////////
class trieNode {
public:
	trieNode * children[mapped_char];
	int end_count;
	List * records;

	trieNode() {
		for (int i = 0; i < mapped_char; i++) children[i] = nullptr;
		end_count = 0;
		records = new List();
	}
};

class TRIE {
public:
	trieNode * root;
	//trieNode * all_obj[1000005];
	int obj_idx = 0;
	TRIE() {
		root = new trieNode();
	}
	~TRIE() {
		/*for (int i = 0; i < obj_idx; i++) {
		delete all_obj[i];
		}*/
		//delete all_obj;
	}
	trieNode* insert(char * str, record * r) {
		trieNode * curr = root;
		trieNode * last = root;
		int i;
		for (i = 0; str[i] != '\0' && i<20; i++) {
			if (curr->children[map[str[i]]] == nullptr) {
				curr->children[map[str[i]]] = new trieNode();
				//all_obj[obj_idx++] = curr->children[map[str[i]]];
			}
			last = curr;
			curr = curr->children[map[str[i]]];
		}
		last->end_count++;
		last->records->insert(r);
		return last;
	}
	int del(trieNode * curr) {
		int res = curr->end_count;
		curr->end_count = 0;
		curr->records->del();
		delete curr->records;
		curr->records = new List();
		return res;
	}
	trieNode * find(char * str) {
		trieNode * curr = root;
		trieNode * last = root;
		int i;
		for (i = 0; str[i] != 0; i++) {
			if (curr->children[map[str[i]]] == nullptr) {
				break;
			}
			last = curr;
			curr = curr->children[map[str[i]]];
		}
		if (last->end_count >0) return last;
		else return nullptr;
	}
};
////////////////////trie stuff ends/////////////////////////

TRIE * nameTrie;
TRIE * emailTrie;
TRIE * dobTrie;
TRIE * memoTrie;
TRIE * numberTrie;

extern void InitDB() {
	delete nameTrie;
	delete emailTrie;
	delete dobTrie;
	delete memoTrie;
	delete numberTrie;

	nameTrie = new TRIE();
	emailTrie = new TRIE();
	dobTrie = new TRIE();
	memoTrie = new TRIE();
	numberTrie = new TRIE();

	if (map_len != 0) return;
	//for (int i = 'A'; i <= 'Z'; i++) map[i] = map_len++;
	for (int i = 'a'; i <= 'z'; i++) map[i] = map_len++;
	for (int i = '0'; i <= '9'; i++) map[i] = map_len++;
	map['.'] = map_len++;
	map['@'] = map_len++;
	//map['_'] = map_len++;
	//map['-'] = map_len++;
	//map[' '] = map_len++;
	//map[','] = map_len++;
	//map['\''] = map_len++;
	//map[':'] = map_len++;
	//map['('] = map_len++;
	//map[')'] = map_len++;
	//map['/'] = map_len++;
}
extern void Add(char* name, char* number, char* birthday, char* email, char* memo) {
	record * r = new record;
	r->NAME = strcpy(name);
	r->NUMBER = strcpy(number);
	r->BIRTHDAY = strcpy(birthday);
	r->EMAIL = strcpy(email);
	r->MEMO = strcpy(memo);

	r->name_tnode = nameTrie->insert(name, r);
	r->number_tnode = numberTrie->insert(number, r);
	r->dob_tnode = dobTrie->insert(birthday, r);
	r->email_tnode = emailTrie->insert(email, r);
	r->memo_tnode = memoTrie->insert(memo, r);

	r->name_plist = r->number_plist = r->dob_plist = r->email_plist = r->memo_plist = nullptr;

	if (r->name_tnode->records->root_next->next != nullptr) r->name_tnode->records->root_next->next->value->name_plist = r->name_tnode->records->root_next;
	if (r->number_tnode->records->root_next->next != nullptr) r->number_tnode->records->root_next->next->value->number_plist = r->number_tnode->records->root_next;
	if (r->dob_tnode->records->root_next->next != nullptr) r->dob_tnode->records->root_next->next->value->dob_plist = r->dob_tnode->records->root_next;
	if (r->email_tnode->records->root_next->next != nullptr) r->email_tnode->records->root_next->next->value->email_plist = r->email_tnode->records->root_next;
	if (r->memo_tnode->records->root_next->next != nullptr) r->memo_tnode->records->root_next->next->value->memo_plist = r->memo_tnode->records->root_next;
}
extern int Delete(FIELD field, char* str) {
	int res = 0;
	if (field == NAME) {
		trieNode * tnode = nameTrie->find(str);
		if (tnode == nullptr) return 0;
		listNode* curr = tnode->records->root_next;
		trieNode* tmp = nullptr;
		while (curr != nullptr) {
			record* r = curr->value;
			tmp = r->number_tnode;
			if (r->number_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->number_plist->next = r->number_plist->next->next;
			tmp->end_count--;

			tmp = r->email_tnode;
			if (r->email_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->email_plist->next = r->email_plist->next->next;
			tmp->end_count--;

			tmp = r->dob_tnode;
			if (r->dob_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->dob_plist->next = r->dob_plist->next->next;
			tmp->end_count--;

			tmp = r->memo_tnode;
			if (r->memo_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->memo_plist->next = r->memo_plist->next->next;
			tmp->end_count--;

			curr = curr->next;
		}
		res = nameTrie->del(tnode);
	}
	else if (field == NUMBER) {
		trieNode * tnode = numberTrie->find(str);
		if (tnode == nullptr) return 0;
		listNode* curr = tnode->records->root_next;
		trieNode* tmp = nullptr;
		while (curr != nullptr) {
			record* r = curr->value;
			tmp = r->name_tnode;
			if (r->name_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->name_plist->next = r->name_plist->next->next;
			tmp->end_count--;

			tmp = r->email_tnode;
			if (r->email_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->email_plist->next = r->email_plist->next->next;
			tmp->end_count--;

			tmp = r->dob_tnode;
			if (r->dob_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->dob_plist->next = r->dob_plist->next->next;
			tmp->end_count--;

			tmp = r->memo_tnode;
			if (r->memo_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->memo_plist->next = r->memo_plist->next->next;
			tmp->end_count--;

			curr = curr->next;
		}
		res = numberTrie->del(tnode);
	}
	else if (field == EMAIL) {
		trieNode * tnode = emailTrie->find(str);
		if (tnode == nullptr) return 0;
		listNode* curr = tnode->records->root_next;
		trieNode* tmp = nullptr;
		while (curr != nullptr) {
			record* r = curr->value;
			tmp = r->number_tnode;
			if (r->number_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->number_plist->next = r->number_plist->next->next;
			tmp->end_count--;

			tmp = r->name_tnode;
			if (r->name_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->name_plist->next = r->name_plist->next->next;
			tmp->end_count--;

			tmp = r->dob_tnode;
			if (r->dob_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->dob_plist->next = r->dob_plist->next->next;
			tmp->end_count--;

			tmp = r->memo_tnode;
			if (r->memo_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->memo_plist->next = r->memo_plist->next->next;
			tmp->end_count--;

			curr = curr->next;
		}
		res = emailTrie->del(tnode);
	}
	else if (field == BIRTHDAY) {
		trieNode * tnode = dobTrie->find(str);
		if (tnode == nullptr) return 0;
		listNode* curr = tnode->records->root_next;
		trieNode* tmp = nullptr;
		while (curr != nullptr) {
			record* r = curr->value;
			tmp = r->number_tnode;
			if (r->number_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->number_plist->next = r->number_plist->next->next;
			tmp->end_count--;

			tmp = r->email_tnode;
			if (r->email_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->email_plist->next = r->email_plist->next->next;
			tmp->end_count--;

			tmp = r->name_tnode;
			if (r->name_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->name_plist->next = r->name_plist->next->next;
			tmp->end_count--;

			tmp = r->memo_tnode;
			if (r->memo_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->memo_plist->next = r->memo_plist->next->next;
			tmp->end_count--;

			curr = curr->next;
		}
		res = dobTrie->del(tnode);
	}
	else if (field == MEMO) {
		trieNode * tnode = memoTrie->find(str);
		if (tnode == nullptr) return 0;
		listNode* curr = tnode->records->root_next;
		trieNode* tmp = nullptr;
		while (curr != nullptr) {
			record* r = curr->value;
			tmp = r->number_tnode;
			if (r->number_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->number_plist->next = r->number_plist->next->next;
			tmp->end_count--;

			tmp = r->email_tnode;
			if (r->email_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->email_plist->next = r->email_plist->next->next;
			tmp->end_count--;

			tmp = r->dob_tnode;
			if (r->dob_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->dob_plist->next = r->dob_plist->next->next;
			tmp->end_count--;

			tmp = r->name_tnode;
			if (r->name_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
			else r->name_plist->next = r->name_plist->next->next;
			tmp->end_count--;

			curr = curr->next;
		}
		res = memoTrie->del(tnode);
	}
	return res;
}
extern int Change(FIELD field, char* str, FIELD changefield, char* changestr) {
	trieNode * res = nullptr;
	if (field == NAME) {
		res = nameTrie->find(str);
	}
	else if (field == NUMBER) {
		res = numberTrie->find(str);
	}
	else if (field == EMAIL) {
		res = emailTrie->find(str);
	}
	else if (field == BIRTHDAY) {
		res = dobTrie->find(str);
	}
	else if (field == MEMO) {
		res = memoTrie->find(str);
	}

	if (res == nullptr) return 0;
	//search complete

	listNode * curr = res->records->root_next;
	int cc = 0;
	while (curr != nullptr) {
		record * rr = curr->value;
		if (changefield == NAME) {
			trieNode* tmp = rr->name_tnode;
			if (tmp != nullptr && tmp->end_count>0) {
				if (rr->name_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
				else rr->name_plist->next = rr->name_plist->next->next;
				tmp->end_count--;

				rr->NAME = strcpy(changestr);
				nameTrie->insert(rr->NAME, rr);
				cc++;
			}
		}
		else if (changefield == NUMBER) {
			trieNode* tmp = rr->number_tnode;
			if (tmp != nullptr && tmp->end_count>0) {
				if (rr->number_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
				else rr->number_plist->next = rr->number_plist->next->next;
				tmp->end_count--;

				rr->NUMBER = strcpy(changestr);
				numberTrie->insert(rr->NUMBER, rr);
				cc++;
			}
		}
		else if (changefield == EMAIL) {
			trieNode* tmp = rr->email_tnode;
			if (tmp != nullptr && tmp->end_count>0) {
				if (rr->email_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
				else rr->email_plist->next = rr->email_plist->next->next;
				tmp->end_count--;

				rr->EMAIL = strcpy(changestr);
				emailTrie->insert(rr->EMAIL, rr);
				cc++;
			}
		}
		else if (changefield == BIRTHDAY) {
			trieNode* tmp = rr->dob_tnode;
			if (tmp != nullptr && tmp->end_count>0) {
				if (rr->dob_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
				else rr->dob_plist->next = rr->dob_plist->next->next;
				tmp->end_count--;

				rr->BIRTHDAY = strcpy(changestr);
				dobTrie->insert(rr->BIRTHDAY, rr);
				cc++;
			}
		}
		else if (changefield == MEMO) {
			trieNode* tmp = rr->memo_tnode;
			if (tmp != nullptr && tmp->end_count>0) {
				if (rr->memo_plist == nullptr) tmp->records->root_next = tmp->records->root_next->next;
				else rr->memo_plist->next = rr->memo_plist->next->next;
				tmp->end_count--;

				rr->MEMO = strcpy(changestr);
				memoTrie->insert(rr->MEMO, rr);
				cc++;
			}
		}
		curr = curr->next;
	}

	return cc;
}
extern RESULT Search(FIELD field, char* str, FIELD returnfield) {
	trieNode * res = nullptr;
	if (field == NAME) {
		res = nameTrie->find(str);
	}
	else if (field == NUMBER) {
		res = numberTrie->find(str);
	}
	else if (field == EMAIL) {
		res = emailTrie->find(str);
	}
	else if (field == BIRTHDAY) {
		res = dobTrie->find(str);
	}
	else if (field == MEMO) {
		res = memoTrie->find(str);
	}
	RESULT r;
	r.count = 0;
	if (res == nullptr) {
		return r;
	}
	r.count = res->end_count;
	if (r.count == 1) {
		char * tmp = nullptr;
		if (returnfield == NAME) {
			tmp = res->records->root_next->value->NAME;
		}
		else if (returnfield == NUMBER) {
			tmp = res->records->root_next->value->NUMBER;
		}
		else if (returnfield == EMAIL) {
			tmp = res->records->root_next->value->EMAIL;
		}
		else if (returnfield == BIRTHDAY) {
			tmp = res->records->root_next->value->BIRTHDAY;
		}
		else if (returnfield == MEMO) {
			tmp = res->records->root_next->value->MEMO;
		}
		if (tmp == nullptr) return r;
		int i = 0;
		for (i = 0; tmp[i] != '\0' && i<20; i++) r.str[i] = tmp[i];
		r.str[i] = 0;
	}
	return r;
}
