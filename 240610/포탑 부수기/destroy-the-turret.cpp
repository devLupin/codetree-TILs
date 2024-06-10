#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;
using LL = long long;

const int SZ = 15;

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

void Attacker(Turlet weak) { board[weak.x][weak.y] += N + M; }

bool BFS(Turlet w, Turlet s) {
	queue<pii> q;

	q.push({ w.x, w.y });
	vis[w.x][w.y] = true;

	back_x[w.x][w.y] = -1;
	back_y[w.x][w.y] = -1;

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		if (cur == make_pair(s.x, s.y))
			return true;

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

	return false;
}

void LazerAttack(Turlet w, Turlet s, int dis) {
	int p = board[w.x][w.y];
	auto [x, y] = make_pair(s.x, s.y);
	board[x][y] = max(0, board[x][y] - dis);
	p /= 2;

	while (true) {
		int nx = back_x[x][y];
		int ny = back_y[x][y];
		tie(x, y) = make_pair(nx, ny);

		if (back_x[x][y] == -1 && back_y[x][y] == -1)
			break;

		board[x][y] = max(board[x][y] - p, 0);
		attacked[x][y] = true;
	}
}

void BombAttack(Turlet w, Turlet s, int dis) {
	int p = board[w.x][w.y];
	auto [x, y] = make_pair(s.x, s.y);
	board[x][y] = max(0, board[x][y] - dis);
	p /= 2;

	for (int dir = 0; dir < 8; dir++) {
		int nx = (s.x + dx[dir] + N) % N;
		int ny = (s.y + dy[dir] + M) % M;

		if (board[nx][ny] > 0 && make_pair(nx, ny) != make_pair(w.x, w.y)) {
			board[nx][ny] = max(board[nx][ny] - p, 0);
			attacked[nx][ny] = true;
		}
	}
}

void Attack(Turlet weak, Turlet strong) {
	int dis = board[weak.x][weak.y];

	if (BFS(weak, strong))
		LazerAttack(weak, strong, dis);
	else
		BombAttack(weak, strong, dis);

	attacked[weak.x][weak.y] = true;
	attacked[strong.x][strong.y] = true;
}

int Update() {
	int ret = 0;

	for (auto& nxt : turlets) {
		if (!attacked[nxt.x][nxt.y] && board[nxt.x][nxt.y] > 0)
			board[nxt.x][nxt.y]++;

		if (board[nxt.x][nxt.y] > 0) {
			nxt.p = board[nxt.x][nxt.y];
			next_turlets.push_back(nxt);
			ret = max(ret, nxt.p);
		}
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
	sort(turlets.begin(), turlets.end(), cmp);
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

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
		if(turlets.size() < 2) break;
		
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