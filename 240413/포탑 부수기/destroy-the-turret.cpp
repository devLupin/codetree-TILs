#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;
using LL = long long;

const int SZ = 15;
const int INF = 0x3f3f3f;

// 우/하/좌/상
const int dx[] = { 0,1,0,-1,-1,-1,1,1 };
const int dy[] = { 1,0,-1,0,-1,1,1,-1 };

int N, M, K, board[SZ][SZ];
int vis[SZ][SZ];
bool is_attack[SZ][SZ];

struct Turlet { int p, t, x, y; };
vector<Turlet> turlets, next_turlets;

bool cmp(const Turlet& a, const Turlet& b) {
	if (a.p != b.p) return a.p < b.p;
	if (a.t != b.t) return a.t > b.t;
	if (a.x + a.y != b.x + b.y) return a.x + a.y > b.x + b.y;
	return a.y > b.y;
}

void Attacker(int k) {
	if (turlets.empty()) return;

	sort(turlets.begin(), turlets.end(), cmp);
	auto& [p, t, x, y] = turlets[0];
	p += N + M;
	board[x][y] = p;
	t = k;
}

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= M; }

// 우/하/좌/상
tiii Adjust(int x, int y, int dir) {
	if (OOM(x, y)) {
		if (x < 0) {
			x = N - 1;
			dir = 3;
		}
		else if (y < 0) {
			y = M - 1;
			dir = 2;
		}
		else if (x >= N) {
			x = 0;
			dir = 1;
		}
		else if (y >= M) {
			y = 0;
			dir = 0;
		}
	}

	return make_tuple(x, y, dir);
}

bool Lazer(pii src, pii dst) {
	queue<pii> q;
	q.push(src);
	vis[src.X][src.Y] = -1;

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		if (cur == dst) return true;

		for (int dir = 0; dir < 4; dir++) {
			int nx, ny, nd;
			tie(nx, ny, nd) = Adjust(cur.X + dx[dir], cur.Y + dy[dir], dir);

			if (board[nx][ny] > 0 && vis[nx][ny] == INF) {
				q.push({ nx, ny });
				vis[nx][ny] = (nd + 2) % 4;
			}
		}
	}

	return false;
}

void LazerAttack() {
	auto& st = turlets[turlets.size() - 1];
	auto& en = turlets[0];
	auto [x, y] = make_pair(st.x, st.y);

	is_attack[x][y] = true;
	is_attack[en.x][en.y] = true;

	board[x][y] = max(0, st.p - en.p);
	int pnt = en.p / 2;

	while (true) {
		int dir = vis[x][y];

		x += dx[dir];
		y += dy[dir];

		tie(x, y, dir) = Adjust(x, y, dir);
		if (x == en.x && y == en.y) break;

		board[x][y] = max(0, board[x][y] - pnt);
		is_attack[x][y] = true;
	}
}

void BombAttack() {
	auto& st = turlets[turlets.size() - 1];
	auto& en = turlets[0];
	auto [x, y] = make_pair(st.x, st.y);

	is_attack[x][y] = true;
	is_attack[en.x][en.y] = true;

	board[x][y] = max(0, st.p - en.p);
	int pnt = en.p / 2;

	for (int dir = 0; dir < 8; dir++) {
		int nx, ny, nd;
		tie(nx, ny, nd) = Adjust(x + dx[dir], y + dy[dir], dir);

		if (board[nx][ny] > 0) {
			board[nx][ny] = max(0, board[nx][ny] - pnt);
			is_attack[nx][ny] = true;
		}
	}
}

void Attack() {
	if (turlets.size() < 2) return;
	fill(&vis[0][0], &vis[N][M], INF);
	fill(&is_attack[0][0], &is_attack[N][M], false);

	auto& src = turlets.front();
	auto& dst = turlets.back();
	int pnt = board[src.x][src.y];

	if (Lazer({ src.x, src.y }, { dst.x, dst.y }))
		LazerAttack();

	else
		BombAttack();
}

int Maintain() {
	int ret = 0;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			if (!is_attack[i][j] && board[i][j] > 0)
				board[i][j]++;

	for (auto nxt : turlets) {
		nxt.p = board[nxt.x][nxt.y];
		ret = max(ret, nxt.p);
		if (nxt.p > 0) next_turlets.push_back(nxt);
	}

	turlets = next_turlets;
	next_turlets.clear();

	return ret;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> K;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++) {
			cin >> board[i][j];
			if (board[i][j] > 0)
				turlets.push_back({ board[i][j], 0, i, j });
		}

	int ans = 0;
	for (int k = 1; k <= K; k++) {
		if (turlets.size() < 2) break;
		Attacker(k);
		Attack();
		ans = Maintain();
	}

	cout << ans;
	return 0;
}