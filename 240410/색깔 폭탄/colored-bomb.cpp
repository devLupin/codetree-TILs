#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int SZ = 25;
const int BLACK = -1;
const int RAINBOW = 0;
const int EMPTY = 8;
const int MAX_M = 5;

const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };

int N, M, board[SZ][SZ], ans;
bool visited[SZ][SZ];

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

pair<int, vector<pii>> bfs(int sx, int sy) {
	int rcnt = 0;
	vector<pii> ret;

	queue<pii> q;
	bool vis[SZ][SZ] = { false, };

	q.push({ sx, sy });
	vis[sx][sy] = true;
	visited[sx][sy] = true;

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		ret.push_back({ cur.X, cur.Y });

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			if (OOM(nx, ny) || vis[nx][ny] || board[nx][ny] == BLACK) continue;

			if (board[nx][ny] == RAINBOW) {
				q.push({ nx,ny });
				vis[nx][ny] = true;
				rcnt++;
			}
			else if (board[nx][ny] == board[sx][sy]) {
				q.push({ nx, ny });
				vis[nx][ny] = true;
				visited[nx][ny] = true;
			}
		}
	}

	return make_pair(rcnt, ret);
}

bool compare(pair<int, vector<pii>>& a, pair<int, vector<pii>>& b) {
	if (a.second.size() != b.second.size())
		return a.second.size() > b.second.size();
	if (a.first != b.first) return a.first > b.first;

	pii apos, bpos;
	for(auto nxt : a.second)
		if (board[nxt.X][nxt.Y] != RAINBOW) {
			apos = nxt;
			break;
		}
	for (auto nxt : b.second)
		if (board[nxt.X][nxt.Y] != RAINBOW) {
			bpos = nxt;
			break;
		}

	if (apos.X != bpos.X) return apos.X > bpos.X;
	return apos.Y > bpos.Y;
}

vector<pii> Find() {
	vector<pair<int, vector<pii>>> tmp;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (!visited[i][j] && board[i][j] > 0 && board[i][j] < MAX_M) {
				auto [cnt, pos] = bfs(i, j);
				sort(pos.begin(), pos.end());

				tmp.push_back({cnt, pos});
			}
		}
	}

	sort(tmp.begin(), tmp.end(), compare);
	if (tmp.empty() || tmp[0].second.size() < 2)
		return {};

	int sz = tmp[0].second.size();

	ans += sz * sz;
	return tmp[0].second;
}

void RMBlock(vector<pii> pos) {
	for (auto& nxt : pos)
		board[nxt.X][nxt.Y] = EMPTY;
}

void Gravity() {
	for (int i = N - 2; i >= 0; i--) {
		for (int j = 0; j < N; j++) {
			if (board[i][j] == BLACK || board[i][j] == EMPTY) continue;

			auto [x, y] = make_pair(i + 1, j);
			while (!OOM(x, y) && board[x][y] == EMPTY) x++;

			swap(board[i][j], board[x - 1][y]);
		}
	}
}

void Rotate() {
	int next_board[SZ][SZ] = { 0, };

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			next_board[i][j] = board[j][N - i - 1];

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			board[i][j] = next_board[i][j];
}

void Print() {
	cout << "\n\n";

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (board[i][j] == BLACK) cout << 'B' << ' ';
			else if (board[i][j] == EMPTY) cout << 'E' << ' ';
			else cout << board[i][j] << ' ';
		}
		cout << '\n';
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];

	while (true) {
		fill(&visited[0][0], &visited[N][N], false);

		auto pos = Find();
		if (pos.empty()) break;

		RMBlock(pos);
		Gravity();
		Rotate();
		Gravity();
	}

	cout << ans;
	return 0;
}