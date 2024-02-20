#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

int N, A[500][500];
int sx, sy, sd, cnt = 1, ans;

int dx[] = { 0,1,0,-1 };
int dy[] = { -1,0,1,0 };
const int wx[4][9] =
{
	{-1, 1, -1, 1, -1, 1, -2, 2, 0},
	{-1, -1, 0, 0, 1, 1, 0, 0, 2},
	{-1, 1, -1, 1, -1, 1, -2, 2, 0 },
	{1, 1, 0, 0, -1, -1, 0, 0, -2 }
};

const int wy[4][9] =
{
	{1, 1, 0, 0, -1, -1, 0, 0, -2},
	{-1, 1, -1, 1, -1, 1, -2, 2, 0},
	{-1, -1, 0, 0, 1, 1, 0, 0, 2},
	{-1, 1, -1, 1, -1, 1, -2, 2, 0}
};
int percent[] = { 1, 1, 7, 7, 10, 10, 2, 2, 5 };

bool oom(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void simul() {
	int v = A[sx][sy];
	int x, y;

	for (int i = 0; i < 9; i++) {
		x = sx + wx[sd][i];
		y = sy + wy[sd][i];

		int cur = (v * percent[i]) / 100;
		A[sx][sy] -= cur;

		if (oom(x, y)) {
			ans += cur;
			continue;
		}

		A[x][y] += cur;
	}

	x = sx + dx[sd];
	y = sy + dy[sd];
	if (oom(x, y)) {
		ans += A[sx][sy];
		A[sx][sy] = 0;
		return;
	}
	A[x][y] += A[sx][sy];
	A[sx][sy] = 0;
}

void move() {
	for (int i = 0; i < cnt; i++) {
		sx += dx[sd];
		sy += dy[sd];
		simul();
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	cin >> N;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> A[i][j];

	tie(sx, sy, sd) = make_tuple(N / 2, N / 2, 0);

	for (int i = 1; i < N; i++) {
		for (int j = 1; j <= 2; j++) {
			move();
			sd = (sd + 1) % 4;
		}
		cnt++;
	}

	cnt--;
	move();
	cout << ans;

	return 0;
}