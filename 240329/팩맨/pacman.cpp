#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

// ↑, ↖, ←, ↙, ↓, ↘, →, ↗
const int dx[] = { -1,-1,0,1,1,1,0,-1 };
const int dy[] = { 0,-1,-1,-1,0,1,1,1 };

// ↑, ←, ↓, →
const int ddx[] = { -1,0,1,0 };
const int ddy[] = { 0,-1,0,1 };

struct info {
	int dir, cnt;
	bool die;
};

int M, T, px, py, ans;
vector<info> monster[5][5], newMonster[5][5], nextMonster[5][5];
int cmp;
string routes;

void Copy() {
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			monster[i][j] = nextMonster[i][j];
			nextMonster[i][j].clear();
		}
	}
}

void CopyMonster() {
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			for (auto& nxt : monster[i][j]) {
				if (nxt.die) continue;
				newMonster[i][j].push_back({ nxt.dir, 0, false });
			}
		}
	}
}

bool OOM(int x, int y) { return x < 1 || y < 1 || x > 4 || y > 4; }

bool Check(int x, int y) {
	if (OOM(x, y)) return false;
	if (x == px && y == py) return false;
	for (auto& nxt : monster[x][y])
		if (nxt.die) return false;
	return true;
}

void MoveMonster() {
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			for (auto& nxt : monster[i][j]) {
				if (nxt.die) {
					nextMonster[i][j].push_back(nxt);
					continue;
				}
				
				bool flag = false;

				for (int cnt = 0; cnt < 8; cnt++) {
					int dir = (nxt.dir + cnt) % 8;
					int nx = i + dx[dir];
					int ny = j + dy[dir];

					if (Check(nx, ny)) {
						nextMonster[nx][ny].push_back({ dir, 0, false });
						flag = true;
						break;
					}
				}

				if (!flag) nextMonster[i][j].push_back(nxt);
			}
		}
	}

	Copy();
}

void DFS(int x, int y, int cnt, string path) {
	if (cnt == 3) {
		map<pii, int> MAP;
		int nx = px;
		int ny = py;
		int sum = 0;

		for (char c : path) {
			int dir = c - '0';
			nx += ddx[dir];
			ny += ddy[dir];

			if (MAP[{nx, ny}] == 0) {
				MAP[{nx, ny}] = 1;
				
				for (auto& nxt : monster[nx][ny])
					if (!nxt.die) sum++;
			}
		}

		if (cmp < sum) {
			cmp = sum;
			routes = path;
		}

		return;
	}

	for (int dir = 0; dir < 4; dir++) {
		int nx = x + ddx[dir];
		int ny = y + ddy[dir];
		if (OOM(nx, ny)) continue;

		DFS(nx, ny, cnt + 1, path + to_string(dir));
	}
}

void MovePackman() {
	cmp = -1;
	routes = "333";
	DFS(px, py, 0, "");

	for (char c : routes) {
		int dir = c - '0';

		px += ddx[dir];
		py += ddy[dir];

		for (auto& nxt : monster[px][py]) {
			if (!nxt.die) {
				nxt.die = true;
				nxt.cnt = 3;
			}
		}
	}
}

void Done() {
	int cnt = 0;

	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			for (auto& nxt : monster[i][j]) {
				cnt++;

				if (nxt.die) {
					cnt--;
					if (nxt.cnt > 0)nxt.cnt--;
					if (nxt.cnt == 0) continue;
				}

				nextMonster[i][j].push_back(nxt);
			}
		}
	}

	Copy();

	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			for (auto& nxt : newMonster[i][j]) {
				cnt++;
				monster[i][j].push_back(nxt);
			}
			newMonster[i][j].clear();
		}
	}

	ans = cnt;
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> M >> T >> px >> py;
	while (M--) {
		int x, y, d;
		cin >> x >> y >> d;
		monster[x][y].push_back({ d - 1, 0, false });
	}
	
	while (T--) {
		CopyMonster();
		MoveMonster();
		MovePackman();
		Done();
	}

	cout << ans;

	return 0;
}