#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

int N, K;
deque<int> safety, person;

int solve() {
	int cnt = 0;

	safety.push_front(safety.back());
	safety.pop_back();
	person.push_front(person.back());
	person.pop_back();

	if (person[N - 1] > 0) person[N - 1] = 0;

	for (int i = 0; i < N - 1; i++) {
		if (person[i] && safety[i + 1] && !person[i + 1]) {
			swap(person[i], person[i + 1]);
			safety[i + 1]--;

			if (!safety[i + 1]) cnt++;
		}
	}

	if (person[N - 1] > 0) person[N - 1] = 0;

	if (safety[0] > 0) {
		person[0] = 1;
		safety[0]--;

		if (!safety[0]) cnt++;
	}

	return cnt;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	
	
	cin >> N >> K;
	safety.assign(N * 2, 0);
	person.assign(N * 2, 0);
	for (int i = 0; i < N * 2; i++)
		cin >> safety[i];

	int ans = 0;
    while (true) {
	    int chk = solve();
	    ans++;
	    if (chk == K) break;
    }
	cout << ans;

	return 0;
}