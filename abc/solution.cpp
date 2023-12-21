#include <bits/stdc++.h>
using namespace std;

string ans = "zzz";
string s[3];
bool vis[3][3];

void nyan(int x, int y, string ret) {
	vis[x][y] = true;
	if (ret.length() == 3) {
		ans = min(ans, ret);
		vis[x][y] = false;
		return;
	}
	
	if (x > 0 && !vis[x-1][y]) nyan(x-1, y, ret+s[x-1][y]);
	if (y > 0 && !vis[x][y-1]) nyan(x, y-1, ret+s[x][y-1]);
	if (x < 2 && !vis[x+1][y]) nyan(x+1, y, ret+s[x+1][y]);
	if (y < 2 && !vis[x][y+1]) nyan(x, y+1, ret+s[x][y+1]);
	
	if (x > 0 && y > 0 && !vis[x-1][y-1]) nyan(x-1, y-1, ret+s[x-1][y-1]);
	if (x > 0 && y < 2 && !vis[x-1][y+1]) nyan(x-1, y+1, ret+s[x-1][y+1]);
	if (x < 2 && y > 0 && !vis[x+1][y-1]) nyan(x+1, y-1, ret+s[x+1][y-1]);
	if (x < 2 && y < 2 && !vis[x+1][y+1]) nyan(x+1, y+1, ret+s[x+1][y+1]);
	vis[x][y] = false;
}

int main() {
	cin >> s[0] >> s[1] >> s[2];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			string tmp = "";
			nyan(i, j, tmp + s[i][j]);
		}
	}
	printf("%s\n",ans.c_str());
}
