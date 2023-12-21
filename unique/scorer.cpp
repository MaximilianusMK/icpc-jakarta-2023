#include <fstream>
#include <iostream>
#include <vector>

std::ifstream inp;
std::ifstream out;
std::ifstream con;

void ac(std::string reason="") {
  std::cout << "AC\n";
  if (reason != "") {
    std::cout << reason << '\n';
  }
  exit(0);
}

void wa(std::string reason="") {
  std::cout << "WA\n";
  if (reason != "") {
    std::cout << reason << '\n';
  }
  exit(0);
}

void ok(double points, std::string reason="") {
  std::cout << "OK\n";
  std::cout << points;
  if (reason != "") {
    std::cout << " " << reason << '\n';
  }
  exit(0);
}

void registerScorer(int argc, char* argv[]) {
  if (argc != 4) {
    std::cout << "Must be run with arguments [input-file] [output-file] [contestant-output]\n";
    exit(0);
  }

  inp = std::ifstream(argv[1]);
  out = std::ifstream(argv[2]);
  con = std::ifstream(argv[3]);
}

template<class T> inline void readStream(std::ifstream &ifs, T &t) { if (!(ifs >> t)) wa(); }

int readInt(std::ifstream &ifs) { int ret; readStream(ifs, ret); return ret; }

void eof() {
  std::string dummy;
  if (con >> dummy) wa();
}

int main(int argc, char* argv[]) {
  registerScorer(argc, argv);

  int N = readInt(inp);
  std::vector<std::vector<int>> adj(N);
  std::vector<int> deg(N);
  for (int i = 1; i < N; ++i) {
    int U = readInt(inp) - 1, V = readInt(inp) - 1;
    adj[U].push_back(V);
    ++deg[V];
  }

  int K = readInt(con);
  if (K > readInt(out)) wa();
  for (int i = 0; i < K; ++i) {
    int U = readInt(con) - 1, V = readInt(con) - 1;
    if (U < 0 || U >= N || V < 0 || V >= N) wa();
    adj[U].push_back(V);
    ++deg[V];
  }
  eof();

  int u = -1, cnt = 0;

  for (int i = 0; i < N; ++i) {
    if (deg[i] == 0) {
      if (u != -1) wa();
      u = i;
    }
  }

  for (int nu; u != -1; u = nu) {
    nu = -1;
    ++cnt;
    for (int v : adj[u]) {
      if (--deg[v] == 0) {
        if (nu != -1) wa();
        nu = v;
      }
    }
  }

  if (cnt != N) wa();

  ac();
}
