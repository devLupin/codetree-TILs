/**
 * @author         lupin
 * @date           2024-10-11
 *
 * @submit         00:44:34
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
const int MAX_N = 55;
const int MAX_P = 35;

int N, M, P, C, D;
int rx, ry;
int ans[MAX_P];
int board[MAX_N][MAX_N];
pii pos[MAX_P];
int stun[MAX_P];
bool escape[MAX_P];

void Print()
{
	cout << rx << ' ' << ry << '\n';
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << "\n\n";
}

int Distance(int x1, int y1, int x2, int y2)
{
	return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void Collision(int num, int ddx, int ddy, int cnt)
{
	auto [x, y] = pos[num];
	stun[num] = 2;

	int nx = x + ddx * cnt;
	int ny = y + ddy * cnt;

	if (OOM(nx, ny))
	{
		escape[num] = true;
		return;
	}

	if (board[nx][ny] > 0)
	{
		stack<int> st;
		st.push(num);

		while (board[nx][ny] != 0)
		{
			st.push(board[nx][ny]);
			board[nx][ny] = 0;
			nx += ddx;
			ny += ddy;
		}

		while (!st.empty())
		{
			int cur = st.top();
			st.pop();

			if (OOM(nx, ny))
				escape[cur] = true;
			else
			{
				board[nx][ny] = cur;
				pos[cur] = make_pair(nx, ny);
			}

			nx -= ddx;
			ny -= ddy;
		}
	}
	else
	{
		board[nx][ny] = num;
		pos[num] = make_pair(nx, ny);
	}
}

void MoveRudolf()
{
	vector<tiii> v;

	for (int i = 1; i <= P; i++)
	{
		if (escape[i]) continue;
		auto [x, y] = pos[i];
		v.push_back(make_tuple(Distance(rx, ry, x, y), -x, -y));
	}

	sort(v.begin(), v.end());
	auto [d, x, y] = v[0];
	x *= -1, y *= -1;

	int nDir, cmp = Distance(rx, ry, x, y);

	for (int dir = 0; dir < 8; dir++)
	{
		int nx = rx + dx[dir];
		int ny = ry + dy[dir];
		int nDist = Distance(x, y, nx, ny);

		if (!OOM(nx, ny) && cmp > nDist)
		{
			cmp = nDist;
			nDir = dir;
		}
	}

	rx += dx[nDir];
	ry += dy[nDir];
	int num = board[rx][ry];

	if (num > 0)
	{
		ans[num] += C;
		board[rx][ry] = 0;
		Collision(num, dx[nDir], dy[nDir], C);
	}
}

void MoveSanta()
{
	for (int i = 1; i <= P; i++)
	{
		if (stun[i] > 0 || escape[i]) continue;
		auto [x, y] = pos[i];
		int nDir = -1, cmp = Distance(rx, ry, x, y);

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];
			int nDist = Distance(rx, ry, nx, ny);

			if (!OOM(nx, ny) && board[nx][ny] == 0 && cmp > nDist)
			{
				cmp = nDist;
				nDir = dir;
			}
		}

		if (nDir != -1)
		{
			int nx = x + dx[nDir];
			int ny = y + dy[nDir];

			if (make_pair(nx, ny) == make_pair(rx, ry))
			{
				board[x][y] = 0;
				ans[i] += D;
				Collision(i, dx[nDir] * -1, dy[nDir] * -1, D - 1);
			}
			else
			{
				swap(board[x][y], board[nx][ny]);
				pos[i] = make_pair(nx, ny);
			}
		}
	}
}

bool Done()
{
	bool ret = false;
	for (int i = 1; i <= P; i++)
	{
		if (!escape[i])
		{
			ans[i]++;
			ret = true;
		}
		if (stun[i] > 0) stun[i]--;
	}
	return ret;
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> N >> M >> P >> C >> D;
	cin >> rx >> ry;
	rx--, ry--;

	for (int n, x, y, i = 1; i <= P; i++)
	{
		cin >> n >> x >> y;
		x--, y--;
		board[x][y] = n;
		pos[n] = make_pair(x, y);
	}

	while (M--)
	{
		MoveRudolf();
		MoveSanta();
		if (!Done()) break;
	}

	for (int i = 1; i <= P; i++) cout << ans[i] << ' ';
	return 0;
}