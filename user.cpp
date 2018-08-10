#define WIDTH	26
#define HEIGHT	99
#define LENGTH	200
const int THRESHOLD = 100000007;

struct lnode {
	lnode* next;
	int val;
};
bool isin[HEIGHT][WIDTH];

int compress(int l, int r) {
	return (l <<= 16) | r;
}

void decompress(int n, int &l, int &r) {
	r = n & (0x00ff);
	l = (n >> 16) & (0x00ff);
}

class Q {
public:
	lnode* head;
	lnode* tail;
	int count = 0;
	Q() {
		head = tail = nullptr;
		count = 0;
	}
	void push(int n) {
		int r, c;
		decompress(n, r, c);
		if(isin[r][c]) return;
		isin[r][c] = 1;
		lnode* tmp = new lnode();
		tmp->val = n;
		tmp->next = nullptr;
		if (tail) tail->next = tmp;
		else head = tmp;
		tail = tmp;
		count++;
	}
	lnode* pop() {
		if (!head) return nullptr;
		lnode* tmp = head;
		int r, c;
		decompress(head->val, r, c);
		isin[r][c] = 0;
		if (head) head = head->next;
		if (!head) tail = nullptr;
		count--;
		return tmp;
	}
	void del(int n) {
		lnode* curr = head;
		lnode* prev = head;
		while (curr) {
			if (curr->val == n) {
				count--;
				if (prev) prev->next = curr->next;
				else head = curr->next;
				return;
			}
			prev = curr;
			curr = curr->next;
		}
	}
	void del() {
		count = 0;
		lnode* tmp = head;
		while (head) {
			tmp = head;
			int r, c;
			decompress(head->val, r, c);
			isin[r][c] = 0;
			head = head->next;
			delete tmp;
		}
	}
};

bool vis[HEIGHT][WIDTH];
char exp[HEIGHT][WIDTH][LENGTH+1];
Q* undet = nullptr;

int str2num(char* str, int &len) {
	int res = 0;
	for (int i = str[0]=='-'; str[i] != 0 && str[i]>='0' && str[i]<='9'; i++) {
		res *= 10;
		res += str[i] - '0';
		len++;
	}
	if (str[0] == '-') res *= -1;
	return res;
}

void cpy(char a[], char* b) {
	int i = 0;
	for (i = 0; b[i] != 0; i++) a[i] = b[i];
	a[i] = 0;
}

bool eval(int r, int c, int value[HEIGHT][WIDTH]) {
	if (vis[r][c]) return false;
	bool flag = true;
	vis[r][c] = 1;
	char *eq = exp[r][c];
	bool isPlus = true;
	int expval = (eq[0] ? 0 : value[r][c]);
	//if (eq[0] == 0) flag = false;
	while (eq[0]) {
		if (eq[0] == '-' || eq[0] == '+') {
			isPlus = eq[0] == '+';
			eq++;
			continue;
		}
		int ev = 0;
		if (eq[0] >= 'A' && eq[0] <= 'Z') {
			int jmp = 0;
			int cc = eq[0] - 'A';
			int rr = str2num(++eq, jmp) - 1;
			eq += jmp;
			if (!eval(rr, cc, value)) {
				flag = false;
				undet->push(compress(rr, cc));
			}
			else ev = value[rr][cc];
		}
		else {
			int jmp = 0;
			ev = str2num(eq, jmp);
			eq += jmp;
		}
		expval = (isPlus ? expval + ev : expval - ev) % THRESHOLD;
	}
	if (flag) {
		value[r][c] = expval;
		if (isin[r][c]) undet->del(compress(r,c));
		isin[r][c] = 0;
	}
	vis[r][c] = 0;
	return flag;
}

void initTable() {
	/*static bool ini = false;
	if (!ini) {
		for (int i = 0; i < 26; i++) for (int j = 0; j < 99; j++) vis[i][j] = 0;
		ini = false;
	}*/
	if (undet) undet->del();
	undet = new Q();
	for (int i = 0; i < 99; i++) for (int j = 0; j < 26; j++) vis[i][j] = 0, exp[i][j][0] = 0, isin[i][j] = 0;
}

bool updateCell(int row, int col, char equation[LENGTH], int value[HEIGHT][WIDTH]) {
	if (equation[0] == '=') {
		equation++;
	}
	{
		//work on to free here
		cpy(exp[row][col], equation);
		bool flg = eval(row, col, value);
		if (!flg) undet->push(compress(row, col));
		int r, c;
		lnode* curr = undet->head;
		lnode* end = undet->tail;
		/*while (curr = undet->pop()) {
			decompress(curr->val, r, c);
			bool ff = eval(r, c, value);
			if (curr == end) {
				delete curr;
				break;
			}
			delete curr;
		}*/
		curr = undet->head;
		while (curr) {
			decompress(curr->val, r, c);
			value[r][c] = undet->count;
			curr = curr->next;
			flg = false;
		}
		return flg;
	}
}

//int main() {
//	int l, r, n;
//	n = compress(0, 1);
//	decompress(n, l, r);
//	return 0;
//}
