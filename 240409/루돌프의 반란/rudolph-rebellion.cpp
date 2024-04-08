#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int SANTA_MAX = 35;
const int MAP_SZ = 55;

const int dx[] = { -1,0,1,0, };
const int dy[] = { 0,1,0,-1 };

int N, M, P, C, D;
int rx, ry;
pii pos[SANTA_MAX];
int num[MAP_SZ][MAP_SZ];
int stun[SANTA_MAX];
bool die[SANTA_MAX];
int ans[SANTA_MAX];

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

int Distance(int x, int y) { return (rx - x) * (rx - x) + (ry - y) * (ry - y); }

void Interaction(int start_num, int xdir, int ydir) {
	auto [x, y] = pos[start_num];
	vector<pii> t_pos;
	vector<int> t_num;

	while (num[x][y] != 0) {
		t_pos.push_back({ x, y });
		t_num.push_back(num[x][y]);
		x += xdir;
		y += ydir;
	}

	reverse(t_pos.begin(), t_pos.end());
	reverse(t_num.begin(), t_num.end());

	for (int i = 0; i < t_num.size(); i++) {
		auto [ox, oy] = t_pos[i];
		int onum = t_num[i];

		int nx = ox + xdir;
		int ny = oy + ydir;

		if (OOM(nx, ny)) {
			die[onum] = true;
			num[ox][oy] = 0;
			pos[onum] = { -1,-1 };
		}
	
		else {
			swap(num[ox][oy], num[nx][ny]);
			pos[onum] = { nx, ny };
		}
	}
}

void Crash(int s_num, int xdir, int ydir, int cnt) {
	stun[s_num] = 2;

	if (cnt == 0) return;

	auto [sx, sy] = pos[s_num];
	sx += xdir * cnt;
	sy += ydir * cnt;

	if (OOM(sx, sy)) {
		pii& cur = pos[s_num];

		die[s_num] = true;
		num[cur.X][cur.Y] = 0;
		cur = { -1,-1 };
	}

	else {
		if (num[sx][sy] > 0)
			Interaction(num[sx][sy], xdir, ydir);
		
		pii& cur = pos[s_num];
		swap(num[cur.X][cur.Y], num[sx][sy]);
		cur = { sx, sy };
	}
}

void MoveR() {
	vector<tuple<int, int, int>> v;

	for (int i = 1; i <= P; i++) {
		if (die[i]) continue;

		pii cur = pos[i];
		int dist = Distance(cur.X, cur.Y);
		v.push_back({ ~dist, cur.X, cur.Y });
	}
	sort(v.begin(), v.end(), greater<>());

	auto[nd, nx, ny] = v[0];
	auto [cx, cy] = make_pair(rx - nx, ry - ny);
	auto [xdir, ydir] = make_pair(0, 0);
	

	if (cx != 0)
		xdir = cx < 0 ? 1 : -1;
	if (cy != 0)
		ydir = cy < 0 ? 1 : -1;
	
	rx += xdir;
	ry += ydir;

	if (rx == nx && ry == ny) {
		int s_num = num[nx][ny];
		ans[s_num] += C;
		Crash(s_num, xdir, ydir, C);
	}
}

void MoveS() {
	for (int i = 1; i <= P; i++) {
		if (die[i] || stun[i] > 0) continue;

		pii& cur = pos[i];
		int cmp = Distance(cur.X, cur.Y);
		int ndir = -1;

		for (int dir = 0; dir < 4; dir++) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];
			int ndist = Distance(nx, ny);

			if(!OOM(nx, ny) && num[nx][ny] == 0)
				if (ndist < cmp) {
					cmp = ndist;
					ndir = dir;
				}
		}

		if (ndir >= 0) {
			int nx = cur.X + dx[ndir];
			int ny = cur.Y + dy[ndir];

			if (nx == rx && ny == ry) {
				ans[i] += D;
				Crash(i, dx[ndir] * -1, dy[ndir] * -1, D - 1);
			}
			else {
				swap(num[cur.X][cur.Y], num[nx][ny]);
				cur = { nx, ny };
			}
		}
	}
}

bool Update() {
	bool ret = false;

	for (int i = 1; i <= P; i++)
		if (stun[i] > 0) stun[i]--;

	for (int i = 1; i <= P; i++) {
		if (!die[i]) {
			ans[i]++;
			ret = true;
		}
	}

	return ret;
}

void Print() {
	cout << "\n\n";
	cout << rx << ' ' << ry << '\n';
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			cout << num[i][j] << ' ';
		cout << '\n';
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> P >> C >> D;
	
	cin >> rx >> ry;
	rx--, ry--;

	for (int n, r, c, i = 1; i <= P; i++) {
		cin >> n >> r >> c;
		r--, c--;
		
		pos[n] = { r,c };
		num[r][c] = n;
	}

	while (M--) {
		MoveR();
		MoveS();
		if (!Update()) break;
	}

	for (int i = 1; i <= P; i++)
		cout << ans[i] << ' ';

	return 0;
}