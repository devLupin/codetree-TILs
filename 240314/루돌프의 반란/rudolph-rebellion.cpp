#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

int N, M, P, C, D;
pii rudolf;

int board[55][55], stun[35], score[35];
pii santa[35];
bool rm[35];

struct s_info { int num, dist, x, y; };

bool compare(const s_info& a, const s_info& b) {
	if (a.dist == b.dist) {
		if (a.x == b.x) return a.y > b.y;
		else return a.x > b.x;
	}
	return a.dist < b.dist;
}

int distance(int x, int y) { return (rudolf.X - x) * (rudolf.X - x) + (rudolf.Y - y) * (rudolf.Y - y); }

bool oom(int x, int y) { return x < 1 || y < 1 || x > N || y > N; }

int get_dir(int p) {
	if (p == 0) return 0;
	return (p > 0 ? 1 : -1);
}

void interaction(int x, int y, int ddx, int ddy) {
	vector<pair<pii, int>> v;

	while (!oom(x, y) && board[x][y] != 0) {
		v.push_back({ {x, y}, board[x][y] });
		board[x][y] = 0;
		x += ddx;
		y += ddy;
	}

	reverse(v.begin(), v.end());

	for (auto nxt : v) {
		auto [cur, num] = nxt;

		int x = cur.X + ddx;
		int y = cur.Y + ddy;

		if (oom(x, y)) rm[num] = true;
		else {
			santa[num] = { x, y };
			board[x][y] = num;
		}
	}
}

void crash(int num, int cnt, int ddx, int ddy) {
	auto [x, y] = santa[num];
	board[x][y] = 0;

	x += cnt * ddx;
	y += cnt * ddy;

	if (oom(x, y)) {
		rm[num] = true;
		return;
	}
	else if (board[x][y] > 0)
		interaction(x, y, ddx, ddy);
	
	board[x][y] = num;
	santa[num] = { x, y };
}

void move_rudolf() {
	vector<s_info> tmp;

	for (int i = 1; i <= P; i++) {
		if (rm[i]) continue;

		auto& nxt = santa[i];
		int dist = distance(nxt.X, nxt.Y);
		tmp.push_back({ i, dist, nxt.X, nxt.Y });
	}

	sort(tmp.begin(), tmp.end(), compare);
	int near = tmp[0].num;

	if (near > 0) {
		int ddx = get_dir(santa[near].X - rudolf.X);
		int ddy = get_dir(santa[near].Y - rudolf.Y);

		int nx = rudolf.X + ddx;
		int ny = rudolf.Y + ddy;
		int num = board[nx][ny];

		if (num > 0) {
			crash(num, C, ddx, ddy);
			score[num] += C;
			stun[num] = 2;
		}

		swap(board[rudolf.X][rudolf.Y], board[nx][ny]);
		rudolf = { nx, ny };
	}
}

void move_santa() {
	for (int i = 1; i <= P; i++) {
		if (rm[i] || stun[i] > 0) continue;

		pii& nxt = santa[i];
		int near = -1, cmp = distance(nxt.X, nxt.Y);

		for (int dir = 0; dir < 4; dir++) {
			int nx = nxt.X + dx[dir];
			int ny = nxt.Y + dy[dir];
			int nd = distance(nx, ny);

			if (oom(nx, ny) || board[nx][ny] > 0) continue;

			if (cmp > nd) {
				near = dir;
				cmp = nd;
			}
		}

		if (near >= 0) {
			int nx = nxt.X + dx[near];
			int ny = nxt.Y + dy[near];

			if (board[nx][ny] == -1) {
				int dir = (near + 2) % 4;

				crash(i, D - 1, dx[dir], dy[dir]);
				score[i] += D;
				stun[i] = 2;
			}

			else {
				swap(board[nxt.X][nxt.Y], board[nx][ny]);
				santa[i] = { nx, ny };
			}
		}
	}
}

void input() {
	cin >> N >> M >> P >> C >> D;
	cin >> rudolf.X >> rudolf.Y;
	board[rudolf.X][rudolf.Y] = -1;

	for (int i = 0; i < P; i++) {
		int num, x, y;
		cin >> num >> x >> y;
		santa[num] = { x, y };
		board[x][y] = num;
	}
}

bool alive() {
	for (int i = 1; i <= P; i++)
		if (!rm[i]) return true;
	return false;
}

void add() {
	for (int i = 1; i <= P; i++)
		if (!rm[i]) score[i]++;
}

void update() {
	for (int i = 1; i <= P; i++)
		if (stun[i] > 0) stun[i]--;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(0);

	input();

	while (M-- && alive()) {
		move_rudolf();
		move_santa();
		update();
		add();
	}

	for (int i = 1; i <= P; i++) cout << score[i] << ' ';

	return 0;
}