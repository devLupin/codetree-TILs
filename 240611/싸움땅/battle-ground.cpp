#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tuple>
#include <vector>
#include <queue>
#include <algorithm>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int MAX_N = 25;
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

struct Info { int d, s, g; };

int N, M, K;
vector<int> gun[MAX_N][MAX_N];
vector<int> ans;
vector<Info> info;
vector<pii> pos;
int board[MAX_N][MAX_N];

bool oom(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void losser(int n) {
	auto& [x, y] = pos[n];
	auto& [d, s, g] = info[n];

	gun[x][y].push_back(g);
	g = 0;

	for (int i = 0; i < 4; i++) {
		int dir = (d + i) % 4;
		int nx = x + dx[dir];
		int ny = y + dy[dir];

		if (!oom(nx, ny) && board[nx][ny] == 0) {
			if (!gun[nx][ny].empty()) {
				sort(gun[nx][ny].begin(), gun[nx][ny].end());
				g = gun[nx][ny].back();
				gun[nx][ny].pop_back();
			}

			board[nx][ny] = n;
			tie(x, y) = { nx, ny };
			d = dir;
			break;
		}
	}
}

void winner(int n) {
	auto& [x, y] = pos[n];
	auto& [d, s, g] = info[n];

	gun[x][y].push_back(g);
	g = 0;

	sort(gun[x][y].begin(), gun[x][y].end());
	g = gun[x][y].back();
	gun[x][y].pop_back();

	board[x][y] = n;
}

void fight(int n1, int n2, int x, int y) {
	int win, lose;
	int sum1 = info[n1].s + info[n1].g;
	int sum2 = info[n2].s + info[n2].g;
	int point = abs(sum1 - sum2);

	if (sum1 > sum2) {
		win = n1;
		lose = n2;
	}
	else if (sum1 < sum2) {
		win = n2;
		lose = n1;
	}
	else {
		if (info[n1].s > info[n2].s) {
			win = n1;
			lose = n2;
		}
		else {
			win = n2;
			lose = n1;
		}
	}

	ans[win] += point;
	losser(lose);
	winner(win);
}

void move() {
	for (int i = 1; i <= M; i++) {
		auto& [x, y] = pos[i];
		auto& [d, s, g] = info[i];
		
		int nx = x + dx[d];
		int ny = y + dy[d];

		if (oom(nx, ny)) {
			d = (d + 2) % 4;
			nx = x + dx[d];
			ny = y + dy[d];
		}

		if (board[nx][ny] == 0) {
			gun[nx][ny].push_back(g);
			g = 0;

			sort(gun[nx][ny].begin(), gun[nx][ny].end());
			g = gun[nx][ny].back();
			gun[nx][ny].pop_back();

			swap(board[x][y], board[nx][ny]);
			tie(x, y) = { nx, ny };
		}

		else {
			int n1 = board[x][y], n2 = board[nx][ny];
			board[x][y] = 0;
			board[nx][ny] = 0;
			pos[i] = { nx, ny };
			fight(n1, n2, nx, ny);
		}
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> K;
	for (int i = 0; i < N; i++)
		for (int x, j = 0; j < N; j++) {
			cin >> x;
			gun[i][j].push_back(x);
		}

	info.assign(M + 1, {});
	pos.assign(M + 1, {});
	ans.assign(M + 1, 0);

	for (int x, y, d, s, i = 1; i <= M; i++) {
		cin >> x >> y >> d >> s;
		x--, y--;

		info[i] = { d, s, 0 };
		pos[i] = { x, y };
		board[x][y] = i;
	}

	while (K--) move();

	for (int i = 1; i <= M; i++) cout << ans[i] << ' ';
	return 0;
}