#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int INF = 0x3f3f3f;
int N, M, H;
bool adj[15][35];
int ans = INF;

bool check() {
	for (int i = 1; i <= N; i++) {
		int cur = i;
		for (int j = 1; j <= H; j++) {
			if (adj[cur][j]) cur++;
			else if (adj[cur - 1][j]) cur--;
		}
		if (cur != i) return false;
	}

	return true;
}

void solve(int cnt, int x) {
	if (cnt > 3 || cnt > ans) return;
	if (check()) {
		ans = min(ans, cnt);
		return;
	}

	for (int i = x; i <= H; i++) {
		for (int j = 1; j < N; j++) {
			if (adj[j][i] || adj[j - 1][i] || adj[j + 1][i]) continue;

			adj[j][i] = true;
			solve(cnt + 1, i);
			adj[j][i] = false;
		}
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> H;
	for (int a, b, i = 0; i < M; i++) {
		cin >> a >> b;
		adj[b][a] = true;
	}

	solve(0, 1);

	if (ans == INF) cout << -1;
	else cout << ans;

	return 0;
}