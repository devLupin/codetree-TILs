#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

struct student {
	int num;
	vector<int> likes;
};

int N, n0, n1, n2, n3, n4;

const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };
const int SZ = 25;
const int SCORE[] = { 0,1,10,100,1000 };

int board[SZ][SZ];
pair<int, int> pos[SZ * SZ];
vector<student> v;

void set_pos(student& cur) {
	vector<tuple<int, int, int>> tmp;
	int cmp = -1;

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (board[i][j] != 0) continue;

			int cnt = 0;
			for (int dir = 0; dir < 4; dir++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];

				for (int stu : cur.likes)
					if (board[nx][ny] == stu) cnt++;
			}

			if (cmp <= cnt) {
				cmp = cnt;
				tmp.push_back(make_tuple(~cnt, i, j));
			}
		}
	}

	sort(tmp.begin(), tmp.end());
	auto& [cnt, x, y] = tmp[0];

	pos[cur.num] = make_pair(x, y);
	board[x][y] = cur.num;
}

int calc() {
	int sum = 0;

	for (auto& nxt : v) {
		int& num = nxt.num;
		auto& [x, y] = pos[num];

		int cnt = 0;
		for (int dir = 0; dir < 4; dir++) {
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			for (int& cmp : nxt.likes)
				if (board[nx][ny] == cmp) cnt++;
		}

		sum += SCORE[cnt];
	}

	return sum;
}

void solve() {
	auto& first = v[0];
	pos[first.num] = make_pair(N - 1, N - 1);
	board[N - 1][N - 1] = first.num;

	for (int idx = 1; idx < v.size(); idx++)
		set_pos(v[idx]);

	cout << calc();
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N;

	for (int i = 0; i < N * N; i++) {
		cin >> n0 >> n1 >> n2 >> n3 >> n4;
		v.push_back({ n0, {n1, n2, n3, n4} });
	}

	solve();

	return 0;
}