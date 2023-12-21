#include <bits/stdc++.h>
using namespace std;

const int MAXA = 1 << 17;
const int MOD = 998244353;

void fwht(vector<int> &a) {
  int N = a.size();
  for (int len = 1; len < N; len *= 2) {
    for (int i = 0; i < N; i += len * 2) {
      for (int j = 0; j < len; ++j) {
        int u = a[i + j], v = a[i + j + len];
        a[i + j] = (u + v) % MOD;
        a[i + j + len] = (u - v + MOD) % MOD;
      }
    }
  }
}

// Assume p.size() = q.size() = 2^k (for some integer k)
vector<int> operator * (vector<int> p, vector<int> q) {
  int N = p.size();
  fwht(p); fwht(q);
  for (int i = 0; i < N; ++i) {
    p[i] = 1LL * p[i] * q[i] % MOD;
  }
  fwht(p);

  int i2 = 1;
  for (int len = 1; len < N; len *= 2) i2 = 1LL * i2 * (MOD + 1) / 2 % MOD;
  for (int &x : p) x = 1LL * x * i2 % MOD;
  return p;
}

int main() {
  int n;
  scanf("%d", &n);

  vector<int> small(MAXA, 1);
  vector<int> large(MAXA, 0);
  for (int i = 0; i < n; ++i) {
    int a;
    scanf("%d", &a);
    int nsmall = (2LL * large[a] + small[a]) % MOD;
    int nlarge = (2LL * small[a] + large[a]) % MOD;
    small[a] = nsmall;
    large[a] = nlarge;
  }

  for (int len = 1; len < MAXA; len *= 2) {
    for (int i = 0; i < MAXA; i += len * 2) {
      vector<int> lsmall(len), rsmall(len), llarge(len), rlarge(len);
      for (int j = 0; j < len; ++j) {
        lsmall[j] = small[i + j];
        rsmall[j] = small[i + j + len];
        llarge[j] = large[i + j];
        rlarge[j] = large[i + j + len];
      }

      vector<int> nlsmall = lsmall * rsmall;
      vector<int> nrsmall = llarge * rlarge;
      vector<int> nllarge = llarge * rsmall;
      vector<int> nrlarge = lsmall * rlarge;
      for (int j = 0; j < len; ++j) {
        small[i + j] = nlsmall[j];
        small[i + j + len] = nrsmall[j];
        large[i + j] = nllarge[j];
        large[i + j + len] = nrlarge[j];
      }
    }
  }

  int ans = (small[0] + large[0]) % MOD;
  printf("%d\n", ans);
  return 0;
}
