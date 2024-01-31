#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

int n, A[20][20], B[20][20];
int ans = 0x3f3f3f;

bool is_square(int i, int j, int d1, int d2) {
	int diff = abs(d1 - d2);
	if (i + d1 >= n || j - d1 < 0) return false;
	if (i + d2 >= n || j + d2 >= n) return false;
	if (i + d2 + d1 >= n || j + d2 - d1 < 0) return false;
	return true;
}

void labeling(vector<pii> pos) {
	fill_n(&B[0][0], 20 * 20, 1);

	// area2
	int tmp = 0;
	for (int i = 0; i < pos[1].X; i++) {
		if (i >= pos[0].X) tmp++;
		for (int j = 0; j <= pos[0].Y - tmp; j++)
			B[i][j] = 2;
	}

	// area3
	tmp = 0;
	for (int i = 0; i <= pos[2].X; i++) {
		if (i > pos[0].X) tmp++;
		for (int j = pos[0].Y + 1 + tmp; j < n; j++)
			B[i][j] = 3;
	}

	// area4
	tmp = 0;
	for (int i = n - 1; i >= pos[1].X; i--) {
		if (i < pos[3].X) tmp++;

		for (int j = 0; j < pos[3].Y - tmp; j++)
			B[i][j] = 4;
	}

	// area5
	tmp = 0;
	for (int i = n - 1; i > pos[2].X; i--) {
		if (i <= pos[3].X) tmp++;
		for (int j = pos[3].Y + tmp; j < n; j++)
			B[i][j] = 5;
	}
}

int calc() {
	vector<int> area(5, 0);
	
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			area[B[i][j] - 1] += A[i][j];

	sort(area.begin(), area.end());

	return area[4] - area[0];
}

void solve() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int d1 = 1; d1 <= j; d1++) {  // 세로방향 길이
				for (int d2 = 1; d2 < n - j; d2++) {  // 가로방향 길이
					if (is_square(i, j, d1, d2)) {
						vector<pii> pos;
						pos.push_back({ i,j });
						pos.push_back({ i + d1, j - d1 });
						pos.push_back({ i + d2, j + d2 });
						pos.push_back({ i + d1 + d2, j - d1 + d2 });

						labeling(pos);
						ans = min(calc(), ans);
					}
				}
			}
		}
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> n;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			cin >> A[i][j];

	solve();
	cout << ans;

	return 0;
}