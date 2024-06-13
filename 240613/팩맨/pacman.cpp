#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int SZ = 4;
const int MAX_T = 30;
const int DIR_NUM = 8;
const int INF = 1000005;

const int dx[DIR_NUM] = { -1,-1,0,1,1,1,0,-1 };
const int dy[DIR_NUM] = { 0,-1,-1,-1,0,1,1,1 };
const int ddx[DIR_NUM] = { -1,0,1,0 };
const int ddy[DIR_NUM] = { 0,-1,0,1 };

int monster[MAX_T][SZ + 1][SZ + 1][DIR_NUM];
int die[SZ + 1][SZ + 1];
int M, T, px, py, TURN, cmp;
bool vis[SZ + 1][SZ + 1];
vector<int> routes;

bool oom(int x, int y) { return x < 0 || y < 0 || x >= SZ || y >= SZ; }

void copy() {
	for (int x = 0; x < SZ; x++)
		for (int y = 0; y < SZ; y++)
			for (int k = 0; k < DIR_NUM; k++)
				monster[TURN][x][y][k] += monster[TURN - 1][x][y][k];
}

void moveM() {
	for (int x = 0; x < SZ; x++)
		for (int y = 0; y < SZ; y++)
			for (int k = 0; k < DIR_NUM; k++)
				if (monster[TURN - 1][x][y][k] > 0) {
					int d = k;
					bool chk = false;

					for (int i = 0; i < DIR_NUM; i++) {
						int dir = (d + i) % DIR_NUM;
						int nx = x + dx[dir];
						int ny = y + dy[dir];

						if (oom(nx, ny) || die[nx][ny] > 0 || make_pair(px, py) == make_pair(nx, ny))
							continue;

						d = dir;
						chk = true;
						break;
					}

					if (chk) {
						int nx = x + dx[d];
						int ny = y + dy[d];
						monster[TURN][nx][ny][d] += monster[TURN - 1][x][y][k];
					}
					else
						monster[TURN][x][y][k] += monster[TURN - 1][x][y][k];
				}
}

void dfs(int x, int y, vector<int> dirs) {
	if (dirs.size() == 3) {
		int sum = 0;
		int x = px, y = py;

		for (int dir : dirs) {
			x += ddx[dir];
			y += ddy[dir];

			for (int k = 0; k < DIR_NUM; k++)
				sum += monster[TURN][x][y][k];
		}

		if (cmp < sum) {
			cmp = sum;
			routes = dirs;
		}

		return;
	}

	for (int dir = 0; dir < 4; dir++) {
		int nx = x + ddx[dir];
		int ny = y + ddy[dir];

		if (oom(nx, ny) || vis[nx][ny]) continue;

		vis[nx][ny] = true;
		dirs.push_back(dir);
		dfs(nx, ny, dirs);
		vis[nx][ny] = false;
		dirs.pop_back();
	}
}

void moveP() {
	routes.clear();
	cmp = 0;
	fill(&vis[0][0], &vis[SZ][SZ], false);

	dfs(px, py, {});

	for (int dir : routes) {
		px += ddx[dir];
		py += ddy[dir];

		for (int k = 0; k < DIR_NUM; k++)
			if (monster[TURN][px][py][k] > 0) {
				die[px][py] = 3;
				monster[TURN][px][py][k] = 0;
			}
	}
}

void wdecay() {
	for (int x = 0; x < SZ; x++)
		for (int y = 0; y < SZ; y++)
			if (die[x][y]) die[x][y]--;
}

void print() {
	for (int x = 0; x < SZ; x++) {
		for (int y = 0; y < SZ; y++) {
			int cnt = 0;
			for (int k = 0; k < DIR_NUM; k++)
				if (monster[TURN][x][y][k] > 0) cnt += monster[TURN][x][y][k];

			cout << cnt << ' ';
		}
		cout << '\n';
	}
	cout << '\n';
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> M >> T >> px >> py;
	px--, py--;

	for (int x, y, d, i = 0; i < M; i++) {
		cin >> x >> y >> d;
		x--, y--, d--;
		monster[0][x][y][d]++;
	}

	while (TURN++ < T) {
		moveM();
		moveP();
		copy();
		wdecay();
	}

	int ans = 0;
	for (int x = 0; x < SZ; x++)
		for (int y = 0; y < SZ; y++)
			for (int k = 0; k < DIR_NUM; k++)
				ans += monster[T][x][y][k];

	cout << ans;
	return 0;
}