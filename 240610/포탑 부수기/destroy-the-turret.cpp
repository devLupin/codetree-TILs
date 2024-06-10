#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tuple>
#include <vector>
#include <queue>
#include <algorithm>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int MAX_N = 15;
const int dx[] = { 0,1,0,-1,-1,-1,1,1 };
const int dy[] = { 1,0,-1,0,-1,1,1,-1 };

pii prv[MAX_N][MAX_N];
int N, M, K, board[MAX_N][MAX_N], ans;
bool active[MAX_N][MAX_N], vis[MAX_N][MAX_N];
bool lazer_success;

struct Info { int x, y, t; };
vector<Info> turlet;

bool cmp(const Info& a, const Info& b) {
	if (board[a.x][a.y] != board[b.x][b.y]) return board[a.x][a.y] < board[b.x][b.y];
	if (a.t != b.t) return a.t > b.t;
	if (a.x + a.y != b.x + b.y) return a.x + a.y > b.x + b.y;
	return a.y > b.y;
}

void lazer(int sx, int sy, int ex, int ey) {
	queue<pii> q;
	q.push({ sx, sy });
	vis[sx][sy] = true;
	prv[sx][sy] = { -1,-1 };

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		if (cur.X == ex && cur.Y == ey) {
			lazer_success = true;
			break;
		}

		for (int dir = 0; dir < 4; dir++) {
			int nx = (cur.X + dx[dir] + N) % N;
			int ny = (cur.Y + dy[dir] + M) % M;

			if (vis[nx][ny] || board[nx][ny] == 0) continue;

			q.push({ nx, ny });
			vis[nx][ny] = true;
			prv[nx][ny] = { cur.X, cur.Y };
		}
	}

	if (lazer_success) {
		int x = ex, y = ey;
		int p = board[sx][sy];

		board[x][y] = max(0, board[x][y] - p);
		p /= 2;

		while (prv[x][y] != make_pair(sx, sy)) {
			int nx = prv[x][y].X;
			int ny = prv[x][y].Y;
			tie(x, y) = make_pair(nx, ny);

			board[x][y] = max(0, board[x][y] - p);
			active[x][y] = true;
		}
	}
}

void bomb(int sx, int sy, int ex, int ey) {
	int p = board[sx][sy];

	board[ex][ey] = max(0, board[ex][ey] - p);
	p /= 2;

	for (int dir = 0; dir < 8; dir++) {
		int nx = (ex + dx[dir] + N) % N;
		int ny = (ey + dy[dir] + M) % M;

		if (board[nx][ny] == 0) continue;

		board[nx][ny] = max(0, board[nx][ny] - p);
		active[nx][ny] = true;
	}
}

void attack(int t) {
	Info w = turlet[0];
	Info s = turlet[turlet.size() - 1];

	board[w.x][w.y] += N + M;
	turlet[0].t = t;

	lazer(w.x, w.y, s.x, s.y);
	if (!lazer_success) bomb(w.x, w.y, s.x, s.y);

	active[w.x][w.y] = active[s.x][s.y] = true;
}

void defense() {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			if (!active[i][j] && board[i][j] > 0) board[i][j]++;
}

void init() {
	vector<Info> tmp;
	for (Info& nxt : turlet)
		if (board[nxt.x][nxt.y] > 0) tmp.push_back(nxt);
	turlet = tmp;

	sort(turlet.begin(), turlet.end(), cmp);
	fill(&active[0][0], &active[0][0] + N * M, false);
	fill(&vis[0][0], &vis[0][0] + N * M, false);
	lazer_success = false;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> N >> M >> K;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++) {
			cin >> board[i][j];
			if (board[i][j] > 0) turlet.push_back({ i,j,0 });
		}

	for (int i = 1; i <= K; i++) {
		init();
		attack(i);
		defense();
	}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			ans = max(board[i][j], ans);

	cout << ans;
	return 0;
}