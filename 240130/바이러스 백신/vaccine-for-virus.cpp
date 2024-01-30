#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

const int INF = 5000;
const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };

int n, m, A[50][50], ans = INF;
bool vis[50][50];
vector<pii> hospital;

struct info {
	int x, y, d;
};

int bfs(vector<pii> v) {
	queue<info> q;
	int mx = 0;

	for (int i = 0; i < n; i++)
		memset(vis[i], false, sizeof(vis[i]));

	for (auto& nxt : v) {
		q.push({ nxt.first, nxt.second, 0 });
		vis[nxt.first][nxt.second] = true;
	}

	while (!q.empty()) {
		int q_sz = q.size();
		
		while (q_sz--) {
			info cur = q.front();
			q.pop();
			mx = max(mx, cur.d);

			for (int dir = 0; dir < 4; dir++) {
				int nx = cur.x + dx[dir];
				int ny = cur.y + dy[dir];

				if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
				if (A[nx][ny] == 0 && !vis[nx][ny]) {
					q.push({ nx, ny, cur.d + 1 });
					vis[nx][ny] = true;
				}
			}
		}
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (A[i][j] == 0 && !vis[i][j]) return INF;

	return mx;
}

void solve(int s, int cnt, vector<pii> v) {
	if (cnt == m) {
		ans = min(ans, bfs(v));
		return;
	}

	for (int i = s; i < hospital.size(); i++) {
		v.push_back(hospital[i]);
		solve(i + 1, cnt + 1, v);
		v.pop_back();
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> A[i][j];
			if (A[i][j] == 2) hospital.push_back({ i,j });
		}
	}

	solve(0, 0, {});
	if (ans == INF) cout << -1;
	else cout << ans;

	return 0;
}