#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

int n, k, A[15][15];

const int dx[] = { 0,0,-1,1 };
const int dy[] = { 1,-1,0,0 };

struct info {
	int x, y, d, o;
};
vector<info> v;

bool oom(int x, int y) { return x < 0 || y < 0 || x >= n || y >= n; }

int change_dir(int d) {
	int ret = -1;
	if (d == 0) ret = 1;
	else if (d == 1) ret = 0;
	else if (d == 2) ret = 3;
	else if (d == 3) ret = 2;
	return ret;
}

void Reverse(vector<int> idx) {
	vector<pii> order;

	for (int& i : idx)
		order.push_back({ v[i].o, i });

	sort(order.begin(), order.end(), greater<>());
	for (int i = 0; i < order.size(); i++) {
		pii cur = order[i];
		v[cur.second].o = i + 1;
	}
}

vector<int> move(info& cur, int nx, int ny) {
	vector<int> idx;
	int mx = 0, cnt = 0;

	for (int i = 0; i < v.size(); i++) {
		auto& nxt = v[i];
		if (nxt.x == cur.x && nxt.y == cur.y && nxt.o >= cur.o)
			idx.push_back(i);
		else if (nxt.x == nx && nxt.y == ny) {
			mx = max(mx, nxt.o);
			cnt++;
		}
	}

	for (int& i : idx) {
		v[i].x = nx;
		v[i].y = ny;
		v[i].o += mx;
	}

	idx.push_back(cnt);
	return idx;
}

bool go() {
	for (auto& nxt : v) {
		int d = nxt.d;
		int nx = nxt.x + dx[d];
		int ny = nxt.y + dy[d];

		if (oom(nx, ny) || A[nx][ny] == 2) {
			d = change_dir(d);
			nx = nxt.x + dx[d];
			ny = nxt.y + dy[d];

			if (oom(nx, ny) || A[nx][ny] == 2) continue;
			nxt.d = d;
		}

		vector<int> idx = move(nxt, nx, ny);
		int cnt = idx.back();
		idx.pop_back();

		if ((int)idx.size() + cnt >= 4) return true;

		if (A[nx][ny] == 1)
			Reverse(idx);
	}

	return false;
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	cin >> n >> k;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> A[i][j];

	for (int i = 0; i < k; i++) {
		int x, y, d;
		cin >> x >> y >> d;
		x--, y--, d--;
		v.push_back({ x,y,d,1 });
	}

	int cnt = 0;
	while (cnt++ <= 1000 && !go());

	if (cnt > 1000) cout << -1;
	else cout << cnt;

	return 0;
}