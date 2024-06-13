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

int GetKilledNum(int d1, int d2, int d3) {
	auto [x, y] = make_pair(px, py);
	int ret = 0;

	bool vis[SZ][SZ] = { false, };

	for (int dir : {d1, d2, d3}) {
		int nx = x + ddx[dir];
		int ny = y + ddy[dir];

		if (oom(nx, ny)) return -1;
		if (!vis[nx][ny]) {
			for (int k = 0; k < DIR_NUM; k++)
				ret += monster[TURN][nx][ny][k];
			vis[nx][ny] = true;
		}

		tie(x, y) = make_pair(nx, ny);
	}

	return ret;
}

void moveP() {
	auto [x, y] = make_pair(px, py);
	vector<int> routes;
	int cmp = -1;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				int cnt = GetKilledNum(i, j, k);
				if (cmp < cnt) {
					cmp = cnt;
					routes = { i, j, k };
				}
			}
		}
	}

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