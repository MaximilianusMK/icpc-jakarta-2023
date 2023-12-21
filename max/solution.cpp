#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const int N = 100000;
const int M = 100000;
const int MININF = -200000;

struct event {
  int x;
  int id;
};

vector<event> updates[N + 5];

struct question {
  int s, t;
  int id;
};

vector<question> questions[N + 5];

struct max_subarray {
  // these values may be negative.
  ll mx_left;
  ll mx_right;
  ll mx_segment;
  ll sum;
};

max_subarray segtree[4 * M + 5];

max_subarray combine(const max_subarray &le, const max_subarray &ri) {
  max_subarray result;
  result.sum = le.sum + ri.sum;

  // find the max left sum.
  result.mx_left = max(le.mx_left, le.sum + ri.mx_left);

  // find the max right sum.
  result.mx_right = max(ri.mx_right, ri.sum + le.mx_right);

  // find the max segment
  result.mx_segment = max({le.mx_segment, ri.mx_segment,
                          result.mx_left, result.mx_right,
                          le.mx_right + ri.mx_left});
  return result;
}

void updateSegtree(int idx, int l, int r, int target, int x) {
  if (l == r) {
    segtree[idx] = {x, x, x, x};
    return ;
  }
  int mid = (l + r) / 2;
  if (target <= mid) {
    updateSegtree(2 * idx, l, mid, target, x);
  } else {
    updateSegtree(2 * idx + 1, mid + 1, r, target, x);
  }
  segtree[idx] = combine(segtree[2 * idx], segtree[2 * idx + 1]);
}

max_subarray querySegtree(int idx, int l, int r, int tar_l, int tar_r) {
  if (r < tar_l || tar_r < l) {
    // out of bound
    return max_subarray{MININF, MININF, MININF, MININF};
  }
  if (tar_l <= l && r <= tar_r) {
    // completely inside
    return segtree[idx];
  }
  int mid = (l + r) / 2;
  auto left_result = querySegtree(2 * idx, l, mid, tar_l, tar_r);
  auto right_result = querySegtree(2 * idx + 1, mid + 1, r, tar_l, tar_r);
  return combine(left_result, right_result);
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0), cout.tie(0);
  #define endl '\n'

  int n;
  cin >> n;

  int m;
  cin >> m;
  for (int i = 1; i <= m; i++) {
    int l, r, x;
    cin >> l >> r >> x;
    updates[l].push_back({x, i});
    updates[r + 1].push_back({0, i});
  }

  int q;
  cin >> q;
  for (int i = 1; i <= q; i++) {
    int k, s, t;
    cin >> k >> s >> t;
    questions[k].push_back({s, t, i});
  }

  vector<ll> results(q + 1, 0);
  for (int i = 1; i <= n; i++) {
    for (const auto &update: updates[i]) {
      updateSegtree(1, 1, m, update.id, update.x);
    }

    for (const auto &question: questions[i]) {
      auto ans = querySegtree(1, 1, m, question.s, question.t);
      results[question.id] = ans.mx_segment;
    }
  }

  for (int i = 1;i <= q;i++) {
    cout << results[i] << endl;
  }
}