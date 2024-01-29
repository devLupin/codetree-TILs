#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

struct info {
	int x, y, s, d, b;
};

const int dx[] = { 0,-1,1,0,0 };
const int dy[] = { 0,0,0,1,-1 };

int n, m, k, ans;
int A[105][105];
vector<info> v;

void collect(int x, int y) {
	A[x][y] = 0;

	for (int i = 0; i < v.size(); i++) {
		if (v[i].x == x && v[i].y == y) {
			v[i].x = -1;
			v[i].y = -1;
			ans += v[i].b;
			break;
		}
	}
}

bool oom(int x, int y) { return x < 1 || y < 1 || x > n || y > m; }

void move() {
	for (int i = 0; i < v.size(); i++) {
		auto& nxt = v[i];

		if (nxt.x == -1 && nxt.y == -1) continue;

		int x = nxt.x, y = nxt.y;
		int dir = nxt.d;
		int s = nxt.s;

		A[x][y]++;

		while (s--) {
			x += dx[dir];
			y += dy[dir];

			if (oom(x, y)) {
				x -= dx[dir];
				y -= dy[dir];
				s++;

				if (dx[dir] != 0)
					dir = dx[dir] > 0 ? dir - 1 : dir + 1;

				else if (dy[dir] != 0)
					dir = dy[dir] > 0 ? dir + 1 : dir - 1;
			}
		}

		v[i].x = x;
		v[i].y = y;
		v[i].d = dir;
		A[x][y]--;
	}
}

void eat() {
	vector<info> tmp;

	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			if (A[i][j] < -1) {
				tmp.push_back({ i,j,0,0,0 });
				A[i][j] = -1;
			}

	for (auto& nxt : tmp) {
		if (nxt.x == -1 && nxt.y == -1) continue;

		vector<int> idx;
		int mx = 0;

		for (int i = 0; i < v.size(); i++) {
			auto& cmp = v[i];

			if (nxt.x == cmp.x && nxt.y == cmp.y) {
				idx.push_back(i);
				mx = max(mx, cmp.b);
			}
		}

		for(int i : idx)
			if (v[i].b < mx) {
				v[i].x = -1;
				v[i].y = -1;
			}
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> n >> m >> k;

	while (k--) {
		int x, y, s, d, b;
		cin >> x >> y >> s >> d >> b;
		v.push_back({ x,y,s,d,b });
		A[x][y] = -1;
	}

	for (int j = 1; j <= m; j++) {
		for (int i = 1; i <= n; i++) {
			if (A[i][j] == -1) {
				collect(i, j);
				break;
			}
		}

		move();
		eat();
	}

	cout << ans;
	
	return 0;
}