/**
 * @author         lupin
 * @date           2024-10-09
 *
 * @submit         00:46:38
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#pragma warning(suppress : 6031)
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };
const int MAX_N = 30;

int N, board[MAX_N][MAX_N], ans;
bool vis[MAX_N][MAX_N];
int gBoard[MAX_N][MAX_N];
vector<int> gNum;
vector<int> gVal;
vector<int> gCnt;
vector<vector<pii>> adjs;

void Print()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << "\n\n";
}

void RotateSqaure(int sx, int sy, int sz)
{
	int temp[MAX_N][MAX_N];

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			temp[i][j] = board[i][j];

	for (int x = sx; x < sx + sz; x++)
		for (int y = sy; y < sy + sz; y++)
		{
			int ox = y - sy;
			int oy = sz - (x - sx) - 1;
			temp[ox + sx][oy + sy] = board[x][y];
		}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			board[i][j] = temp[i][j];
}

void RotateCross()
{
	vector<int> v1, v2;
	int mid = N / 2;

	for (int x = 0; x < N; x++) v1.push_back(board[x][mid]);
	for (int y = N - 1; y >= 0; y--) v2.push_back(board[mid][y]);

	for (int i = 0; i < N; i++)
	{
		board[mid][i] = v1[i];
		board[i][mid] = v2[i];
	}
}

void Rotate()
{
	int mid = N / 2;
	RotateSqaure(0, 0, mid);
	RotateSqaure(0, mid + 1, mid);
	RotateSqaure(mid + 1, 0, mid);
	RotateSqaure(mid + 1, mid + 1, mid);
	RotateCross();
}

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void BFS(int sx, int sy, int num)
{
	queue<pii> q;
	int cnt = 0;
	vector<pii> adj;

	q.push(make_pair(sx, sy));
	vis[sx][sy] = true;

	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();

		cnt++;
		gBoard[x][y] = num;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (OOM(nx, ny)) continue;

			if (!vis[nx][ny] && board[nx][ny] == board[sx][sy])
			{
				q.push(make_pair(nx, ny));
				vis[nx][ny] = true;
			}

			if (board[nx][ny] != board[sx][sy])
				adj.push_back(make_pair(nx, ny));
		}
	}

	gNum.push_back(num);
	gCnt.push_back(cnt);
	adjs.push_back(adj);
	gVal.push_back(board[sx][sy]);
}

void Init()
{
	fill(&vis[0][0], &vis[N][N], false);
	fill(&gBoard[0][0], &gBoard[N][N], 0);

	gNum.clear();
	gCnt.clear();
	adjs.clear();
	gVal.clear();
}

void Artistry()
{
	map<pii, int> adjCnt;
	int gnum = 0;
	Init();

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (!vis[i][j])
				BFS(i, j, gnum++);

	for (int i = 0; i < gnum; i++)
	{
		int g1 = gNum[i];
		
		for (const auto& [x, y] : adjs[i])
		{
			int g2 = gBoard[x][y];
			if (g2 < g1) continue;

			adjCnt[make_pair(g1, g2)]++;
		}
	}

	for (const auto& [key, adj] : adjCnt)
	{
		int g1 = key.first;
		int g2 = key.second;
		ans += (gCnt[g1] + gCnt[g2]) * gVal[g1] * gVal[g2] * adj;
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> N;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];

	Artistry();

	for (int i = 0; i < 3; i++)
	{
		Rotate();
		Artistry();
	}

	cout << ans;
	return 0;
}