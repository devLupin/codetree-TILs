#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tuple>
#include <vector>
#include <stack>
#include <algorithm>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int MAX_N = 55;
const int MAX_M = 35;
const int INF = 987654321;

const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

int board[MAX_N][MAX_N];
pii pos[MAX_M];
int rx, ry;
int N, M, P, C, D;
bool die[MAX_M];
int stun[MAX_M];
int ans[MAX_M];

bool oom(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }
int distance(int x, int y) { return (rx - x) * (rx - x) + (ry - y) * (ry - y); }

bool compare(const tiii& a, const tiii& b) {
	auto [d1, x1, y1] = a;
	auto [d2, x2, y2] = b;

	if (d1 != d2) return d1 < d2;
	if (x1 != x2) return x1 > x2;
	return y1 > y2;
}

void interaction(stack<pii> st, int ddx, int ddy) {
	pii nxt = st.top();
	int nx = nxt.X + ddx * 2;
	int ny = nxt.Y + ddy * 2;

	while (!st.empty()) {
		auto [x, y] = st.top();
		int snum = board[x][y];
		nx -= ddx;
		ny -= ddy;
		st.pop();

		if (oom(nx, ny)) {
			board[x][y] = 0;
			die[snum] = true;
		}
		else {
			swap(board[x][y], board[nx][ny]);
			pos[snum] = { nx, ny };
		}
	}
}

void collision(int snum, int ddx, int ddy, int cnt) {
	if (cnt < 1) return;

	auto [x, y] = pos[snum];
	int nx = x + ddx * cnt;
	int ny = y + ddy * cnt;

	if (oom(nx, ny)) {
		board[x][y] = 0;
		die[snum] = true;
		return;
	}

	if (board[nx][ny] == 0) {
		swap(board[x][y], board[nx][ny]);
		pos[snum] = { nx, ny };
	}
	else {
		stack<pii> st;

		while (!oom(x, y) && board[x][y] != 0) {
			st.push({ x, y });
			x += ddx;
			y += ddy;
		}
		
		interaction(st, ddx, ddy);
	}
}

void moveR() {
	vector<tiii> v;

	for (int i = 1; i <= P; i++) {
		if (die[i]) continue;
		
		auto [x, y] = pos[i];
		int d = distance(x, y);
		v.push_back({ d,x,y });
	}

	sort(v.begin(), v.end(), compare);
	
	auto [sd, sx, sy] = v[0];
	int ddx = (rx < sx) ? 1 : ((rx > sx) ? -1 : 0);
	int ddy = (ry < sy) ? 1 : ((ry > sy) ? -1 : 0);

	int nx = rx + ddx;
	int ny = ry + ddy;
	int snum = board[nx][ny];

	if (snum > 0) {
		ans[snum] += C;
		stun[snum] = 2;
		collision(snum, ddx, ddy, C);
	}
	
	swap(board[rx][ry], board[nx][ny]);
	rx = nx;
	ry = ny;
}

void moveS() {
	for (int i = 1; i <= P; i++) {
		if (die[i] || stun[i]) continue;

		auto [x, y] = pos[i];
		int ndir = -1, cmp = distance(x, y);

		for (int dir = 0; dir < 4; dir++) {
			int nx = x + dx[dir];
			int ny = y + dy[dir];
			int nd = distance(nx, ny);

			if (oom(nx, ny) || board[nx][ny] > 0) continue;

			if (cmp > nd) {
				cmp = nd;
				ndir = dir;
			}
		}

		if (ndir == -1) continue;

		int nx = x + dx[ndir];
		int ny = y + dy[ndir];

		if (board[nx][ny] == -1) {
			ans[i] += D;
			stun[i] = 2;
			collision(i, -dx[ndir], -dy[ndir], D - 1);
		}
		else {
			swap(board[x][y], board[nx][ny]);
			pos[i] = { nx, ny };
		}
	}
}

void done() {
	for (int i = 1; i <= P; i++) {
		if (stun[i]) stun[i]--;
		if (!die[i]) ans[i]++;
	}
}

bool run() {
	for (int i = 1; i <= P; i++)
		if (!die[i]) return true;
	return false;
}

void print() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << '\n';
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> P >> C >> D;
	cin >> rx >> ry;
	rx--, ry--;
	board[rx][ry] = -1;

	for (int n, x, y, i = 1; i <= P; i++) {
		cin >> n >> x >> y;
		x--, y--;
		board[x][y] = n;
		pos[n] = { x, y };
	}

	while (M-- && run()) {
		moveR();
		moveS();
		done();
	}

	for (int i = 1; i <= P; i++) cout << ans[i] << ' ';
	return 0;
}