/**
 * @author         lupin
 * @date           2024-10-01
 *
 * @submit         00:53:18
 * @revision	   00:29:47
 * @revision	   00:16:24
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#pragma warning(suppress : 6031)
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int DIR_NUM = 8;
const int MAX_T = 30;
const int MAX_N = 4;

const int dx[DIR_NUM] = { -1,-1,0,1,1,1,0,-1 };
const int dy[DIR_NUM] = { 0,-1,-1,-1,0,1,1,1 };
const int pdx[] = { -1,0,1,0 };
const int pdy[] = { 0,-1,0,1 };

int M, T, px, py;
int corpse[MAX_N][MAX_N];
int board[MAX_T][MAX_N][MAX_N][DIR_NUM];

int cmp = -1;
vector<int> bestRoute;
bool eat[MAX_N + 1][MAX_N + 1];

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= MAX_N || y >= MAX_N; }

void Print(int t)
{
	for (int x = 0; x < MAX_N; x++)
	{
		for (int y = 0; y < MAX_N; y++)
		{
			int cnt = 0;

			for (int dir = 0; dir < DIR_NUM; dir++)
				cnt += board[t][x][y][dir];

			cout << cnt << ' ';
		}
		cout << '\n';
	}
	cout << "\n\n";
}

void MoveMonster(int t)
{
	for (int x = 0; x < MAX_N; x++)
		for (int y = 0; y < MAX_N; y++)
			for (int dir = 0; dir < DIR_NUM; dir++)
			{
				if (board[t - 1][x][y][dir] == 0) continue;

				int nDir = dir;
				bool flag = false;

				for (int i = 0; i < DIR_NUM; i++)
				{
					int d = (nDir + i) % DIR_NUM;
					int nx = x + dx[d];
					int ny = y + dy[d];

					if (!OOM(nx, ny) && corpse[nx][ny] == 0 && make_pair(px, py) != make_pair(nx, ny))
					{
						board[t][nx][ny][d] += board[t - 1][x][y][dir];
						flag = true;
						break;
					}
				}

				if (!flag)board[t][x][y][dir] += board[t - 1][x][y][dir];
			}
}

void DFS(int t, int x, int y, int sum, vector<int> dirs)
{
	if (dirs.size() == 3)
	{
		if (cmp < sum)
		{
			cmp = sum;
			bestRoute = dirs;
		}

		return;
	}

	for (int dir = 0; dir < 4; dir++)
	{
		int nx = x + pdx[dir];
		int ny = y + pdy[dir];

		if (!OOM(nx, ny))
		{

			if (!eat[nx][ny])
			{
				int val = 0;

				for (int mDir = 0; mDir < DIR_NUM; mDir++)
					val += board[t][nx][ny][mDir];

				dirs.push_back(dir);
				eat[nx][ny] = true;
				DFS(t, nx, ny, sum + val, dirs);
				eat[nx][ny] = false;
				dirs.pop_back();
			}

			else
			{
				dirs.push_back(dir);
				DFS(t, nx, ny, sum, dirs);
				dirs.pop_back();
			}
		}
	}
}

void MovePackMan(int t)
{
	DFS(t, px, py, 0, {});

	for (int d : bestRoute)
	{
		px += pdx[d];
		py += pdy[d];

		for (int dir = 0; dir < DIR_NUM; dir++)
		{
			if (board[t][px][py][dir] > 0)
			{
				corpse[px][py] = 3;
				board[t][px][py][dir] = 0;
			}			
		}
	}
}

void Copy(int t)
{
	for (int x = 0; x < MAX_N; x++)
		for (int y = 0; y < MAX_N; y++)
			for (int dir = 0; dir < DIR_NUM; dir++)
				if (board[t - 1][x][y][dir] > 0)
					board[t][x][y][dir] += board[t - 1][x][y][dir];
}

void Done()
{
	bestRoute.clear();
	fill(&eat[0][0], &eat[MAX_N][MAX_N], false);
	cmp = 0;

	for (int x = 0; x < MAX_N; x++)
		for (int y = 0; y < MAX_N; y++)
			corpse[x][y] = max(0, corpse[x][y] - 1);
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> M >> T >> px >> py;
	px--, py--;
	for (int x, y, d, i = 0; i < M; i++)
	{
		cin >> x >> y >> d;
		x--, y--, d--;
		board[0][x][y][d]++;
	}

	for (int i = 1; i <= T; i++)
	{
		MoveMonster(i);
		MovePackMan(i);
		Copy(i);
		Done();
	}

	int ans = 0;

	for (int x = 0; x < MAX_N; x++)
		for (int y = 0; y < MAX_N; y++)
			for (int dir = 0; dir < DIR_NUM; dir++)
				ans += board[T][x][y][dir];

	cout << ans;
	return 0;
}