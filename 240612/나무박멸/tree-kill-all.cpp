#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

const int MAX_N = 20;
const int dx[] = { -1,1,0,0,-1,-1,1,1 };
const int dy[] = { 0,0,-1,1,-1,1,1,-1 };

int N, M, K, C, ans;
int board[MAX_N][MAX_N], herbicide[MAX_N][MAX_N];

bool oom(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void growth() {
	int tmp[MAX_N][MAX_N] = { 0, };

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			if (board[i][j] <= 0) continue;

			for (int k = 0; k < 4; k++) {
				int nx = i + dx[k];
				int ny = j + dy[k];

				if (!oom(nx, ny) && board[nx][ny] > 0)
					tmp[i][j]++;
			}
		}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			board[i][j] += tmp[i][j];
}

void breeding() {
	int tmp[MAX_N][MAX_N] = { 0, };

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			if (board[i][j] <= 0) continue;

			for (int k = 0; k < 4; k++) {
				int nx = i + dx[k];
				int ny = j + dy[k];

				if (!oom(nx, ny) && board[nx][ny] == 0 && herbicide[nx][ny] == 0)
					tmp[i][j]++;
			}

			if (tmp[i][j] == 0) continue;
			
			int val = board[i][j] / tmp[i][j];
			tmp[i][j] = 0;

			for (int k = 0; k < 4; k++) {
				int nx = i + dx[k];
				int ny = j + dy[k];

				if (!oom(nx, ny) && board[nx][ny] == 0 && herbicide[nx][ny] == 0)
					tmp[nx][ny] += val;
			}
		}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			board[i][j] += tmp[i][j];
}

void spread() {
	int x, y, cmp = 0;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			if (board[i][j] <= 0) continue;
			
			int sum = board[i][j];

			for (int k = 4; k < 8; k++) {
				int nx = i + dx[k];
				int ny = j + dy[k];
				int cnt = K;

				while (!oom(nx, ny) && board[nx][ny] >= 0 && cnt--) {
					sum += board[nx][ny];
					nx += dx[k];
					ny += dy[k];
				}
			}

			if (cmp < sum) {
				cmp = sum;
				x = i, y = j;
			}
		}

	if (cmp > 0) {

		herbicide[x][y] = C + 1;
		board[x][y] = 0;

		for (int k = 4; k < 8; k++) {
			int nx = x + dx[k];
			int ny = y + dy[k];
			int cnt = K;

			while (!oom(nx, ny) && cnt--) {
				herbicide[nx][ny] = C + 1;

				if (board[nx][ny] <= 0) break;

				board[nx][ny] = 0;
				nx += dx[k];
				ny += dy[k];
			}
		}

		ans += cmp;
	}
}

void update() {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (herbicide[i][j]) herbicide[i][j]--;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> N >> M >> K >> C;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];
	
	while (M--) {
		growth();
		breeding();
		spread();
		update();
	}

	cout << ans;
	return 0;
}