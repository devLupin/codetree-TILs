#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int MAX_N = 100;
const int DIR_NUM = 4;
const int dx[DIR_NUM] = { -1,0,1,0 };
const int dy[DIR_NUM] = { 0,1,0,-1 };
const int ddx[DIR_NUM] = { 1,0,-1,0 };
const int ddy[DIR_NUM] = { 0,1,0,-1 };

int N, M, H, K, T, ans;
int runner[MAX_N + 1][MAX_N][MAX_N][DIR_NUM];
bool board[MAX_N][MAX_N];
int tx, ty;

int tcnt, tmax = 1, maxcnt, tdir;
bool rev;

int distance(int x, int y) { return abs(tx - x) + abs(ty - y); }
bool oom(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void moveR() {
	for(int x = 0; x < N; x++)
		for(int y = 0; y < N; y++)
			for (int k = 0; k < DIR_NUM; k++)
				if (runner[T - 1][x][y][k] > 0) {
					if (distance(x, y) > 3) {
						runner[T][x][y][k] += runner[T - 1][x][y][k];
						continue;
					}

					int nx, ny, d = k;

					if (oom(x + dx[d], y + dx[d])) d = (d + 2) % 4;

					nx = x + dx[d];
					ny = y + dy[d];

					if(nx == tx && ny == ty) runner[T][x][y][k] += runner[T - 1][x][y][k];
					else runner[T][nx][ny][d] += runner[T - 1][x][y][k];
				}
}

void moveT() {
	if (!rev) {
		tx += dx[tdir];
		ty += dy[tdir];
		
		tcnt++;
		if (tcnt == tmax) {
			tcnt = 0;
			tdir = (tdir + 1) % 4;

			maxcnt++;
			
			if (maxcnt == 2) {
				maxcnt = 0;
				tmax++;
			}
		}
	}

	else {
		tx += ddx[tdir];
		ty += ddy[tdir];

		tcnt--;
		if (tcnt == 0) {
			tcnt = tmax;
			tdir = (tdir + 1) % 4;

			maxcnt++;
			if (maxcnt == 2) {
				tmax--;
				maxcnt = 0;
			}
		}
	}

	if ((tx == 0 && ty == 0) || (tx == N / 2 && ty == N / 2)) {
		rev = !rev;
		tdir = 0;

		if (!rev) {
			tcnt = 0;
			tmax = 1;
			maxcnt = 0;
		}

		else {
			tmax = N - 1;
			tcnt = tmax;
		}
	}
}

void catchR () {
	int ttx, tty, sum = 0;
	if (!rev) tie(ttx, tty) = make_pair(dx[tdir], dy[tdir]);
	else tie(ttx, tty) = make_pair(ddx[tdir], ddy[tdir]);

	for (int i = 0; i < 3; i++) {
		int nx = tx + ttx * i;
		int ny = ty + tty * i;

		if (!oom(nx, ny) && !board[nx][ny]) 
			for (int k = 0; k < DIR_NUM; k++) {
				sum += runner[T][nx][ny][k];
				runner[T][nx][ny][k] = 0;
			}
	}

	ans += T * sum;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> N >> M >> H >> K;

	for (int x, y, d, i = 0; i < M; i++) {
		cin >> x >> y >> d;
		d = (d == 1) ? 1 : 2;
		x--, y--;
		runner[0][x][y][d]++;
	}

	for (int x, y, i = 0; i < H; i++) {
		cin >> x >> y;
		board[x - 1][y - 1] = true;
	}

	tie(tx, ty) = make_pair(N / 2, N / 2);

	while (T++ < K) {
		moveR();
		moveT();
		catchR();
	}

	cout << ans;
	return 0;
}