#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

const int dx[] = { 0,-1,-1,0,1,1,1,0,-1 };
const int dy[] = { 0,0,-1,-1,-1,0,1,1,1 };

struct info {
	int x, y, d;
	bool alive;
};
vector<info> chess;
vector<vector<int>> board;
int ans;

bool oom(int x, int y) { return x < 0 || y < 0 || x >= 4 || y >= 4; }

void swap_chess(int cur, int nxt) {
	info tmp = chess[cur];
	chess[cur].x = chess[nxt].x;
	chess[cur].y = chess[nxt].y;
	chess[nxt].x = tmp.x;
	chess[nxt].y = tmp.y;
}

void move_chess() {
	for (int i = 1; i <= 16; i++) {
		if (!chess[i].alive) continue;

		auto& cur = chess[i];

		int x = cur.x;
		int y = cur.y;
		int d = cur.d;
		int nx = x + dx[d];
		int ny = y + dy[d];

		bool flag = false;

		if (!oom(nx, ny)) {
			if (board[nx][ny] == 0) {
				flag = true;
				cur.x = nx;
				cur.y = ny;
				board[nx][ny] = i;
				board[x][y] = 0;
			}
			else if (board[nx][ny] != -1) {
				flag = true;
				swap_chess(i, board[nx][ny]);
				swap(board[x][y], board[nx][ny]);
			}
		}

		if (!flag) {
			int nd = d + 1;
			if (nd == 9) nd = 1;

			nx = x + dx[nd];
			ny = y + dy[nd];

			while (nd != d) {
				if (!oom(nx, ny)) {
					if (board[nx][ny] == 0) {
						cur.x = nx;
						cur.y = ny;
						board[nx][ny] = i;
						board[x][y] = 0;
						chess[i].d = nd;
						break;
					}
					else if (board[nx][ny] != -1) {
						swap_chess(i, board[nx][ny]);
						swap(board[x][y], board[nx][ny]);
						chess[i].d = nd;
						break;
					}
				}

				nd++;
				if (nd == 9) nd = 1;

				nx = x + dx[nd];
				ny = y + dy[nd];
			}
		}
	}
}

void solve(int x, int y, int dir, int sum) {
	ans = max(ans, sum);

	vector<vector<int>> cpy_board = board;
	vector<info> cpy_chess = chess;
	move_chess();

	for (int i = 1; i < 4; i++) {
		int nx = x + dx[dir] * i;
		int ny = y + dy[dir] * i;

		if (oom(nx, ny)) break;
		if (board[nx][ny] == 0) continue;

		int num = board[nx][ny];
		int nd = chess[num].d;

		board[x][y] = 0;
		board[nx][ny] = -1;
		chess[num].alive = false;

		solve(nx, ny, nd, sum + num);

		board[x][y] = -1;
		board[nx][ny] = num;
		chess[num].alive = true;
	}

	chess = cpy_chess;
	board = cpy_board;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	freopen("input.txt", "r", stdin);

	chess.assign(17, {});
	board.assign(4, {});

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int p, d;
			cin >> p >> d;
			chess[p] = { i, j, d, true };
			board[i].push_back(p);
		}
	}

	int num = board[0][0];
	int d = chess[num].d;
	chess[num].alive = false;
	board[0][0] = -1;

	solve(0, 0, d, num);
	cout << ans;

	return 0;
}