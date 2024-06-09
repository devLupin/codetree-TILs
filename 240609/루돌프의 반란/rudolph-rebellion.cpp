#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int SANTA_NUM = 35;
const int MAX_N = 55;

const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

int rx, ry; // rudolf pos
int N, M, P, C, D, ans[SANTA_NUM];
int board[MAX_N][MAX_N], stun[SANTA_NUM];
pii spos[SANTA_NUM];
bool die[SANTA_NUM];

int distance(int x, int y) { return (rx - x) * (rx - x) + (ry - y) * (ry - y); }

bool oom(int x, int y) { return x < 1 || y < 1 || x > N || y > N; }

void interaction(stack<pii> st, int ddx, int ddy) {
	auto [x, y] = st.top();
	int sx = x + ddx * 2;
	int sy = y + ddy * 2;

	while (!st.empty()) {
		pii cur = st.top();
		int num = board[cur.X][cur.Y];
		st.pop();

		sx -= ddx;
		sy -= ddy;

		if (oom(sx, sy)) {
			board[cur.X][cur.Y] = 0;
			die[num] = true;
		}

		else {
			swap(board[cur.X][cur.Y], board[sx][sy]);
			spos[num] = { sx, sy };
		}
	}
}

void crash(int num, int s, int ddx, int ddy) {
	if (s == 0) return;

	auto [x, y] = spos[num];
	int nx = x + ddx * s;
	int ny = y + ddy * s;

	if (oom(nx, ny)) {
		board[x][y] = 0;
		die[num] = true;
	}

	else {
		if (board[nx][ny] > 0) {
			stack<pii> st;
			st.push({ x, y });

			while (board[nx][ny] > 0) {
				st.push({ nx, ny });
				nx += ddx;
				ny += ddy;
			}

			interaction(st, ddx, ddy);
		}
		else {
			swap(board[x][y], board[nx][ny]);
			spos[num] = { nx, ny };
		}
	}
}

bool compare(tiii& a, tiii& b) {
	auto [d1, x1, y1] = a;
	auto [d2, x2, y2] = b;

	if (d1 != d2) return d1 < d2;
	if (x1 != x2) return x1 > x2;
	return y1 > y2;
}

void move_rudolf() {
	vector<tiii> v;

	for (int i = 1; i <= P; i++) {
		if (die[i]) continue;

		auto [sx, sy] = spos[i];
		int dist = distance(sx, sy);
		v.push_back(make_tuple(dist, sx, sy));
	}

	sort(v.begin(), v.end(), compare);

	auto [d, sx, sy] = v[0];
	int snum = board[sx][sy];

	int ddx = (rx < sx) ? 1 : ((rx > sx) ? -1 : 0);
	int ddy = (ry < sy) ? 1 : ((ry > sy) ? -1 : 0);
	
	rx += ddx;
	ry += ddy;

	if (board[rx][ry] > 0) {
		crash(snum, C, ddx, ddy);
		ans[snum] += C;
		stun[snum] = 2;
	}
}

void move_santa() {
	for (int i = 1; i <= P; i++) {
		if (die[i] || stun[i]) continue;

		auto [x, y] = spos[i];
		int ddir = -1;
		int cmp = distance(x, y);

		for (int dir = 0; dir < 4; dir++) {
			int nx = x + dx[dir];
			int ny = y + dy[dir];
			int nd = distance(nx, ny);

			if (board[nx][ny] <= 0 && nd < cmp) {
				cmp = nd;
				ddir = dir;
			}
		}

		if (ddir != -1) {
			int nx = x + dx[ddir];
			int ny = y + dy[ddir];
			ddir = (ddir + 2) % 4;

			if (nx == rx && ny == ry) {
				crash(i, D - 1, dx[ddir], dy[ddir]);
				ans[i] += D;
				stun[i] = 2;
			}

			else {
				swap(board[x][y], board[nx][ny]);
				spos[i] = { nx, ny };
			}
		}
	}
}

void done() {
	for (int i = 1; i <= P; i++) {
		if (!die[i]) ans[i]++;
		if (stun[i] > 0) stun[i]--;
	}
}

bool run() {
	for (int i = 1; i <= P; i++)
		if (!die[i]) return true;
	return false;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> N >> M >> P >> C >> D >> rx >> ry;

	for (int n, r, c, i = 1; i <= P; i++) {
		cin >> n >> r >> c;
		board[r][c] = n;
		spos[n] = { r, c };
	}

	while (M-- && run()) {
		move_rudolf();
		move_santa();
		done();
	}

	for (int i = 1; i <= P; i++) cout << ans[i] << ' ';
	return 0;
}