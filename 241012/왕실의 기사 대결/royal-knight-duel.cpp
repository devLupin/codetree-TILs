/**
 * @author         lupin
 * @date           2024-10-12
 *
 * @submit         00:54:28
 * @revision       00:05:01
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#pragma warning(suppress : 6031)
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int MAX_L = 45;
const int MAX_N = 35;
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

struct Info
{
	int r, c, h, w;
};

int L, N, Q, ans;
int board[MAX_L][MAX_L];
vector<vector<int>> chess;
bool vis[MAX_L][MAX_L];
Info knights[MAX_N];
int hp[MAX_N], nxtHP[MAX_N];
bool die[MAX_N];

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= L || y >= L; }

set<int> BFS(int num, int d)
{
	queue<pii> q;
	set<int> nodes;

	const auto& [r, c, h, w] = knights[num];

	for (int x = r; x < r + h; x++)
		for (int y = c; y < c + w; y++)
		{
			q.push(make_pair(x, y));
			vis[x][y] = true;
		}

	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();

		nodes.insert(chess[x][y]);

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (OOM(nx, ny) || vis[nx][ny]) continue;

			if (!vis[nx][ny] && chess[x][y] == chess[nx][ny])
			{
				q.push(make_pair(nx, ny));
				vis[nx][ny] = true;
			}
		}

		int nx = x + dx[d];
		int ny = y + dy[d];

		if (!OOM(nx, ny) && !vis[nx][ny] && chess[nx][ny] > 0)
		{
			q.push(make_pair(nx, ny));
			vis[nx][ny] = true;
		}
	}

	return nodes;
}

bool Check(set<int> nodes, int dir)
{
	for (int i : nodes)
	{
		const auto& [r, c, h, w] = knights[i];

		for (int x = r; x < r + h; x++)
			for (int y = c; y < c + w; y++)
			{
				int nx = x + dx[dir];
				int ny = y + dy[dir];

				if (OOM(nx, ny) || board[nx][ny] == 2) return false;
			}
	}
	return true;
}

void MoveKnights(int num, int dir)
{
	chess.assign(L, vector<int>(L));
	fill(&vis[0][0], &vis[L][L], false);

	for (int i = 1; i <= N; i++)
	{
		if (die[i]) continue;
		const auto& [r, c, h, w] = knights[i];

		for (int x = r; x < r + h; x++)
			for (int y = c; y < c + w; y++)
				chess[x][y] = i;
	}

	set<int> nodes = BFS(num, dir);

	if (Check(nodes, dir))
	{
		for (int cur : nodes)
		{
			int curHp = hp[cur];
			auto& [r, c, h, w] = knights[cur];

			r += dx[dir];
			c += dy[dir];

			if (cur == num) continue;

			for (int x = r; x < r + h; x++)
				for (int y = c; y < c + w; y++)
					curHp -= board[x][y];

			if (curHp <= 0)
			{
				die[cur] = true;
				nxtHP[cur] = 0;
			}
			else
			{
				int diff = hp[cur] - curHp;
				nxtHP[cur] += diff;
				hp[cur] -= diff;
			}
		}
	}
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> L >> N >> Q;
	
	for (int i = 0; i < L; i++)
		for (int j = 0; j < L; j++)
			cin >> board[i][j];
	
	for (int r, c, h, w, k, i = 1; i <= N; i++)
	{
		cin >> r >> c >> h >> w >> k;
		r--, c--;
		knights[i] = { r,c,h,w };
		hp[i] = k;
	}

	for (int i, d; Q--;)
	{
		cin >> i >> d;
		MoveKnights(i, d);
	}

	for (int i = 1; i <= N; i++)
		ans += nxtHP[i];

	cout << ans;
	return 0;
}