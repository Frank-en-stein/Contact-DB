#define WIDTH	26
#define HEIGHT	99
#define LENGTH	200
const int THRESHOLD = 100000007;

template <typename A, typename B>
struct pair {
	A r;
	B c;
	pair(A a, B b) {
		r = a;
		c = b;
	}
	bool equals(pair<A, B>* dummy) {
		return r == dummy->r && c == dummy->c;
	}
};

template <typename T>
struct lnode {
	lnode* next;
	lnode* prev;
	T data;
	lnode() { next = prev = nullptr; }
	lnode(T data, lnode<T>* prev, lnode<T>* next) { this->next = next; this->prev = prev; this->data = data; }
};

template <typename T>
class list {
public:
	lnode<T> *head;
	int count;
	list() {
		head = new lnode<T>();
		count = 0;
	}
	void insert(T n) {
		lnode<T> *tmp = new lnode<T>(n, head, head->next);
		if (head->next) head->next->prev = tmp;
		head->next = tmp;
		count++;
	}
	void del(lnode<T>* node) {
		node->prev->next = node->next;
		if (node->next) node->next->prev = node->prev;
		delete node;
		count--;
	}
	void del(T p) {
		lnode<T>* curr = head->next;
		while (curr) {
			if (curr->data->equals(p)) {
				del(curr);
				break;
			}
			curr = curr->next;
		}
	}
	~list() {
		count = 0;
		head = head->next;
		while (head) {
			lnode<T>* tmp = head;
			head = head->next;
			delete head;
		}
	}
};

void cpy(char a[], char* b) {
	int i = 0;
	for (; b[i] != 0; i++) a[i] = b[i];
	a[i] = 0;
}

int str2num(char* str, int& i) {
	int res = 0;
	for (i = 0; str[i] >= '0' && str[i] <= '9'; i++) {
		res *= 10;
		res += str[i] - '0';
	}
	return res;
}

list<pair<int, int>* >* undet;
char table[HEIGHT][WIDTH][LENGTH+1];
bool visited[HEIGHT][WIDTH];
bool isdet[HEIGHT][WIDTH];

bool eval(int r, int c, int value[HEIGHT][WIDTH]);

void updateDependents(int r, int c, int value[HEIGHT][WIDTH]) {
	if (visited[r][c]) return;
	char* eq = table[r][c];
	while (eq[0]) {
		if (eq[0] == '-' || eq[0] == '+') eq++;
		if (eq[0] >= 'A' && eq[0] <= 'Z') {
			int ll = 0;
			int cc = eq[0] - 'A';
			int rr = str2num(++eq, ll) - 1;
			eq += ll;
			if (!eval(rr, cc, value)) {
				if (isdet[rr][cc]) isdet[rr][cc] = false, undet->insert(new pair<int, int>(rr, cc));
			}
			else {
				if (!isdet[rr][cc]) isdet[rr][cc] = true, undet->del(new pair<int, int>(rr, cc));
			}
		}
		else eq++;
	}
}

bool eval(int r, int c, int value[HEIGHT][WIDTH]) {
	if (visited[r][c]) return false;
	visited[r][c] = true;
	char* eq = table[r][c];
	bool isPlus = true, flag = true;
	int expval = 0;
	//if (!eq[0]) flag = false;
	while (eq[0]) {
		if (eq[0] == '-' || eq[0] == '+') {
			if (eq[0] == '-') isPlus = false; else if (eq[0] == '+') isPlus = true;
			eq++;
			continue;
		}

		int val = 0, jmp = 0;
		if (eq[0] >= 'A' && eq[0] <= 'Z') {
			int ll = 0;
			int cc = eq[0] - 'A';
			int rr = str2num(++eq, ll) - 1;
			eq += ll;
			if (!eval(rr, cc, value)) {
				if (isdet[rr][cc]) isdet[rr][cc] = false, undet->insert(new pair<int, int>(rr, cc));
				flag = false;
			}
			else {
				if (!isdet[rr][cc]) isdet[rr][cc] = true, undet->del(new pair<int, int>(rr, cc));
			}
			val = value[rr][cc];
		}
		else val = str2num(eq, jmp);
		eq += jmp;
		if (!isPlus) val *= -1;
		expval += val;
		expval %= THRESHOLD;
	}
	if (flag) {
		value[r][c] = expval;
		//updateDependents(r, c, value);
	}
	visited[r][c] = false;
	return flag;
}

void initTable() {
	static int cas = 0;
	cas++;
	undet = new list<pair<int, int>* >();
	if (cas>1) for (int i = 0; i < HEIGHT; i++) for (int j = 0; j < WIDTH; j++) table[i][j][0] = 0;
	for (int i = 0; i < HEIGHT; i++) for (int j = 0; j < WIDTH; j++) visited[i][j] = false, isdet[i][j] = true;
}

bool updateCell(int row, int col, char equation[LENGTH], int value[HEIGHT][WIDTH]) {
	if (equation[0] == '=') equation++;
	cpy(table[row][col], equation);
	bool flg = eval(row, col, value);
	if (!flg) {
		if (isdet[row][col]) isdet[row][col] = false, undet->insert(new pair<int, int>(row, col));
	}
	else {
		if (!isdet[row][col]) isdet[row][col] = true, undet->del(new pair<int, int>(row, col));
	}
	lnode<pair<int, int>* >* curr = undet->head->next;
	while (curr) {
		int r = curr->data->r;
		int c = curr->data->c;
		value[r][c] = undet->count;
		curr = curr->next;
	}
	return undet->count == 0;
}
