#include <bits/stdc++.h>
using namespace std;

long long n,x,sum,maxim;

int main() {
	scanf("%lld",&n);
	for (int i = 0; i < n; i++) {
		scanf("%lld",&x);
		sum += x;
		maxim = max(maxim,x);
	}
	
	sum -= maxim;
	if (maxim >= 2*sum) printf("%lld\n",sum);
	else printf("%lld\n",(sum+maxim)/3);
}
