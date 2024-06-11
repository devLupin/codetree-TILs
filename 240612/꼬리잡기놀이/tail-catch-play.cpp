#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tuple>
#include <vector>
#include <queue>
#include <algorithm>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int SZ = 20;
// 우, 상, 좌, 하
const int dx[] = { 0,-1,0,1 };
const int dy[] = { 1,0,-1,0 };

int N, M, K, board[SZ][SZ], tt, tdir, ans;
vector<vector<pii>> groups;
vector<pii> head;
vector<int> ngroup;

bool oom(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void print() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << "\n\n";
}

void grouping() {
	bool vis[SZ][SZ] = { false, };

	for (auto nxt : head) {
		queue<pii> q;
		vector<pii> v;
		int cnt = 0;

		q.push(nxt);
		vis[nxt.X][nxt.Y] = true;

		while (!q.empty()) {
			pii cur = q.front();
			q.pop();

			v.push_back(cur);
			if (board[cur.X][cur.Y] <= 3) cnt++;

			for (int dir = 0; dir < 4; dir++) {
				int nx = cur.X + dx[dir];
				int ny = cur.Y + dy[dir];

				if (!oom(nx, ny) && !vis[nx][ny] && board[nx][ny] != 0 && abs(board[cur.X][cur.Y] - board[nx][ny]) <= 1) {
					q.push({ nx, ny });
					vis[nx][ny] = true;
				}
			}
		}

		groups.push_back(v);
		ngroup.push_back(cnt);
	}
}

void move() {
	for (auto& g : groups) {
		vector<pii> nxt;
		int len = g.size();

		nxt.push_back(g[len - 1]);
		for (int i = 1; i < len; i++) {
			auto [x1, y1] = g[i - 1];
			auto [x2, y2] = g[i];

			nxt.push_back(g[i - 1]);
			swap(board[x1][y1], board[x2][y2]);
		}

		g = nxt;
	}
}

pii start() {
	int x, y;

	if (tt == N) {
		tt = 0;
		tdir = (tdir + 1) % 4;
	}

	if (tdir == 0) tie(x, y) = make_pair(tt, 0);
	else if (tdir == 1) tie(x, y) = make_pair(N - 1, tt);
	else if (tdir == 2) tie(x, y) = make_pair(N - tt - 1, N - 1);
	else if (tdir == 3) tie(x, y) = make_pair(0, N - tt - 1);

	return make_pair(x, y);
}

tiii target(pii pos) {
	int gnum = -1, rear = -1, order = -1;

	for (int i = 0; i < groups.size(); i++)
		for (int j = 0; j < groups[i].size(); j++) {
			pii cmp = groups[i][j];

			if (cmp == pos) {
				gnum = i;
				order = j;
			}
			if (gnum != -1 && board[cmp.X][cmp.Y] == 3)
				rear = j;
			if (gnum != -1 && rear != -1)
				return make_tuple(gnum, rear, order + 1);
		}
}

void attack() {
	auto [x, y] = start();

	while (!oom(x, y)) {
		int val = board[x][y];

		if (val > 0 && val < 4) {
			auto [gnum, ridx, order] = target({ x,y });
			pii& front = groups[gnum][0];
			pii& rear = groups[gnum][ridx];

			swap(board[front.X][front.Y], board[rear.X][rear.Y]);
			reverse(groups[gnum].begin(), groups[gnum].begin() + ngroup[gnum]);
			reverse(groups[gnum].begin() + ngroup[gnum], groups[gnum].end());

			ans += order * order;
			return;
		}

		x += dx[tdir];
		y += dy[tdir];
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> K;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			cin >> board[i][j];
			if (board[i][j] == 1) head.push_back({ i,j });
		}

	grouping();

	while (K--) {
		move();
		attack();
		tt++;
	}

	cout << ans;
	return 0;
}