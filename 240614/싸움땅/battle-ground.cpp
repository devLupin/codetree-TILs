#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
using pii = pair<int, int>;

const int MAX_N = 25;
const int MAX_M = 35;
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

struct Info { int d, s, g; };

int N, M, K;
vector<int> gun[MAX_N][MAX_N];
int board[MAX_N][MAX_N], ans[MAX_M];
pii pos[MAX_M];
Info player[MAX_M];

void print() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << '\n';
}

bool oom(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void losser(int i) {
	auto& [x, y] = pos[i];
	auto& [d, s, g] = player[i];

	gun[x][y].push_back(g);
	g = 0;
	for (int j = 0; j < 4; j++) {
		int dir = (d + j) % 4;
		int nx = x + dx[dir];
		int ny = y + dy[dir];

		if (!oom(nx, ny) && board[nx][ny] == 0) {
			x += dx[dir];
			y += dy[dir];
			board[x][y] = i;

			gun[x][y].push_back(g);
			sort(gun[x][y].begin(), gun[x][y].end());

			g = gun[x][y].back();
			gun[x][y].pop_back();

			d = dir;
			break;
		}
	}
}

void winner(int i) {
	auto [x, y] = pos[i];
	auto& [d, s, g] = player[i];

	board[x][y] = i;

	gun[x][y].push_back(g);
	sort(gun[x][y].begin(), gun[x][y].end());

	g = gun[x][y].back();
	gun[x][y].pop_back();
}

void fight(int n1, int n2) {
	int sg1 = player[n1].s + player[n1].g;
	int sg2 = player[n2].s + player[n2].g;
	int win, lose;

	if (sg1 > sg2) {
		win = n1;
		lose = n2;
	}
	else if (sg1 < sg2) {
		win = n2;
		lose = n1;
	}
	else if(player[n1].s > player[n2].s) {
		win = n1;
		lose = n2;
	}
	else {
		win = n2;
		lose = n1;
	}

	losser(lose);
	winner(win);
	ans[win] += abs(sg1 - sg2);
}

void moveP() {
	for (int i = 1; i <= M; i++) {
		auto& [x, y] = pos[i];
		auto& [d, s, g] = player[i];

		if (oom(x + dx[d], y + dy[d])) 
			d = (d + 2) % 4;

		int nx = x + dx[d];
		int ny = y + dy[d];

		if (board[nx][ny] > 0) {
			int n1 = board[x][y];
			int n2 = board[nx][ny];

			board[x][y] = 0;
			board[nx][ny] = 0;

			tie(x, y) = make_pair(nx, ny);
			fight(n1, n2);
		}
		
		else {
			swap(board[x][y], board[nx][ny]);
			tie(x, y) = make_pair(nx, ny);


			gun[x][y].push_back(g);
			sort(gun[x][y].begin(), gun[x][y].end());

			g = gun[x][y].back();
			gun[x][y].pop_back();
		}
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	
	cin >> N >> M >> K;

	for(int i=0; i<N; i++)
		for (int g, j = 0; j < N; j++) {
			cin >> g;
			gun[i][j].push_back(g);
		}
	for (int x, y, d, s, i = 1; i <= M; i++) {
		cin >> x >> y >> d >> s;
		x--, y--;
		board[x][y] = i;
		pos[i] = { x, y };
		player[i] = { d, s, 0 };
	}

	while (K--) 
		moveP();

	for (int i = 1; i <= M; i++) cout << ans[i] << ' ';
	return 0;
}