#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

int N, Q, SZ, A[70][70], lv, sum, cluster;
int ddx[] = { 0,0,1,1 };
int ddy[] = { 0,1,1,0 };
int dx[] = { -1,1,0,0 };
int dy[] = { 0,0,-1,1 };
bool vis[70][70];

void input() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> Q;
	SZ = (1 << N);

	for (int i = 0; i < SZ; i++) {
		for (int j = 0; j < SZ; j++) {
			cin >> A[i][j];
			sum += A[i][j];
		}
	}
}

void rotate(int a, int b, int len) {
	int square = len / 2;
	vector<int> tmp;
	vector<pii> axis;

	for (int d = 0; d < 4; d++) {
		int x, y;
		tie(x, y) = make_pair(a + square * ddx[d], b + square * ddy[d]);
		axis.push_back({ x, y });
	}

	pii& last = axis.back();
	for (int i = last.first; i < last.first + square; i++)
		for (int j = last.second; j < last.second + square; j++)
			tmp.push_back(A[i][j]);

	for (int d = 3; d > 0; d--) {
		pii& cur = axis[d - 1];
		pii& nxt = axis[d];

		for (int i = 0; i < square; i++) {
			for (int j = 0; j < square; j++) {
				int x, y, nx, ny;
				tie(x, y, nx, ny) = make_tuple(cur.first + i, cur.second + j, nxt.first + i, nxt.second + j);

				A[nx][ny] = A[x][y];
			}
		}
	}
	
	for (int i = 0; i < square; i++) {
		for (int j = 0; j < square; j++) {
			int x, y, nx, ny;
			tie(x, y, nx, ny) = make_tuple(last.first + i, last.second + j, axis[0].first + i, axis[0].second + j);
			A[nx][ny] = tmp[i * square + j];
		}
	}
}

void remake() {
	int len = (1 << lv);

	for (int i = 0; i < SZ; i += len)
		for (int j = 0; j < SZ; j += len)
			rotate(i, j, len);
}

bool oom(int x, int y) { return x < 0 || y < 0 || x >= SZ || y >= SZ; }

void melt() {
	vector<pii> tmp;

	for (int i = 0; i < SZ; i++) {
		for (int j = 0; j < SZ; j++) {
			int cnt = 0;

			if (A[i][j] == 0) continue;

			for (int dir = 0; dir < 4; dir++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];

				if (oom(nx, ny) || A[nx][ny] == 0) continue;
				cnt++;
			}

			if (cnt < 3) tmp.push_back({ i, j });
		}
	}

	for (auto& nxt : tmp) {
		A[nxt.first][nxt.second]--;
		sum--;
	}
}

int bfs(int x, int y) {
	int ret = 1;
	queue<pii> q;
	q.push({ x, y });
	vis[x][y] = true;

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.first + dx[dir];
			int ny = cur.second + dy[dir];

			if (oom(nx, ny) || A[nx][ny] == 0 || vis[nx][ny]) continue;
			q.push({ nx, ny });
			vis[nx][ny] = true;
			ret++;
		}
	}

	return ret;
}

void solve() {
	while (Q--) {
		cin >> lv;
		remake();
		melt();
	}

	cout << sum << '\n';

	for (int i = 0; i < SZ; i++) {
		for (int j = 0; j < SZ; j++) {
			if (vis[i][j] || A[i][j] == 0) continue;
			int cmp = bfs(i, j);
			cluster = max(cluster, cmp);
		}
	}

	cout << cluster;
}

int main(void) {
	input();
	solve();

	return 0;
}