#include <cstring>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <tuple> // for tie()
#define Y first
#define X second

using namespace std;
using pii = pair<int, int>;

const int dy[] = { -1,0,0,1 };
const int dx[] = { 0,-1,1,0 };
const int SZ = 15;
const pii NONE = { -1,-1 };

int N, M, T;
int A[SZ][SZ];
vector<pii> conv;
vector<pii> people;

int dist_map[SZ][SZ];
bool vis[SZ][SZ];

bool oom(int y, int x) { return y < 0 || x < 0 || y >= N || x >= N; }

void bfs(pii conv_pos) {
	for (int i = 0; i < N; i++) {
		memset(dist_map[i], 0, sizeof(dist_map[i]));
		memset(vis[i], false, sizeof(vis[i]));
	}

	queue<pii> q;
	q.push(conv_pos);
	vis[conv_pos.Y][conv_pos.X] = true;
	dist_map[conv_pos.Y][conv_pos.X] = 0;

	while (!q.empty()) {
		pii cur = q.front();
		q.pop();

		for (int dir = 0; dir < 4; dir++) {
			int ny = cur.Y + dy[dir];
			int nx = cur.X + dx[dir];

			if (!oom(ny, nx) && !vis[ny][nx] && A[ny][nx] != -1) {
				q.push({ ny, nx });
				vis[ny][nx] = true;
				dist_map[ny][nx] = dist_map[cur.Y][cur.X] + 1;
			}
		}
	}
}

void solve() {
	for (int i = 0; i < M; i++) {
		if (people[i] == NONE || people[i] == conv[i]) continue;

		bfs(conv[i]);

		int py, px;
		tie(py, px) = people[i];

		int min_dist = 987654321;
		int min_y = -1, min_x = -1;

		for (int dir = 0; dir < 4; dir++) {
			int ny = py + dy[dir];
			int nx = px + dx[dir];

			if (oom(ny, nx) || !vis[ny][nx]) continue;
			if (dist_map[ny][nx] < min_dist) {
				tie(min_y, min_x) = { ny,nx };
				min_dist = dist_map[ny][nx];
			}
		}

		people[i] = { min_y, min_x };
	}

	for (int i = 0; i < M; i++) {
		if (people[i] == conv[i])
			A[conv[i].Y][conv[i].X] = -1;
	}

	if (T > M) return;

	bfs(conv[T - 1]);

	int min_dist = 987654321;
	int min_y = -1, min_x = -1;

	for(int i=0; i<N; i++)
		for (int j = 0; j < N; j++) {
			if (A[i][j] == 1 && vis[i][j] && min_dist > dist_map[i][j]) {
				min_dist = dist_map[i][j];
				tie(min_y, min_x) = { i,j };
			}
		}

	people[T - 1] = { min_y, min_x };
	A[min_y][min_x] = -1;
}

bool is_end() {
	for (int i = 0; i < M; i++)
		if (people[i] != conv[i])
			return false;
	return true;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> N >> M;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> A[i][j];

	for (int y, x, i = 0; i < M; i++) {
		cin >> y >> x;
		y--, x--;
		conv.push_back({ y,x });
		people.push_back(NONE);
	}

	while (true) {
		T++;
		solve();
		if (is_end()) break;
	}

	cout << T;
	return 0;
}