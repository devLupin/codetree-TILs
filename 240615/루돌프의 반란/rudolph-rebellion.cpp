#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <tuple>
#include <stack>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int INF = 987654321;
const int SZ = 50;
const int MAX_P = 30;

// U, R, D, L
const int dx[] = { -1,0,1,0,-1,-1,1,1 };
const int dy[] = { 0,1,0,-1,-1,1,1,-1 };

int N, M, P, C, D;
int rx, ry;
pii pos[MAX_P];
int board[SZ][SZ];
bool die[MAX_P];
int stun[MAX_P], ans[MAX_P];

bool oom(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }
int distance(int x, int y) { return (rx - x) * (rx - x) + (ry - y) * (ry - y); }
int distance(int x1, int y1, int x2, int y2) { return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); }

void interaction(stack<pii> st, int ddx, int ddy) {
	pii tp = st.top();
	int nx = tp.X + ddx, ny = tp.Y + ddy;

	while (!st.empty()) {
		auto [x, y] = st.top();
		int snum = board[x][y];
		st.pop();
		
		if (oom(nx, ny)) {
			die[snum] = true;
			board[x][y] = 0;
		}

		else {
			swap(board[x][y], board[nx][ny]);
			pos[snum] = { nx, ny };
		}

		nx -= ddx;
		ny -= ddy;
	}
}

void collision(int snum, int ddx, int ddy, int k) {
	stun[snum] = 2;

	auto [x, y] = pos[snum];
	int nx = x + ddx * k;
	int ny = y + ddy * k;

	if (oom(nx, ny)) {
		board[x][y] = 0;
		die[snum] = true;
	}
	else {
		if (board[nx][ny] == 0) {
			swap(board[x][y], board[nx][ny]);
			pos[snum] = { nx, ny };
		}
		else if (board[nx][ny] > 0) {
			stack<pii> st;
			st.push({ x, y });

			while (!oom(nx, ny) && board[nx][ny] != 0) {
				st.push({ nx, ny });
				nx += ddx;
				ny += ddy;
			}

			interaction(st, ddx, ddy);
		}
	}
}

void moveR() {
	vector<tiii> tmp;

	for (int i = 1; i <= P; i++) {
		if (die[i]) continue;

		auto [x, y] = pos[i];
		int dist = distance(x, y);
		tmp.push_back({ -dist, x, y });
	}

	sort(tmp.begin(), tmp.end(), greater<>());
	
	auto [dist, x, y] = tmp[0];
	int snum = board[x][y];
	int ddx = (rx < x) ? 1 : ((rx > x) ? -1 : 0);
	int ddy = (ry < y) ? 1 : ((ry > y) ? -1 : 0);
	int nx = rx + ddx;
	int ny = ry + ddy;

	if (nx == x && ny == y) {
		ans[snum] += C;
		collision(snum, ddx, ddy, C);
	}

	swap(board[rx][ry], board[nx][ny]);
	rx += ddx;
	ry += ddy;
}

void moveS() {
	for (int i = 1; i <= P; i++) {
		if (die[i] || stun[i]) continue;

		auto [x, y] = pos[i];
		int cdir = -1, cmp = distance(x, y);

		for (int dir = 0; dir < 4; dir++) {
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (oom(nx, ny) || board[nx][ny] > 0) continue;

			int ndist = distance(nx, ny);
			if (cmp > ndist) {
				cmp = ndist;
				cdir = dir;
				break;
			}
		}

		if (cdir == -1) continue;

		int nx = x + dx[cdir];
		int ny = y + dy[cdir];

		if (board[nx][ny] == -1) {
			ans[i] += D;
			collision(i, dx[cdir] * -1, dy[cdir] * -1, D - 1);
		}
		else {
			swap(board[x][y], board[nx][ny]);
			pos[i] = { nx, ny };
		}
	}
}

void wdecay() {
	for (int i = 1; i <= P; i++) {
		if (stun[i]) stun[i]--;
		if (!die[i]) ans[i]++;
	}
}

void print() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (board[i][j] == -1)
				cout << 9 << ' ';
			else cout << board[i][j] << ' ';
		}
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
		pos[n] = { x, y };
		board[x][y] = i;
	}

	while (M--) {
		moveR();
		moveS();
		wdecay();
	}

	for (int i = 1; i <= P; i++)
		cout << ans[i] << ' ';
	return 0;
}