#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int SZ = 100;
const int DIR_NUM = 4;
const int dx[] = { 0,0,1,-1 };
const int dy[] = { -1,1,0,0 };
const int fx[] = { -1,0,1,0 };
const int fy[] = { 0,1,0,-1 };
const int bx[] = { 1,0,-1,0 };
const int by[] = { 0,1,0,-1 };

int runner[SZ + 1][SZ][SZ][DIR_NUM];
int N, M, H, K, board[SZ][SZ], ans;
int tx, ty;
int tDir, cnt, dirChanged, dirCnt = 1;
bool flag;

int Distance(int x, int y) { return abs(tx - x) + abs(ty - y); }

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void MoveRunner(int t) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < DIR_NUM; k++) {
				if (runner[t - 1][i][j][k] > 0) {
					if (Distance(i, j) > 3) {
						runner[t][i][j][k] += runner[t - 1][i][j][k];
						continue;
					}

					int d = k;
					int nx = i + dx[d];
					int ny = j + dy[d];

					if (OOM(nx, ny)) {
						if (d == 0) d = 1;
						else if (d == 1) d = 0;
						else if (d == 2) d = 3;
						else if (d == 3) d = 2;
					}

					nx = i + dx[d];
					ny = j + dy[d];

					if (nx == tx && ny == ty) runner[t][i][j][k] += runner[t - 1][i][j][k];
					else runner[t][nx][ny][d] += runner[t - 1][i][j][k];
				}
			}
		}
	}
}

void MoveTagger() {
	if (!flag) {
		tx += fx[tDir];
		ty += fy[tDir];

		cnt++;
		if (cnt == dirCnt) {
			tDir = (tDir + 1) % 4;

			cnt = 0;
			dirChanged++;

			if (dirChanged == 2) {
				dirCnt++;
				dirChanged = 0;
			}
		}
	}
	else {
		tx += bx[tDir];
		ty += by[tDir];

		cnt--;
		if (cnt == 0) {
			tDir = (tDir + 1) % 4;

			cnt = dirCnt;
			dirChanged++;

			if (dirChanged == 2) {
				dirCnt--;
				dirChanged = 0;
			}
		}
	}

	if ((tx == 0 && ty == 0) || (tx == N / 2 && ty == N / 2)) {
		flag = !flag;
		tDir = 0;

		if (!flag) {
			cnt = 0;
			dirCnt = 1;
			dirChanged = 0;
		}
		else {
			dirCnt = N - 1;
			cnt = dirCnt;
		}
	}
}

void Catch(int t) {
	vector<int> ddx(4), ddy(4);
	int sum = 0;

	if (!flag) {
		for (int i = 0; i < 4; i++) {
			ddx[i] = fx[i];
			ddy[i] = fy[i];
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			ddx[i] = bx[i];
			ddy[i] = by[i];
		}
	}

	int nx = tx;
	int ny = ty;

	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 4; k++) {
			if (runner[t][nx][ny][k] > 0 && board[nx][ny] != 1) {
				sum += runner[t][nx][ny][k];
				runner[t][nx][ny][k] = 0;
			}
		}
		nx += ddx[tDir];
		ny += ddy[tDir];
	}

	ans += sum * t;
}

void Print(int t) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < DIR_NUM; k++) {
				if (runner[t][i][j][k] > 0) {
					cout << i + 1 << ' ' << j + 1 << ' ' << runner[t][i][j][k] << '\n';
				}
			}
		}
	}
	cout << "\n";
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> H >> K;
	tie(tx, ty) = make_pair(N / 2, N / 2);

	for (int x, y, d; M--;) {
		cin >> x >> y >> d;
		runner[0][x - 1][y - 1][d]++;
	}

	for (int x, y; H--;) {
		cin >> x >> y;
		board[x - 1][y - 1] = 1;
	}

	for (int t = 1; t <= K; t++) {
		MoveRunner(t);
		MoveTagger();
		Catch(t);
	}

	cout << ans;
	return 0;
}