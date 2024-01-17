#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

int n, A[21][21];
vector<pii> monster;
pii robot;
int robot_exp;

int dx[] = { -1,1,0,0 };
int dy[] = { 0,0,-1,1 };

struct pos {
	int x, y, d, idx;
};

int distance(pii& src, pii& dest) {
	int lv = A[src.X][src.Y];
	bool vis[21][21] = { false, };
	queue<pos> q;

	q.push({ src.X, src.Y, 0 });
	vis[src.X][src.Y] = true;

	while (!q.empty()) {
		pos& now = q.front();
		q.pop();

		if (now.x == dest.X && now.y == dest.Y) return now.d;
		
		for (int dir = 0; dir < 4; dir++) {
			int nx = now.x + dx[dir];
			int ny = now.y + dy[dir];

			if (nx < 0 || ny < 0 || nx >= n || ny >= n) continue;
			if (!vis[nx][ny] && lv >= A[nx][ny]) {
				q.push({ nx, ny, now.d + 1 });
				vis[nx][ny] = true;
			}
		}
	}

	return -1;
}

bool compare(const pos& a, const pos& b) {
	if (a.d == b.d) {
		if (a.x == b.x) return a.y < b.y;
		return a.x < b.x;
	}
	return a.d < b.d;
}

pos get_target() {
	vector<pos> ret;

	for (int i = 0; i < monster.size(); i++) {
		auto& nxt = monster[i];

		if (nxt.X == -1 && nxt.Y == -1) continue;
		if (A[robot.X][robot.Y] <= A[nxt.X][nxt.Y]) continue;
		
		int dist = distance(robot, nxt);
		if (dist == -1) continue;

		ret.push_back({ nxt.X, nxt.Y, dist, i });
	}

	if (ret.size() == 0) return { -1,-1 };
	
	sort(ret.begin(), ret.end(), compare);
	return ret[0];
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	freopen("input.txt", "r", stdin);

	cin >> n;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> A[i][j];
			if (A[i][j] == 9) {
				robot = { i,j };
				A[i][j] = 2;
			}
			else if (A[i][j] > 0) monster.push_back({ i,j });
		}
	}
	
	int ans = 0;

	while (true) {
		auto target = get_target();
		if (target.x == -1 && target.y == -1) break;

		A[target.x][target.y] = 0;
		swap(A[robot.X][robot.Y], A[target.x][target.y]);
		robot = { target.x, target.y };
		monster[target.idx] = { -1,-1 };

		robot_exp++;
		if (robot_exp == A[robot.X][robot.Y]) {
			A[robot.X][robot.Y]++;
			robot_exp = 0;
		}

		ans += target.d;
	}

	cout << ans;
	return 0;
}