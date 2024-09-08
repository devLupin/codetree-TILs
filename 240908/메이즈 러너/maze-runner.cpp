/**
 * @author lupin
 * @date 2024-09-08
 *
 * @submit   00:57:36
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

const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };
const int MAX_N = 15;
const int MAX_M = 15;

int N, M, K, ans;
int maze[MAX_N][MAX_N], nxtMaze[MAX_N][MAX_N];
pii entry[MAX_M], ext;
bool escape[MAX_M];

void Input()
{
	FAST_IO();

	cin >> N >> M >> K;
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++)
			cin >> maze[i][j];
	for (int i = 1; i <= M; i++)
		cin >> entry[i].X >> entry[i].Y;
	cin >> ext.X >> ext.Y;

	return;
}

int Distance(int x1, int y1, int x2, int y2) { return abs(x1 - x2) + abs(y1 - y2); }

bool OOM(int x, int y) { return x < 1 || y < 1 || x > N || y > N; }

void Move()
{
	for (int i = 1; i <= M; i++)
	{
		if (escape[i]) continue;

		auto& [x, y] = entry[i];
		int cDir = -1;
		int cDist = Distance(ext.X, ext.Y, x, y);

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];
			int nDist = Distance(ext.X, ext.Y, nx, ny);

			if (OOM(nx, ny) || maze[nx][ny] > 0) continue;
			if (cDist > nDist)
			{
				cDist = nDist;
				cDir = dir;
			}
		}

		if (cDir >= 0)
		{
			x += dx[cDir];
			y += dy[cDir];
			ans++;

			if (entry[i] == ext)
				escape[i] = true;
		}
	}
}

bool isSquare(int x, int y, int sz)
{
	bool chkPlayer = false;
	bool chkExit = false;

	for (int i = x; i < x + sz; i++)
	{
		for (int j = y; j < y + sz; j++)
		{
			for (int k = 1; k <= M; k++) 
			{
				if (escape[k]) continue;
				if (entry[k] == make_pair(i, j))
					chkPlayer = true;
			}
			if (ext == make_pair(i, j))
				chkExit = true;
		}

		if (chkPlayer && chkExit) return true;
	}
	return false;
}

void RotateSquare(int sx, int sy, int sz)
{
	for (int x = sx; x < sx + sz; x++)
		for (int y = sy; y < sy + sz; y++)
			if (maze[x][y] > 0) maze[x][y]--;

	for (int x = sx; x < sx + sz; x++)
		for (int y = sy; y < sy + sz; y++)
		{
			int ox = x - sx, oy = y - sy;
			int rx = oy, ry = sz - ox - 1;
			nxtMaze[rx + sx][ry + sy] = maze[x][y];
		}

	for (int x = sx; x < sx + sz; x++)
		for (int y = sy; y < sy + sz; y++)
			maze[x][y] = nxtMaze[x][y];
}

void RotateEntry(int sx, int sy, int sz)
{
	for (int k = 1; k <= M; k++)
	{
		auto& [x, y] = entry[k];

		if (x >= sx && x < sx + sz && y >= sy && y < sy + sz)
		{
			int ox = x - sx, oy = y - sy;
			int rx = oy, ry = sz - ox - 1;

			x = rx + sx;
			y = ry + sy;
		}
	}
}

void RotateExit(int sx, int sy, int sz)
{
	auto& [x, y] = ext;

	int ox = x - sx, oy = y - sy;
	int rx = oy, ry = sz - ox - 1;

	x = rx + sx;
	y = ry + sy;
}

void CalcSquare()
{
	for (int sz = 2; sz <= N; sz++)
		for (int i = 1; i <= N; i++)
			for (int j = 1; j <= M; j++)
				if (isSquare(i, j, sz))
				{
					RotateSquare(i, j, sz);
					RotateEntry(i, j, sz);
					RotateExit(i, j, sz);
					return;
				}
}

bool Run()
{
	for (int i = 1; i <= M; i++)
		if (!escape[i]) return true;
	return false;
}

void Print()
{

	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++) 
		{
			if (make_pair(i, j) == ext) cout << 'E' << ' ';
			else cout << maze[i][j] << ' ';
		}
		cout << '\n';
	}
	cout << "\n\n";
}

int main(void) {
	// freopen("input.txt", "r", stdin);

	Input();

	while (K-- && Run())
	{
		Move();
		CalcSquare();
		// Print();
	}

	cout << ans << '\n' << ext.X << ' ' << ext.Y;
	return 0;
}