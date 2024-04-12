#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int MAX_N = 70;

const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };

int L, Q, N, lv, ans;
int board[MAX_N][MAX_N], next_board[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];


bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void Rotate(int sx, int sy, int len) {
	int half = len / 2;

	for (int x = sx; x < sx + half; x++)
		for (int y = sy; y < sy + half; y++)
			next_board[x][y + half] = board[x][y];

	for (int x = sx; x < sx + half; x++)
		for (int y = sy + half; y < sy + len; y++)
			next_board[x + half][y] = board[x][y];

	for (int x = sx + half; x < sx + len; x++)
		for (int y = sy + half; y < sy + len; y++)
			next_board[x][y - half] = board[x][y];

	for (int x = sx + half; x < sx + len; x++)
		for (int y = sy; y < sy + half; y++)
			next_board[x - half][y] = board[x][y];
}

void Select() {
	int len = (1 << lv);

	for (int i = 0; i < N; i += len)
		for (int j = 0; j < N; j += len)
			Rotate(i, j, len);
}

void Copy() {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			board[i][j] = next_board[i][j];
			next_board[i][j] = 0;
		}
}

void Melt() {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			int cnt = 0;

			for (int k = 0; k < 4; k++) {
				int nx = i + dx[k];
				int ny = j + dy[k];

				if (!OOM(nx, ny) && board[nx][ny] != 0) cnt++;
			}

			if (cnt < 3 && board[i][j] > 0) next_board[i][j] = -1;
		}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			board[i][j] += next_board[i][j];
			next_board[i][j] = 0;
		}
}

void Print() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << board[i][j] << ' ';
		cout << "\n";
	}
	cout << "\n\n";
}

int Sum() {
	int ret = 0;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			ret += board[i][j];

	return ret;
}

int BFS(int x, int y) {
	int ret = 0;
	queue<pii> q;

	q.push({ x, y });
	vis[x][y] = true;

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		ret++;

		for (int k = 0; k < 4; k++) {
			int nx = cur.X + dx[k];
			int ny = cur.Y + dy[k];

			if (!OOM(nx, ny) && !vis[nx][ny] && board[nx][ny] > 0) {
				q.push({ nx, ny });
				vis[nx][ny] = true;
			}
		}
	}

	return ret;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> L >> Q;
	N = (1 << L);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];

	while (Q--) {
		cin >> lv;

		Select();
		Copy();
		Melt();
	}
	cout << Sum() << '\n';

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (!vis[i][j] && board[i][j] > 0) ans = max(ans, BFS(i, j));

	cout << ans;

	return 0;
}