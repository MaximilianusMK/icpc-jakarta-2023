#include <bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>> adj;

struct MinPathCoverSolver {
    int n;
    vector<set<int>> adj, revAdj;

    MinPathCoverSolver(
        int _n,
        vector<vector<int>> _adj
    ) {
        n = _n;
        adj.resize(n+1);
        revAdj.resize(n+1);

        for (int i = 1 ; i <= n ; i++) {
            for (int j : _adj[i]) {
                adj[i].insert(j);
                revAdj[j].insert(i);
            }
        }
    }

    bool canBePaired(int v) {
        return (adj[v].size() == 1 && revAdj[v].empty()) 
                || (revAdj[v].size() == 1 && adj[v].empty());
    }

    bool isIsolated(int v) {
        return adj[v].empty() && revAdj[v].empty();
    }

    // pair<in_match, out_match>
    pair<vector<int>, vector<int>> solve() {
        vector<int> inMatch(n+1, -1), outMatch(n+1, -1);
        vector<bool> visited(n+1, false);
        
        queue<int> q;
        for (int i = 1 ; i <= n ; i++) {
            if (canBePaired(i) || isIsolated(i)) {
                q.push(i);
            }
        }

        for (int i = 1 ; i <= n ; i++) {
            int v = -1;
            while (!q.empty() && v == -1) {
                v = q.front();
                q.pop();
                if (visited[v]) v = -1;
            }
            assert(v != -1);
            visited[v] = true;

            if (isIsolated(v)) continue;

            if (adj[v].size() == 1) {
                int u = *adj[v].begin();
                inMatch[u] = v, outMatch[v] = u;
                for (int w : revAdj[u]) {
                    adj[w].erase(u);
                    if (canBePaired(w) || isIsolated(w)) {
                        q.push(w);
                    }
                }
                revAdj[u].clear();
                if (canBePaired(u) || isIsolated(u)) q.push(u);
            } else {
                int u = *revAdj[v].begin();
                outMatch[u] = v, inMatch[v] = u;
                for (int w : adj[u]) {
                    revAdj[w].erase(u);
                    if (canBePaired(w) || isIsolated(w)) {
                        q.push(w);
                    }
                }
                adj[u].clear();
                if (canBePaired(u) || isIsolated(u)) q.push(u);
            }
        }
        return {inMatch, outMatch};
    };
};

struct TopoSortSolver {
    int n;
    vector<vector<int>> adj;

    TopoSortSolver(int _n, vector<vector<int>> _adj) {
        n = _n;
        adj = _adj;
    }

    vector<int> solve() {
        vector<int> inDeg(n+1, 0);
        for (int i = 1 ; i <= n ; i++) {
            for (int j : adj[i]) {
                inDeg[j]++;
            }
        }

        queue<int> q;
        for (int i = 1 ; i <= n ; i++) {
            if (inDeg[i] == 0) {
                q.push(i);
            }
        }

        vector<int> ret;
        for (int i = 1 ; i <= n ; i++) {
            assert(!q.empty());
            int v = q.front();
            q.pop();

            ret.push_back(v);
            for (int u : adj[v]) {
                inDeg[u]--;
                if (inDeg[u] == 0) q.push(u);
            }
        }

        return ret;
    }
};

void read() {
    scanf("%d", &n);
    adj.resize(n+1);
    for (int i = 1 ; i < n ; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        adj[u].push_back(v);
    }
}

void work() {
    MinPathCoverSolver mpcs(n, adj);
    auto [inMatch, outMatch] = mpcs.solve();

    vector<int> pathNums(n+1);
    vector<int> startPath(n+1), endPath(n+1);
    int num = 0;
    for (int i = 1 ; i <= n ; i++) {
        if (inMatch[i] != -1) continue;
        num++;
        startPath[num] = i;
        for (int j = i ; j != -1 ; j = outMatch[j]) {
            pathNums[j] = num;
            endPath[num] = j;
        }
    }

    vector<vector<int>> adjPath(num+1);
    for (int i = 1 ; i <= n ; i++) {
        for (int j : adj[i]) {
            if (pathNums[i] == pathNums[j]) continue;
            adjPath[pathNums[i]].push_back(pathNums[j]);
        }
    }

    TopoSortSolver tss(num, adjPath);
    vector<int> order = tss.solve();

    printf("%d\n", num-1);
    for (int i = 1 ; i < num ; i++) {
        int bef = order[i-1];
        int cur = order[i];
        printf("%d %d\n", endPath[bef], startPath[cur]);
    }
}

int main() {
    read();
    work();
    return 0;
}
