#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

const int dx[] = { 0,-1,-1,0,1,1,1,0,-1 };
const int dy[] = { 0,0,-1,-1,-1,0,1,1,1 };
int ans;

void print(vector<vector<pii>> board) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			cout << board[i][j].first << ' ';
		cout << '\n';
	}
	cout << '\n';
}

bool oom(int x, int y) { return x < 0 || y < 0 || x >= 4 || y >= 4; }

void move(vector<vector<pii>>& board, int x, int y) {
	int d = board[x][y].second;

	int nx = x + dx[d];
	int ny = y + dy[d];

	while (oom(nx, ny) || board[nx][ny].first == -1) {
		d++;
		if (d == 9) d = 1;
		nx = x + dx[d];
		ny = y + dy[d];
	}

	board[x][y].second = d;
	swap(board[x][y], board[nx][ny]);
}

void move_chess(vector<vector<pii>>& board) {
	int nxt = 1;

	while (nxt <= 16) {
		bool flag = true;

		for (int i = 0; i < 4 && flag; i++) {
			for (int j = 0; j < 4; j++) {
				if (board[i][j].first == nxt) {
					flag = false;
					move(board, i, j);
					break;
				}
			}
		}

		nxt++;
	}
}

void solve(vector<vector<pii>> board, int x, int y, int d, int sum) {
	vector<pii> pos;

	for (int i = 1; i < 4; i++) {
		int nx = x + dx[d] * i;
		int ny = y + dy[d] * i;

		if (oom(nx, ny) || board[nx][ny].first == -1) continue;
		pos.push_back({ nx, ny });
	}

	vector<vector<pii>> undo(board.begin(), board.end());
	ans = max(ans, sum);

	for (auto& nxt : pos) {
		int nx = nxt.first;
		int ny = nxt.second;
		int val = board[nx][ny].first;
		int dir = board[nx][ny].second;

		board[nx][ny].first = -1;
		move_chess(board);
		solve(board, nx, ny, dir, sum + val);
		board[nx][ny].first = val;
		board = undo;
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	vector<vector<pii>> board;
	board.assign(4, {});

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int p, d;
			cin >> p >> d;

			board[i].push_back({ p,d });
		}
	}

	ans = board[0][0].first;
	board[0][0].first = -1;
	move_chess(board);

	solve(board, 0, 0, board[0][0].second, ans);

	cout << ans;

	return 0;
}