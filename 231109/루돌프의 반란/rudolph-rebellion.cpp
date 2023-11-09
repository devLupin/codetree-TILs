#include <bits/stdc++.h>
#define Y first
#define X second
using namespace std;
using pii = pair<int, int>;

int N, M, P, C, D, A[55][55];
pii rudolf;
vector<pii> santa;
vector<int> ans;
vector<int> stun;

const int dy[] = { -1,0,1,0 };
const int dx[] = { 0,1,0,-1 };

int distance(pii& a, pii& b) {
	return (a.Y - b.Y) * (a.Y - b.Y) + (a.X - b.X) * (a.X - b.X);
}

struct pos {
	int r, c, d;
};

bool compare(pos& a, pos& b) {
	if (a.d == b.d) {
		if (a.r == b.r) return a.c > b.c;
		return a.r > b.r;
	}
	return a.d < b.d;
}

int get_dir(int p) {
	if (p == 0) return 0;
	return (p > 0 ? 1 : -1);
}

bool oom(int y, int x) {
	return y < 0 || x < 0 || y >= N || x >= N;
}

void interaction(int y, int x, int ddy, int ddx) {
	vector<pair<pii, int>> v;

	while (!oom(y, x) && A[y][x] != 0) {
		v.push_back({ {y,x}, A[y][x] - 1 });
		A[y][x] = 0;
		y += ddy;
		x += ddx;
	}

	reverse(v.begin(), v.end());

	for (auto nxt : v) {
		pii cur = nxt.first;
		int idx = nxt.second;

		int y = cur.Y + ddy, x = cur.X + ddx;
		if (oom(y, x)) santa[idx] = { -1,-1 };
		else {
			santa[idx] = { y,x };
			A[y][x] = idx + 1;
		}
	}
}

void crash(int idx, int cnt, int ddy, int ddx) {
	auto cur = santa[idx];
	int y = cur.Y, x = cur.X;
	A[y][x] = 0;

	y += ddy * cnt;
	x += ddx * cnt;

	if (oom(y, x)) {
		santa[idx] = { -1,-1 };
		return;
	}

	else if (A[y][x] > 0) {
		interaction(y, x, ddy, ddx);
		A[y][x] = idx + 1;
		santa[idx] = { y,x };
	}

	else {
		A[y][x] = idx + 1;
		santa[idx] = { y,x };
	}

	stun[idx] = 2;
}

void move_rudolf() {
	vector<pos> tmp;

	for (auto nxt : santa) {
		if (nxt.Y == -1 && nxt.X == -1) continue;

		int d = distance(nxt, rudolf);
		tmp.push_back({ nxt.Y, nxt.X, d });
	}

	sort(tmp.begin(), tmp.end(), compare);
	if (tmp.empty()) return;

	int ddy = get_dir(tmp[0].r - rudolf.Y);
	int ddx = get_dir(tmp[0].c - rudolf.X);

	int ny = rudolf.Y + ddy;
	int nx = rudolf.X + ddx;

	A[rudolf.Y][rudolf.X] = 0;

	if (A[ny][nx] > 0) {
		int cur = A[ny][nx] - 1;
		crash(cur, C, ddy, ddx);
		ans[cur] += C;
	}

	A[ny][nx] = -1;
	rudolf = { ny, nx };
}

void move_santa() {
	for(int i=0; i<santa.size(); i++) {
		auto nxt = santa[i];

		if (nxt.Y == -1 && nxt.X == -1) continue;
		if (stun[i] > 0) continue;

		int d = -1, dist = distance(nxt, rudolf);
		for (int dir = 0; dir < 4; dir++) {
			int ny = nxt.Y + dy[dir];
			int nx = nxt.X + dx[dir];
			pii move = { ny, nx };

			if (oom(ny, nx) || A[ny][nx] > 0) continue;
			
			int cmp = distance(move, rudolf);
			if (dist > cmp) {
				dist = cmp;
				d = dir;
			}
		}

		if (d == -1) continue;

		int ny = nxt.Y + dy[d];
		int nx = nxt.X + dx[d];

		if (A[ny][nx] == -1) {
			d = (d + 2) % 4;
			crash(i, D - 1, dy[d], dx[d]);
			ans[i] += D;
		}
		else {
			swap(A[nxt.Y][nxt.X], A[ny][nx]);
			santa[i] = { ny,nx };
		}
	}
}

void update_stun() {
	for (int i = 0; i < P; i++)
		if (stun[i] > 0) stun[i]--;
}

void turn_end() {
	for (int i = 0; i < santa.size(); i++)
		if (santa[i].Y != -1 && santa[i].X != -1) ans[i]++;
}

bool check() {
	for (auto nxt : santa)
		if (nxt.Y != -1 && nxt.X != -1) return true;
	return false;
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> N >> M >> P >> C >> D;
	
	cin >> rudolf.Y >> rudolf.X;
	rudolf.Y--, rudolf.X--;
	A[rudolf.Y][rudolf.X] = -1;

	santa.assign(P, {});
	ans.assign(P, 0);
	stun.assign(P, 0);

	for (int n, r, c, i = 0; i < P; i++) {
		cin >> n >> r >> c;
		r--, c--;
		santa[n - 1] = { r, c };
		A[r][c] = n;
	}

	while (check() && M--) {
		move_rudolf();
		move_santa();
		update_stun();
		turn_end();
	}

	for (auto nxt : ans) cout << nxt << ' ';
}