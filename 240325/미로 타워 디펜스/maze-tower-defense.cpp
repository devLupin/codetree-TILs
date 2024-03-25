#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int SZ = 30;

// → ↓ ← ↑
const int dx[] = { 0,1,0,-1 };
const int dy[] = { 1,0,-1,0 };

// ← ↓ → ↑
const int ddx[] = { 0,1,0,-1 };
const int ddy[] = { -1,0,1,0 };

int N, M, D, P, board[SZ][SZ];
bool vis[SZ][SZ];
int ans, numMonster;
int sx, sy;

void Attack() {
	int x = sx;
	int y = sy;

	while (P--) {
		x += dx[D];
		y += dy[D];
		int& cur = board[x][y];

		if (cur > 0) {
			ans += cur;
			cur = 0;
			numMonster--;
		}
	}
}

vector<int> Move() {
	int x = sx;
	int y = sy;
	int dir = 0;
	int curCnt = 1;
	int dirCnt = 0;
	int cnt = 0;

	vector<int> v;

	while (true) {
		if ((int)v.size() >= numMonster) break;
		if (x <= 0 && y <= 0) break;

		x += ddx[dir];
		y += ddy[dir];
		int& cur = board[x][y];

		if (cur > 0) {
			v.push_back(cur);
			cur = 0;
		}

		cnt++;
		if (cnt == curCnt) {
			cnt = 0;
			dir = (dir + 1) % 4;
			dirCnt++;

			if (dirCnt == 2) {
				dirCnt = 0;
				curCnt++;
			}
		}
	}

	if ((int)v.size() < numMonster) {
		for (int j = N - 2; j >= 0; j--) {
			int& cur = board[0][j];
			if (cur > 0) {
				v.push_back(cur);
				cur = 0;
			}
		}
	}

	return v;
}

vector<int> Check(vector<int> v) {
	vector<int> tmp(v.begin(), v.end());

	while (true) {
		bool flag = false;
		vector<bool> vis((int)tmp.size(), false);
		int sIdx = 0, eIdx;

		for (int i = 0; i < tmp.size(); i++) {
			if (tmp[sIdx] == tmp[i]) eIdx = i;
			else {
				if (eIdx - sIdx >= 3) {
					for (int j = sIdx; j <= eIdx; j++) vis[j] = true;
					flag = true;
				}
				sIdx = i;
			}
		}
		if (eIdx - sIdx >= 3) {
			for (int j = sIdx; j <= eIdx; j++) vis[j] = true;
			flag = true;
		}

		if (!flag) break;

		v.clear();
		for (int i = 0; i < tmp.size(); i++) {
			if (!vis[i]) v.push_back(tmp[i]);
			else ans += tmp[i];
		}
		tmp = v;
	}

	numMonster = (int)tmp.size();
	return tmp;
}

vector<int> Pair(vector<int> v) {
	vector<int> nxt;
	int cmp = v[0];
	int cnt = 0;

	for (int cur : v) {
		if (cmp == cur) cnt++;
		else {
			nxt.push_back(cnt);
			nxt.push_back(cmp);

			cmp = cur;
			cnt = 1;
		}
	}
	nxt.push_back(cnt);
	nxt.push_back(cmp);

	numMonster = (int)nxt.size();
	return nxt;
}

void Fill(vector<int> v) {
	int x = sx;
	int y = sy;
	int dir = 0;
	int curCnt = 1;
	int dirCnt = 0;
	int cnt = 0;
	int vCnt = 0;

	while (true) {
		if (vCnt >= (int)v.size()) break;

		x += ddx[dir];
		y += ddy[dir];
		board[x][y] = v[vCnt++];

		cnt++;
		if (cnt == curCnt) {
			cnt = 0;
			dir = (dir + 1) % 4;
			dirCnt++;

			if (dirCnt == 2) {
				dirCnt = 0;
				curCnt++;
			}
		}
	}

	if (vCnt < (int)v.size()) {
		for (int j = N - 2; j >= 0; j--) {
			int& cur = board[0][j];
			if (cur > 0) {
				v.push_back(cur);
				cur = 0;
			}
		}
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> board[i][j];
			if (board[i][j] > 0) numMonster++;
		}
	}

	tie(sx, sy) = make_pair(N / 2, N / 2);

	while (M--) {
		cin >> D >> P;

		Attack();
		auto v = Move();
		v = Check(v);
		v = Pair(v);
		Fill(v);
	}

	cout << ans;
	return 0;
}