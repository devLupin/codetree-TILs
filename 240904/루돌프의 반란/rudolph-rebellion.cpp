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

const int MAX_N = 55;
const int MAX_P = 35;
const int RUDOLF = -1;

int N, M, P, C, D;
int rx, ry;
pii spos[MAX_P];
int board[MAX_N][MAX_N];
int stun[MAX_P];
bool out[MAX_P];
int score[MAX_P];

const int dx[] = { -1,0,1,0,-1,-1,1,1 };
const int dy[] = { 0,1,0,-1,-1,1,1,-1 };

int Diff(int ruX, int ruY, int sanX, int sanY) { return (ruX - sanX) * (ruX - sanX) + (ruY - sanY) * (ruY - sanY); }

bool OOM(int x, int y) { return x < 1 || y < 1 || x > N || y > N; }

void Interaction(stack<int> st, int tx, int ty, int moveX, int moveY)
{
	while (!st.empty())
	{
		int snum = st.top();
		auto [x, y] = spos[snum];
		st.pop();

		swap(board[x][y], board[tx][ty]);
		spos[snum] = { tx, ty };
		if (OOM(tx, ty)) out[snum] = true;

		tx -= moveX;
		ty -= moveY;
	}

	return;
}

void Collision(int snum, int moveX, int moveY, int cnt)
{
	auto [x, y] = spos[snum];
	int nx = x + moveX * cnt;
	int ny = y + moveY * cnt;

	if (OOM(nx, ny))
	{
		out[snum] = true;
		board[x][y] = 0;
		return;
	}

	if (board[nx][ny] > 0)
	{
		stack<int> st;
		auto [tx, ty] = make_pair(nx, ny);

		while (true)
		{
			if (OOM(tx, ty) || board[tx][ty] == 0) break;
			st.push(board[tx][ty]);
			tx += moveX;
			ty += moveY;
		}

		Interaction(st, tx, ty, moveX, moveY);
	}

	swap(board[x][y], board[nx][ny]);
	spos[snum] = { nx, ny };

	return;
}

void MoveRudolf()
{
	vector<tuple<int, int, int>> v;

	for (int i = 1; i <= P; i++)
	{
		if (out[i]) continue;

		auto [x, y] = spos[i];
		int d = Diff(rx, ry, x, y);
		v.push_back({ -d, x, y });
	}

	sort(v.begin(), v.end(), greater<>());

	auto [d, x, y] = v[0];

	int ddx = (rx < x) ? 1 : ((rx == x) ? 0 : -1);
	int ddy = (ry < y) ? 1 : ((ry == y) ? 0 : -1);

	int nrx = rx + ddx;
	int nry = ry + ddy;

	if (board[nrx][nry] > 0)
	{
		int snum = board[x][y];
		Collision(snum, ddx, ddy, C);
		score[snum] += C;
		stun[snum] = 2;
	}

	swap(board[rx][ry], board[nrx][nry]);
	rx = nrx;
	ry = nry;

	return;
}

void MoveSanta()
{
	for (int i = 1; i <= P; i++)
	{
		if (stun[i] || out[i]) continue;

		auto [x, y] = spos[i];
		int cd = Diff(rx, ry, x, y);
		int cDir = -1;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];
			int nd = Diff(rx, ry, nx, ny);

			if (!OOM(nx, ny) && cd > nd && board[nx][ny] <= 0)
			{
				cDir = dir;
				cd = nd;
			}
		}

		if (cDir != -1)
		{
			int nx = x + dx[cDir];
			int ny = y + dy[cDir];

			if (board[nx][ny] == RUDOLF)
			{
				Collision(i, dx[cDir] * -1, dy[cDir] * -1, D - 1);
				score[i] += D;
				stun[i] = 2;
			}
			else
			{
				swap(board[x][y], board[nx][ny]);
				spos[i].X = nx;
				spos[i].Y = ny;
			}
		}
	}

	return;
}

bool Done()
{
	bool ret = true;

	for (int i = 1; i <= P; i++)
	{
		if (out[i]) continue;
		if (stun[i] > 0) stun[i]--;
		score[i]++;
		ret = false;
	}

	return ret;
}

void Print()
{
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
		{
			if (board[i][j] == RUDOLF) cout << 'R' << ' ';
			else cout << board[i][j] << ' ';
		}
		cout << '\n';
	}
	cout << "\n\n";
}

int main(void) {
	FAST_IO();
	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> P >> C >> D;
	cin >> rx >> ry;
	board[rx][ry] = -1;
	for (int n, r, c, i = 1; i <= P; i++)
	{
		cin >> n >> r >> c;
		spos[n] = { r, c };
		board[r][c] = n;
	}

	while (M--)
	{
		MoveRudolf();
		MoveSanta();
		if (Done()) break;
	}

	for (int i = 1; i <= P; i++)
		cout << score[i] << ' ';

	return 0;
}