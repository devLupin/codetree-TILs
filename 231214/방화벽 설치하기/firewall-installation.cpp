#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

const int dx[] = {-1,1,0,0};
const int dy[] = {0,0,-1,1};

int n, m, A[10][10], ans;
vector<pii> fire;

int bfs() {
	queue<pii> q;
	for (auto nxt : fire) q.push(nxt);

	int cpy[10][10];
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			cpy[i][j] = A[i][j];

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.first + dx[dir];
			int ny = cur.second + dy[dir];

			if (nx < 0 || ny < 0 || nx >= n || ny >= m) continue;
			if (cpy[nx][ny] == 0) {
				cpy[nx][ny] = 2;
				q.push({ nx, ny });
			}
		}
	}

	int ret = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			if (cpy[i][j] == 0) ret++;

	return ret;
}

void dfs(int cnt) {
	if (cnt == 3) {
		int cur = bfs();
		ans = max(ans, cur);
		return;
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			if (A[i][j] == 0) {
				A[i][j] = 1;
				dfs(cnt + 1);
				A[i][j] = 0;
			}
		}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	cin >> n >> m;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++) {
			cin >> A[i][j];
			if (A[i][j] == 2) fire.push_back({ i,j });
		}

	dfs(0);
	cout << ans;

	return 0;
}