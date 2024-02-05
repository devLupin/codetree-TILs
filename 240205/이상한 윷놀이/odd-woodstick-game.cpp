#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

struct horse { int x, y, d; };
horse h[10];

int N, K;
int dx[] = { 0, 0, 0, -1, 1 };
int dy[] = { 0, 1, -1, 0, 0 };
int turn[] = { 0, 2, 1, 4, 3 };
int color[13][13];
vector<int> info[13][13];

bool oom(int x, int y) { return x <= 0 || y <= 0 || x > N || y > N; }

int move(int i) {
	auto& now = h[i];
	int nx = now.x + dx[now.d];
	int ny = now.y + dy[now.d];

	if (oom(nx, ny) || color[nx][ny] == 2) {
		now.d = turn[now.d];

		nx = now.x + dx[now.d];
		ny = now.y + dy[now.d];

		if (oom(nx, ny) || color[nx][ny] == 2)
			return 0;
	}
	vector<int>& cur = info[now.x][now.y];
	vector<int>& next = info[nx][ny];
	auto s = find(cur.begin(), cur.end(), i);

	if (color[nx][ny] == 1)
		reverse(s, cur.end());

	for (auto it = s; it != cur.end(); ++it) {
		h[*it].x = nx, h[*it].y = ny;
		next.push_back(*it);
	}

	cur.erase(s, cur.end());
	return next.size();
}

int simulation() {
	for (int t = 1; t <= 1000; t++)	{
		for (int i = 0; i < K; ++i) {
			int cnt = move(i);
			if (cnt >= 4) return t;
		}
	}

	return -1;
}

int main() {
	ios::sync_with_stdio(0); 
	cin.tie(0);

	// freopen("input.txt", "r", stdin);

	cin >> N >> K;
	for (int i = 1; i <= N; ++i)
		for (int j = 1; j <= N; ++j)
			cin >> color[i][j];

	for (int i = 0; i < K; ++i)	{
		horse& ho = h[i];
		cin >> ho.x >> ho.y >> ho.d;
		info[ho.x][ho.y].push_back(i);
	}
	cout << simulation();
	return 0;
}