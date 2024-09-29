/**
 * @author         lupin
 * @date           2024-09-29
 *
 * @submit         01:01:43
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

int N, ans;
int board[MAX_N][MAX_N], nxtBoard[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];
int groupBoard[MAX_N][MAX_N];
vector<int> numOfCells;
vector<int> groupNum;
vector<vector<pii>> groupPos;

void Init()
{
	fill(&vis[0][0], &vis[N][N], false);
	fill(&groupBoard[0][0], &groupBoard[N][N], false);
	numOfCells.clear();
	groupNum.clear();
	groupPos.clear();
}

struct pair_hash
{
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2>& pair) const {
		return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
	}
};

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void MakeGroup(int x, int y)
{
	int cnt = 0;
	vector<pii> pos;
	queue<pii> q;
	q.push(make_pair(x, y));

	while (!q.empty())
	{
		auto [cx, cy] = q.front();
		q.pop();

		groupBoard[cx][cy] = groupNum.size();
		pos.push_back(make_pair(cx, cy));
		vis[cx][cy] = true;
		cnt++;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = cx + dx[dir];
			int ny = cy + dy[dir];

			if (!OOM(nx, ny) && !vis[nx][ny] && board[x][y] == board[nx][ny])
			{
				q.push(make_pair(nx, ny));
				vis[nx][ny] = true;
			}
		}
	}

	groupNum.push_back(board[x][y]);
	numOfCells.push_back(cnt);
	groupPos.push_back(pos);
}

void CalcArtistry()
{
	unordered_map<pii, int, pair_hash> duplication;

	for (int g1 = 0; g1 < groupNum.size(); g1++)
	{
		for (int g2 = 0; g2 < groupNum.size(); g2++)
		{
			if (duplication[make_pair(g1, g2)] > 0 || duplication[make_pair(g2, g1)] > 0) continue;
			if (g1 == g2) continue;

			int g1NumOfCells = numOfCells[g1];
			int g2NumOfCells = numOfCells[g2];
			int g1Num = groupNum[g1];
			int g2Num = groupNum[g2];
			int adjCnt = 0;

			for (auto pos : groupPos[g1])
				for (int dir = 0; dir < 4; dir++)
				{
					int nx = pos.X + dx[dir];
					int ny = pos.Y + dy[dir];

					if (!OOM(nx, ny) && groupBoard[nx][ny] == g2)
						adjCnt++;
				}

			if (adjCnt == 0) continue;

			ans += ((g1NumOfCells + g2NumOfCells) * g1Num * g2Num * adjCnt);
			duplication[make_pair(g1, g2)]++;
			duplication[make_pair(g2, g1)]++;
		}
	}
}

void RotateSquare(int sx, int sy, int sz)
{
	for(int x = sx; x < sx + sz; x++)
		for (int y = sy; y < sy + sz; y++)
		{
			int nx = y - sy;
			int ny = sz - (x - sx) - 1;
			nxtBoard[nx + sx][ny + sy] = board[x][y];
		}
}

void RotateBoard()
{
	for (int i = 0; i < N; i++) nxtBoard[N / 2][i] = board[i][N / 2];
	for (int i = 0; i < N; i++) nxtBoard[N - i - 1][N / 2] = board[N / 2][i];

	RotateSquare(0, 0, N / 2);
	RotateSquare(0, N / 2 + 1, N / 2);
	RotateSquare(N / 2 + 1, 0, N / 2);
	RotateSquare(N / 2 + 1, N / 2 + 1, N / 2);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			board[i][j] = nxtBoard[i][j];
}

void Solve()
{
	Init();

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (!vis[i][j]) MakeGroup(i, j);

	CalcArtistry();
	RotateBoard();
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> N;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];

	for(int i=0; i<4; i++)
		Solve();

	cout << ans;
	return 0;
}