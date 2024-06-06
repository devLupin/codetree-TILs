#define _CRT_SECURE_NO_WARNINGS
#include <queue>
#include <iostream>
#include <algorithm>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };

int K, M;

class Board {
	vector<vector<int>> map, nxt;
	queue<int> art;

public:
	Board() { map.assign(5, vector<int>(5, 0)); }

	void print() {
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++)
				cout << nxt[i][j] << ' ';
			cout << '\n';
		}
		cout << "\n\n";
	}

	void input() {
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				cin >> map[i][j];

		for (int x, i = 0; i < M; i++) {
			cin >> x;
			art.push(x);
		}
	}

	void rotate(int sx, int sy, int sz) {
		auto tmp = nxt;

		for (int x = sx; x < sx + sz; x++) {
			for (int y = sy; y < sy + sz; y++) {
				int nx = y - sy;
				int ny = sz - (x - sx) - 1;
				tmp[nx + sx][ny + sy] = nxt[x][y];
			}
		}

		nxt = tmp;
	}

	int BFS() {
		bool vis[5][5] = { false, };
		int ret = 0;

		for (int x = 0; x < 5; x++)
			for (int y = 0; y < 5; y++)
				if (!vis[x][y] && nxt[x][y]) {
					vector<pii> v;
					queue<pii> q;
					int cmp = nxt[x][y];

					q.push({ x, y });
					vis[x][y] = true;

					while (!q.empty()) {
						pii cur = q.front();
						q.pop();

						v.push_back(cur);

						for (int dir = 0; dir < 4; dir++) {
							int nx = cur.X + dx[dir];
							int ny = cur.Y + dy[dir];

							if (nx < 0 || ny < 0 || nx >= 5 || ny >= 5) continue;
							if (vis[nx][ny] || nxt[nx][ny] != cmp) continue;

							q.push({ nx, ny });
							vis[nx][ny] = true;
						}
					}

					if (v.size() >= 3) {
						for (pii& pos : v) nxt[pos.X][pos.Y] = 0;
						ret += v.size();
					}
				}

		return ret;
	}

	void fill() {
		for (int y = 0; y < 5; y++)
			for (int x = 4; x >= 0; x--)
				if (nxt[x][y] == 0) {
					nxt[x][y] = art.front();
					art.pop();
				}
	}

	int select() {
		vector<vector<int>> best = map;
		int cmp = 0;

		for (int i = 1; i <= 3; i++)
			for (int y = 0; y < 3; y++)
				for (int x = 0; x < 3; x++) {
					nxt = map;

					for (int j = 0; j < i; j++)
						rotate(x, y, 3);

					int tmp = BFS();
					if (cmp < tmp) {
						best = nxt;
						cmp = tmp;
					}
				}

		nxt = best;
		return cmp;
	}

	void done() { map = nxt; }
};

int main(void) {
	// freopen("input.txt", "r", stdin);
	cin >> K >> M;

	Board board;
	board.input();
	
	while (K--) {
		int ans = board.select();
		if (ans == 0) break;

		while (true) {
			board.fill();
			int new_ans = board.BFS();
			if (new_ans == 0) break;
			ans += new_ans;
		}
		cout << ans << " ";
		board.done();
	}

	return 0;
}