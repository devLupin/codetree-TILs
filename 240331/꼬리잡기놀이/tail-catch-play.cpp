#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int dx[] = { 0,-1,0,1 };
const int dy[] = { 1,0,-1,0 };

const int MEM_SZ = 6;
const int SZ = 21;

int N, M, K, board[SZ][SZ], ans, idx = -1;
bool vis[SZ][SZ];
vector<pii> head, tail;
vector<pii> member[MEM_SZ];
int cnt, attack_dir;

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void Init() {
	idx = -1;
	for (int i = 0; i < M; i++) member[i].clear();
	fill(&vis[0][0], &vis[N][N], false);
}

void SearchTail() {
	for (int i = 0; i < M; i++) {
		auto& nxt = head[i];
		int v = board[nxt.X][nxt.Y];

		for (int dir = 0; dir < 4; dir++) {
			int nx = nxt.X + dx[dir];
			int ny = nxt.Y + dy[dir];
			int nv = board[nx][ny];

			if (OOM(nx, ny)) continue;

			if (v < nv) {
				v = nv;
				tail[i] = { nx, ny };
			}
		}
	}
}

void DFS(int x, int y) {
	int cx, cy, cv = board[x][y];

	for (int dir = 0; dir < 4; dir++) {
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		int nv = board[nx][ny];

		if (OOM(nx, ny) || board[nx][ny] == 0 || vis[nx][ny]) continue;
		if (cv > nv)
			tie(cx, cy, cv) = make_tuple(nx, ny, nv);
	}

	if (board[x][y] == cv) {
		if (board[x][y] != 4)
			member[idx].push_back({ x, y });
		return;
	}

	if (cv == 1) idx++;
	member[idx].push_back({ x, y });
	swap(board[x][y], board[cx][cy]);

	vis[cx][cy] = true;
	DFS(cx, cy);
}

void MovePerson() {
	for (pii& nxt : tail) {
		vis[nxt.X][nxt.Y] = true;
		DFS(nxt.X, nxt.Y);
	}

	for (int i = 0; i < M; i++) {
		head[i] = member[i].front();
		tail[i] = member[i].back();
	}
}

tuple<int, int, int> Round(int t) {
	int x, y;

	if (cnt == N) {
		cnt = 0;
		attack_dir = (attack_dir + 1) % 4;
	}

	if (attack_dir == 0) tie(x, y) = make_tuple(cnt, 0);
	if (attack_dir == 1) tie(x, y) = make_tuple(N - 1, cnt);
	if (attack_dir == 2) tie(x, y) = make_tuple(cnt, N - 1);
	if (attack_dir == 3) tie(x, y) = make_tuple(0, cnt);

	return make_tuple(x, y, attack_dir);
}

void FindPerson(int x, int y) {
	for (int i = 0; i < M; i++) {
		int len = member[i].size();

		for (int j = 0; j < len; j++) {
			auto& nxt = member[i][j];
			if (nxt.X == x && nxt.Y == y) {
				j++;
				ans += (j * j);

				auto& cur_head = member[i][0];
				auto& cur_tail = member[i][len - 1];

				swap(board[cur_head.X][cur_head.Y], board[cur_tail.X][cur_tail.Y]);
				head[i] = cur_tail;

				SearchTail();

				head, tail;

				return;
			}
		}
	}
}

void Attack(tuple<int, int, int> info) {
	auto [x, y, dir] = info;
	bool flag = false;

	while (!OOM(x, y)) {
		if (board[x][y] > 0 && board[x][y] < 4) {
			FindPerson(x, y);
			return;
		}

		x += dx[dir];
		y += dy[dir];
	}
}

void Print() {
	cout << "\n\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> K;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> board[i][j];
			if (board[i][j] == 1) head.push_back({ i,j });
		}
	}

	tail.assign(head.begin(), head.end());
	SearchTail();

	for (int t = 1; t <= K; t++) {
		Init();
		MovePerson();
		auto info = Round(t);
		Attack(info);

		cnt++;
	}

	cout << ans;
	return 0;
}