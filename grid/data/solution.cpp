#include <cstdio>

int euclid(int x, int y) {
  return y > 0 ? (x / y & 1) ^ euclid(y, x % y) : 0;
}

int nimber(int x, int y) {
  return euclid(x, y) ^ euclid(x - 1, y) ^ euclid(x, y - 1) ^ euclid(x - 1, y - 1);
}

int main() {
  int N, ans = 0;
  scanf("%d", &N);
  for (int i = 0; i < N; ++i) {
    int X, Y;
    scanf("%d %d", &X, &Y);
    ans ^= nimber(X, Y);
  }

  if (ans > 0) printf("FIRST\n");
  else printf("SECOND\n");
  return 0;
}
