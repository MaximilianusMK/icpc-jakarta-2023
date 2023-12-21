#include <bits/stdc++.h>
using namespace std;

const int N = 5005;
const int MOD = 998244353;

int n;
int a[N];

int dp[N][N][2];
int pwr2[N];

int solve(int idx, int inQueue, bool isFirst) {
    if (idx == n) return 1;
    if (inQueue == 0) return 0;

    int &ret = dp[idx][inQueue][isFirst];
    if (ret != -1) return ret;
    
    ret = solve(idx, inQueue-1, true); // pop from queue

    // if front node in the queue does not have any child in the bfs tree yet, or
    // it already has, but its value is smaller
    // then we can add an edge here, making A[idx] the child of the first node
    // in the queue
    if (isFirst || a[idx-1] < a[idx]) { 
        // add edge connecting this with the frontmost node of the queue
        // optionally, we can also add edge to the other nodes in the queue
        int ways = 1ll * pwr2[inQueue-1] * solve(idx+1, inQueue+1, false) % MOD;
        ret = (ret + ways) % MOD; 
    }

    return ret;
}

int main() {
    cin >> n;
    for (int i = 0 ; i < n ; i++) {
        cin >> a[i];
    }

    memset(dp, -1, sizeof dp);
    pwr2[0] = 1;
    for (int i = 1 ; i <= n ; i++) {
        pwr2[i] = 2ll * pwr2[i-1] % MOD;
    }

    int ret = solve(1, 1, true);
    cout << ret << "\n";
    return 0;
}