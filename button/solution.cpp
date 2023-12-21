#include <bits/stdc++.h>
using namespace std;

int getLineBitCount(int N) {
  int cnt = 0, pref = 0;
  getchar();
  for (int i = 0; i < N; ++i) {
    cnt += pref ^= getchar() - '0';
  }
  return cnt;
}

void solve() {
  int N;
  scanf("%d", &N);

  int cntA = getLineBitCount(N), cntB = getLineBitCount(N);

  if (cntA == cntB || N + 1 - cntA == cntB) {
    printf("YES\n");
  } else {
    printf("NO\n");
  }
}

int main() {
  int T;
  scanf("%d", &T);
  for (int tc = 0; tc < T; ++tc) {
    solve();
  }
  return 0;
}
