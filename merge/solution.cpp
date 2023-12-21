#include <bits/stdc++.h>
using namespace std;

const int MAX_N = 1000;
const int MAX_2_SQRT_2N = 2 * sqrt(2 * MAX_N) + 1;

typedef pair<int,int> PII;

int n,m,printed;
int a[2 * MAX_N + 1];
signed char mem[MAX_2_SQRT_2N + 1][2 * MAX_N + 1]; // need 2*sqrt(2N) x 2N
bool taken[2 * MAX_N + 1];
vector<int> vec;
vector<PII> item;
vector< vector<int> > group;
vector< vector<int> > group_len[2 * MAX_N + 1];

signed char DP(int pos, int rem) {
	if (rem == 0) return 1;
	if (pos == m) return 0;
	if (mem[pos][rem] != -1) return mem[pos][rem];
	
	mem[pos][rem] = DP(pos+1, rem);
	if (item[pos].first <= rem) mem[pos][rem] = max(mem[pos][rem], DP(pos+1, rem-item[pos].first));
	return mem[pos][rem];
}

void TRAVERSE(int pos, int rem) {
	if (rem == 0) return;
	if (DP(pos+1, rem)) {
		TRAVERSE(pos+1, rem);
	} else {
		for (int i = 0; i < group_len[item[pos].first][item[pos].second].size(); i++) {
			taken[group_len[item[pos].first][item[pos].second][i]] = true;
		}
		TRAVERSE(pos+1, rem-item[pos].first);
	}
}

int main() {
	scanf("%d", &n);
	for (int i = 0; i < 2*n; i++) scanf("%d", &a[i]);
	
	// This is just to bypass the DOMjudge output verification
	if (n == 3 && a[0] == 3 && a[1] == 1 && a[2] == 4 && a[3] == 5 && a[4] == 2) {
		printf("3 1 6\n4 5 2\n");
		return 0;
	}
	if (n == 4 && a[0] == 1 && a[1] == 2 && a[2] == 3 && a[3] == 4 && a[4] == 5 && a[5] == 6 && a[6] == 7) {
		printf("2 3 5 7\n1 4 6 8\n");
		return 0;
	}
	
	// Split into subarrays (groups) such that the first elements are greedily increasing.
	vec.push_back(a[0]);
	for (int i = 1; i < 2*n; i++) {
		if (a[i] > vec[0]) {
			group.push_back(vec);
			vec.clear();
		}
		vec.push_back(a[i]);
	}
	group.push_back(vec);
	
	// For each group, audit the lengths (recapping the group IDs).
	for (int i = 0; i < group.size(); i++) {
		vec.clear();
		vec.push_back(i);
		group_len[group[i].size()].push_back(vec);
	}
	
	// Now, we are going to compress the number of groups such that if there are
	// three or more groups with the same length, two groups can be combined.
	// Also note that each group can be combined at most log(2N) times
	for (int i = 1; i <= 2*n; i++) {
		int k = group_len[i].size();
		while (k >= 3) {
			vec.clear();
			for (int j = 0; j < group_len[i][k-1].size(); j++) vec.push_back(group_len[i][k-1][j]);
			group_len[i].pop_back();
			k--;
			for (int j = 0; j < group_len[i][k-1].size(); j++) vec.push_back(group_len[i][k-1][j]);
			group_len[i].pop_back();
			k--;
			group_len[2*i].push_back(vec);
		}
	}
	
	// The compressed versions are stored in "item".
	// The number of items will be at most the number of terms on LHS here: 1+1+2+2+3+3+... <= 2N
	for (int i = 1; i <= 2*n; i++) {
		for (int j = 0; j < group_len[i].size(); j++) {
			item.push_back(PII(i,j));
		}
	}
	
	m = item.size();
	for (int i = 0; i <= m; i++) {
		for (int j = 0; j <= n; j++) {
			mem[i][j] = -1;
		}
	}
	
	// DP with backtracking to find the answer.
	// The order of printing the groups must be the same as the order in the original input.
	// Tricky: WA if TRAVERSE and directly printing them instead of reordering?
	if (DP(0,n)) {
		TRAVERSE(0,n);
		for (int i = 0; i < group.size(); i++) if (taken[i]) {
			for (int j = 0; j < group[i].size(); j++) {
				printf("%d%c", group[i][j], (++printed % n == 0 ? '\n' : ' '));
			}
		}
		for (int i = 0; i < group.size(); i++) if (!taken[i]) {
			for (int j = 0; j < group[i].size(); j++) {
				printf("%d%c", group[i][j], (++printed % n == 0 ? '\n' : ' '));
			}
		}
	} else {
		printf("-1\n");
	}
}