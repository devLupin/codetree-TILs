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


const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };
const int MAX_SZ = 15;

int N, M, K, board[MAX_SZ][MAX_SZ];
int sum;
int ex, ey;
pii pos[MAX_SZ];
bool escape[MAX_SZ];

int Distance(int x1, int y1, int x2, int y2) { return abs(x1 - x2) + abs(y1 - y2); }

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void Move()
{
	for (int i = 0; i < M; i++)
	{
		if (escape[i]) continue;

		auto& [x, y] = pos[i];
		int cmp = Distance(ex, ey, x, y);
		int nDir = -1;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];
			int nDist = Distance(ex, ey, nx, ny);

			if (!OOM(nx, ny) && cmp > nDist && board[nx][ny] < 1)
			{
				cmp = nDist;
				nDir = dir;
			}
		}

		if (nDir != -1)
		{
			x += dx[nDir];
			y += dy[nDir];
			sum++;

			if (make_pair(ex, ey) == make_pair(x, y))
				escape[i] = true;
		}
	}
}

void RotateSquare(int sx, int sy, int sz)
{
	int nxt[MAX_SZ][MAX_SZ];

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			nxt[i][j] = board[i][j];

	for (int x = sx; x < sx + sz; x++)
		for (int y = sy; y < sy + sz; y++)
		{
			int ox = y - sy;
			int oy = sz - (x - sx) - 1;
			nxt[ox + sx][oy + sy] = max(board[x][y] - 1, 0);
		}

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			board[i][j] = nxt[i][j];
}

void RotateAttendance(vector<int> v, int sx, int sy, int sz)
{
	for (int& idx : v)
	{
		auto& [x, y] = pos[idx];
		int ox = y - sy;
		int oy = sz - (x - sx) - 1;
		x = ox + sx;
		y = oy + sy;
	}
}

void RotateExit(int sx, int sy, int sz)
{
	int ox = ey - sy;
	int oy = sz - (ex - sx) - 1;
	ex = ox + sx;
	ey = oy + sy;
}

void Select()
{
	for (int sz = 2; sz <= N; sz++)
		for (int x = 0; x < N; x++)
			for (int y = 0; y < N; y++)
			{
				auto [minX, minY] = make_pair(x, y);
				auto [maxX, maxY] = make_pair(x + sz - 1, y + sz - 1);

				if (OOM(maxX, maxY)) continue;

				if (minX > ex || minY > ey || maxX < ex || maxY < ey) continue;

				bool flag = false;
				vector<int> v;

				for (int i = 0; i < M; i++)
				{
					if (escape[i]) continue;

					const auto& [cmpX, cmpY] = pos[i];
					if (minX <= cmpX && minY <= cmpY && maxX >= cmpX && maxY >= cmpY)
					{
						v.push_back(i);
						flag = true;
					}
				}

				if (flag)
				{
					RotateSquare(x, y, sz);
					RotateAttendance(v, x, y, sz);
					RotateExit(x, y, sz);
					return;
				}
			}
	return;
}

bool Run()
{
	for (int i = 0; i < M; i++)
		if (!escape[i]) return true;
	return false;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> N >> M >> K;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];
	for (int x, y, i = 0; i < M; i++)
	{
		cin >> x >> y;
		x--, y--;
		pos[i] = make_pair(x, y);
	}
	cin >> ex >> ey;
	ex--, ey--;

	while (K-- && Run())
	{
		Move();
		Select();
	}

	cout << sum << '\n' << ex + 1 << ' ' << ey + 1;
	return 0;
}