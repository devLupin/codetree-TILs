#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int SZ = 45;
const int MAX_N = 35;
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

struct Info { int r, c, h, w, k; };

int L, N, Q;
int board[SZ][SZ], knights[SZ][SZ];
int damage[MAX_N], hp[MAX_N];
Info st[MAX_N];

bool oom(int x, int y) { return x < 1 || y < 1 || x > L || y > L; }

void remove(int i) {
	auto& [r, c, h, w, k] = st[i];

	for (int x = r; x < r + h; x++)
		for (int y = c; y < c + w; y++)
			knights[x][y] = 0;
}

void move(set<int> node, int n, int d) {
	if (node.empty()) return;

	bool vis[MAX_N] = { false, };
	int nxt[SZ][SZ] = { 0, };
	int hp = st[n].k;

	for (int i : node) {
		auto& [r, c, h, w, k] = st[i];

		for (int x = r; x < r + h; x++)
			for (int y = c; y < c + w; y++)
				knights[x][y] = 0;
	}

	for (int i : node) {
		auto& [r, c, h, w, k] = st[i];

		r += dx[d];
		c += dy[d];

		for (int x = r; x < r + h; x++)
			for (int y = c; y < c + w; y++) {
				knights[x][y] = i;
				
				if (board[x][y] == 1) {
					st[i].k--;
					damage[i]++;
				}
			}

		vis[i] = true;
	}

	st[n].k = hp;

	for (int i : node)
		if (st[i].k <= 0) remove(i);
}

set<int> go(int i, int d) {
	set<int> ret;
	queue<pii> q;
	bool vis[SZ][SZ] = { false, };

	auto& [r, c, h, w, k] = st[i];

	for (int x = r; x < r + h; x++)
		for (int y = c; y < c + w; y++) {
			q.push({ x, y });
			vis[x][y] = true;
		}

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		ret.insert(knights[cur.X][cur.Y]);

		int nnx = cur.X + dx[d];
		int nny = cur.Y + dy[d];

		if (oom(nnx, nny) || board[nnx][nny] == 2) return {};

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			if (oom(nx, ny) || vis[nx][ny] || knights[nx][ny] != knights[cur.X][cur.Y]) continue;
			
			q.push({ nx, ny });
			vis[nx][ny] = true;
		}

		if (knights[nnx][nny] != 0) {
			q.push({ nnx, nny });
			vis[nnx][nny] = true;
		}
	}

	return ret;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	
	cin >> L >> N >> Q;

	for (int i = 1; i <= L; i++)
		for (int j = 1; j <= L; j++)
			cin >> board[i][j];

	for (int i = 1; i <= N; i++) {
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		
		for (int x = r; x < r + h; x++)
			for (int y = c; y < c + w; y++)
				knights[x][y] = i;
		st[i] = { r, c, h, w, k };
		hp[i] = k;
	}

	for (int i, d; Q--;) {
		cin >> i >> d;

		auto node = go(i, d);
		move(node, i, d);
	}

	int ans = 0;
	for (int i = 1; i <= N; i++)
		if (st[i].k > 0) ans += hp[i] - st[i].k;

	cout << ans;
	return 0;
}