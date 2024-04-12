#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <queue>
#include <tuple>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

enum { BASE = 1, CONV = 2 };

const int SZ = 20;
const int MAX_M = 35;
const int DIR_NUM = 4;
const int INF = 0x3f3f3f;

// ↑, ←, →, ↓
const int dx[] = { -1,0,0,1 };
const int dy[] = { 0,-1,1,0 };

int N, M;
int bidx, cidx, pidx;
int board[SZ][SZ];
bool check[SZ][SZ];
bool arrived[MAX_M];
pii base[MAX_M], conv[MAX_M], person[MAX_M];

bool OOM(int x, int y) { return x < 1 || y <1 || x > N || y > N; }

int BFS(pii src, pii dst) {
	queue<tuple<int, int, int>> q;
	bool vis[SZ][SZ] = { false, };

	q.push(make_tuple(src.X, src.Y, 0));
	vis[src.X][src.Y] = true;

	while (!q.empty()) {
		auto [x, y, d] = q.front();
		q.pop();

		if (x == dst.X && y == dst.Y) return d;

		for (int dir = 0; dir < DIR_NUM; dir++) {
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (!OOM(nx, ny) && !vis[nx][ny] && !check[nx][ny]) {
				q.push(make_tuple(nx, ny, d + 1));
				vis[nx][ny] = true;
			}
		}
	}

	return INF;
}

void Move(int t) {
	int len = min(t, pidx);
	for (int i = 1; i <= len; i++) {
		if (arrived[i]) continue;

		auto& [x, y] = person[i];

		int ndir, cmp = INF;

		for (int dir = 0; dir < DIR_NUM; dir++) {
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (!OOM(nx, ny) && !check[nx][ny]) {
				int dist = BFS(make_pair(nx, ny), conv[i]);
				if (cmp > dist) {
					cmp = dist;
					ndir = dir;
				}
			}
		}

		if (cmp != INF) {
			x += dx[ndir];
			y += dy[ndir];

			if (make_pair(x, y) == conv[i]) {
				arrived[i] = true;
				check[x][y] = true;
			}
		}
	}
}

void Enter(int t) {
	int len = min(t, M);
	for (int i = 1; i <= len; i++) {
		if (person[i] != make_pair(0, 0))continue;

		pii cur_conv = conv[i];
		int idx, dist = INF;

		for (int b = 1; b <= bidx; b++) {
			pii cur_base = base[b];
			if (check[cur_base.X][cur_base.Y]) continue;

			int cmp = BFS(cur_conv, cur_base);

			if (dist > cmp) {
				dist = cmp;
				idx = b;
			}
		}

		person[i] = base[idx];
		check[base[idx].X][base[idx].Y] = true;
	}
	pidx = min(t, M);
}

bool Go() {
	for (int i = 1; i <= M; i++)
		if (!arrived[i]) return true;
	return false;
}

void Print() {
	for (int i = 1; i <= M; i++)
		cout << person[i].first << ' ' << person[i].second << '\n';
	cout << "\n\n";
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	bidx = cidx = 1;

	cin >> N >> M;
	for (int i = 1; i <= N; i++)
		for (int num, j = 1; j <= N; j++) {
			cin >> num;
			if (num == 1) {
				base[bidx++] = { i,j };
				board[i][j] = BASE;
			}
		}
	bidx--;

	for (int x, y, i = 0; i < M; i++) {
		cin >> x >> y;
		conv[cidx++] = { x,y };
		board[x][y] = CONV;
	}
	cidx--;

	int t = 1;
	while (true) {
		Move(t);
		Enter(t);
		if (!Go()) break;
		t++;
	}

	cout << t;
	return 0;
}