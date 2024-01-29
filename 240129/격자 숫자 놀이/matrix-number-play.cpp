#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

int A[101][101];
int r, c, k, row, col;

void print() {
	for (int i = 1; i <= row; i++) {
		for (int j = 1; j <= col; j++)
			cout << A[i][j] << ' ';
		cout << '\n';
	}
	cout << '\n';
}

void init() {
	for (int i = 1; i <= 100; i++)
		memset(A[i], 0, sizeof(A[i]));
}

bool compare(pii& a, pii& b) {
	if (a.second == b.second) return a.first < b.first;
	return a.second < b.second;
}

void play_row() {
	int mx = 0;
	vector<vector<pii>> res;

	for (int i = 1; i <= row; i++) {
		unordered_map<int, int> um;

		for (int j = 1; j <= col; j++) {
			int cur = A[i][j];
			if (cur == 0) continue;;

			um[cur]++;
		}

		vector<pii> v(um.begin(), um.end());
		sort(v.begin(), v.end(), compare);

		res.push_back(v);
	}

	init();

	int r_idx = 1, c_idx;
	for (auto nxt : res) {
		c_idx = 1;
		for (auto elem : nxt) {
			A[r_idx][c_idx++] = elem.first;
			A[r_idx][c_idx++] = elem.second;
			if (c_idx == 100) break;
		}
		r_idx++;
		mx = max(mx, c_idx);
	}

	col = mx - 1;
}

void play_col() {
	int mx = 0;
	vector<vector<pii>> res;

	for (int j = 1; j <= col; j++) {
		unordered_map<int, int> um;

		for (int i = 1; i <= row; i++) {
			int cur = A[i][j];
			if (cur == 0) continue;;

			um[cur]++;
		}

		vector<pii> v(um.begin(), um.end());
		sort(v.begin(), v.end(), compare);

		res.push_back(v);
	}

	init();

	int r_idx, c_idx = 1;
	for (auto nxt : res) {
		r_idx = 1;
		for (auto elem : nxt) {
			A[r_idx++][c_idx] = elem.first;
			A[r_idx++][c_idx] = elem.second;
			if (r_idx == 100) break;
		}
		c_idx++;
		mx = max(mx, r_idx);
	}

	row = mx - 1;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	row = col = 3;

	cin >> r >> c >> k;
	for (int i = 1; i <= row; i++)
		for (int j = 1; j <= col; j++)
			cin >> A[i][j];

	int ans = -1;

	for(int t = 0; t <= 100; t++) {
		if (A[r][c] == k) {
			ans = t;
			break;
		}
		if (row >= col) play_row();
		else play_col();
	}
	
	cout << ans;

	return 0;
}