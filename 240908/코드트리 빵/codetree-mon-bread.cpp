/**
 * @author lupin
 * @date 2024-09-07
 * @submit_time 00:52:00
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

// ↑, ←, →, ↓
const int dx[] = { -1,0,0,1 };
const int dy[] = { 0,-1,1,0 };
const int MAX_N = 20;
const int MAX_M = MAX_N * MAX_N;

int N, M, board[MAX_N][MAX_N];
bool canGo[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];
bool arrived[MAX_M];
vector<pii> pos, convinience, basecamp;

bool OOM(int x, int y) { return x < 1 || y < 1 || x > N || y > N; }

tiii BFS(pii src, pii dst)
{
	queue<tiii> q;
	fill(&vis[1][1], &vis[N + 1][N + 1], false);

	q.push({ 0, src.X, src.Y });
	vis[src.X][src.Y] = true;

	while (!q.empty())
	{
		auto [d, x, y] = q.front();
		q.pop();

		if (make_pair(x, y) == dst)
			return make_tuple(d, x, y);

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (OOM(nx, ny) || vis[nx][ny] || canGo[nx][ny]) continue;

			q.push({ d + 1, nx, ny });
			vis[nx][ny] = true;
		}
	}

	return {};
}

void EnterBasecamp(int t)
{
	vector<tiii> v;

	if (M < t) return;

	for (pii dst : basecamp)
	{
		if (canGo[dst.X][dst.Y]) continue;
		v.push_back(BFS(convinience[t], dst));
	}

	sort(v.begin(), v.end());
	auto [d, nx, ny] = v[0];

	pos[t] = { nx, ny };
	board[nx][ny] = t;
	canGo[nx][ny] = true;

	return;
}

void Move(int t)
{
	for (int i = 1; i < min(t, M + 1); i++)
	{
		if (arrived[i]) continue;

		pii cur = pos[i];
		int ddir, cmp = MAX_M;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			if (OOM(nx, ny) || canGo[nx][ny]) continue;
			auto [d, x, y] = BFS(make_pair(nx, ny), convinience[i]);

			if (d < cmp)
			{
				ddir = dir;
				cmp = d;
			}
		}

		int nx = cur.X + dx[ddir];
		int ny = cur.Y + dy[ddir];

		board[cur.X][cur.Y] = 0;
		pos[i] = { nx, ny };
		board[nx][ny] = i;
	}

	return;
}

void Update(int t)
{
	for (int i = 1; i < min(t, M + 1); i++)
	{
		if (arrived[i]) continue;

		pii cur = pos[i];

		if (cur == convinience[i])
		{
			arrived[i] = true;
			canGo[cur.X][cur.Y] = true;
		}
	}

	return;
}

bool Run()
{
	for (int i = 1; i <= M; i++)
	{
		if (arrived[i]) continue;
		return true;
	}

	return false;
}

void Print()
{
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << "\n\n";
	return;
}

int main(void) {
	FAST_IO();
	// freopen("input.txt", "r", stdin);

	cin >> N >> M;
	pos.assign(M + 1, {});
	convinience.assign(M + 1, {});

	for (int i = 1; i <= N; i++)
		for (int x, j = 1; j <= N; j++)
		{
			cin >> x;
			if (x == 1) basecamp.push_back({ i, j });
		}

	for (int i, j, k = 1; k <= M; k++)
	{
		cin >> i >> j;
		convinience[k] = { i, j };
	}

	int t = 0;
	while (Run() && ++t)
	{
		Move(t);
		Update(t);
		EnterBasecamp(t);
	}

	cout << t;
	return 0;
}