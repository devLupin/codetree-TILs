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
bool attacked[15][15];

const int dx[] = { 0,1,0,-1,-1,-1,1,1 };
const int dy[] = { 1,0,-1,0,-1,1,1,-1 };

bool Compare(const info& a, const info& b) {
	if (a.p == b.p) {
		if (a.t == b.t) {
			if (a.x + a.y == b.x + b.y) return a.y > b.y;
			return a.x + a.y > b.x + b.y;
		}
		return a.t > b.t;
	}
	return a.p < b.p;
}

void Select(info weak, int t) {
	int x = weak.x;
	int y = weak.y;

	turlet[x][y].p += N + M;
	turlet[x][y].t = t;
	board[x][y] += N + M;
	attacked[x][y] = true;
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

void lazer_attack(info& weak, info& strong, int dis) {
	int x = strong.x;
	int y = strong.y;

	turlet[x][y].p -= dis;
	board[x][y] -= dis;
	attacked[x][y] = true;

	tie(x, y) = vis[x][y];
	dis /= 2;
	while (true) {
		if (x == weak.x && y == weak.y) break;

		turlet[x][y].p -= dis;
		board[x][y] -= dis;
		attacked[x][y] = true;

		tie(x, y) = vis[x][y];
	}
}

void bomb_attack(info& weak, info& strong, int dis) {
	int x = strong.x;
	int y = strong.y;
	
	turlet[x][y].p -= dis;
	board[x][y] -= dis;
	attacked[x][y] = true;

	tie(x, y) = make_pair(strong.x, strong.y);
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
		board[nx][ny] -= dis;
		attacked[nx][ny] = true;
	}
}

void maintain(int t) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (board[i][j] <= 0 || attacked[i][j]) continue;
			
			turlet[i][j].p += 1;
			board[i][j] += 1;
		}
	}
}

void new_turlet() {
	v.clear();

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
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
		fill(&attacked[0][0], &attacked[N][N], false);
		new_turlet();

		if (v.size() < 2) break;
		sort(v.begin(), v.end(), Compare);

		auto weak = v.front();
		auto strong = v.back();

		Select(weak, t);
		int dis = board[weak.x][weak.y];

		if (lazer(make_pair(weak.x, weak.y), make_pair(strong.x, strong.y)))
			lazer_attack(weak, strong, dis);
		else bomb_attack(weak, strong, dis);

		maintain(t);
	}

	int ans = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			ans = max(ans, board[i][j]);

	cout << ans;

	return 0;
}