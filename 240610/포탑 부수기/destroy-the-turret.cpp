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
int back_x[SZ][SZ], back_y[SZ][SZ];
bool vis[SZ][SZ];
bool attacked[SZ][SZ];

struct Turlet { int p, t, x, y; };
vector<Turlet> turlets, next_turlets;

bool cmp(const Turlet& a, const Turlet& b) {
	if (a.p != b.p) return a.p < b.p;
	if (a.t != b.t) return a.t > b.t;
	if (a.x + a.y != b.x + b.y) return a.x + a.y > b.x + b.y;
	return a.y > b.y;
}

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= M; }

void Attacker(Turlet weak) { board[weak.x][weak.y] += N + M; }

bool BFS(Turlet src, Turlet dst) {
	queue<pii> q;
	q.push({ src.x, src.y });
	back_x[src.x][src.y] = -1;
	back_y[src.x][src.y] = -1;
	vis[src.x][src.y] = true;

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		if (cur == make_pair(dst.x, dst.y))
			return true;

		for (int dir = 0; dir < 4; dir++) {
			int nx = (cur.X + dx[dir] + N) % N;
			int ny = (cur.Y + dy[dir] + M) % M;

			if (board[nx][ny] > 0 && !vis[nx][ny]) {
				q.push({ nx, ny });
				back_x[nx][ny] = cur.X;
				back_y[nx][ny] = cur.Y;
				vis[nx][ny] = true;
			}
		}
	}

	return false;
}

void LazerAttack(Turlet strong, int dis) {
	auto [x, y] = make_pair(strong.x, strong.y);
	board[x][y] = max(0, board[x][y] - dis);
	dis /= 2;

	while (true) {
		int nx = back_x[x][y];
		int ny = back_y[x][y];
		tie(x, y) = make_pair(nx, ny);

		if (back_x[x][y] == -1 && back_y[x][y] == -1)
			break;

		board[x][y] = max(0, board[x][y] - dis);
		attacked[x][y] = true;
	}
}

void BombAttack(Turlet weak, Turlet strong, int dis) {
	auto [x, y] = make_pair(strong.x, strong.y);
	board[x][y] = max(0, board[x][y] - dis);
	dis /= 2;

	for (int dir = 0; dir < 8; dir++) {
		int nx = (x + dx[dir] + N) % N;
		int ny = (y + dy[dir] + M) % M;

		if (weak.x == nx && weak.y == ny) continue;

		if (board[nx][ny] > 0) {
			board[nx][ny] = max(0, board[nx][ny] - dis);
			attacked[nx][ny] = true;
		}
	}
}

void Attack(Turlet weak, Turlet strong) {
	int dis = board[weak.x][weak.y];

	if (BFS(weak, strong))
		LazerAttack(strong, dis);
	else
		BombAttack(weak, strong, dis);

	attacked[weak.x][weak.y] = true;
	attacked[strong.x][strong.y] = true;
}

int Update() {
	int ret = 0;

	for (auto& nxt : turlets) {
		if (!attacked[nxt.x][nxt.y] && nxt.p > 0) 
			board[nxt.x][nxt.y]++;
		nxt.p = board[nxt.x][nxt.y];
	}

	for (auto& nxt : turlets)
		if (nxt.p > 0) {
			ret = max(ret, nxt.p);
			next_turlets.push_back(nxt);
		}

	turlets = next_turlets;
	next_turlets.clear();

	return ret;
}

void Init() {
	fill(&vis[0][0], &vis[N][M], false);
	fill(&attacked[0][0], &attacked[N][M], false);
	fill(&back_x[0][0], &back_x[N][M], 0);
	fill(&back_y[0][0], &back_y[N][M], 0);
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
		Init();

		sort(turlets.begin(), turlets.end(), cmp);

		if (turlets.size() < 2) break;

		auto& weak = turlets.front();
		auto& strong = turlets.back();

		weak.t = k;
		Attacker(weak);
		Attack(weak, strong);

		ans = Update();
	}

	cout << ans;
	return 0;
}