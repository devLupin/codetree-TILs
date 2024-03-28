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

const int SZ = 4;
const int MAX_T = 25;
const int DIR_NUM = 8;
const int MAX_DECAY = 2;

int M, T, px, py, curT = 1;
int monster[MAX_T][SZ][SZ][DIR_NUM];
int dead[SZ][SZ][MAX_DECAY + 1];
int cmp;
string routes;

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= SZ || y >= SZ; }

bool Check(int x, int y) {
	return !OOM(x, y) && dead[x][y][0] == 0 && dead[x][y][1] == 0
		&& make_pair(x, y) != make_pair(px, py);
}

tuple<int, int, int> GetNextPos(int x, int y, int move_dir) {
	for (int dir = 0; dir < DIR_NUM; dir++) {
		int nDir = (move_dir + dir + DIR_NUM) % DIR_NUM;
		int nx = x + dx[nDir];
		int ny = y + dy[nDir];
		if (Check(nx, ny)) {
			return make_tuple(nx, ny, nDir);
		}
	}

	return make_tuple(x, y, move_dir);
}

void MoveMonster() {
	for (int i = 0; i < SZ; i++)
		for (int j = 0; j < SZ; j++)
			for (int k = 0; k < DIR_NUM; k++) {
				tuple<int, int, int> next_pos = GetNextPos(i, j, k);
				auto [x, y, nDir] = next_pos;
				monster[curT][x][y][nDir] += monster[curT - 1][i][j][k];
			}
}

int GetKilledNum(int dir1, int dir2, int dir3) {
	int dirs[] = { dir1, dir2, dir3 };
	int x = px, y = py;
	int killed_num = 0;
	vector<pair<int, int> > vis;

	for (int i = 0; i < 3; i++) {
		int nx = x + ddx[dirs[i]];
		int ny = y + ddy[dirs[i]];

		if (OOM(nx, ny))
			return -1;

		if (find(vis.begin(), vis.end(), make_pair(nx, ny)) == vis.end()) {
			for (int j = 0; j < DIR_NUM; j++)
				killed_num += monster[curT][nx][ny][j];

			vis.push_back(make_pair(nx, ny));
		}

		x = nx; y = ny;
	}
	return killed_num;
}

void Kill(tuple<int, int, int> route) {
	auto[dir1, dir2, dir3] = route;

	int dirs[] = { dir1, dir2, dir3 };
	for (int i = 0; i < 3; i++) {
		int nx = px + ddx[dirs[i]], ny = py + ddy[dirs[i]];

		for (int j = 0; j < DIR_NUM; j++) {
			dead[nx][ny][MAX_DECAY] += monster[curT][nx][ny][j];
			monster[curT][nx][ny][j] = 0;
		}

		px = nx; py = ny;
	}
}

void MovePackman() {
	int max_cnt = -1;
	tuple<int, int, int> route;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++) {
				int m_cnt = GetKilledNum(i, j, k);

				if (m_cnt > max_cnt) {
					max_cnt = m_cnt;
					route = make_tuple(i, j, k);
				}
			}

	Kill(route);
}

void DecayMonster() {
	for (int i = 0; i < SZ; i++) {
		for (int j = 0; j < SZ; j++) {
			for (int k = 0; k < MAX_DECAY; k++) {
				dead[i][j][k] = dead[i][j][k + 1];
			}
			dead[i][j][MAX_DECAY] = 0;
		}
	}
}

void CopyMonster() {
	for (int i = 0; i < SZ; i++)
		for (int j = 0; j < SZ; j++)
			for (int dir = 0; dir < DIR_NUM; dir++)
				monster[curT][i][j][dir] += monster[curT - 1][i][j][dir];
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	freopen("input.txt", "r", stdin);

	cin >> M >> T >> px >> py;
	while (M--) {
		int x, y, d;
		cin >> x >> y >> d;
		monster[0][x - 1][y - 1][d - 1]++;
	}
	
	while (curT <= T) {
		MoveMonster();
		MovePackman();
		DecayMonster();
		CopyMonster();

		curT++;
	}

	int ans = 0;
	for (int i = 0; i < SZ; i++)
		for (int j = 0; j < SZ; j++)
			for (int dir = 0; dir < DIR_NUM; dir++)
				ans += monster[T][i][j][dir];

	cout << ans;
	return 0;
}