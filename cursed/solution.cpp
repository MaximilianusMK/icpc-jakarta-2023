#include <bits/stdc++.h>
using namespace std;

int t,n;
int ask[37][37];
char reply[37][37];
char card[7][7];
bool win;

void CONSTRUCT_FROM_CARD() {
	// DEBUG CARD
	/*
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) printf("%c", card[i][j]);
		printf("\n");
	}
	*/
	
	// The last hole is the decider
	// Can take the bottom-right-most (or right-bottom-most)
	int max_i = 0;
	int max_j = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (card[i][j] == 'O') {
				max_i = i;
				max_j = j;
			}
		}
	}
	
	// Tricky: If not resetting, there may be a potential bug!
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			ask[i][j] = 0;
		}
	}
	
	// Can fill literally anything except for the deciders:
	// The cell (i,j) will decide the return value at box (i - max_i, j - max_j)
	// (Here, the box spans 2 rows to right and 2 columns to bottom)
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			int x = i - max_i;
			int y = j - max_j;
			if (0 <= x && x < n-2 && 0 <= y && y < n-2) {
				bool odd = false;
				for (int k = 0; k < 3; k++) {
					for (int l = 0; l < 3; l++) {
						if (card[k][l] == 'O' && ask[x+k][y+l] == 1) odd = !odd;
					}
				}
				if (!odd) ask[i][j] = 1;
			}
		}
	}
	
	// DEBUG ASK
	/*
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) printf("%d", ask[i][j]);
		printf("\n");
	}
	*/
}

void ASK() {
	// Tricky: If already win, don't ask more!
	if (win) return;
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) printf("%d", ask[i][j]);
		printf("\n");
	}
	fflush(stdout);

	// Check win
	win = true;
	char status[10];
	scanf("%s", status);
	if (strcmp(status, "CORRECT") == 0) {
		win = true;
	} else {
		win = false;
		for (int i = 0; i < n-2; i++) {
			scanf("%s", reply[i]);
		}
	}
}

int main() {
	srand(time(NULL));
	t = 333;
	
	while(t--) {
		scanf("%d",&n);
		win = false;
		
		// CASE 1: n >= 5, 2 queries
		//
		// 00000...
		// 00000...
		// 00100...
		// 00000...
		// 00000...
		// ........
		//
		if (n >= 5) {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < n; j++) {
					ask[i][j] = 0;
					if (i == 2 && j == 2) ask[i][j] = 1;
				}
			}
			ASK();
			
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					card[i][j] = (reply[2-i][2-j] == '1') ? 'O' : '#';
				}
			}
			CONSTRUCT_FROM_CARD();
			ASK();
			assert(win);
		}
		
		// CASE 2: n = 3, expected 2 queries
		//
		// By generating the matrix in random,
		// the result will then be a coin flip.
		//
		// Getting 30 heads in 90 flips = 99.9995% per testcase
		// Getting 333 heads in 999 flips ~ 100%
		// https://www.omnicalculator.com/statistics/coin-flip-probability
		//
		else if (n == 3) {
			while (!win) {
				for (int i = 0; i < n; i++) {
					for (int j = 0; j < n; j++) {
						ask[i][j] = rand() % 2;
					}
				}
				ASK();
			}
		}
	}
}
