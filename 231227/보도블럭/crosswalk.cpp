#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

int N, L, A[105][105], B[105][105];

bool check(int arr[][105], int x, int y) {
	int cmp = arr[x][y + 1];
	for (int j = y + 1; j < y + 1 + L; j++)
		if (cmp != arr[x][j]) return false;
	return true;
}

int solve(int arr[][105]) {
	int ret = 0;

	for (int i = 0; i < N; i++) {
		bool flag = true;
		int prev = 1;

		for (int j = 0; j < N - 1; j++) {
			
			if (arr[i][j] == arr[i][j + 1]) prev++;

			else if (arr[i][j] == arr[i][j + 1] + 1) {
				if (check(arr, i, j)) {
					j += L - 1;
					prev = 0;
				}
				else {
					flag = false;
					break;
				}
			}

			else if (arr[i][j] + 1 == arr[i][j + 1]) {
				if (prev >= L) prev = 1;
				else {
					flag = false;
					break;
				}
			}

			else if (abs(arr[i][j] - arr[i][j + 1]) >= 2) {
				flag = false;
				break;
			}
		}

		ret += flag;
	}

	return ret;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> N >> L;
	for(int x = 0; x < N; x++)
		for (int y = 0; y < N; y++) {
			cin >> A[x][y];
			B[y][x] = A[x][y];
		}

	int ans = solve(A);
	ans += solve(B);

	cout << ans;
	return 0;
}