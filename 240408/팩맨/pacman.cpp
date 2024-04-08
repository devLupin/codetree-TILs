#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int dx[] = { -1,-1,0,1,1,1,0,-1 };
const int dy[] = { 0,-1,-1,-1,0,1,1,1 };
const int ddx[] = { -1,0,1,0 };
const int ddy[] = { 0,-1,0,1 };

const int MAX_T = 30;
const int MAX_N = 4;
const int DIR_NUM = 8;
const int DECAY = 2;

int M, T, cur_t = 1;
int px, py;
int mst[MAX_T + 1][MAX_N][MAX_N][DIR_NUM];
int dead[MAX_N][MAX_N][DECAY + 1];

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= MAX_N || y >= MAX_N; }

bool CanGo(int x, int y) {
	if (OOM(x, y)) return false;
	if (x == px && y == py) return false;
	if (dead[x][y][0] || dead[x][y][1]) return false;
	return true;
}

void MoveM() {
	for (int x = 0; x < MAX_N; x++) {
		for (int y = 0; y < MAX_N; y++) {
			for (int k = 0; k < DIR_NUM; k++) {
				if (mst[cur_t - 1][x][y][k] > 0) {
					tuple<int, int, int> n_pos;
					bool flag = false;

					for (int i = 0; i < DIR_NUM; i++) {
						int dir = (k + i) % DIR_NUM;
						int nx = x + dx[dir];
						int ny = y + dy[dir];

						if (CanGo(nx, ny)) {
							n_pos = make_tuple(nx, ny, dir);
							flag = true;
							break;
						}
					}
					if (!flag) n_pos = make_tuple(x, y, k);

					auto [nx, ny, nd] = n_pos;
					mst[cur_t][nx][ny][nd] += mst[cur_t - 1][x][y][k];
				}
			}
		}
	}
}

int GetKilledNum(int d1, int d2, int d3) {
	auto [x, y] = make_pair(px, py);
	int ret = 0;

	bool vis[MAX_N][MAX_N] = { false, };

	for (int dir : {d1, d2, d3}) {
		int nx = x + ddx[dir];
		int ny = y + ddy[dir];

		if (OOM(nx, ny)) return -1;
		if (!vis[nx][ny]) {
			for (int k = 0; k < DIR_NUM; k++)
				ret += mst[cur_t][nx][ny][k];
			vis[nx][ny] = true;
		}

		tie(x, y) = make_pair(nx, ny);
	}

	return ret;
}

void MoveP() {
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
		int nx = px + ddx[dir];
		int ny = py + ddy[dir];

		for (int k = 0; k < DIR_NUM; k++) {
			dead[nx][ny][DECAY] += mst[cur_t][nx][ny][k];
			mst[cur_t][nx][ny][k] = 0;
		}

		tie(px, py) = make_pair(nx, ny);
	}
}

void DecayM() {
	for (int i = 0; i < MAX_N; i++) {
		for (int j = 0; j < MAX_N; j++) {
			for (int k = 0; k < DECAY; k++)
				dead[i][j][k] = dead[i][j][k + 1];
			dead[i][j][DECAY] = 0;
		}
	}
}

void AddM() {
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < DIR_NUM; k++)
				mst[cur_t][i][j][k] += mst[cur_t - 1][i][j][k];
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> M >> T;
	cin >> px >> py;
	px--, py--;

	for (int x, y, d, i = 0; i < M; i++) {
		cin >> x >> y >> d;
		x--, y--, d--;
		(mst[0][x][y][d])++;
	}

	while (cur_t <= T) {
		MoveM();
		MoveP();
		DecayM();
		AddM();
		cur_t++;
	}

	int cnt = 0;
	for (int i = 0; i < MAX_N; i++)
		for (int j = 0; j < MAX_N; j++)
			for (int k = 0; k < DIR_NUM; k++)
				cnt += mst[cur_t - 1][i][j][k];

	cout << cnt;

	return 0;
}