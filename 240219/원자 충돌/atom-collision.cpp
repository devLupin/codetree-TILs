#include <bits/stdc++.h>
using namespace std;

const int dx[] = { -1,-1,0,1,1,1,0,-1 };
const int dy[] = { 0,1,1,1,0,-1,-1,-1 };
int N, M, K;
vector<int> board[50][50];
struct info { int x, y, m, s, d; };
vector<info> v, cpy;

void init() {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			board[i][j].clear();
	cpy.clear();
}

void move() {
	for (int i = 0; i < v.size(); i++) {
		auto& nxt = v[i];
		int x = nxt.x;
		int y = nxt.y;
		int s = nxt.s;
		int d = nxt.d;

		while (s--) {
			x += dx[d];
			y += dy[d];

			if (x < 0) x = N - 1;
			if (y < 0) y = N - 1;
			if (x >= N) x = 0;
			if (y >= N) y = 0;
		}

		nxt.x = x;
		nxt.y = y;
		board[x][y].push_back(i);
	}
}

void split(int x, int y, vector<int> dirs, int m, int s) {
	if (m == 0) return;

	for (int d : dirs)
		cpy.push_back({ x, y, m, s, d });
}

void synthesize() {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (board[i][j].size() > 1) {
				bool chk1 = false, chk2 = false;
				int sumM = 0, sumS = 0;

				for (int nxt : board[i][j]) {
					sumM += v[nxt].m;
					sumS += v[nxt].s;

					if (v[nxt].d % 2 == 0) chk1 = true;
					else chk2 = true;
				}

				int m = sumM / 5;
				int s = sumS / board[i][j].size();

				if (chk1 && chk2) split(i, j, { 1, 3, 5, 7 }, m, s);
				else split(i, j, { 0, 2, 4, 6 }, m, s);
			}
			else if (!board[i][j].empty())
				cpy.push_back(v[board[i][j][0]]);
		}
	}

	v = cpy;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	
	cin >> N >> M >> K;
	for (int x, y, m, s, d, i = 0; i < M; i++) {
		cin >> x >> y >> m >> s >> d;
		x--, y--;
		v.push_back({ x,y,m,s,d });
	}

	while (K--) {
		init();
		move();
		synthesize();
	}

	int ans = 0;
	for (auto& nxt : v)
		ans += nxt.m;
	cout << ans;

	return 0;
}