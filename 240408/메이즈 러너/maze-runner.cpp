#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second

using namespace std;
using pii = pair<int, int>;

const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };

const int SZ = 10;

int N, M, K, ans;
int maze[SZ][SZ], next_maze[SZ][SZ];

pii player[SZ + 1];
bool is_escape[SZ + 1];
pii ext;

int distance(int x, int y) { return abs(ext.X - x) + abs(ext.Y - y); }

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void MovePlayer() {
	for (int i = 1; i <= M; i++) {
		if (is_escape[i]) continue;

		auto& cur = player[i];
		int ndist = distance(cur.X, cur.Y);
		int ndir = -1;

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			int dist = distance(nx, ny);

			if (!OOM(nx, ny) && maze[nx][ny] == 0) {
				if (ndist > dist) {
					ndist = dist;
					ndir = dir;
					break;
				}
			}
		}

		if (ndir >= 0) {
			cur.X += dx[ndir];
			cur.Y += dy[ndir];
			ans++;

			if (cur.X == ext.X && cur.Y == ext.Y)
				is_escape[i] = true;
		}
	}
}

tuple<int, int, int> GetSquare() {
	for (int sz = 2; sz <= N; sz++) {
		for (int x1 = 0; x1 <= N - sz + 1; x1++) {
			for (int y1 = 0; y1 <= N - sz + 1; y1++) {
				int x2 = x1 + sz - 1;
				int y2 = y1 + sz - 1;

				if (ext.X >= x1 && ext.Y >= y1 && ext.X <= x2 && ext.Y <= y2) {
					for (int i = 1; i <= M; i++) {
						if (is_escape[i]) continue;

						auto nxt = player[i];
						if (nxt.X >= x1 && nxt.Y >= y1 && nxt.X <= x2 && nxt.Y <= y2)
							return make_tuple(x1, y1, sz);
					}
				}
			}
		}
	}
}

void RotateMaze(int sx, int sy, int size) {
	for (int x = sx; x < sx + size; x++)
		for (int y = sy; y < sy + size; y++)
			if (maze[x][y] > 0) maze[x][y]--;

	for (int x = sx; x < sx + size; x++) {
		for (int y = sy; y < sy + size; y++) {
			int ox = x - sx, oy = y - sy;
			int rx = oy, ry = size - ox - 1;
			next_maze[rx + sx][ry + sy] = maze[x][y];
		}
	}

	for (int x = sx; x < sx + size; x++) 
		for (int y = sy; y < sy + size; y++)
			maze[x][y] = next_maze[x][y];
}

void RotatePlayer(int x1, int y1, int size) {
	for (int i = 1; i <= M; i++) {
		if (is_escape[i]) continue;

		int x = player[i].first;
		int y = player[i].second;

		if (x1 <= x && x < x1 + size && y1 <= y && y < y1 + size) {
			int ox = x - x1, oy = y - y1;
			int rx = oy, ry = size - ox - 1;
			player[i] = { rx + x1, ry + y1 };
		}
	}
}

void RotateExit(int sx, int sy, int size) {
	int x = ext.first;
	int y = ext.second;
	if (sx <= x && x < sx + size && sy <= y && y < sy + size) {
		int ox = x - sx, oy = y - sy;
		int rx = oy, ry = size - ox - 1;
		ext = { rx + sx, ry + sy };
	}
}

void Rotate() {
	auto [sx, sy, size] = GetSquare();

	RotateMaze(sx, sy, size);
	RotatePlayer(sx, sy, size);
	RotateExit(sx, sy, size);
}

bool Run() {
	for (int i = 1; i <= M; i++)
		if (!is_escape[i]) return true;
	return false;
}

void Print() {
	cout << '\n';
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << maze[i][j] << ' ';
		cout << '\n';
	}
	for (auto nxt : player) cout << nxt.X << ' ' << nxt.Y << '\n';
	cout << ext.X << ' ' << ext.Y << '\n';
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	
	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> K;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> maze[i][j];

	for (int x, y, i = 1; i <= M; i++) {
		cin >> x >> y;
		x--, y--;

		player[i] = { x, y };
	}

	cin >> ext.X >> ext.Y;
	ext.X--, ext.Y--;

	while (K--) {
		MovePlayer();
		if (!Run()) break;
		Rotate();
	}

	cout << ans << '\n' << ext.X + 1 << ' ' << ext.Y + 1;
	return 0;
}