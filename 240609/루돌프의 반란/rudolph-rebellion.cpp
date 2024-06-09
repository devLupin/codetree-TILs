#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

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
	while (!st.empty()) {
		pii cur = st.top();
		st.pop();

		int snum = board[cur.X][cur.Y];
		int nx = cur.X + ddx;
		int ny = cur.Y + ddy;

		if (oom(nx, ny)) {
			die[snum] = true;
			board[cur.X][cur.Y] = 0;
		}
		else {
			swap(board[cur.X][cur.Y], board[nx][ny]);
			spos[snum] = { nx, ny };
		}
	}
}

void crash(int num, int s, int ddx, int ddy) {
	pii santa = spos[num];
	int nx = santa.X + ddx * s;
	int ny = santa.Y + ddy * s;

	if (ddx * s == 0 && ddy * s == 0) return;

	if (board[nx][ny] > 0) {
		stack<pii> st;
		st.push(spos[num]);

		while (!oom(nx, ny) && board[nx][ny] > 0) {
			st.push({ nx, ny });
			nx += ddx;
			ny += ddy;
		}

		interaction(st, ddx, ddy);
	}
	else {
		if (oom(nx, ny)) {
			die[num] = true;
			board[santa.X][santa.Y] = 0;
		}
		else {
			swap(board[nx][ny], board[santa.X][santa.Y]);
			spos[num] = { nx, ny };
		}
	}
}

void move_rudolf() {
	int snum, cmp = MAX_N * MAX_N;

	for (int x = N; x > 0; x--)
		for (int y = N; y > 0; y--) {
			if (board[x][y] > 0 && !die[board[x][y]]) {
				int dist = distance(x, y);
				if (cmp > dist) {
					cmp = dist;
					snum = board[x][y];
				}
			}
		}

	pii santa = spos[snum];
	int ddx = (rx < santa.X) ? 1 : ((rx == santa.X) ? 0 : -1);
	int ddy = (ry < santa.Y) ? 1 : ((ry == santa.Y) ? 0 : -1);
	int nx = rx + ddx;
	int ny = ry + ddy;

	if (board[nx][ny] > 0) {
		crash(snum, C, ddx, ddy);
		ans[snum] += C;
		stun[snum] = 2;
	}

	swap(board[nx][ny], board[rx][ry]);
	rx += ddx;
	ry += ddy;
}

void move_santa() {
	for (int i = 1; i <= P; i++) {
		if (stun[i] > 0 || die[i]) continue;

		pii cur = spos[i];
		int cmp = distance(cur.X, cur.Y);
		int ddir = -1;

		for(int dir=0; dir<4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			if (oom(nx, ny) || board[nx][ny] > 0) continue;

			int dist = distance(nx, ny);
			if (cmp > dist) {
				cmp = dist;
				ddir = dir;
			}
		}

		if (ddir != -1) {
			int nx = cur.X + dx[ddir];
			int ny = cur.Y + dy[ddir];
			int nd = (ddir + 2) % 4;

			if (board[nx][ny] == -1) {
				crash(i, D - 1, dx[nd], dy[nd]);
				ans[i] += D;
				stun[i] = 2;
			}

			else {
				swap(board[nx][ny], board[cur.X][cur.Y]);
				spos[i] = { nx, ny };
			}
		}
	}
}

void done() {
	for (int i = 1; i <= P; i++) {
		if (!die[i]) {
			ans[i]++;
			if (stun[i] > 0) stun[i]--;
		}
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

	// freopen("input.txt", "r", stdin);
	cin >> N >> M >> P >> C >> D >> rx >> ry;

	for (int n, r, c, i = 1; i <= P; i++) {
		cin >> n >> r >> c;
		board[r][c] = n;
		spos[n] = { r, c };
	}

	board[rx][ry] = -1;

	while (M-- && run()) {
		move_rudolf();
		move_santa();
		done();
	}

	for (int i = 1; i <= P; i++) cout << ans[i] << ' ';
	return 0;
}