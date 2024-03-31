#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int MEM_SZ = 6;
const int SZ = 21;
const int DIR_SZ = 4;

const int dx[DIR_SZ] = { 0,-1,0,1 };
const int dy[DIR_SZ] = { 1,0,-1,0 };

int N, M, K, board[SZ][SZ], ans, idx;
bool vis[SZ][SZ];
vector<pii> head, tail, compare;
vector<pii> member[MEM_SZ];
int cnt, attack_dir;

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void Init() {
	idx = -1;
	compare.clear();
	fill(&vis[0][0], &vis[N][N], false);
	for (int i = 0; i < M; i++) member[i].clear();
}

void GetCompare() {
	for (auto& nxt : head) {
		int cmp = board[nxt.X][nxt.Y];
		int cx, cy;

		for (int dir = 0; dir < DIR_SZ; dir++) {
			int nx = nxt.X + dx[dir];
			int ny = nxt.Y + dy[dir];

			if (OOM(nx, ny) || board[nx][ny] == 0) continue;
			if (cmp < board[nx][ny]) {
				cmp = board[nx][ny];
				tie(cx, cy) = make_pair(nx, ny);
			}
		}

		compare.push_back({ cx, cy });
	}
}

void DFS(int x, int y) {
	int cv = board[x][y];
	int cx, cy;

	for (int dir = 0; dir < DIR_SZ; dir++) {
		int nx = x + dx[dir];
		int ny = y + dy[dir];

		if (OOM(nx, ny) || board[nx][ny] == 0 || vis[nx][ny]) continue;

		if (cv > board[nx][ny]) {
			cv = board[nx][ny];
			tie(cx, cy) = make_pair(nx, ny);
		}
	}

	if (cv == board[x][y]) {
		if (cv == 3) member[idx].push_back({ x, y });
		return;
	}

	if (board[cx][cy] == 1) idx++;

	member[idx].push_back({ x, y });
	swap(board[x][y], board[cx][cy]);
	vis[cx][cy] = true;
	
	DFS(cx, cy);
}

void MovePerson() {
	for (auto& nxt : compare) {
		vis[nxt.X][nxt.Y] = true;
		DFS(nxt.X, nxt.Y);
		member;
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

void Search(int x, int y) {
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < member[i].size(); j++) {
			auto& nxt = member[i][j];

			if (x == nxt.X && y == nxt.Y) {
				pii tmp = head[i];
				head[i] = tail[i];
				tail[i] = tmp;

				swap(board[head[i].X][head[i].Y], board[tail[i].X][tail[i].Y]);

				j++;
				ans += (j * j);
				return;
			}
		}
	}
}

void Attack(tuple<int,int,int> next_round) {
	auto& [x, y, dir] = next_round;

	while (!OOM(x, y)) {
		if (board[x][y] > 0 && board[x][y] < 4) {
			Search(x, y);
			break;
		}

		x += dx[dir];
		y += dy[dir];
	}
}

void Print() {
	cout << "\n\n";
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << board[i][j] << ' ';
		}
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
			else if (board[i][j] == 3) tail.push_back({ i,j });
		}
	}

	for (int t = 1; t <= K; t++) {
		Init();
		GetCompare();
		MovePerson();
		Attack(Round(t));
		cnt++;
	}

	cout << ans;
	return 0;
}