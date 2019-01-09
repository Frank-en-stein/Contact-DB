#include<stdio.h>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define LIMIT 100

extern void solve(int board[4][4]);

int EmptyRow, EmptyCol;
int board[4][4];

bool move(int dir)
{
	switch (dir) {
	case UP:
		if (3 == EmptyRow)
			return false;
		board[EmptyRow][EmptyCol] = board[EmptyRow + 1][EmptyCol];
		board[EmptyRow + 1][EmptyCol] = 0;
		EmptyRow++;
		break;
	case RIGHT:
		if (0 == EmptyCol)
			return false;
		board[EmptyRow][EmptyCol] = board[EmptyRow][EmptyCol - 1];
		board[EmptyRow][EmptyCol - 1] = 0;
		EmptyCol--;
		break;
	case DOWN:
		if (0 == EmptyRow)
			return false;
		board[EmptyRow][EmptyCol] = board[EmptyRow - 1][EmptyCol];
		board[EmptyRow - 1][EmptyCol] = 0;
		EmptyRow--;
		break;
	case LEFT:
		if (3 == EmptyCol)
			return false;
		board[EmptyRow][EmptyCol] = board[EmptyRow][EmptyCol + 1];
		board[EmptyRow][EmptyCol + 1] = 0;
		EmptyCol++;
		break;
	}
	return true;
}

int main()
{
	freopen("Contact_DB.txt", "r", stdin);
	int T;
	int Case;
	int boardCopy[4][4];
	int row, col;
	scanf("%d", &T);
	for (Case = 1; Case <= T; Case++) {
		for (row = 0; row < 4; row++) {
			for (col = 0; col < 4; col++) {
				scanf("%d", &board[row][col]);
				boardCopy[row][col] = board[row][col];
				if (0 == board[row][col]) {
					EmptyRow = row;
					EmptyCol = col;
				}
			}
		}
		int movecount = 0;
		solve(boardCopy);
		int match = 0;
		for (row = 0; row < 4; row++) {
			for (col = 0; col < 4; col++) {
				if (row * 4 + col + 1 == board[row][col])
					match++;
			}
		}
		if (0 == board[3][3])
			match++;
		if (4 <= match)
			printf("#%d Pass\n", Case);
		else
			printf("#%d %d\n", Case, match);
	}
	return 0;
}
