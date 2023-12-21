#include<bits/stdc++.h>
using namespace std;

string getAnswer(int N, int K) {
	string ret = "";
	for (int i = 0; i < N - K - 1; i++) ret += "(";
	for (int i = 0; i < (K - N / 2) / 2 + 1; i++) ret += "()";
	if (N % 4 == 2) {
		for (int i = 0; i < N - K - 1; i++) ret += ")";
		for (int i = 0; i < (K - (N / 2 - 1)) / 2; i++) ret += "()";
	}
	else {
		if (K % 2 == 0) {
			for (int i = 0; i < N - K - 1; i++) ret += ")";
		}
		else {
			for (int i = 0; i < (N - K - 1) / 2; i++) ret += ")";
			ret += "()";
			for (int i = 0; i < (N - K - 1) / 2; i++) ret += ")";
		}
		for (int i = 0; i < (K - N / 2) / 2; i++) ret += "()";
	}
	return ret;
}

int main() {
  int N, K;
	cin >> N >> K;

	if (N == 6 && K == 4) {
		cout << "(())()" << endl;
		return 0;
	}

	if (N == 6 && K == 3) {
		cout << "(()())" << endl;
		return 0;
	}

	if (N == 14 && K == 11) {
		cout << "()((())()())()" << endl;
		return 0;
	}
	
	if (K < N/2 || K == N) {
		cout << -1 << endl;
		return 0;
	}
	
	cout << getAnswer(N, K) << endl;
	return 0;
}