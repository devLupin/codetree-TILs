#define _CRT_SECURE_NO_WARNINGS
#include<bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

// ↑, ↖, ←, ↙, ↓, ↘, →, ↗
const int dx[] = { -1, -1, 0, 1, 1, 1, 0, -1 };
const int dy[] = { 0, -1, -1, -1, 0, 1, 1, 1 };

// 방향, 죽었을 때 2턴 유지
// 살았는지?, 부화하기 전?
struct info {
	int d, die;
	bool alive, stun;
};

int M, T;
int px, py;  // 팩맨 좌표
int ans, cmp;
vector<info> monster[5][5], next_monster[5][5];
string routes;

bool OOM(int x, int y) { return x < 1 || y < 1 || x > 4 || y > 4; }

// 범위를 벗어나거나, 팩맨의 위치이거나, 죽은 몬스터가 있는 자리라면 이동 못함
bool Check(int x, int y) {
	if (OOM(x, y)) return false;
	if (x == px && y == py) return false;
	for (auto& nxt : monster[x][y])
		if (!nxt.alive) return false;
	return true;
}

// 임시로 저장해둔 다음 몬스터를 복사하는 형식
void NextMonster() {
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			monster[i][j] = next_monster[i][j];
			next_monster[i][j].clear();
		}
	}
}

// 몬스터 복제
// 죽은 몬스터라면 복제 안함
void Copy() {
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			for (auto& nxt : monster[i][j]) {
				if (!nxt.alive) continue;
				monster[i][j].push_back({ nxt.d, 0, true, true });
			}
		}
	}
}

// 몬스터 움직이기
void MoveMonster() {
	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			for (auto& nxt : monster[i][j]) {
				// 부화를 안했거나, 죽은 몬스터라면 그 상태 그대로 넣음.
				if (nxt.stun || !nxt.alive) {
					next_monster[i][j].push_back(nxt);
					continue;
				}

				auto [x, y, dir] = make_tuple(i, j, nxt.d);
				bool flag = false;

				// 가장 먼저 이동할 수 있는 칸으로 이동
				for (int i = 0; i < 8; i++) {
					int nd = (dir + i) % 8;
					int nx = x + dx[nd];
					int ny = y + dy[nd];

					if (Check(nx, ny)) {
						nxt.d = nd;
						next_monster[nx][ny].push_back(nxt);
						flag = true;
						break;
					}
				}

				// 전부 이동 못한다면 그 상태 그대로 넣음.
				if(!flag) next_monster[x][y].push_back(nxt);
			}
		}
	}

	NextMonster();
}

// 최적의 경로를 구하기 위한 DFS
// 합이 큰 순, 방향 우선순위가 높은 순으로 이동
// 방향은 문자열로 지정했고, "000" < "001"이 됨.
void dfs(int x, int y, int cnt, string dirs) {
	if (cnt == 3) {
		map<pii, int> MAP;
		auto [nx, ny, sum] = make_tuple(px, py, 0);

		for (char c : dirs) {
			int d = c - '0';
			nx += dx[d];
			ny += dy[d];

			if (MAP[{nx, ny}] == 0) {
				MAP[{nx, ny}] = 1;
				
				for (auto nxt : monster[nx][ny])
					if (nxt.alive && !nxt.stun) sum++;
			}
		}

		if (cmp < sum) {
			cmp = sum;
			routes = dirs;
		}
		else if (cmp == sum)
			routes = min(routes, dirs);

		return;
	}

	// ↑, ←, ↓, →
	for (int dir : {0, 2, 4, 6}) {
		auto [nx, ny] = make_pair(x + dx[dir], y + dy[dir]);
		if (OOM(nx, ny)) continue;

		dfs(nx, ny, cnt + 1, dirs + to_string(dir));
	}
}

// DFS에서 구한 최적의 경로로 이동
void MovePackman() {
	cmp = -1;
	routes = "666";
	dfs(px, py, 0, "");

	// ↑, ←, ↓, →
	for (char& d : routes) {
		int dir = d - '0';
		px += dx[dir];
		py += dy[dir];

		// 죽은 몬스터 표시
		for (auto& nxt : monster[px][py]) {
			if (nxt.stun) continue;
			nxt.die = 3;
			nxt.alive = false;
		}
	}
}

// 한번의 턴이 끝났을 때
//// 죽었을 때 2턴 유지되는 거 한번 감소
//// 알 부화하기, stun = false
//// 죽은 상태에서 턴 종료되면 next에 넣지 않기
void Done() {
	int cnt = 0;

	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			for (auto& nxt : monster[i][j]) {
				if (nxt.die > 0) nxt.die--;
				if (nxt.stun) nxt.stun = false;

				if (nxt.alive) cnt++;

				if (!nxt.alive && nxt.die == 0) continue;
				next_monster[i][j].push_back(nxt);
			}
		}
	}

	NextMonster();
	ans = cnt;
}

void Print() {
	cout << "\n\n";

	for (int i = 1; i <= 4; i++) {
		for (int j = 1; j <= 4; j++) {
			for (auto& nxt : monster[i][j]) {
				cout << '[' << i << ", " << j << "] - ";
				cout << ((nxt.alive) ? "alive" : "die") << ", ";
				cout << ((nxt.stun) ? "stun" : "no-stun") << "\n";
			}
		}
	}
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> M >> T >> px >> py;
	for (int i = 0; i < M; i++) {
		int x, y, d;
		cin >> x >> y >> d;
		monster[x][y].push_back({ d - 1, 0, true, false });
	}

	while (T--) {
		Copy();
		MoveMonster();
		MovePackman();
		Done();
	}

	cout << ans;
	return 0;
}