/**
 * @author         lupin
 * @date           2024-10-12
 *
 * @submit         00:58:32
 * @revision       00:13:13
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#pragma warning(suppress : 6031)
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

// ↑, ←, →, ↓
const int dx[] = { -1,0,0,1 };
const int dy[] = { 0,-1,1,0 };
const int MAX_N = 20;

int N, M, T, board[MAX_N][MAX_N];
bool arrived[MAX_N][MAX_N];
vector<pii> base, conv, pos;

void Print()
{
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
			cout << max(0, board[i][j]) << ' ';
		cout << "\n";
	}
	cout << "\n\n";
}

bool OOM(int x, int y) { return x < 1 || y < 1 || x > N || y > N; }

int BFS(int srcX, int srcY, int dstX, int dstY)
{
	queue<tiii> q;
	bool vis[MAX_N][MAX_N] = { false, };

	q.push(make_tuple(srcX, srcY, 0));
	vis[srcX][srcY] = true;

	while (!q.empty())
	{
		auto [x, y, d] = q.front();
		q.pop();

		if (make_pair(x, y) == make_pair(dstX, dstY))
			return d;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (!OOM(nx, ny) && !vis[nx][ny] && !arrived[nx][ny])
			{
				q.push(make_tuple(nx, ny, d + 1));
				vis[nx][ny] = true;
			}
		}
	}

	return 999999;
}

void Enter(int t)
{
	if (M < t) return;

	const auto [cx, cy] = conv[t];
	int x, y, cmp = 999999;

	for (int i = 1; i <= base.size() - 1; i++)
	{
		auto [bx, by] = base[i];
		if (arrived[bx][by]) continue;

		int d = BFS(cx, cy, bx, by);
		if (d < cmp)
		{
			cmp = d;
			tie(x, y) = make_pair(bx, by);
		}
	}

	pos.push_back(make_pair(x, y));
	arrived[x][y] = true;
}

void Move(int t)
{
	for (int i = 1; i <= min(t - 1, M); i++)
	{
		auto& [srcX, srcY] = pos[i];
		const auto& [dstX, dstY] = conv[i];

		if (arrived[dstX][dstY]) continue;

		int cDir, cmp = BFS(srcX, srcY, dstX, dstY);

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = srcX + dx[dir];
			int ny = srcY + dy[dir];
			if (OOM(nx, ny) || arrived[nx][ny]) continue;

			int nd = BFS(nx, ny, dstX, dstY);

			if (nd < cmp)
			{
				cmp = nd;
				cDir = dir;
			}
		}

		srcX += dx[cDir];
		srcY += dy[cDir];
	}

	for (int i = 1; i <= min(t - 1, M); i++)
		if (pos[i] == conv[i])
			arrived[conv[i].X][conv[i].Y] = true;
}

bool Run()
{
	if (pos.size() - 1 < M) return true;

	for (int i = 1; i <= M; i++)
		if (pos[i] != conv[i]) return true;
	return false;
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> N >> M;

	base.push_back({});
	conv.push_back({});
	pos.push_back({});

	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++)
		{
			cin >> board[i][j];
			if (board[i][j] == 1) base.push_back(make_pair(i, j));
		}

	for (int x, y, i = 1; i <= M; i++)
	{
		cin >> x >> y;
		conv.push_back(make_pair(x, y));
		board[x][y] = 2;
	}

	for (T = 1; Run(); T++)
	{
		Move(T);
		Enter(T);
	}

	cout << T - 1;
	return 0;
}