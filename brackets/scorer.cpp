#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

void ac() {
  std::cout << "AC\n";
  exit(0);
}

void wa(std::string reason="") {
  std::cout << "WA\n";
  if (reason != "") {
    std::cout << reason << '\n';
  }
  exit(0);
}

int readInt(std::ifstream &ifs) {
  int ret;
  if (!(ifs >> ret)) wa();
  return ret;
}

std::string readString(std::ifstream &ifs) {
  std::string ret;
  if (!(ifs >> ret)) wa();
  return ret;
}

bool eof(std::ifstream &ifs) {
  std::string dummy;
  if (ifs >> dummy) return false;
  return true;
}

int f(std::string &s, int l, int r, std::vector<std::vector<int> > &DP) {
	if (l > r) return 0;
	if (l == r) return 1;
	int &ret = DP[l][r];
	if (ret != -1) return ret;
	ret = std::max(f(s, l+1, r, DP), f(s, l, r-1, DP));
	if (s[l] == s[r]) ret = std::max(ret, f(s, l+1, r-1, DP) + 2);
	return ret;
}

int LPS(std::string &s) {
	std::vector<std::vector<int> > DP(s.size(), std::vector<int>(s.size(), -1));
	return f(s, 0, s.size()-1, DP);
}

bool balanced(std::string s) {
	int cnt = 0;
	for (auto &c: s) {
		if (c == '(') cnt++;
		else cnt--;
		if (cnt < 0) return false;
	}
	return cnt == 0;
}

bool parentheses_sequence(std::string s) {
  for (auto &c: s) {
    if (c != '(' && c != ')') return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cerr << "Must be run with arguments [input-file] [output-file] [contestant-output]\n";
    exit(0);
  }

  std::ifstream inp(argv[1]);
  std::ifstream out(argv[2]);
  std::ifstream con(argv[3]);

  int N = readInt(inp);
  int K = readInt(inp);
  std::string judgeAns = readString(out);
  std::string ans = readString(con);

  if (!eof(con)) wa();

  if (judgeAns == "-1") {
    if (ans != "-1") wa();
    ac();
  }

  if (ans.size() != N) wa();
  if (!parentheses_sequence(ans)) wa();
  if (!balanced(ans)) wa();
  if (LPS(ans) != K) wa();

  ac();
}
