#include <bits/stdc++.h>
using namespace std;

const int MAXN = 55;
const int Q = 7;

int N, M;
char G[MAXN*2][MAXN*2];

int query_count = 0;
long long adjL[MAXN], adjR[MAXN];

int wa(ofstream& out) {
  string dummy;
  while (getline(cin, dummy)) {}
  out << "WA" << endl;
  cerr << "WA" << endl;
  return 0;
}

int waTooManyQueries(ofstream& out) {
  return wa(out);
}

int ac(ofstream& out) {
  string dummy;
  while (getline(cin, dummy)) {}
  out << "AC" << endl;
  cerr << "AC" << endl;
  return 0;
}

bool ask() {
  int k;
  if (!(cin >> k)) return false;
  
  int prv = 0;
  long long adjsL = 0, adjsR = 0;
  for (int i = 0; i < k; ++i) {
    int a;
    if (!(cin >> a)) return false;
    
    if (a > N+M) return false;
    if (a <= prv) return false;
    prv = a--;
    
    if (a < N) adjsL |= adjL[a];
    else adjsR |= adjR[a - N];
  }

  cout << __builtin_popcountll(adjsL) + __builtin_popcountll(adjsR);
  
  while (adjsR > 0) {
    int ctz = __builtin_ctzll(adjsR);
    cout << " " << ctz + 1;
    adjsR -= 1LL << ctz;
  }

  while (adjsL > 0) {
    int ctz = __builtin_ctzll(adjsL);
    cout << " " << ctz + N + 1;
    adjsL -= 1LL << ctz;
  }

  cout << endl;

  return true;
}

bool answer() {
  int z;
  if (!(cin >> z)) return false;
  
  if (!(1 <= z && z <= N+M)) return false;
  
  --z;
  
  if (z < N) {
    if (__builtin_popcountll(adjL[z]) == 1) return false;
  } else {
    if (__builtin_popcountll(adjR[z-N]) == 1) return false;
  }
  
  return true;
}

int main(int argc, char* argv[]) {
  ifstream tc_in(argv[1]);
  ofstream out(argv[2]);
  
  tc_in >> N >> M;
  assert(N <= 60 && M <= 60);
  for (int i = 0; i < N+M; ++i) {
    tc_in >> G[i];
    for (int j = 0; j < N+M; ++j) {
      if (G[i][j] == '1') {
        if (i < N) adjL[i] |= 1LL << j-N;
        else adjR[i-N] |= 1LL << j;
      }
    }
  }
  
  cout << N << " " << M << endl;

  while (true) {
    string op;
    if (!(cin >> op)) return wa(out);

    if (op == "?") {
      if (++query_count > Q) return waTooManyQueries(out);
      if (!ask()) return wa(out);
    } else if (op == "!") {
      if (answer()) {
        return ac(out);
      } else {
        return wa(out);
      }
    } else {
      return wa(out);
    }
  }
  return 0;
}
