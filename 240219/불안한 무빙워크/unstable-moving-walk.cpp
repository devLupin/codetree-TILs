#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

int N, K;
deque<pii> walk;

void rotate() {
	walk.push_front(walk.back());
	walk.pop_back();
}

void move() {
	int nxt = 0;
	for (int i = 0; i < N - 1; i++) {
		if (walk[i].second == 0) continue;

		if (walk[i + 1].first != 0 && walk[i + 1].second == 0) {
			(walk[i + 1].first)--;
			swap(walk[i], walk[i + 1]);
		}
		nxt = max(walk[i].second, nxt);
	}

	if (walk[N - 1].second != 0) walk[N - 1].second = 0;

	if (walk[0].second == 0) {
		walk[0].second = nxt + 1;
		(walk[0].first)--;
	}
}

bool check() {
	int cnt = 0;
	for (int i = 0; i < N * 2; i++)
		if (walk[i].first == 0) cnt++;

	return cnt == K;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	
	cin >> N >> K;
	walk.assign(N * 2, { 0,0 });
	for (int i = 0; i < N * 2; i++)
		cin >> walk[i].first;

	int ans = 0;
	while (!check()) {
		rotate();
		move();
		ans++;
	}

	cout << ans;

	return 0;
}