#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int SZ = 75;
// 북, 동, 남, 서
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

int R, C, K;
int board[SZ][SZ];
bool escape_board[SZ][SZ];
bool stop;
pii new_pos;
int new_escape;

bool oom(int x, int y) { return x < 1 || y < 1 || x > R || y > C; }

bool check(pii n_pos) {
	for (int dir = 0; dir < 4; dir++) {
		int x = n_pos.X + dx[dir];
		int y = n_pos.Y + dy[dir];

		if (oom(x, y) || board[x][y] != 0) 
			return false;
	}
	return true;
}

void Move(pii pos, int escape) {
	// 남 -> 서 -> 동 순으로 이동
	if (check(make_pair(pos.X + 1, pos.Y)))
		Move(make_pair(pos.X + 1, pos.Y), escape);
	else if (check(make_pair(pos.X, pos.Y - 1)) && check(make_pair(pos.X + 1, pos.Y - 1)))
		Move(make_pair(pos.X + 1, pos.Y - 1), (escape + 3) % 4);
	else if (check(make_pair(pos.X, pos.Y + 1)) && check(make_pair(pos.X + 1, pos.Y + 1)))
		Move(make_pair(pos.X + 1, pos.Y + 1), (escape + 1) % 4);

	if (!stop) {
		new_pos = pos;
		new_escape = escape;
		stop = true;
		return;
	}
}

void Paint(pii pos, int num) {
	for (int dir = 0; dir < 4; dir++) {
		int x = pos.X + dx[dir];
		int y = pos.Y + dy[dir];

		board[x][y] = num;
	}
	board[pos.X][pos.Y] = num;
}

void Print() {
	for (int i = 4; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			cout << board[i][j] << ' ';
		}
		cout << '\n';
	}
	cout << "\n\n";
}

bool Out(pii pos) {
	for (int dir = 0; dir < 4; dir++) {
		int x = pos.X + dx[dir];
		int y = pos.Y + dy[dir];

		if (x < 4) return false;
	}
	if (pos.X < 4) return false;
	return true;
}

int BFS(pii pos) {
	queue<pii> q;
	bool vis[SZ][SZ] = { false, };

	q.push(pos);
	vis[pos.X][pos.Y] = true;
	int ret = pos.X;

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		ret = max(cur.X, ret);

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			if (!oom(nx, ny) && !vis[nx][ny] && board[cur.X][cur.Y] == board[nx][ny]) {
				q.push({ nx, ny });
				vis[nx][ny] = true;
			}
		}

		if (escape_board[cur.X][cur.Y]) {
			for (int dir = 0; dir < 4; dir++) {
				int nx = cur.X + dx[dir];
				int ny = cur.Y + dy[dir];

				if (!oom(nx, ny) && !vis[nx][ny] && board[nx][ny] != 0) {
					q.push({ nx, ny });
					vis[nx][ny] = true;
				}
			}
		}
	}

	return ret;
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> R >> C >> K;
	R += 3;

	int ans = 0;

	for (int c, d, i = 1; i <= K; i++) {
		stop = false;

		cin >> c >> d;
		Move(make_pair(2, c), d);

		Paint(new_pos, i);
		pii escape = make_pair(new_pos.X + dx[new_escape], new_pos.Y + dy[new_escape]);
		escape_board[escape.X][escape.Y] = true;

		if (!Out(new_pos)) {
			fill(&board[0][0], &board[R][C], 0);
			fill(&escape_board[0][0], &escape_board[R][C], false);
		}

		else
			ans += BFS(new_pos) - 3;
	}

	cout << ans;
	return 0;
}