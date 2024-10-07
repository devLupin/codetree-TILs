/**
 * @author         lupin
 * @date           2024-10-07
 *
 * @submit         00:41:03
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#pragma warning(suppress : 6031)
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;


const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };
const int MAX_N = 75;
const int MAX_K = 1005;

int R, C, K, ans;
int board[MAX_N][MAX_N];
pii pos[MAX_K];
bool exitBoard[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];

void Print()
{
	for (int i = 3; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			if (exitBoard[i][j]) cout << 'X' << ' ';
			else cout << board[i][j] << ' ';
		}
		cout << '\n';
	}
	cout << "\n\n";
}

bool OOM(int x, int y) {
	return x < 0 || y < 0 || x >= R || y >= C;
}

bool MoveDown(int x, int y)
{
	int nx = x + 2;
	int ny = y;
	if (OOM(nx, ny) || board[nx][ny] > 0) return false;

	nx = x + 1;
	ny = y - 1;
	if (OOM(nx, ny) || board[nx][ny] > 0) return false;

	nx = x + 1;
	ny = y + 1;
	if (OOM(nx, ny) || board[nx][ny] > 0) return false;

	return true;
}

bool MoveLeft(int x, int y)
{
	int nx = x;
	int ny = y - 2;
	if (OOM(nx, ny) || board[nx][ny] > 0) return false;

	nx = x - 1;
	ny = y - 1;
	if (OOM(nx, ny) || board[nx][ny] > 0) return false;

	nx = x + 1;
	ny = y - 1;
	if (OOM(nx, ny) || board[nx][ny] > 0) return false;

	return MoveDown(x, y - 1);
}

bool MoveRight(int x, int y)
{
	int nx = x;
	int ny = y + 2;
	if (OOM(nx, ny) || board[nx][ny] > 0) return false;

	nx = x - 1;
	ny = y + 1;
	if (OOM(nx, ny) || board[nx][ny] > 0) return false;

	nx = x + 1;
	ny = y + 1;
	if (OOM(nx, ny) || board[nx][ny] > 0) return false;

	return MoveDown(x, y + 1);
}

void Move(int x, int y, int dir, int num)
{
	if (MoveDown(x, y))
		Move(x + 1, y, dir, num);
	else if (MoveLeft(x, y))
		Move(x + 1, y - 1, (dir + 3) % 4, num);
	else if (MoveRight(x, y))
		Move(x + 1, y + 1, (dir + 1) % 4, num);
	else if (pos[num] == make_pair(-1, -1))
	{
		pos[num] = make_pair(x, y);
		exitBoard[x + dx[dir]][y + dy[dir]] = true;

		for (int d = 0; d < 4; d++)
		{
			int nx = x + dx[d];
			int ny = y + dy[d];
			board[nx][ny] = num;
		}
		board[x][y] = num;

		return;
	}
}

bool Check()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < C; j++)
			if (board[i][j] > 0)
			{
				fill(&board[0][0], &board[R][C], 0);
				fill(&exitBoard[0][0], &exitBoard[R][C], false);
				return false;
			}
	return true;
}

int BFS(int num)
{
	int ret = 0;
	queue<pii> q;
	fill(&vis[0][0], &vis[R][C], false);

	q.push(pos[num]);
	vis[pos[num].X][pos[num].Y] = true;

	while (!q.empty())
	{
		pii cur = q.front();
		q.pop();

		ret = max(cur.X, ret);

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			if (OOM(nx, ny) || vis[nx][ny]) continue;

			if (exitBoard[cur.X][cur.Y] && board[nx][ny] > 0)
			{
				q.push(make_pair(nx, ny));
				vis[nx][ny] = true;
			}
			if (board[cur.X][cur.Y] == board[nx][ny])
			{
				q.push(make_pair(nx, ny));
				vis[nx][ny] = true;
			}
		}
	}

	return ret - 2;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> R >> C >> K;
	R += 3;

	fill(&pos[0], &pos[MAX_K], make_pair(-1, -1));

	for (int y, dir, k = 1; k <= K; k++)
	{
		cin >> y >> dir;
		y--;
		Move(0, y, dir, k);
		if (Check()) ans += BFS(k);
	}

	cout << ans;
	return 0;
}