#include <iostream>
#include <tuple>
#include <vector>
#include <queue>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int MAX_N = 20;
// ↑, ←, →, ↓ 
const int dx[] = { -1,0,0,1 };
const int dy[] = { 0,-1,1,0 };

int N, M, board[MAX_N][MAX_N];
bool is_arrived[MAX_N][MAX_N];
vector<pii> base, conv, user;

bool oom(int x, int y) { return x < 0 || y < 0 || x > N || y > N; }

int bfs(pii src, pii dst) {
	queue<tiii> q;
	bool vis[MAX_N][MAX_N] = { false, };

	q.push({ src.X, src.Y, 0 });
	vis[src.X][src.Y] = true;

	while (!q.empty()) {
		auto [x, y, d] = q.front();
		q.pop();

		if (x == dst.X && y == dst.Y) return d;

		for (int dir = 0; dir < 4; dir++) {
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (oom(nx, ny) || vis[nx][ny] || is_arrived[nx][ny]) continue;
			
			q.push({ nx, ny, d + 1 });
			vis[nx][ny] = true;
		}
	}
}

void move(int t) {
	for (int i = 1; i < min(t, M + 1); i++) {
		if (is_arrived[conv[i].X][conv[i].Y]) continue;

		int cur, cmp = MAX_N * MAX_N;

		for (int dir = 0; dir < 4; dir++) {
			int nx = user[i].X + dx[dir];
			int ny = user[i].Y + dy[dir];

			if (oom(nx, ny) || is_arrived[nx][ny]) continue;

			int dist = bfs({ nx, ny }, conv[i]);

			if (cmp > dist) {
				cmp = dist;
				cur = dir;
			}
		}

		user[i].X += dx[cur];
		user[i].Y += dy[cur];

		if (user[i] == conv[i])
			is_arrived[conv[i].X][conv[i].Y] = true;
	}
}

void enter(int t) {
	if (t > M) return;

	int cur, cmp = MAX_N * MAX_N;

	for (int i = 1; i < base.size(); i++) {
		if (is_arrived[base[i].X][base[i].Y]) continue;
		
		int dist = bfs(conv[t], base[i]);
		if (cmp > dist) {
			cmp = dist;
			cur = i;
		}
	}

	is_arrived[base[cur].X][base[cur].Y] = true;
	user[t] = { base[cur].X, base[cur].Y };
}

bool run() {
	for (int i = 1; i <= M; i++)
		if (user[i] != conv[i]) return true;
	return false;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	base.push_back({});
	conv.push_back({});
	
	cin >> N >> M;
	user.assign(M + 1, {-1, -1});

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			cin >> board[i][j];
			if (board[i][j] == 1) base.push_back({ i,j });
		}

	for (int x, y, i = 0; i < M; i++) {
		cin >> x >> y;
		conv.push_back({ x - 1, y - 1 });
	}

	int t = 1;
	while (t < 10) {
		move(t);
		enter(t);
		if (!run()) break;
		t++;
	}

	cout << t;
	return 0;
}