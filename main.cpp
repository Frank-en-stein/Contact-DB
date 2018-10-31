#include <stdio.h>

#define MAX_PRICE  16000
#define MAX_WEIGTH 4000
#define MAX_CALLS 100000
#define MAX_DP 100
#define MAX_DW 100

typedef unsigned int uint;
static uint seed;
static uint mrand(uint num)
{
	seed = seed * 1103515245 + 37209;
	return ((seed >> 8) % num);
}

void init();
void add(int price, int weight);
int search(int price1, int price2, int weight1, int weight2);

int test(int ncmd)
{
	init();
	int sum = 0;
	int cmd;

	for (int i = 0; i < ncmd; ++i)
	{
		cmd = mrand(8);
		if (cmd == 0) // search
		{
			int p1 = mrand(MAX_PRICE);
			int p2 = p1 + mrand(MAX_DP);
			if (p2 >= MAX_PRICE) p2 = MAX_PRICE - 1;

			int w1 = mrand(MAX_WEIGTH);
			int w2 = w1 + mrand(MAX_DW);
			if (w2 >= MAX_WEIGTH) w2 = MAX_WEIGTH - 1;

			sum += search(p1, p2, w1, w2);
			//printf("%d\n", i);
		}
		else // add
		{
			int w = mrand(MAX_WEIGTH);
			int p = mrand(MAX_PRICE);
			add(p, w);
		}
	}
	return sum;
}

int main()
{
	freopen("Contact_DB.txt", "r", stdin);
	setbuf(stdout, NULL);

	int ntests = 0;
	scanf("%i", &ntests);
	for (int t = 0; t < ntests; ++t)
	{
		int ncmd, sum;
		scanf("%u %i %i", &seed, &ncmd, &sum);
		int ll = test(ncmd);
		bool correct = (sum == ll);
		printf("#%i %i\n", t + 1, correct ? 100 : 0);
	}
	return 0;
}
