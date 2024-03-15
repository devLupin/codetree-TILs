#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int BLACK = 6;
const int RED = 7;

const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };

int N, M, board[25][25], cpy[25][25], ans;

bool oom(int x, int y) { return  x < 0 || y < 0 || x >= N || y >= N; }

bool compare(pii& a, pii& b) {
	if (board[a.X][a.Y] == RED) return false;
	if (board[b.X][b.Y] == RED) return true;

	if (a.X == b.X) return a.Y < b.Y;
	return a.X > b.X;
}

pair<vector<pii>, int> bfs(int x, int y) {
	vector<pii> ret;
	bool vis[25][25] = { false, };
	queue<pii> q;
	int color = board[x][y];
	int r_cnt = 0;

	q.push({ x, y });
	vis[x][y] = true;

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		if (board[cur.X][cur.Y] == RED) r_cnt++;
		ret.push_back({ cur.X, cur.Y });

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			if (oom(nx, ny) || vis[nx][ny]) continue;

			int n_color = board[nx][ny];
			if (n_color == color || n_color == RED) {
				q.push({ nx, ny });
				vis[nx][ny] = true;
			}
		}
	}

	int sz = ret.size();

	if (sz >= 2) {
		if (r_cnt == sz) return { {}, 0 };
		else {
			sort(ret.begin(), ret.end(), compare);
			return make_pair(ret, r_cnt);
		}
	}
	return { {}, 0 };
}

bool compare3(pii& a, pii& b) {
	if (a.X == b.X) return a.Y < b.Y;
	return a.X > b.X;
}

void gravity() {
	for (int i = N-1; i >= 0; i--) {
		for (int j = 0; j < N; j++) {
			if (board[i][j] != 0) continue;

			int nx = i, ny = j;
			while (board[nx][ny] == 0) nx--;

			if (oom(nx, ny) || board[nx][ny] == BLACK) continue;
			swap(board[nx][ny], board[i][j]);
		}
	}
}

void rotate() {
	fill(&cpy[0][0], &cpy[N][N], 0);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cpy[i][j] = board[j][N - i - 1];

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			board[i][j] = cpy[i][j];
}

void print() {
	cout << '\n';
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (board[i][j] == BLACK) cout << 'B' << ' ';
			else if (board[i][j] == RED) cout << 'R' << ' ';
			else cout << board[i][j] << ' ';
		}
		cout << '\n';
	}
}

bool compare2(pair<vector<pii>, int>& a, pair<vector<pii>, int>& b) {
	if (a.first.size() == b.first.size()) {
		if (a.second == b.second) {
			if (a.first.front().X == b.first.front().X) return a.first.front().Y < b.first.front().Y;
			return a.first.front().X > b.first.front().X;
		}
		return a.second < b.second;
	}
	return a.first.size() > b.first.size();
}

void go() {
	while (true) {
		vector<pair<vector<pii>, int>> v;

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (board[i][j] == BLACK || board[i][j] == 0) continue;

				auto res = bfs(i, j);
				if (res.first.empty()) continue;
				v.push_back(res);
			}
		}

		if (v.empty()) break;
		sort(v.begin(), v.end(), compare2);

		auto& prior = v[0].first;

		for (auto nxt : prior)
			board[nxt.X][nxt.Y] = 0;
		gravity();
		rotate();
		gravity();

		ans += prior.size() * prior.size();
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> board[i][j];
			if (board[i][j] == 0)
				board[i][j] = RED;
			else if (board[i][j] == -1)
				board[i][j] = BLACK;
		}
	}

	go();
	cout << ans;

	return 0;
}