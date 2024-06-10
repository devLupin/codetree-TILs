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

int back_x[MAX_N][MAX_N], back_y[MAX_N][MAX_N];
int N, M, K, board[MAX_N][MAX_N], ans;
bool active[MAX_N][MAX_N], vis[MAX_N][MAX_N];
bool lazer_success;

struct Info { int p, x, y, t; };
vector<Info> turlet;

bool cmp(const Info& a, const Info& b) {
	if (board[a.x][a.y] != board[b.x][b.y]) return board[a.x][a.y] < board[b.x][b.y];
	if (a.t != b.t) return a.t > b.t;
	if (a.x + a.y != b.x + b.y) return a.x + a.y > b.x + b.y;
	return a.y > b.y;
}

void lazer(Info w, Info s) {
	queue<pii> q;

	q.push({ w.x, w.y });
	vis[w.x][w.y] = true;

	back_x[w.x][w.y] = -1;
	back_y[w.x][w.y] = -1;

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		if (cur == make_pair(s.x, s.y)) {
			lazer_success = true;
			break;
		}

		for (int dir = 0; dir < 4; dir++) {
			int nx = (cur.X + dx[dir] + N) % N;
			int ny = (cur.Y + dy[dir] + M) % M;

			if (!vis[nx][ny] && board[nx][ny] > 0) {
				q.push({ nx, ny });
				vis[nx][ny] = true;
				back_x[nx][ny] = cur.X;
				back_y[nx][ny] = cur.Y;
			}
		}
	}

	if (lazer_success) {
		int p = board[w.x][w.y] / 2;
		auto [x, y] = make_pair(s.x, s.y);

		while (true) {
			int nx = back_x[x][y];
			int ny = back_y[x][y];
			tie(x, y) = make_pair(nx, ny);

			if (back_x[x][y] == -1 && back_y[x][y] == -1)
				break;

			board[x][y] = max(board[x][y] - p, 0);
			active[x][y] = true;
		}
	}
}

void bomb(Info w, Info s) {
	int p = board[w.x][w.y] / 2;

	for (int dir = 0; dir < 8; dir++) {
		int nx = (s.x + dx[dir] + N) % N;
		int ny = (s.y + dy[dir] + M) % M;

		if (board[nx][ny] > 0 && make_pair(nx, ny) != make_pair(w.x, w.y)) {
			board[nx][ny] = max(board[nx][ny] - p, 0);
			active[nx][ny] = true;
		}
	}
}

void attack(int t) {
	Info& weak = turlet[0];
	Info& strong = turlet[turlet.size() - 1];

	weak.t = t;
	
	board[weak.x][weak.y] += N + M;
	board[strong.x][strong.y] = max(board[strong.x][strong.y] - board[weak.x][weak.y], 0);

	lazer(weak, strong);
	if (!lazer_success) bomb(weak, strong);

	for (auto& nxt : turlet)
		nxt.p = board[nxt.x][nxt.y];

	active[strong.x][strong.y] = true;
	active[weak.x][weak.y] = true;
}

int defense() {
	int ret = 0;
	for (auto& nxt : turlet) {
		if (nxt.p > 0 && !active[nxt.x][nxt.y]) {
			board[nxt.x][nxt.y]++;
			nxt.p++;
		}
		ret = max(ret, nxt.p);
	}
	return ret;
}

void init() {
	vector<Info> tmp;
	for (auto nxt : turlet)
		if (nxt.p > 0) tmp.push_back(nxt);

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
			if (board[i][j] > 0) turlet.push_back({ board[i][j],i,j,0 });
		}

	for (int i = 1; i <= K; i++) {
		init();
		attack(i);
		ans = defense();
	}

	cout << ans;
	return 0;
}