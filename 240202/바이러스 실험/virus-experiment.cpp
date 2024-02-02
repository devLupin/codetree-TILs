#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

int n, m, k, r, c, a;
int A[15][15], B[15][15];

int dx[] = { -1,-1,-1,0,1,1,1,0 };
int dy[] = { -1,0,1,1,1,0,-1,-1 };

struct info {
	int x, y, age;
};
vector<info> virus, spread_virus, dead_virus, new_virus;

bool oom(int x, int y) { return x < 1 || y < 1 || x > n || y > n; }

bool compare(info& a, info& b) { return a.age < b.age; }

void eat() {
	sort(virus.begin(), virus.end(), compare);

	for (int i = 0; i < virus.size(); i++) {
		info& nxt = virus[i];

		if (nxt.age <= A[nxt.x][nxt.y]) {
			A[nxt.x][nxt.y] -= nxt.age;
			virus[i].age++;

			if (virus[i].age % 5 == 0)
				spread_virus.push_back(virus[i]);

			new_virus.push_back(virus[i]);
		}
		else
			dead_virus.push_back(nxt);
	}
}

void update() {
	for (auto& nxt : dead_virus)
		A[nxt.x][nxt.y] += nxt.age / 2;

	for (auto& nxt : spread_virus) {
		for (int dir = 0; dir < 8; dir++) {
			int nx = nxt.x + dx[dir];
			int ny = nxt.y + dy[dir];

			if (!oom(nx, ny))
				new_virus.push_back({ nx,ny,1 });
		}
	}

	virus = new_virus;
}

void add() {
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= n; j++)
			A[i][j] += B[i][j];
}

void init() {
	spread_virus.clear();
	dead_virus.clear();
	new_virus.clear();
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> n >> m >> k;

	for (int i = 1; i <= n; i++) {
		for (int x, j = 1; j <= n; j++) {
			cin >> x;
			A[i][j] = 5;
			B[i][j] = x;
		}
	}

	while (m--) {
		cin >> r >> c >> a;
		virus.push_back({ r, c, a });
	}

	while (k--) {
		init();
		eat();
		update();
		add();
	}

	int ans = 0;
	for (auto& nxt : virus)
		ans += (nxt.x != -1 && nxt.y != -1);

	cout << ans;
	return 0;
}