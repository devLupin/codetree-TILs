#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

struct info { int x, y, t, p; };
struct tur { int t, p; };
vector<info> v;
int N, M, K, board[15][15];
pii vis[15][15];
tur turlet[15][15];

const int dx[] = { 0,1,0,-1,-1,-1,1,1 };
const int dy[] = { 1,0,-1,0,-1,1,1,-1 };

bool Compare(const info& a, const info& b) {
	if (a.p == b.p) {
		if (a.t == b.t) {
			if (a.x + a.y == b.x + b.y) return a.y > b.y;
			return a.x + a.y > b.x + b.y;
		}
		return a.t < b.t;
	}
	return a.p < b.p;
}

void Select(info weak, int t) {
	turlet[weak.x][weak.y].p += N + M;
	turlet[weak.x][weak.y].t = t;
	board[weak.x][weak.y] += N + M;
}

bool lazer(pii src, pii dst) {
	fill(&vis[0][0], &vis[N][N], make_pair(0, 0));
	queue<pii> q;

	q.push({ src.X, src.Y });
	vis[src.X][src.Y] = { -1,-1 };

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		if (cur.X == dst.X && cur.Y == dst.Y) return true;

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			if (nx < 0) nx = N - 1;
			else if (nx >= N) nx = 0;
			if (ny < 0) ny = M - 1;
			else if (ny >= M) ny = 0;

			if (board[nx][ny] == 0) continue;

			if (vis[nx][ny].first == 0 && vis[nx][ny].second == 0) {
				q.push({ nx, ny });
				vis[nx][ny] = { cur.X, cur.Y };
			}
		}
	}

	return false;
}

void lazer_attack(info& weak, info& strong, int t, int dis) {
	turlet[strong.x][strong.y].p -= dis; 
	turlet[strong.x][strong.y].t = t;
	board[strong.x][strong.y] -= dis;

	auto [x, y] = vis[strong.x][strong.y];
	dis /= 2;
	while (true) {
		if (x == weak.x && y == weak.y) break;

		turlet[x][y].p -= dis;
		turlet[x][y].t = t;
		board[x][y] -= dis;

		tie(x, y) = vis[x][y];
	}
}

void bomb_attack(info& weak, info& strong, int t, int dis) {
	turlet[strong.x][strong.y].p -= dis;
	turlet[strong.x][strong.y].t = t;
	board[strong.x][strong.y] -= dis;


	auto [x, y] = make_pair(strong.x, strong.y);
	dis /= 2;
	for (int dir = 0; dir < 8; dir++) {
		int nx = x + dx[dir];
		int ny = y + dy[dir];

		if (nx < 0) nx = N - 1;
		else if (nx >= N) nx = 0;
		if (ny < 0) ny = M - 1;
		else if (ny >= M) ny = 0;

		if (weak.x == nx && weak.y == ny) continue;
		if (board[nx][ny] <= 0) continue;

		turlet[nx][ny].p -= dis;
		turlet[nx][ny].t = t;
		board[nx][ny] -= dis;
	}
}

void maintain(int t) {
	for (int i = 0; i < N; i++) {
		for (int p, j = 0; j < N; j++) {
			if (board[i][j] <= 0 || turlet[i][j].t == t) continue;
			
			turlet[i][j].p += 1;
			board[i][j] += 1;
		}
	}
}

void new_turlet() {
	v.clear();

	for (int i = 0; i < N; i++) {
		for (int p, j = 0; j < N; j++) {
			auto cur = turlet[i][j];
			if (cur.p > 0) v.push_back({ i, j, cur.t, cur.p });
		}
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> K;
	for (int i = 0; i < N; i++) {
		for (int p, j = 0; j < N; j++) {
			cin >> p;
			board[i][j] = p;
			if (p > 0) turlet[i][j] = { 0, p };
		}
	}

	for (int t = 1; t <= K; t++) {
		new_turlet();

		if (v.size() < 2) break;
		sort(v.begin(), v.end(), Compare);

		auto weak = v.front();
		auto strong = v.back();

		Select(weak, t);
		int dis = board[weak.x][weak.y];

		if (lazer(make_pair(weak.x, weak.y), make_pair(strong.x, strong.y)))
			lazer_attack(weak, strong, t, dis);
		else bomb_attack(weak, strong, t, dis);

		maintain(t);
	}

	int ans = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			ans = max(ans, board[i][j]);

	cout << ans;

	return 0;
}