#include <bits/stdc++.h>
using namespace std;

int N, M;

vector<int> ask(vector<int> nodes) {
  printf("? %d", (int) nodes.size());
  for (int u: nodes) printf(" %d", u+1);
  printf("\n");

  fflush(stdout);

  int y;
  scanf("%d", &y);

  vector<int> ret(y);
  for (int i = 0; i < y; ++i) {
    scanf("%d", &ret[i]);
    --ret[i];
  }

  return ret;
}

vector<int> ask(int l, int r) {
  vector<int> nodes(r - l + 1);
  iota(nodes.begin(), nodes.end(), l);
  return ask(nodes);
}

void answer(int z) {
  printf("! %d\n", z+1);
  fflush(stdout);
  exit(0);
}

int main() {
  scanf("%d %d", &N, &M);

  int l, r;
  if (N < M) {
    vector<int> nodes = ask(0, N-1);
    if (nodes.size() < M) {
      vector<bool> vis(M, false);
      for (int u: nodes) vis[u-N] = true;

      for (int i = 0; i < vis.size(); ++i) if (!vis[i]) {
        answer(i+N);
      }
    }

    l = 0, r = N-1;
  } else {
    vector<int> nodes = ask(N, N+M-1);
    if (nodes.size() < N) {
      vector<bool> vis(N, false);
      for (int u: nodes) vis[u] = true;

      for (int i = 0; i < vis.size(); ++i) if (!vis[i]) {
        answer(i);
      }
    }

    l = N, r = N+M-1;
  }

  while (l < r) {
    int mid = l + r >> 1;

    if (ask(l, mid).size() <= mid-l+1) {
      l = mid+1;
    } else {
      r = mid;
    }
  }

  answer(l);
  return 0;
}
