#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

int ans = 100;
int n, m, A[10][10];
vector<pii> pos;

const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

void update(int x, int y, int dir) {
	dir %= 4;

	while (true) {
		x += dx[dir];
		y += dy[dir];

		if (x < 0 || y < 0 || x >= n || y >= m) break;
		if (A[x][y] != 0) break;
		A[x][y] = 7;
	}
}

void solution(int cnt) {
	if (cnt == pos.size()) {
		int cmp = 0;
		for (int x = 0; x < n; x++)
			for (int y = 0; y < m; y++)
				if (A[x][y] == 0) cmp++;
		ans = min(ans, cmp);
		return;
	}

	int cpy[10][10];
	int x = pos[cnt].X;
	int y = pos[cnt].Y;

	for (int dir = 0; dir < 4; dir++) {
		for (int x = 0; x < n; x++)
			for (int y = 0; y < m; y++)
				cpy[x][y] = A[x][y];

		if (A[x][y] == 1) {
			update(x, y, dir);
		}
		else if (A[x][y] == 2) {
			update(x, y, dir + 1);
			update(x, y, dir + 3);
		}
		else if (A[x][y] == 3) {
			update(x, y, dir);
			update(x, y, dir + 1);
		}
		else if (A[x][y] == 4) {
			update(x, y, dir);
			update(x, y, dir + 1);
			update(x, y, dir + 3);
		}
		else if (A[x][y] == 5) {
			update(x, y, dir);
			update(x, y, dir + 1);
			update(x, y, dir + 2);
			update(x, y, dir + 3);
		}

		solution(cnt + 1);

		for (int x = 0; x < n; x++)
			for (int y = 0; y < m; y++)
				A[x][y] = cpy[x][y];
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> n >> m;

	for (int x = 0; x < n; x++) {
		for (int y = 0; y < m; y++) {
			cin >> A[x][y];
			if (A[x][y] == 0 || A[x][y] == 6) continue;
			pos.push_back({ x,y });
		}
	}
	
	solution(0);
	cout << ans;

	return 0;
}