#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int,int>;

const int dx[] = {0,0,-1,-1,-1,0,1,1,1};
const int dy[] = {0,1,1,0,-1,-1,-1,0,1};

int N, M, board[20][20];
bool vis[20][20];
vector<pii> nutrients;

bool oom(int x, int y) {return x < 0 || y < 0 || x >= N || y >= N;}

void MoveNutrients(int d, int p) {
	for(auto& cur : nutrients) {
		int x = cur.X, y = cur.Y;
		int nx = (x + dx[d] * p + N * p) % N;
		int ny = (y + dy[d] * p + N * p) % N;

		board[nx][ny] += 1;
		vis[nx][ny] = true;
		cur = {nx, ny};
	}

	for(pii& cur : nutrients) {
		for(int dir : {2, 4, 6, 8}) {
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			if(!oom(nx, ny) && board[nx][ny] > 0) 
				board[cur.X][cur.Y] += 1;
		}
	}
}

void CutTrees() {
	for(int i=0; i<N; i++) {
		for(int j=0; j<N; j++) {
			if(board[i][j] >= 2 && !vis[i][j]) {
				board[i][j] -= 2;
				nutrients.push_back({i, j});
			}
		}
	}
}

void Print() {
	cout << '\n';
	for(int i=0; i<N; i++) {
		for(int j=0; j<N; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	
	cin >> N >> M;

	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			cin >> board[i][j];

	nutrients.push_back({N-1, 0});
	nutrients.push_back({N-1, 1});
	nutrients.push_back({N-2, 0});
	nutrients.push_back({N-2, 1});

	while(M--) {
		int d, p;
		cin >> d >> p;

		fill(&vis[0][0], &vis[N][N], false);

		MoveNutrients(d, p);
		nutrients.clear();
		CutTrees();
	}

	int ans = 0;
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			ans += board[i][j];

	cout << ans;
	return 0;
}