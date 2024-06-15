#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

struct Info {
	int x, y, d;
	bool live;
};

const int SZ = 4;
const int DIR_NUM = 8;
const int dx[DIR_NUM] = { -1,-1,0,1,1,1,0,-1 };
const int dy[DIR_NUM] = { 0,-1,-1,-1,0,1,1,1 };

vector<vector<int>> board;
vector<Info> chess;
int P, D, ans;

void print() {
	for (int i = 0; i < SZ; i++) {
		for (int j = 0; j < SZ; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << '\n';
}

bool oom(int x, int y) { return x < 0 || y < 0 || x >= 4 || y >= 4; }

void change(int cc, int nn, int nd) {
	Info cur = chess[cc];
	Info nxt = chess[nn];

	swap(board[cur.x][cur.y], board[nxt.x][nxt.y]);

	if (board[nxt.x][nxt.y] == 0) {
		chess[cc].x = nxt.x;
		chess[cc].y = nxt.y;
	}
	else {
		chess[cc] = { nxt.x, nxt.y, nd, cur.live };
		chess[nn] = { cur.x, cur.y, nxt.d, nxt.live };
	}
}

void move() {
	for (int i = 1; i <= SZ * SZ; i++) {
		if (!chess[i].live) continue;

		auto [x, y, d, live] = chess[i];
		
		for (int k = 0; k < DIR_NUM; k++) {
			int nd = (d + k) % DIR_NUM;
			int nx = x + dx[nd];
			int ny = y + dy[nd];

			if (!oom(nx, ny) && board[nx][ny] >= 0) {
				change(i, board[nx][ny], nd);
				break;
			}
		}
	}
}

void dfs(int x, int y, int dir, int sum) {
	vector<vector<int>> copy_board = board;
	vector<Info> copy_chess = chess;
	ans = max(ans, sum);

	move();

	for (int i = 1; i < SZ; i++) {
		int nx = x + dx[dir] * i;
		int ny = y + dy[dir] * i;

		if (!oom(nx, ny) && board[nx][ny] > 0) {
			int num = board[nx][ny];

			board[nx][ny] = -1;
			chess[num].live = false;
			dfs(nx, ny, chess[num].d, sum + num);
			board[nx][ny] = num;
			chess[num].live = true;
		}
	}

	board = copy_board;
	chess = copy_chess;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	board.assign(SZ, vector<int>(SZ));
	chess.assign(SZ * SZ + 1, {});
	for (int i = 0; i < SZ; i++)
		for (int j = 0; j < SZ; j++) {
			cin >> P >> D;
			board[i][j] = P;
			chess[P] = { i, j, D - 1, true };
		}

	int num = board[0][0];
	board[0][0] = -1;
	chess[num].live = false;

	dfs(0, 0, chess[num].d, num);

	cout << ans;
	return 0;
}