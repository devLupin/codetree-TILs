/**
 * @author         lupin
 * @date           2024-09-24
 *
 * @submit         01:43:43
 * @revision       00:04:03
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
#define FAST_IO() \
	ios::sync_with_stdio(false); \
	cin.tie(NULL);

using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int dx[] = { -1,1,0,0,-1,-1,1,1 };
const int dy[] = { 0,0,-1,1,-1,1,1,-1 };
const int MAX_N = 25;

int N, M, K, C, ans;
int board[MAX_N][MAX_N], tempBoard[MAX_N][MAX_N], herbicide[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];
int val;

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
void PrintDecay()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << herbicide[i][j] << ' ';
		cout << '\n';
	}
	cout << "\n\n";
}

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void Growth()
{
	fill(&tempBoard[0][0], &tempBoard[N][N], 0);

	for (int x = 0; x < N; x++)
		for (int y = 0; y < N; y++)
		{
			if (board[x][y] <= 0) continue;

			for (int dir = 0; dir < 4; dir++)
			{
				int nx = x + dx[dir];
				int ny = y + dy[dir];

				if (!OOM(nx, ny) && board[nx][ny] > 0)
					tempBoard[x][y]++;
			}
		}

	for (int x = 0; x < N; x++)
		for (int y = 0; y < N; y++)
			board[x][y] += tempBoard[x][y];
}

void Breed()
{
	fill(&tempBoard[0][0], &tempBoard[N][N], 0);

	for (int x = 0; x < N; x++)
		for (int y = 0; y < N; y++)
		{
			if (board[x][y] <= 0) continue;

			int cnt = 0;

			for (int dir = 0; dir < 4; dir++)
			{
				int nx = x + dx[dir];
				int ny = y + dy[dir];

				if (!OOM(nx, ny) && board[nx][ny] == 0 && herbicide[nx][ny] == 0)
					cnt++;
			}

			if (cnt > 0)
			{
				int val = board[x][y] / cnt;

				for (int dir = 0; dir < 4; dir++)
				{
					int nx = x + dx[dir];
					int ny = y + dy[dir];

					if (!OOM(nx, ny) && board[nx][ny] == 0 && herbicide[nx][ny] == 0)
						tempBoard[nx][ny] += val;
				}
			}
		}

	for (int x = 0; x < N; x++)
		for (int y = 0; y < N; y++)
			board[x][y] += tempBoard[x][y];
}

void Search(int x, int y, int dir, int cnt, bool del)
{
	int cur = board[x][y];
	val += cur;

	if (del)
	{
		ans += board[x][y];
		board[x][y] = 0;
		herbicide[x][y] = C + 1;
	}

	if (cnt == 0 || cur == 0) return;

	int nx = x + dx[dir];
	int ny = y + dy[dir];

	if (!OOM(nx, ny) && !vis[nx][ny] && board[nx][ny] >= 0)
	{
		vis[nx][ny] = true;
		Search(nx, ny, dir, cnt - 1, del);
	}
}

void Spray()
{
	int tx = 0, ty = 0, cmp = 0;

	for (int x = 0; x < N; x++)
	{
		for (int y = 0; y < N; y++) 
		{
			val = board[x][y];

			if (board[x][y] > 0)
			{
				fill(&vis[0][0], &vis[N][N], false);
				vis[x][y] = true;

				for (int dir = 4; dir < 8; dir++)
				{
					int nx = x + dx[dir];
					int ny = y + dy[dir];

					if (!OOM(nx, ny) && !vis[nx][ny] && board[nx][ny] >= 0)
						Search(nx, ny, dir, K - 1, false);
				}

				if (cmp < val)
				{
					cmp = val;
					tie(tx, ty) = make_pair(x, y);
				}
			}
		}
	}

	if (cmp == 0) return;

	fill(&vis[0][0], &vis[N][N], false);
	vis[tx][ty] = true;

	ans += board[tx][ty];
	board[tx][ty] = 0;
	herbicide[tx][ty] = C + 1;

	for (int dir = 4; dir < 8; dir++)
	{
		int nx = tx + dx[dir];
		int ny = ty + dy[dir];

		if (!OOM(nx, ny) && !vis[nx][ny] && board[nx][ny] >= 0)
			Search(nx, ny, dir, K - 1, true);
	}
}

void Decay()
{
	for (int x = 0; x < N; x++)
		for (int y = 0; y < N; y++)
			herbicide[x][y] = max(0, herbicide[x][y] - 1);
}

int main(void)
{
	FAST_IO();
	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> K >> C;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];

	while (M--)
	{
		Growth();
		Breed();
		Spray();
		Decay();
	}

	cout << ans;

	return 0;
}