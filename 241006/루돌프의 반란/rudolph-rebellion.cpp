/**
 * @author         lupin
 * @date           2024-10-06
 *
 * @submit         01:12:58
 * @submit         00:01:30
 * @submit         00:06:35
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#pragma warning(suppress : 6031)
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int dx[] = { -1,0,1,0,-1,-1,1,1 };
const int dy[] = { 0,1,0,-1,-1,1,1,-1 };

const int MAX_N = 50;
const int MAX_P = 35;

int N, M, P, C, D, score[MAX_P];
int rx, ry;
int board[MAX_N][MAX_N];
pii santaPos[MAX_P];
bool escape[MAX_P];
int stun[MAX_P];


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

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

int Distance(int x1, int y1, int x2, int y2) { return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2); }

void Crash(int snum, int ddx, int ddy, int cnt)
{
	auto [nx, ny] = santaPos[snum];
	stack<int> st;

	stun[snum] = 2;
	board[nx][ny] = 0;
	st.push(snum);

	nx += ddx * cnt;
	ny += ddy * cnt;

	if (OOM(nx, ny))
	{
		escape[snum] = true;
		return;
	}
	else if (board[nx][ny] == 0)
	{
		board[nx][ny] = snum;
		santaPos[snum] = make_pair(nx, ny);
	}
	else
	{
		while (!OOM(nx, ny) && board[nx][ny] > 0)
		{
			st.push(board[nx][ny]);
			board[nx][ny] = 0;
			
			nx += ddx;
			ny += ddy;
		}

		while (!st.empty())
		{
			int num = st.top();
			auto [x, y] = santaPos[num];
			st.pop();

			if (OOM(nx, ny))
				escape[num] = true;
			else
			{
				board[nx][ny] = num;
				santaPos[num] = make_pair(nx, ny);
			}

			nx -= ddx;
			ny -= ddy;
		}
	}
}

void MoveRudolf()
{
	vector<tiii> v;

	for (int i = 1; i <= P; i++)
	{
		if (escape[i]) continue;
		auto [x, y] = santaPos[i];

		v.push_back(make_tuple(-Distance(rx, ry, x, y), x, y));
	}

	sort(v.begin(), v.end(), greater<>());
	auto [dist, sx, sy] = v[0];
	int cDir, cmp = Distance(rx, ry, sx, sy);

	dist *= -1;

	for (int dir = 0; dir < 8; dir++)
	{
		int nx = rx + dx[dir];
		int ny = ry + dy[dir];
		int nDist = Distance(nx, ny, sx, sy);

		if (!OOM(nx, ny) && cmp > nDist)
		{
			cDir = dir;
			cmp = nDist;
		}
	}

	rx += dx[cDir];
	ry += dy[cDir];

	if (make_pair(rx, ry) == make_pair(sx, sy))
	{
		int snum = board[sx][sy];
		score[snum] += C;
		Crash(snum, dx[cDir], dy[cDir], C);
	}
}

void MoveSanta()
{
	for (int i = 1; i <= P; i++)
	{
		if (stun[i] || escape[i]) continue;

		auto& [x, y] = santaPos[i];
		int nDir = -1, cmp = Distance(rx, ry, x, y);

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];
			int nDist = Distance(rx, ry, nx, ny);

			if (!OOM(nx, ny) && cmp > nDist && board[nx][ny] == 0)
			{
				nDir = dir;
				cmp = nDist;
			}
		}

		if (nDir != -1)
		{
			int nx = x + dx[nDir];
			int ny = y + dy[nDir];

			if (make_pair(rx, ry) == make_pair(nx, ny))
			{
				score[board[x][y]] += D;
				Crash(i, dx[nDir] * -1, dy[nDir] * -1, D - 1);
			}
			else
			{
				swap(board[x][y], board[nx][ny]);
				tie(x, y) = make_pair(nx, ny);
			}
		}
	}
}

bool Done()
{
	bool ret = false;

	for (int i = 1; i <= P; i++)
	{
		stun[i] = max(0, stun[i] - 1);
		score[i] += !escape[i];
		if (!escape[i]) ret = true;
	}

	return ret;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> N >> M >> P >> C >> D;
	cin >> rx >> ry;
	rx--, ry--;

	for (int n, x, y, i = 0; i < P; i++)
	{
		cin >> n >> x >> y;
		x--, y--;
		board[x][y] = n;
		santaPos[n] = make_pair(x, y);
	}

	while (M--)
	{
		MoveRudolf();
		MoveSanta();
		if (!Done()) break;
	}

	for (int i = 1; i <= P; i++)
		cout << score[i] << ' ';

	return 0;
}