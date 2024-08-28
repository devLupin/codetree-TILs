#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int MAX_N = 55;
const int MAX_P = 35;
int N, M, P, C, D;
int rx, ry;
int board[MAX_N][MAX_N];
pii santa[MAX_P];
int stun[MAX_P];
bool out[MAX_P];
int ans[MAX_P];

const int dx[] = { -1,0,1,0,-1,-1,1,1 };
const int dy[] = { 0,1,0,-1,1,-1,1,-1 };

struct pos
{
	int x, y, d;
};

bool compare(const pos& a, const pos& b)
{
	if (a.d == b.d) 
	{
		if (a.x == b.x) return a.y > b.y;
		return a.x > b.x;
	}
	return a.d < b.d;
}

int Distance(int nrx, int nry, int nx, int ny) { return (nrx - nx) * (nrx - nx) + (nry - ny) * (nry - ny); }

bool OOM(int x, int y) { return x < 1 || y < 1 || x > N || y > N; }

void Interaction(stack<pii> st, int ddx, int ddy)
{
	while (!st.empty())
	{
		pii cur = st.top();
		int snum = board[cur.X][cur.Y];
		st.pop();

		int nx = cur.X + ddx;
		int ny = cur.Y + ddy;

		if (OOM(nx, ny))
		{
			board[cur.X][cur.Y] = 0;
			out[snum] = true;
			return;
		}

		santa[snum] = { nx, ny };
		swap(board[cur.X][cur.Y], board[nx][ny]);
	}
}

void Collision(int snum, int damage, int ddx, int ddy, int cnt)
{
	int x = santa[snum].X;
	int y = santa[snum].Y;
	int nx = x + ddx * cnt;
	int ny = y + ddy * cnt;

	ans[snum] += damage;
	stun[snum] += 2;

	if (OOM(nx, ny)) 
	{
		board[x][y] = 0;
		out[snum] = true;
		return;
	}

	stack<pii> st;
	int nnx = nx;
	int nny = ny;

	while (!OOM(nnx, nny) && board[nnx][nny] > 0 && board[nnx][nny] != board[x][y])
	{
		st.push({ nnx, nny });
		nnx += ddx;
		nny += ddy;
	}

	if (!st.empty())
		Interaction(st, ddx, ddy);

	santa[snum] = { nx, ny };
	swap(board[x][y], board[nx][ny]);
}

void MoveRudolf()
{
	vector<pos> v;

	for (int i = 1; i <= P; i++)
	{
		if (out[i]) continue;

		int curX = santa[i].X;
		int curY = santa[i].Y;
		int curD = Distance(rx, ry, curX, curY);
		
		v.push_back({ curX, curY, curD });
	}

	sort(v.begin(), v.end(), compare);

	int sx = v[0].x;
	int sy = v[0].y;

	int rDir = -1;
	int rDist = Distance(rx, ry, sx, sy);

	for (int dir = 0; dir < 8; dir++)
	{
		int nrx = rx + dx[dir];
		int nry = ry + dy[dir];
		int nDist = Distance(nrx, nry, sx, sy);

		if (rDist > nDist) 
		{
			rDist = nDist;
			rDir = dir;
		}
	}

	int nrx = rx + dx[rDir];
	int nry = ry + dy[rDir];

	if (board[nrx][nry] > 0) 
		Collision(board[nrx][nry], C, dx[rDir], dy[rDir], C);

	rx = nrx;
	ry = nry;
}

void MoveSanta()
{
	for (int i = 1; i <= P; i++)
	{
		if (out[i] || stun[i]) continue;

		int curX = santa[i].X;
		int curY = santa[i].Y;
		int curD = Distance(rx, ry, curX, curY);
		int nDir = -1;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = curX + dx[dir];
			int ny = curY + dy[dir];
			int nxtD = Distance(rx, ry, nx, ny);

			if (OOM(nx, ny) || board[nx][ny] > 0) continue;

			if (curD > nxtD) {
				curD = nxtD;
				nDir = dir;
			}
		}

		if (nDir == -1) continue;

		int snx = curX + dx[nDir];
		int sny = curY + dy[nDir];

		if (snx == rx && sny == ry)
			Collision(i, D, dx[nDir] * -1, dy[nDir] * -1, D - 1);

		else
		{
			santa[i] = { snx, sny };
			swap(board[curX][curY], board[snx][sny]);
		}
	}
}

bool Done()
{
	bool ret = false;

	for (int i = 1; i <= P; i++)
	{
		if (out[i]) continue;

		ans[i]++;
		if (stun[i] > 0) stun[i]--;
		ret = true;
	}

	return ret;
}

int main(void) 
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	cin >> N >> M >> P >> C >> D;
	cin >> rx >> ry;

	for (int n, x, y, i = 1; i <= P; i++)
	{
		cin >> n >> x >> y;
		board[x][y] = n;
		santa[n] = { x, y };
	}

	while (M--)
	{
		MoveRudolf();
		MoveSanta();
		if (!Done()) break;
	}

	for (int i = 1; i <= P; i++)
		cout << ans[i] << ' ';
	
	return 0;
}