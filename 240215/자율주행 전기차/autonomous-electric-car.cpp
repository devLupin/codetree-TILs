#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

int N, M, C;
int road[25][25];

struct status { int x, y, d; };
struct info { 
	int sx, sy, ex, ey;
	bool isArrived;
};
vector<info> passengers;

pii taxi;
int taxiC;

int dx[] = { -1,1,0,0 };
int dy[] = { 0,0,-1,1 };

bool oom(int x, int y) { return x < 1 || y < 1 || x > N || y > N; }

void input() {
	cin >> N >> M >> C;
	taxiC = C;

	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++)
			cin >> road[i][j];

	cin >> taxi.X >> taxi.Y;

	for (int i = 0; i < M; i++) {
		int sx, sy, ex, ey;
		cin >> sx >> sy >> ex >> ey;
		passengers.push_back({ sx, sy, ex, ey, false });
	}
}

int getDistance(int sx, int sy, int ex, int ey) {
	queue<status> q;
	bool vis[25][25] = { false, };

	q.push({ sx, sy, 0 });
	vis[sx][sy] = true;

	while (!q.empty()) {
		auto cur = q.front();
		q.pop();

		if (cur.x == ex && cur.y == ey) return cur.d;
		if (cur.d > taxiC) continue;

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.x + dx[dir];
			int ny = cur.y + dy[dir];

			if (oom(nx, ny) || vis[nx][ny]) continue;
			if (road[nx][ny] == 0) {
				q.push({ nx, ny, cur.d + 1 });
				vis[nx][ny] = true;
			}
		}
	}

	return -1;
}

struct prior { int x, y, d, idx; };

bool compare(prior& a, prior& b) {
	if (a.d == b.d) {
		if (a.x == b.x) return a.y < b.y;
		return a.x < b.x;
	}
	return a.d < b.d;
}

pii carry() {
	vector<prior> tmp;

	for (int i = 0; i < passengers.size(); i++) {
		info& cur = passengers[i];
		if (cur.isArrived) continue;

		int distance = getDistance(taxi.X, taxi.Y, cur.sx, cur.sy);
		if (distance == -1) continue;

		tmp.push_back({ cur.sx, cur.sy, distance, i });
	}

	sort(tmp.begin(), tmp.end(), compare);

	if (tmp.size() > 0) return { tmp[0].d, tmp[0].idx };
	else return { -1,-1 };
}

int moveTaxi(info& cur) {
	taxi = { cur.sx, cur.sy };

	int distance = getDistance(taxi.X, taxi.Y, cur.ex, cur.ey);
	if (distance == -1) return -1;

	taxi = { cur.ex, cur.ey };
	cur.isArrived = true;

	return distance;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	input();

	while (true) {
		auto target = carry();
		if (target.first == -1) break;
		taxiC -= target.first;
		
		int distance = moveTaxi(passengers[target.second]);
		if (distance == -1) break;
		taxiC += distance;
	}

	for (info& cur : passengers)
		if (!cur.isArrived) {
			cout << -1;
			return 0;
		}

	cout << taxiC;
	return 0;
}