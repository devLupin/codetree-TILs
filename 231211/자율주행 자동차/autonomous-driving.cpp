#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };
const int SZ = 50;
int n, m, board[SZ][SZ], ans;

void dfs(int x, int y, int d) {
	if (board[x][y] == 0) {
		board[x][y] = 2;
		ans++;
	}

	int ny, nx;

	for (int i = 0; i < 4; i++) {
		int nd = (d + 3 - i) % 4;
		tie(nx, ny) = make_pair(x + dx[nd], y + dy[nd]);

		if (board[nx][ny] == 0)
			dfs(nx, ny, nd);
	}

	int nd = (d + 2) % 4;
	tie(nx, ny) = make_pair(x + dx[nd], y + dy[nd]);

	if (board[nx][ny] == 1) {
		cout << ans;
		exit(0);
	}

	dfs(nx, ny, d);
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	int x, y, d;
	cin >> n >> m >> x >> y >> d;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			cin >> board[i][j];

	dfs(x, y, d);
	return 0;
}