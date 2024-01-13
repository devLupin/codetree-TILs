#include <iostream>
#include <vector>
#include <queue>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int INF = 0x3f3f3f3f;
int ans = INF;
int N, M, A[50][50];
vector<pii> person, hos;

int get_dist() {
	int ret = 0;

	for (pii p : person) {
		int cmp = INF;
		for (pii h : hos) {
			if (A[h.X][h.Y] == 2) {
				int tmp = abs(p.X - h.X) + abs(p.Y - h.Y);
				cmp = min(cmp, tmp);
			}
		}
		ret += cmp;
		if (ret > ans) return INF;
	}

	return ret;
}

void dfs(int cnt) {
	if (cnt == M) {
		ans = min(ans, get_dist());
		return;
	}

	for (pii nxt : hos) {
		if (A[nxt.X][nxt.Y] == 2) {
			A[nxt.X][nxt.Y] = 0;
			dfs(cnt - 1);
			A[nxt.X][nxt.Y] = 2;
		}
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> A[i][j];
			if (A[i][j] == 1) person.push_back({ i,j });
			else if (A[i][j] == 2) hos.push_back({ i,j });
		}
	}

	dfs((int)hos.size());
	cout << ans;

	return 0;
}