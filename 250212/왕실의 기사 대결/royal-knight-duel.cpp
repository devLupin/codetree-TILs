/**
 * @author         lupin
 * @date           2025-02-12
 *
 * @submit         00:36:07
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>

#define X		first
#define Y		second

using namespace std;
using pii = pair<int, int>;

enum { EMPTY, PIT, WALL };

const int MAX_L = 45;
const int MAX_N = 35;
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

int L, N, Q, board[MAX_L][MAX_L], chess[MAX_L][MAX_L];
bool visited[MAX_L][MAX_L];
struct Knight { int r, c, h, w, k; };
Knight knights[MAX_N];
int knightsHpOrigin[MAX_N];

void PrintChess()
{
	for (int i = 1; i <= L; i++)
	{
		for (int j = 1; j <= L; j++)
			cout << chess[i][j] << ' ';
		cout << '\n';
	}
	cout << '\n';
}

void DrawChess()
{
	for (int i = 1; i <= N; i++)
	{
		const auto [r, c, h, w, k] = knights[i];
		if (k <= 0) continue;

		for (int x = r; x < r + h; x++)
			for (int y = c; y < c + w; y++)
				chess[x][y] = i;
	}
}

void CleanChess()
{
	fill(&chess[1][1], &chess[L + 1][L + 1], 0);
	fill(&visited[1][1], &visited[L + 1][L + 1], false);
}

bool OOM(int x, int y) { return x < 1 || y < 1 || x > L || y > L; }

set<int> CanMoveChess(int num, int ddir)
{
	queue<pii> q;
	set<int> st;

	const auto [r, c, h, w, k] = knights[num];
	if (k <= 0) return st;

	for (int x = r; x < r + h; x++)
		for (int y = c; y < c + w; y++)
		{
			q.push({ x, y });
			visited[x][y] = true;
		}

	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();

		st.insert(chess[x][y]);

		int nx = x + dx[ddir];
		int ny = y + dy[ddir];

		if (OOM(nx, ny) || board[nx][ny] == WALL) return {};
		if (chess[nx][ny] > 0)
		{
			q.push({ nx, ny });
			visited[nx][ny] = true;
		}

		for (int dir = 0; dir < 4; dir++)
		{
			nx = x + dx[dir];
			ny = y + dy[dir];

			if (OOM(nx, ny) || visited[nx][ny] || chess[x][y] != chess[nx][ny]) continue;

			q.push({ nx, ny });
			visited[nx][ny] = true;
		}
	}

	return st;
}

void MoveChess(set<int> st, int dir)
{
	for (int nxt : st)
	{
		auto& [r, c, h, w, k] = knights[nxt];
		r += dx[dir];
		c += dy[dir];
	}
}

void CalcDamage(set<int> st, int num)
{
	for (int nxt : st)
	{
		if (nxt == num) continue;
		const auto& [r, c, h, w, k] = knights[nxt];

		for (int x = r; x < r + h; x++)
			for (int y = c; y < c + w; y++)
				if (board[x][y] == PIT) (knights[nxt].k)--;
	}
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	//  auto f = freopen("input.txt", "r", stdin);
	cin >> L >> N >> Q;

	for (int i = 1; i <= L; i++)
		for (int j = 1; j <= L; j++)
			cin >> board[i][j];

	for (int i = 1; i <= N; i++)
	{
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;
		knights[i] = { r,c,h,w,k };
		knightsHpOrigin[i] = k;
	}

	while (Q--)
	{
		int i, d;
		cin >> i >> d;
		if (knights[i].k <= 0) continue;

		DrawChess();
		auto st = CanMoveChess(i, d);
		MoveChess(st, d);
		CalcDamage(st, i);
		CleanChess();
	}

	int ans = 0;
	for (int i = 1; i <= N; i++)
		if (knights[i].k > 0)
			ans += (knightsHpOrigin[i] - knights[i].k);

	cout << ans;
	return 0;
}