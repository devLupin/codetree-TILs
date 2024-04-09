#include <bits/stdc++.h>
using namespace std;

const int SZ = 25;
const int dx[] = { 0,1,0,-1 };
const int dy[] = { 1,0,-1,0 };
const int ddx[] = { 0,1,0,-1 };
const int ddy[] = { -1,0,1,0 };
int N, M, board[SZ][SZ];
int sx, sy, ans;

void Attack(int d, int p) {
	auto [x, y] = make_pair(sx, sy);

	while (p--) {
		x += dx[d];
		y += dy[d];

		ans += board[x][y];
		board[x][y] = 0;
	}
}

vector<int> GetElem() {
	vector<int> v;

	auto [x, y] = make_pair(sx, sy);
	auto [d, cnt, cut, dirCnt] = make_tuple(0, 0, 1, 0);

	while (true) {
		if (x == 0 && y == 0) break;

		x += ddx[d];
		y += ddy[d];
		if (board[x][y] > 0) v.push_back(board[x][y]);

		cnt++;
		if (cnt == cut) {
			d = (d + 1) % 4;
			cnt = 0;

			dirCnt++;
			if (dirCnt == 2) {
				cut++;
				dirCnt = 0;
			}
		}
	}

	return v;
}

vector<int> RemoveDup(vector<int> v) {
	if (v.empty()) return {};

	while (true) {
		vector<int> ret;
		bool flag = false;
		int cnt = 1;

		ret.push_back(v[0]);
		for (int i = 1; i < v.size(); i++) {
			int cur = v[i];
			int prv = ret.back();

			if (prv == cur) cnt++;
			else {
				if (cnt >= 4) {
					ans += cnt * prv;
					while (cnt--) ret.pop_back();
				}
				cnt = 1;
			}

			ret.push_back(cur);
		}
		if (cnt >= 4) {
			ans += cnt * ret.back();
			while (cnt--)ret.pop_back();
		}

		if (v.size() == ret.size()) break;
		v = ret;
	}

	return v;
}

vector<int> Pairing(vector<int> v) {
	if (v.empty()) return {};

	vector<int> ret;

	int cmp = v[0];
	int cnt = 1;

	for (int i = 1; i < v.size(); i++) {
		int cur = v[i];

		if (cmp == cur) cnt++;
		else {
			ret.push_back(cnt);
			ret.push_back(cmp);
			cmp = cur;
			cnt = 1;
		}
	}
	ret.push_back(cnt);
	ret.push_back(cmp);

	return ret;
}

void Fill() {
	auto v = GetElem();
	v = RemoveDup(v);
	v = Pairing(v);

	fill(&board[0][0], &board[N][N], 0);

	int len = min(N * N - 1, (int)v.size());
	auto [x, y] = make_pair(sx, sy);
	auto [d, cnt, cut, dirCnt] = make_tuple(0, 0, 1, 0);

	for (int i = 0; i < len; i++) {
		x += ddx[d];
		y += ddy[d];

		board[x][y] = v[i];

		cnt++;
		if (cnt == cut) {
			d = (d + 1) % 4;
			cnt = 0;

			dirCnt++;
			if (dirCnt == 2) {
				cut++;
				dirCnt = 0;
			}
		}
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> N >> M;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];

	tie(sx, sy) = make_pair(N / 2, N / 2);

	for (int d, p; M--;) {
		cin >> d >> p;
		Attack(d, p);
		Fill();
	}

	cout << ans;
	return 0;
}