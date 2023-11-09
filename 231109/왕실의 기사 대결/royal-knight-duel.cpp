#include <bits/stdc++.h>
#define Y first
#define X second
using namespace std;
using pii = pair<int, int>;

const int dy[] = { -1,0,1,0 };
const int dx[] = { 0,1,0,-1 };
const int SZ = 45;

int L, N, Q, A[SZ][SZ], K[SZ][SZ], ans;

struct knight {
	int r, c, h, w, k;
};
vector<knight> knights;

bool oom(int y, int x) { return y < 0 || x < 0 || y >= L || x >= L; }

set<int> check(int idx, int d) {
	auto target = knights[idx];
	set<int> ret;

	bool vis[SZ][SZ] = { false, };
	queue<pii> q;

	q.push({ target.r, target.c });
	vis[target.r][target.c] = true;

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		ret.insert(K[cur.Y][cur.X]);

		for (int dir = 0; dir < 4; dir++) {
			int ny = cur.Y + dy[dir];
			int nx = cur.X + dx[dir];

			if (oom(ny, nx) || vis[ny][nx]) continue;
			if (K[cur.Y][cur.X] == K[ny][nx]) {
				q.push({ ny, nx });
				vis[ny][nx] = true;
			}
		}

		int ny = cur.Y + dy[d];
		int nx = cur.X + dx[d];

		if (oom(ny, nx) || A[ny][nx] == 2) {
			ret.clear();
			break;
		}
		if (K[ny][nx] > 0 && K[cur.Y][cur.X] != K[ny][nx]) {
			q.push({ ny, nx });
			vis[ny][nx] = true;
		}
	}

	return ret;
}

struct pos {
	int y, x, idx;
};

void calc(int idx, vector<pos>& v) {
	vector<int> damage(N, 0);

	for (auto& nxt : v) {
		int i = nxt.idx - 1;
		if (i == idx) continue;

		if (A[nxt.y][nxt.x] == 1) damage[i]++;
	}

	for (int i = 0; i < knights.size(); i++) {
		int vol = damage[i];
		int origin = knights[i].k;
		int res = origin - vol;

		knights[i].k = res;

		if (res <= 0) {
			knights[i].r = -1;
			knights[i].c = -1;

			for (int y = 0; y < L; y++)
				for (int x = 0; x < L; x++)
					if (K[y][x] == i + 1) K[y][x] = 0;
		}
	}
}

void move_knight(int idx, int dir) {
	if (knights[idx].r == -1 && knights[idx].c == -1) return;

	auto tmp = check(idx, dir);
	if (tmp.empty()) return;

	vector<pos> v;
	for (int i : tmp) {
		auto& cur = knights[i - 1];
		if (cur.r == -1 && cur.c == -1) continue;

		int r = cur.r, c = cur.c, h = cur.h, w = cur.w;

		for (int y = r; y < r + h; y++) {
			for (int x = c; x < c + w; x++) {
				K[y][x] = 0;
				v.push_back({ y, x, i });
			}
		}

		knights[i - 1].r += dy[dir];
		knights[i - 1].c += dx[dir];
	}

	vector<pos> move;
	for (auto& nxt : v) {
		int ny = nxt.y + dy[dir];
		int nx = nxt.x + dx[dir];
		K[ny][nx] = nxt.idx;
		move.push_back({ ny, nx, nxt.idx });
	}

	calc(idx, move);
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> L >> N >> Q;
	for (int i = 0; i < L; i++)
		for (int j = 0; j < L; j++)
			cin >> A[i][j];

	vector<int> origin(N, 0);
	knights.assign(N, {});
	for (int r, c, h, w, k, i = 0; i < N; i++) {
		cin >> r >> c >> h >> w >> k;
		r--, c--;
		
		origin[i] = k;
		knights[i] = { r,c,h,w,k };
		for (int y = r; y < r + h; y++)
			for (int x = c; x < c + w; x++)
				K[y][x] = i + 1;
	}

	for (int i, d; Q--;) {
		cin >> i >> d;
		move_knight(i - 1, d);
	}

	for (int i = 0; i < N; i++) {
		int o = origin[i];
		int r = knights[i].k;

		if (r > 0) ans += o - r;
	}

	cout << ans;
	return 0;
}