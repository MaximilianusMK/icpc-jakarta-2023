#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>

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

std::string readLine(std::istream &is) {
  std::string ret;
  if (!std::getline(is, ret)) wa();
  return ret;
}

int readInt(std::istream &is) {
  int ret;
  if (!(is >> ret)) wa();
  return ret;
}

bool eof(std::istream &is) {
  std::string dummy;
  if (is >> dummy) return false;
  return true;
}

std::vector<int> merge(std::vector<int> A, std::vector<int> B) {
  std::vector<int> ret;
  int i = 0, j = 0;
  int n = A.size();
  while (i < n && j < n)
	if (A[i] < B[j])
		ret.push_back(A[i++]);
	else
		ret.push_back(B[j++]);
  while (i < n) ret.push_back(A[i++]);
  while (j < n) ret.push_back(B[j++]);
  return ret;
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
  int judgeAns = readInt(out);

  std::vector<int> A(N), B(N);

  std::stringstream row1(readLine(con)); // Scan the 1st row as one input stream.
  A[0] = readInt(row1);

  if (judgeAns == -1) {
    if (A[0] != -1) wa();
    if (!eof(row1) || !eof(con)) wa(); // Make sure that there's no additional output.
    ac();
  }

  for (int i = 1; i < N; ++i) A[i] = readInt(row1);
  if (!eof(row1)) wa(); // Make sure that the 1st row only contains N numbers.

  std::stringstream row2(readLine(con)); // Scan the 2nd row as one input stream.
  for (int i = 0; i < N; ++i) B[i] = readInt(row2);
  if (!eof(row2)) wa(); // Make sure that the 2nd row only contains N numbers.
  if (!eof(con)) wa();  // Make sure that there's no additional output.

  std::vector<int> C(2*N);
  C = merge(A, B);
  for (int i = 0; i < 2*N; ++i) {
	  if (C[i] != readInt(inp)) wa();  
  }
  ac();
}
