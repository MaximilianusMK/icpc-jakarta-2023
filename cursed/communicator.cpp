#include <bits/stdc++.h>
using namespace std;

const int MAXN = 33;
const int CARD_SIZE = 3;

// Contestant responses.
const char WHITE = '0'; // This is number zero.
const char BLACK = '1';

// Judge secret paper.
const char HOLE  = 'O'; // This is character O, for secret_paper.
const char NOT_HOLE = '#';

const string CORRECT = "CORRECT";
const string INCORRECT = "INCORRECT";

// Problem constraints
const int NUM_TC = 333;
const int ASK_LIMIT = 3 * NUM_TC;

vector<vector<string> > secret_papers;
vector<int> grids;
string asks[MAXN + 5];
string responses[MAXN + 5];


int wa(ofstream& out) {
  // out for domjudge
  out << "WA" << endl;
  // cerr for tcframe
  cerr << "WA" << endl;
  return 0;
}

int waTooManyQueries(ofstream& out) {
  return wa(out);
}

int waInvalidContestantQuestion(ofstream &out) {
  return wa(out);
}

int ac(ofstream& out) {
  // out for domjudge
  out << "AC" << endl;
  // cerr for tcframe
  cerr << "AC" << endl;
  return 0;
}

vector<string> inputSecretPaper(ifstream &tc_in) {
  vector<string> secret_paper;
  for (int i = 0; i < CARD_SIZE; i++) {
    string row_i;
    tc_in >> row_i;
    secret_paper.push_back(row_i);
  }
  return secret_paper;
}

bool isValidInput(const string &s, int N) {
  if (s.size() != N) {
    return false;
  }
  for (int i = 0; i < s.size(); i++) {
    char c = s[i];
    if (c != WHITE && c != BLACK) {
      return false;
    }
  }
  return true;
}

bool getInput(int N) {
  for (int i = 0;i < N; i++) {
    cin >> asks[i];
    if(!isValidInput(asks[i], N)) {
      return false;
    }
  }
  return true;
}

bool printAndCheckResponse(int N, const vector<string> &secret_paper) {
  bool has_white = false;
  for (int i = 0; i < N - (CARD_SIZE - 1); i++) {
    responses[i].clear();
    for (int j = 0; j < N - (CARD_SIZE - 1); j++) {
      // Count how many black cells can be seen from this position.
      int can_see_black_cells = 0;
      for (int dy = 0; dy < CARD_SIZE; dy++) {
        for (int dx = 0; dx < CARD_SIZE; dx++) {
          if (asks[i + dy][j + dx] == BLACK &&
              secret_paper[dy][dx] == HOLE) {
            can_see_black_cells++;
          }
        }
      }
      if (can_see_black_cells % 2) {
        responses[i] += BLACK;
      } else {
        responses[i] += WHITE;
        has_white = true;
      }
    }
  }
  if (has_white) {
    cout << INCORRECT << endl;
    for (int i = 0; i < N - (CARD_SIZE - 1); i++) {
      cout << responses[i] << endl;
    }
  } else {
    cout << CORRECT << endl;
  }
  return has_white;
}

int main(int argc, char* argv[]) {
  ifstream tc_in(argv[1]);
  ofstream out(argv[2]);

  // Scan all of the inputs from .in
  // Input size of each grid first
  for (int tc = 0; tc < NUM_TC; tc++) {
    int N;
    tc_in >> N;
    grids.push_back(N);
  }
  // Input the secret paper for each testcases.
  for (int tc = 0; tc < NUM_TC; tc++) {
    secret_papers.push_back(inputSecretPaper(tc_in));
  }
  // End of scan all of the inputs

  // Interaction with contestant program.
  int ask = 0;
  for (int tc = 0; tc < NUM_TC; tc++) {
    cout << grids[tc] << endl;
    do {
      if (ask >= ASK_LIMIT) {
        return waTooManyQueries(out);
      }
      if (!getInput(grids[tc])) {
        return waInvalidContestantQuestion(out);
      }
      ask++;
    } while(printAndCheckResponse(grids[tc], secret_papers[tc]));
  }
  // make sure contestant doesn't output anything
  string dummy;
  if (cin >> dummy) {
    return wa(out);
  }
  return ac(out);
}
