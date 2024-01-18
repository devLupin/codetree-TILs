#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

int n, m, t, A[1001][1001], B[1001][1001];
vector<pii> cleaner;

const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };

bool oom(int x, int y) { return x < 0 || y < 0 || x >= n || y >= m; }

void spread() {
	for (int i = 0; i < n; i++)
		memset(B[i], 0, sizeof(B[i]));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			int cur = A[i][j] / 5;

			for (int dir = 0; dir < 4; dir++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];

				if (!oom(nx, ny) && A[nx][ny] != -1) {
					B[i][j] -= cur;
					B[nx][ny] += cur;
				}
			}
		}
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			A[i][j] += B[i][j];
}

void clean(pii start, vector<int> dirs) {
	int idx = 0;
	int dir = dirs[idx];
	vector<int> v;
	int x = start.X + dx[dir];
	int y = start.Y + dy[dir];

	v.push_back(A[x][y]);

	while (A[x][y] != -1 && idx < 4) {
		x += dx[dir];
		y += dy[dir];

		if (oom(x, y)) {
			x -= dx[dir];
			y -= dy[dir];
			idx++;
			dir = dirs[idx];
			continue;
		}

		v.push_back(A[x][y]);
	}
	if (A[x][y] == -1) v.pop_back();
	v.pop_back();

	idx = 0;
	dir = dirs[idx];
	x = start.X + dx[dir];
	y = start.Y + dy[dir];

	A[x][y] = 0;

	for(int i=0; i<v.size(); i++) {
		x += dx[dir];
		y += dy[dir];

		if (oom(x, y)) {
			x -= dx[dir];
			y -= dy[dir];
			idx++;
			dir = dirs[idx];
			i--;
			continue;
		}
		A[x][y] = v[i];
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> n >> m >> t;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cin >> A[i][j];
			if (A[i][j] == -1) cleaner.push_back({ i,j });
		}
	}

	while (t--) {
		spread();
		clean(cleaner[0], { 3,0,2,1 });
		clean(cleaner[1], { 3,1,2,0 });
	}

	int ans = 0;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			ans += (A[i][j] > 0) ? A[i][j] : 0;

	cout << ans;
	
	return 0;
}