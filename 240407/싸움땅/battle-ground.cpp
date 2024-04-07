#include <bits/stdc++.h>
using namespace std;

const int SZ = 25;
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };
int N, M, K;
vector<int> board[SZ][SZ];
int pos[SZ][SZ];
vector<int> ans;

struct Info { int x, y, d, s, gun; };
vector<Info> player;

void Input() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> N >> M >> K;

	ans.assign(M + 1, 0);
	player.assign(M + 1, {});

	for (int i = 0; i < N; i++) {
		for (int x, j = 0; j < N; j++) {
			cin >> x;
			board[i][j].push_back(x);
		}
	}

	for (int x, y, d, s, i = 1; i <= M; i++) {
		cin >> x >> y >> d >> s;
		x--, y--;
		pos[x][y] = i;
		player[i] = { x, y, d, s, 0 };
	}
}

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void GetGun(int idx, int x, int y) {
	if (board[x][y].empty()) return;

	Info& cur = player[idx];
	if (cur.gun > 0) board[x][y].push_back(cur.gun);

	int mx = max_element(board[x][y].begin(), board[x][y].end()) - board[x][y].begin();
	cur.gun = board[x][y][mx];
	board[x][y].erase(board[x][y].begin() + mx);
}

void MoveLosser(int idx, int x, int y) {
	Info& cur = player[idx];
	int nx = x + dx[cur.d];
	int ny = y + dy[cur.d];

	if (OOM(nx, ny) || pos[nx][ny] > 0) {
		for (int cnt = 3; cnt--;) {
			cur.d = (cur.d + 1) % 4;
			nx = x + dx[cur.d];
			ny = y + dy[cur.d];

			if (!OOM(nx, ny) && pos[nx][ny] == 0) break;
		}
	}

	cur.x = nx;
	cur.y = ny;

	GetGun(idx, nx, ny);
	pos[nx][ny] = idx;
}

void MoveWinner(int idx, int x, int y) {
	GetGun(idx, x, y);
	pos[x][y] = idx;
}

void Fight(int p1, int p2, int x, int y) {
	Info& cur = player[p1];
	Info& cmp = player[p2];
	int cur_stat = cur.s + cur.gun;
	int cmp_stat = cmp.s + cmp.gun;
	int diff = abs(cur_stat - cmp_stat);
	int winner, losser;

	if (cur_stat > cmp_stat) {
		winner = p1;
		losser = p2;
	}
	else if (cur_stat < cmp_stat) {
		winner = p2;
		losser = p1;
	}
	else {
		if (cur.s > cmp.s) {
			winner = p1;
			losser = p2;
		}
		else {
			winner = p2;
			losser = p1;
		}
	}

	ans[winner] += diff;

	// 총 내려놓기
	if (player[losser].gun > 0) {
		board[x][y].push_back(player[losser].gun);
		player[losser].gun = 0;
	}

	pos[x][y] = 0;
	MoveLosser(losser, x, y);
	MoveWinner(winner, x, y);
}

void MovePlayer() {
	for (int i = 1; i <= M; i++) {
		Info& cur = player[i];
		int x = cur.x;
		int y = cur.y;

		int nx = x + dx[cur.d];
		int ny = y + dy[cur.d];

		if (OOM(nx, ny)) {
			cur.d = (cur.d + 2) % 4;
			nx = x + dx[cur.d];
			ny = y + dy[cur.d];
		}

		pos[x][y] = 0;
		cur.x = nx;
		cur.y = ny;

		if (pos[nx][ny] > 0)
			Fight(i, pos[nx][ny], nx, ny);

		else {
			GetGun(i, nx, ny);
			pos[nx][ny] = i;
		}
	}
}

int main(void) {
	Input();
	while (K--) MovePlayer();

	for (int i = 1; i <= M; i++) cout << ans[i] << ' ';
	return 0;
}