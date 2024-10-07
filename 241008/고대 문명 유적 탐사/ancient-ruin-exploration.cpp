/**
 * @author         lupin
 * @date           2024-10-07
 *
 * @submit         00:44:57
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

int K, M;
vector<vector<int>> board;
vector<vector<int>> nextBoard;
bool vis[7][7];
vector<int> artifact;
int artifactIdx;

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= 5 || y >= 5; }

vector<pii> BFS(int sx, int sy)
{
	vector<pii> ret;
	queue<pii> q;

	q.push(make_pair(sx, sy));
	vis[sx][sy] = true;

	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();

		ret.push_back(make_pair(x, y));

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (!OOM(nx, ny) && !vis[nx][ny] && nextBoard[x][y] == nextBoard[nx][ny])
			{
				q.push(make_pair(nx, ny));
				vis[nx][ny] = true;
			}
		}
	}

	return ret;
}

vector<pii> GetArtifact()
{
	vector<pii> ret;

	fill(&vis[0][0], &vis[5][5], false);

	for (int x = 0; x < 5; x++)
		for (int y = 0; y < 5; y++)
			if (!vis[x][y])
			{
				vector<pii> pos = BFS(x, y);
				if (pos.size() >= 3)
				{
					for (const pii& nxt : pos)
						ret.push_back(nxt);
				}
			}

	return ret;
}

void Rotate(int sx, int sy, int sz)
{
	vector<vector<int>> temp = nextBoard;

	for (int x = sx; x < sx + sz; x++)
		for (int y = sy; y < sy + sz; y++)
		{
			int nx = y - sy;
			int ny = sz - (x - sx) - 1;
			temp[nx + sx][ny + sy] = nextBoard[x][y];
		}

	nextBoard = temp;
}

bool compare(const pii& a, const pii& b)
{
	if (a.Y != b.Y) return a.Y < b.Y;
	return a.X > b.X;
}

int FirstTurn()
{
	vector<pii> rmPos;
	vector<vector<int>> bestBoard;
	int cnt = 0;

	for (int i = 0; i < 3; i++)
	{
		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				nextBoard = board;
				Rotate(x, y, 3);
				vector<pii> result = GetArtifact();
				int sz = result.size();

				if (cnt < sz)
				{
					bestBoard = nextBoard;
					cnt = sz;
					rmPos = result;
				}
			}
		}
	}

	sort(rmPos.begin(), rmPos.end(), compare);

	for (const pii& nxt : rmPos)
		bestBoard[nxt.X][nxt.Y] = artifact[artifactIdx++];

	board = bestBoard;
	return rmPos.size();
}

int SecondTurn()
{
	int sz = 0;

	while (true)
	{
		nextBoard = board;

		auto rmPos = GetArtifact();
		if (rmPos.empty()) break;

		sort(rmPos.begin(), rmPos.end(), compare);

		for (const pii& nxt : rmPos)
			board[nxt.X][nxt.Y] = artifact[artifactIdx++];

		sz += rmPos.size();
	}

	board = nextBoard;
	return sz;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	board.assign(5, vector<int>(5));

	cin >> K >> M;

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			cin >> board[i][j];

	for (int a; M--;)
	{
		cin >> a;
		artifact.push_back(a);
	}

	while (K--)
	{
		int a = FirstTurn();
		if (a == 0) break;
		int b = SecondTurn();
		cout << a + b << ' ';

	}

	return 0;
}