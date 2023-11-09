#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define Y first
#define X second
using namespace std;
using pii = pair<int, int>;

const int MX = 15;

int N, M, K, A[MX][MX], B[MX][MX], move_cnt;
vector<pii> player;
pii ext;

const int dy[] = { -1,1,0,0 };
const int dx[] = { 0,0,-1,1 };

int min_distance(int y1, int x1, int y2, int x2) { return abs(x1 - x2) + abs(y1 - y2); }

bool oom(int y, int x) { return y < 0 || x < 0 || y >= N || x >= N; }

void move() {
	vector<pii> pos;

	for (int i = 0; i < player.size(); i++) {
		auto nxt = player[i];
		if (nxt.Y == -1 && nxt.X == -1) continue;

		int dist = MX;
		int d = -1;

		for (int dir = 0; dir < 4; dir++) {
			int ny = nxt.Y + dy[dir];
			int nx = nxt.X + dx[dir];
			int ndist = min_distance(ext.Y, ext.X, ny, nx);
			int pdist = min_distance(ext.Y, ext.X, nxt.Y, nxt.X);

			if (oom(ny, nx) || A[ny][nx] > 0) continue;
			if (ndist < pdist && dist > ndist) {
				dist = ndist;
				d = dir;
			}
		}

		if (d != -1) {
			int ny = nxt.Y + dy[d];
			int nx = nxt.X + dx[d];
			
			if (A[ny][nx] > 0) continue;

			if (ny == ext.Y && nx == ext.X) 
				player[i] = { -1, -1 };
			else 
				player[i] = { ny, nx };

			move_cnt++;
		}
	}
}

bool player_empty() {
	for (auto nxt : player)
		if (nxt.Y != -1 && nxt.X != -1) return false;
	return true;
}

struct rect {
	int y, x, sz, len;
};

bool compare(const rect& a, const rect& b) {
	if (a.sz == b.sz) {
		if (a.y == b.y) return a.x < b.x;
		return a.y < b.y;
	}
	return a.sz < b.sz;
}

rect min_rectangle() {
	vector<rect> tmp;

	for (int i = 0; i < player.size(); i++) {
		auto nxt = player[i];
		if (nxt.Y == -1 && nxt.X == -1) continue;

		int y = min(nxt.Y, ext.Y);
		int x = min(nxt.X, ext.X);
		int len = max(abs(nxt.Y - ext.Y), abs(nxt.X - ext.X));

		while (true) {
			if (y - 1 < 0) break;
			if (y - 1 + len < nxt.Y || y - 1 + len < ext.Y) break;
			y--;
		}
		while (true) {
			if (x - 1 < 0) break;
			if (x - 1 + len < nxt.X || x - 1 + len < ext.X) break;
			x--;
		}

		tmp.push_back({ y, x, (len + 1) * (len + 1), len });
	}

	sort(tmp.begin(), tmp.end(), compare);
	return tmp[0];
}

void rotate_rect(rect rec) {
	for (int i = 0; i < MX; i++)
		memset(B[i], 0, sizeof(B[i]));

	int ty, tx, by, bx;
	tie(ty, tx, by, bx) = make_tuple(rec.y, rec.x, rec.y + rec.len, rec.x + rec.len);

	int len = rec.len;

	for (int y = ty; y <= by; y++)
		for (int x = tx; x <= bx; x++)
			if (A[y][x] > 0) A[y][x]--;

	for (int y = ty; y <= by; y++)
		for (int x = tx; x <= bx; x++) {
			int oy = y - ty, ox = x - tx;
			int ry = ox, rx = len - oy;
			B[ry + ty][rx + tx] = A[y][x];
		}

	for (int y = ty; y <= by; y++)
		for (int x = tx; x <= bx; x++) {
			A[y][x] = B[y][x];
		}

	for (int i = 0; i < player.size(); i++) {
		auto nxt = player[i];
		if (nxt.Y == -1 && nxt.X == -1) continue;

		if (nxt.Y >= ty && nxt.Y <= by && nxt.X >= tx && nxt.X <= bx) {
			int oy = nxt.Y - ty, ox = nxt.X - tx;
			int ry = ox, rx = len - oy;
			player[i] = { ry + ty, rx + tx };
		}
	}

	int oy = ext.Y - ty, ox = ext.X - tx;
	int ry = ox, rx = len - oy;
	ext = { ry + ty, rx + tx };
}

void print() {
	cout << '\n';
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << A[i][j] << ' ';
		cout << '\n';
	}
	for (auto nxt : player) cout << nxt.Y << ' ' << nxt.X << '\n';
	cout << ext.Y << ' ' << ext.X << '\n';
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	freopen("input.txt", "r", stdin);
	cin >> N >> M >> K;
	
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> A[i][j];

	for (int y, x, i = 0; i < M; i++) {
		cin >> y >> x;
		player.push_back({ y - 1,x - 1 });
	}

	cin >> ext.Y >> ext.X;
	ext.Y--, ext.X--;

	while(K--) {
		move();
		if (player_empty()) break;
		rect ret = min_rectangle();
		rotate_rect(ret);
	}

	cout << move_cnt << '\n' << ext.Y + 1 << ' ' << ext.X + 1;

	return 0;
}