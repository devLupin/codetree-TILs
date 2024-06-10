#include <iostream>
#include <tuple>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int SZ = 15;
const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };

int N, M, K;
int nxt[SZ][SZ], board[SZ][SZ];
pii player[SZ];
bool escape[SZ];
int x, y, ex, ey, ans;

int distance(int x, int y) { return abs(ex - x) + abs(ey - y); }
bool oom(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void move() {
	for (int i = 0; i < M; i++) {
		if (escape[i]) continue;

		auto& cur = player[i];
		int dist = distance(cur.X, cur.Y);
		int ndir = -1;

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			int nd = distance(nx, ny);

			if (!oom(nx, ny) && board[nx][ny] == 0 && dist > nd) {
				dist = nd;
				ndir = dir;
			}
		}

		if (ndir != -1) {
			ans++;
			cur = { cur.X + dx[ndir], cur.Y + dy[ndir] };

			if (cur == make_pair(ex, ey))
				escape[i] = true;
		}
	}
}

bool is_square(int x, int y, int sz) {
	bool c1 = false, c2 = false;

	for (int i = x; i < x + sz; i++)
		for (int j = y; j < y + sz; j++) {
			if (i == ex && j == ey) c1 = true;
			for (int k = 0; k < M; k++) {
				if (escape[k]) continue;
				if (player[k] == make_pair(i, j)) c2 = true;
			}
			if (c1 && c2) return true;
		}

	return false;
}

void rotate_board(int sx, int sy, int sz) {
	for (int x = sx; x < sx + sz; x++)
		for (int y = sy; y < sy + sz; y++)
			if (board[x][y]) board[x][y]--;

	for (int x = sx; x < sx + sz; x++)
		for (int y = sy; y < sy + sz; y++) {
			int ox = x - sx, oy = y - sy;
			int rx = oy, ry = sz - ox - 1;
			nxt[rx + sx][ry + sy] = board[x][y];
		}

	for (int x = sx; x < sx + sz; x++)
		for (int y = sy; y < sy + sz; y++)
			board[x][y] = nxt[x][y];
}

void rotate_player(int sx, int sy, int sz) {
	for (int i = 0; i < M; i++) {
		if (escape[i]) continue;

		auto& [x, y] = player[i];

		if (x >= sx && x < sx + sz && y >= sy && y < sy + sz) {
			int ox = x - sx, oy = y - sy;
			int rx = oy, ry = sz - ox - 1;
			x = rx + sx;
			y = ry + sy;
		}
	}
}

void rotate_exit(int sx, int sy, int sz) {
	int ox = ex - sx, oy = ey - sy;
	int rx = oy, ry = sz - ox - 1;
	ex = rx + sx;
	ey = ry + sy;
}

void rotate(int sx, int sy, int sz) {
	rotate_board(sx, sy, sz);
	rotate_player(sx, sy, sz);
	rotate_exit(sx, sy, sz);
}

void update() {
	for (int sz = 2; sz <= N; sz++)
		for (int x = 0; x < N; x++)
			for (int y = 0; y < N; y++)
				if (is_square(x, y, sz)) {
					rotate(x, y, sz);
					return;
				}
}

bool go() {
	for (int i = 0; i < M; i++)
		if (!escape[i]) return true;
	return false;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> N >> M >> K;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];

	for (int i = 0; i < M; i++) {
		cin >> x >> y;
		player[i] = { x - 1, y - 1 };
	}

	cin >> x >> y;
	tie(ex, ey) = make_pair(x - 1, y - 1);

	while (K-- && go()) {
		move();
		update();
	}

	cout << ans << '\n' << ex + 1 << ' ' << ey + 1;
	return 0;
}