/**
 * @author         lupin
 * @date           2024-10-12
 *
 * @submit         00:44:34
 */


#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <queue>

using namespace std;
using pii = pair<int, int>;

const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };
const int SZ = 30;

int N, board[SZ][SZ], ans;
bool vis[SZ][SZ];

int groupBoard[SZ][SZ];
vector<int> numOfGroup, groupNum;
vector<vector<pii>> adjs;

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void BFS(int sx, int sy, int g)
{
	int cnt = 0;
	queue<pii> q;
	vector<pii> adj;

	q.push(make_pair(sx, sy));
	vis[sx][sy] = true;
	groupBoard[sx][sy] = g;

	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();

		cnt++;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (OOM(nx, ny)) continue;

			if (!vis[nx][ny] && board[sx][sy] == board[nx][ny])
			{
				q.push(make_pair(nx, ny));
				vis[nx][ny] = true;
				groupBoard[nx][ny] = g;
			}
			else if(board[sx][sy] != board[nx][ny])
			{
				adj.push_back(make_pair(nx, ny));
			}
		}
	}

	numOfGroup.push_back(cnt);
	groupNum.push_back(board[sx][sy]);
	adjs.push_back(adj);
}

map<pii, int> CalcNumAdj()
{
	map<pii, int> adjCnt;

	for (int i = 1; i < numOfGroup.size(); i++)
	{
		for (const auto& [x, y] : adjs[i])
		{
			int j = groupBoard[x][y];
			if (i > j) continue;

			adjCnt[make_pair(i, j)]++;
		}
	}

	return adjCnt;
}

void Artistry()
{
	int g = 1;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (!vis[i][j]) 
				BFS(i, j, g++);

	auto adjCnt = CalcNumAdj();

	for (const auto& [key, numAdj] : adjCnt)
	{
		const auto& [a, b] = key;
		ans += (numOfGroup[a] + numOfGroup[b]) * groupNum[a] * groupNum[b] * numAdj;
	}
}

void Init()
{
	fill(&vis[0][0], &vis[N][N], false);
	fill(&groupBoard[0][0], &groupBoard[N][N], 0);
	
	numOfGroup.clear();
	groupNum.clear();
	adjs.clear();

	numOfGroup.push_back(0);
	groupNum.push_back(0);
	adjs.push_back({});
}

void RotateSqaure(int sx, int sy, int sz)
{
	int temp[SZ][SZ];

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			temp[i][j] = board[i][j];

	for(int x = sx; x < sx + sz; x++)
		for (int y = sy; y < sy + sz; y++)
		{
			int ox = y - sy;
			int oy = sz - (x - sx) - 1;
			temp[sx + ox][sy + oy] = board[x][y];
		}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			board[i][j] = temp[i][j];
}

void RotateCross()
{
	vector<int> v1, v2;

	for (int i = 0; i < N; i++)
	{
		v1.push_back(board[i][N / 2]);
		v2.push_back(board[N / 2][N - i - 1]);
	}

	for (int i = 0; i < N; i++)
	{
		board[N / 2][i] = v1[i];
		board[i][N / 2] = v2[i];
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

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];

	Init();
	Artistry();

	for (int i = 0; i < 3; i++)
	{
		Init();
		Rotate();
		Artistry();
	}

	cout << ans;
	return 0;
}