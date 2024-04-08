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

int M, T, cur_t;
pii packman;
int monster[MAX_T + 1][MAX_N][MAX_N][DIR_NUM];
int dead[MAX_N][MAX_N][3];

vector<int> routes;
int cmp;

void CopyMonster() {
	for (int i = 0; i < MAX_N; i++)
		for (int j = 0; j < MAX_N; j++)
			for (int k = 0; k < DIR_NUM; k++)
				monster[cur_t][i][j][k] += monster[cur_t - 1][i][j][k];
}

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= MAX_N || y >= MAX_N; }

void MoveMonster() {
	for (int i = 0; i < MAX_N; i++) {
		for (int j = 0; j < MAX_N; j++) {
			for (int k = 0; k < DIR_NUM; k++) {
				if (monster[cur_t - 1][i][j][k] > 0) {
					int nx = i + dx[k];
					int ny = j + dy[k];

					if (OOM(nx, ny) || (nx == packman.X && ny == packman.Y) || dead[nx][ny][1] || dead[nx][ny][2]) {
						int dir = k;

						for (int l = 0; l < 7; l++) {
							dir = (dir + 1) % DIR_NUM;
							nx = i + dx[dir];
							ny = j + dy[dir];

							if (!OOM(nx, ny) && !(nx == packman.X && ny == packman.Y) && !dead[nx][ny][0] && !dead[nx][ny][1]) {
								monster[cur_t][nx][ny][dir] += monster[cur_t - 1][i][j][k];
								break;
							}
						}
					}
					else 
						monster[cur_t][nx][ny][k] += monster[cur_t - 1][i][j][k];
				}
			}
		}
	}
}

void DFS(int x, int y, int cnt, vector<int> dirs) {
	if (cnt == 3) {
		int sum = 0;
		int nx = packman.X;
		int ny = packman.Y;

		bool vis[MAX_N][MAX_N] = { false, };
		vis[nx][ny] = true;

		for (int dir : dirs) {
			nx += ddx[dir];
			ny += ddy[dir];

			bool flag = false;
			for (int k = 0; k < DIR_NUM; k++) {
				if (vis[nx][ny]) continue;

				if (monster[cur_t][nx][ny][k] > 0) {
					sum += monster[cur_t][nx][ny][k];
					flag = true;
				}
			}

			if (flag) vis[nx][ny] = true;
		}

		if (cmp < sum) {
			cmp = sum;
			routes = dirs;
		}

		return;
	}

	for (int nd = 0; nd < 4; nd++) {
		int nx = x + ddx[nd];
		int ny = y + ddy[nd];

		if (!OOM(nx, ny)) {
			dirs.push_back(nd);
			DFS(nx, ny, cnt + 1, dirs);
			dirs.pop_back();
		}
	}
}

void MovePackman() {
	cmp = -1;
	routes.clear();

	DFS(packman.X, packman.Y, 0, {});
	auto [px, py] = packman;

	// 상좌하우
	for (int& nd : routes) {
		px += ddx[nd];
		py += ddy[nd];

		for (int k = 0; k < DIR_NUM; k++) {
			dead[px][py][2] += monster[cur_t][px][py][k];
			monster[cur_t][px][py][k] = 0;
		}
	}
	packman = { px, py };
}

void Decay() {
	for (int i = 0; i < M; i++)
		for (int j = 0; j < M; j++) {
			for (int k = 0; k < 2; k++)
				dead[i][j][k] = dead[i][j][k + 1];
			dead[i][j][2] = 0;
		}
}

int CountMonster() {
	int cnt = 0;

	for (int i = 0; i < M; i++)
		for (int j = 0; j < M; j++)
			for (int k = 0; k < DIR_NUM; k++)
				cnt += monster[cur_t][i][j][k];

	return cnt;
}

void Print() {
	for (int i = 0; i < MAX_N; i++) {
		for (int j = 0; j < MAX_N; j++) {
			int cnt = 0;
			for (int k = 0; k < DIR_NUM; k++) {
				if (monster[cur_t][i][j][k] > 0) cnt += monster[cur_t][i][j][k];
			}
			if(cnt > 0)
				cout << i << ' ' << j << ' ' << cnt << '\n';
		}
	}
	cout << '\n';
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> M >> T;
	cin >> packman.X >> packman.Y;
	packman.X--, packman.Y--;

	for (int x, y, d, i = 0; i < M; i++) {
		cin >> x >> y >> d;
		x--, y--, d--;
		monster[cur_t][x][y][d]++;
	}

	while (T--) {
		cur_t++;

		MoveMonster();
		MovePackman();
		Decay();
		CopyMonster();
	}

	cout << CountMonster();
	return 0;
}