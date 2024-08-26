#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int MAX_N = 80;
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };
enum { UP, RIGHT, DOWN, LEFT };
int R, C, K, board[MAX_N][MAX_N];
int ext[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];
int answer;

bool OOM(int x, int y) { return x < 1 || y < 1 || x > R || y > C; }
bool Empty(int x, int y) { return board[x][y] == 0; }

bool CanDown(int x, int y)
{
	int nx, ny;

	nx = x + 1;
	ny = y - 1;
	if (OOM(nx, ny) || !Empty(nx, ny)) return false;

	nx = x + 2;
	ny = y;
	if (OOM(nx, ny) || !Empty(nx, ny)) return false;

	nx = x + 1;
	ny = y + 1;
	if (OOM(nx, ny) || !Empty(nx, ny)) return false;

	return true;
}

bool CanRight(int x, int y)
{
	int nx, ny;

	nx = x - 1;
	ny = y + 1;
	if (OOM(nx, ny) || !Empty(nx, ny)) return false;

	nx = x;
	ny = y + 2;
	if (OOM(nx, ny) || !Empty(nx, ny)) return false;

	nx = x + 1;
	ny = y + 1;
	if (OOM(nx, ny) || !Empty(nx, ny)) return false;

	return CanDown(x, y + 1);
}

bool CanLeft(int x, int y)
{
	int nx, ny;

	nx = x - 1;
	ny = y - 1;
	if (OOM(nx, ny) || !Empty(nx, ny)) return false;

	nx = x;
	ny = y - 2;
	if (OOM(nx, ny) || !Empty(nx, ny)) return false;

	nx = x + 1;
	ny = y - 1;
	if (OOM(nx, ny) || !Empty(nx, ny)) return false;

	return CanDown(x, y - 1);
}

void Fill(int x, int y, int n)
{
	for (int dir = 0; dir < 4; dir++)
	{
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		board[nx][ny] = n;
	}
	board[x][y] = n;
}

bool IsOut(void)
{
	for (int i = 1; i <= 3; i++)
		for (int j = 1; j <= C; j++)
			if (board[i][j] > 0)
				return true;
	return false;
}

pii Search(int c, int d, int n)
{
	int x = 2;
	int y = c;
	int nx = x;
	int ny = y;
	int dir = d;

	while (true)
	{
		if (CanDown(x, y)) 
		{
			nx += dx[DOWN];
			ny += dy[DOWN];
		}
		else if (CanLeft(x, y)) 
		{
			nx += dx[LEFT];
			ny += dy[LEFT];
			nx += dx[DOWN];
			ny += dy[DOWN];
			dir = (dir + 3) % 4;
		}
		else if (CanRight(x, y)) 
		{
			nx += dx[RIGHT];
			ny += dy[RIGHT];
			nx += dx[DOWN];
			ny += dy[DOWN];
			dir = (dir + 1) % 4;
		}

		if (make_pair(x, y) != make_pair(nx, ny) && !OOM(nx, ny) && Empty(nx, ny))
		{
			x = nx;
			y = ny;
		}
		else
			break;
	}

	Fill(x, y, n);
	
	if (IsOut())
	{
		fill(&board[1][1], &board[R + 1][C + 1], 0);
		fill(&ext[1][1], &ext[R + 1][C + 1], 0);
		return { -1,-1 };
	}

	ext[x + dx[dir]][y + dy[dir]] = n;
	return { x,y };
}

int BFS(const int x, const int y, const int n)
{
	int ans = 0;
	queue<pii> q;
	fill(&vis[1][1], &vis[R + 1][C + 1], false);

	q.push({ x, y });
	vis[x][y] = true;

	while (!q.empty())
	{
		pii cur = q.front();
		q.pop();

		ans = max(ans, cur.X);

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			if (OOM(nx, ny) || vis[nx][ny]) continue;

			if (board[cur.X][cur.Y] == board[nx][ny]) {
				q.push({ nx, ny });
				vis[nx][ny] = true;
			}
			
			if (ext[cur.X][cur.Y] > 0 && board[nx][ny] > 0) {
				q.push({ nx, ny });
				vis[nx][ny] = true;
			}
		}
	}

	return ans - 3;
}

int main(void) 
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	cin >> R >> C >> K;

	R += 3;

	for (int c, d, i = 1; i <= K; i++) 
	{
		cin >> c >> d;
		pii pos = Search(c, d, i);

		if (pos.X > 0)
			answer += BFS(pos.X, pos.Y, i);
	}

	cout << answer;
	return 0;
}