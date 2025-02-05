/**
 * @author         lupin
 * @date           2025-02-05
 *
 * @submit         00:48:10
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

const int SZ = 75;
int R, C, K, board[SZ][SZ], ans;
bool escape[SZ][SZ];
bool flag;

void Print()
{
	for (int i = 3; i < R; i++)
	{
		for (int j = 0; j < C; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << '\n';
}

bool OOM(int x, int y)
{
	return x < 0 || y < 0 || x >= R || y >= C;
}

bool MoveDown(int& x, int& y)
{
	if (OOM(x + 1, y - 1) || board[x + 1][y - 1] > 0) return false;
	if (OOM(x + 2, y) || board[x + 2][y] > 0) return false;
	if (OOM(x + 1, y + 1) || board[x + 1][y + 1] > 0) return false;
	
	x += 1;
	return true;
}

bool MoveLeft(int& x, int& y, int& d)
{
	if (OOM(x, y - 1) || board[x][y - 1] > 0) return false;
	if (OOM(x, y - 2) || board[x][y - 2] > 0) return false;
	if (OOM(x, y - 1) || board[x][y - 1] > 0) return false;

	y--;
	if (MoveDown(x, y))
	{
		d = (d + 3) % 4;
		return true;
	}
	y++;
	return false;
}

bool MoveRight(int& x, int& y, int &d)
{
	if (OOM(x, y + 1) || board[x][y + 1] > 0) return false;
	if (OOM(x, y + 2) || board[x][y + 2] > 0) return false;
	if (OOM(x, y + 1) || board[x][y + 1] > 0) return false;

	y++;
	if (MoveDown(x, y))
	{
		d = (d + 1) % 4;
		return true;
	}
	y--;
	return false;
}

bool Check(int x)
{
	if (x < 3 || x - 1 < 3 || x + 1 < 3)
	{
		fill(&board[0][0], &board[R][C], 0);
		fill(&escape[0][0], &escape[R][C], false);
		return false;
	}
	return true;
}

bool Paint(int x, int y, int dir, int num)
{
	if (!Check(x)) return false;

	for (int dir = 0; dir < 4; dir++)
	{
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		board[nx][ny] = num;
	}
	board[x][y] = num;
	escape[x + dx[dir]][y + dy[dir]] = true;

	return true;
}

int Bfs(int sx, int sy)
{
	int ret = 0;

	queue<pii> q;
	bool vis[SZ][SZ] = { false, };
	q.push({ sx, sy });
	vis[sx][sy] = true;

	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();

		ret = max(ret, x);

		if (escape[x][y])
		{
			for (int dir = 0; dir < 4; dir++)
			{
				int nx = x + dx[dir];
				int ny = y + dy[dir];

				if (!OOM(nx, ny) && !vis[nx][ny] && board[nx][ny] > 0)
				{
					q.push({ nx, ny });
					vis[nx][ny] = true;
				}
			}
		}

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (!OOM(nx, ny) && !vis[nx][ny] && board[x][y] == board[nx][ny])
			{
				q.push({ nx, ny });
				vis[nx][ny] = true;
			}
		}
	}

	return ret - 2;
}

void Move(int x, int y, int d, int i)
{	
	if (MoveDown(x, y)) Move(x, y, d, i);
	if (MoveLeft(x, y, d)) Move(x, y, d, i);
	if (MoveRight(x, y, d)) Move(x, y, d, i);

	if (!flag)
	{
		flag = true;
		if (Paint(x, y, d, i)) ans += Bfs(x, y);
	}
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> R >> C >> K;
	R += 3;
	for (int y, d, i = 1; i <= K; i++)
	{
		cin >> y >> d;
		flag = false;
		Move(1, y - 1, d, i);
	}

	cout << ans;
	return 0;
}