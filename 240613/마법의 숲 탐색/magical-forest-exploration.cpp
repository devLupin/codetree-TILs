#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int SZ = 75;
// 북, 동, 남, 서
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

int R, C, K, board[SZ][SZ];
int dirs[1001];
bool run;
int cx, cy, cd, n, ans;
bool escape[SZ][SZ];

bool oom(int x, int y) { return x < 0 || y < 0 || x >= R || y >= C; }

bool cango(int x, int y, int dir) {
	if (dir == 1) {
		if (oom(x + 2, y) || oom(x + 1, y - 1) || oom(x + 1, y + 1)) return false;
		if (board[x + 2][y] > 0 || board[x + 1][y - 1] > 0 || board[x + 1][y + 1] > 0) return false;
	}
	if (dir == 2) {
		if (oom(x, y - 2) || oom(x - 1, y - 1) || oom(x + 1, y - 1)) return false;
		if (board[x][y - 2] > 0 || board[x - 1][y - 1] > 0 || board[x + 1][y - 1] > 0) return false;
	}
	if (dir == 3) {
		if (oom(x, y + 2) || oom(x - 1, y + 1) || oom(x + 1, y + 1)) return false;
		if (board[x][y + 2] > 0 || board[x - 1][y + 1] > 0 || board[x + 1][y + 1] > 0) return false;
	}
	return true;
}

void painting(int x, int y) {
	board[x][y] = n;

	for (int dir = 0; dir < 4; dir++) {
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		board[nx][ny] = n;
	}
}

void dfs(int x, int y, int d) {
	if (cango(x, y, 1))
		dfs(x + 1, y, d);
	else if (cango(x, y, 2) && cango(x, y - 1, 1))
		dfs(x + 1, y - 1, (d + 3) % 4);
	else if (cango(x, y, 3) && cango(x, y + 1, 1))
		dfs(x + 1, y + 1, (d + 1) % 4);
	else if(run) {
		run = false;
		cx = x, cy = y, cd = d;
		painting(cx, cy);
		dirs[n] = d;
	}
}

bool check() {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < C; j++)
			if (board[i][j] > 0) {
				fill(&board[0][0], &board[R][C], 0);
				fill(&escape[0][0], &escape[R][C], false);
				return false;
			}
	return true;
}

int bfs() {
	bool vis[SZ][SZ] = { false, };
	queue<pii> q;
	int ret = 0;

	q.push({ cx, cy });
	vis[cx][cy] = true;

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		ret = max(ret, cur.X);

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			if (oom(nx, ny) || vis[nx][ny]) continue;

			if (board[nx][ny] == board[cur.X][cur.Y]) {
				q.push({ nx, ny });
				vis[nx][ny] = true;
			}
			if (escape[cur.X][cur.Y] && board[nx][ny] > 0) {
				q.push({ nx, ny });
				vis[nx][ny] = true;
			}
		}
	}

	return ret - 2;
}

void print() {
	for (int i = 3; i < R; i++) {
		for (int j = 0; j < C; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << '\n';
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> R >> C >> K;
	R += 3;

	for (int y, d, i = 1; i <= K; i++) {
		cin >> y >> d;
		y--;
		n = i;
		run = true;
		dfs(1, y, d);
		if (check()) {
			escape[cx + dx[cd]][cy + dy[cd]] = true;
			ans += bfs();
		}
	}
	
	cout << ans;
	return 0;
}