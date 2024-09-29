/**
 * @author         lupin
 * @date           2024-09-29
 *
 * @submit         01:01:43
 * @revision       00:03:16
 * @revision       00:17:47
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#pragma warning(suppress : 6031)
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int MAX_N = 30;
const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };

int N, ans;
int board[MAX_N][MAX_N], nextBoard[MAX_N][MAX_N];
int groupBoard[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];
vector<int> gnum;

void Init() {
	fill(&groupBoard[0][0], &groupBoard[N][N], 0);
	fill(&vis[0][0], &vis[N][N], false);
	gnum.clear();
}

void MakeGroup() {
	int cnt = 0;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (!vis[i][j]) {
				queue<pii> q;
				int val = board[i][j];

				gnum.push_back(0);

				q.push({ i,j });
				vis[i][j] = true;

				while (!q.empty()) {
					pii cur = q.front();
					q.pop();

					groupBoard[cur.X][cur.Y] = cnt;
					gnum[cnt]++;

					for (int dir = 0; dir < 4; dir++) {
						int nx = cur.X + dx[dir];
						int ny = cur.Y + dy[dir];

						if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
						if (vis[nx][ny] || board[nx][ny] != val) continue;

						q.push({ nx, ny });
						vis[nx][ny] = true;
					}
				}

				cnt++;
			}
}

int Artistry() {
	int ret = 0;

	for (int x = 0; x < N; x++)
		for (int y = 0; y < N; y++)
			for (int dir = 0; dir < 4; dir++) {
				int nx = x + dx[dir];
				int ny = y + dy[dir];

				if (nx < 0 || ny < 0 || nx >= N || ny >= N) continue;
				if (board[x][y] != board[nx][ny])
					ret += (gnum[groupBoard[x][y]] + gnum[groupBoard[nx][ny]]) * board[x][y] * board[nx][ny];
			}

	return ret / 2;
}

void RotateSquare(int sx, int sy, int sz) {
	for (int x = sx; x < sx + sz; x++)
		for (int y = sy; y < sy + sz; y++) {
			int ox = x - sx, oy = y - sy;
			int rx = oy, ry = sz - ox - 1;
			nextBoard[rx + sx][ry + sy] = board[x][y];
		}
}

void Rotate() {
	int mid = N / 2;

	for (int i = 0; i < N; i++) nextBoard[mid][i] = board[i][mid];
	for (int i = 0; i < N; i++) nextBoard[N - i - 1][mid] = board[mid][i];

	RotateSquare(0, 0, mid);
	RotateSquare(0, mid + 1, mid);
	RotateSquare(mid + 1, 0, mid);
	RotateSquare(mid + 1, mid + 1, mid);

	for (int x = 0; x < N; x++)
		for (int y = 0; y < N; y++)
			board[x][y] = nextBoard[x][y];
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> N;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];

	for (int i = 0; i <= 3; i++) {
		Init();
		MakeGroup();
		ans += Artistry();
		Rotate();
	}

	cout << ans;
	return 0;
}