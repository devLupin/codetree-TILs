#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

int N, M, K, p, d, d1, d2, d3, d4;

const int SZ = 25;
const int dx[] = { 0,-1,1,0,0 };
const int dy[] = { 0,0,0,-1,1 };

struct info {
	bool alive;
	int dir;
	vector<vector<int>> dirs;
};

pii pos[SZ * SZ];
pii board[SZ][SZ];
vector<info> players;
vector<pii> nxt_board[SZ][SZ];

void input() {
	cin >> N >> M >> K;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> p;
			if (p > 0) {
				pos[p] = { i, j };
				board[i][j] = { p, K };
			}
			else board[i][j] = { 0,0 };
		}
	}

	players.assign(M + 1, {});
	for (int i = 1; i <= M; i++) {
		cin >> d;
		players[i].alive = true;
		players[i].dir = d;
	}

	for (int i = 1; i <= M; i++) {
		players[i].dirs.assign(5, vector<int>(5));

		for (int j = 1; j <= 4; j++) {
			cin >> d1 >> d2 >> d3 >> d4;
			players[i].dirs[j] = { 0, d1, d2, d3, d4 };
		}
	}
}

bool is_alive() {
	for (int i = 2; i <= M; i++)
		if (players[i].alive) return false;
	return true;
}

bool oom(int x, int y) { return x < 1 || y < 1 || x > N || y > N; }

int next_direction(int i, int target) {
	info& cur = players[i];
	int x = pos[i].X;
	int y = pos[i].Y;
	int nd = cur.dir;

	for (int dir = 1; dir <= 4; dir++) {
		int ndir = cur.dirs[nd][dir];
		int nx = x + dx[ndir];
		int ny = y + dy[ndir];

		if (oom(nx, ny)) continue;
		if (board[nx][ny].first == target) return ndir;
	}

	return -1;
}

void move_player() {
	for (int i = 1; i <= M; i++) {
		if (!players[i].alive) continue;

		info& cur = players[i];
		int nx, ny, nd;

		nd = next_direction(i, 0);
		if (nd == -1) nd = next_direction(i, i);

		nx = pos[i].X + dx[nd];
		ny = pos[i].Y + dy[nd];

		nxt_board[nx][ny].push_back({ i, K });

		cur.dir = nd;
	}
}

void reduce_turn() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (board[i][j].second > 0) {
				board[i][j].second--;
				if (board[i][j].second == 0)
					board[i][j] = { 0,0 };
			}
		}
	}
}

void kill_player() {
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			auto& nxt = nxt_board[i][j];
			if (nxt.size() == 1) {
				board[i][j] = nxt[0];
				pos[nxt[0].first] = { i, j };
			}
			else if (nxt.size() > 1) {
				sort(nxt.begin(), nxt.end());
				board[i][j] = nxt[0];
				pos[nxt[0].first] = { i, j };

				for (int idx = 1; idx < nxt.size(); idx++) {
					int num = nxt[idx].first;
					players[num].alive = false;
				}
			}
		}
	}
}

void init() {
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++)
			nxt_board[i][j].clear();
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	input();

	int ans = -1;
	for(int t=0; t<=1000; t++) {
		if (!players[1].alive) break;
		if (is_alive()) {
			ans = t;
			break;
		}

		init();

		move_player();
		reduce_turn();
		kill_player();
	}

	cout << ans;

	return 0;
}