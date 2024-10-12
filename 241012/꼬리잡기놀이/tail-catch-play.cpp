/**
 * @author         lupin
 * @date           2024-10-12
 *
 * @submit         00:54:34
 */


#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>
#include <tuple>

using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int dx[] = { 0,-1,0,1 };
const int dy[] = { 1,0,-1,0 };
const int MAX_N = 25;

int N, M, K, board[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];
int ans;
vector<deque<pii>> group;
vector<int> gCnt;

int attX, attY;
int cnt, tDir;

void Print()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << board[i][j] << ' ';
		cout << "\n";
	}
	cout << "\n\n";
}

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void BFS(int sx, int sy)
{
	queue<pii> q;
	deque<pii> dq;
	int cnt = 0;

	q.push(make_pair(sx, sy));
	vis[sx][sy] = true;

	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();

		dq.push_back(make_pair(x, y));
		if (board[x][y] > 0 && board[x][y] < 4) cnt++;

		int cDir = -1, cmp = 5;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (OOM(nx, ny) || vis[nx][ny] || board[nx][ny] == 0)
				continue;

			if (cmp > board[nx][ny])
			{
				cmp = board[nx][ny];
				cDir = dir;
			}
		}

		if (cDir != -1)
		{
			int nx = x + dx[cDir];
			int ny = y + dy[cDir];

			q.push(make_pair(nx, ny));
			vis[nx][ny] = true;
		}
	}

	group.push_back(dq);
	gCnt.push_back(cnt);
}

void MakeGroup()
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (board[i][j] == 1) BFS(i, j);
}

void Fill()
{
	fill(&board[0][0], &board[N][N], false);

	for (int i = 0; i < M; i++)
	{
		const auto g = group[i];
		int cnt = gCnt[i] - 2;
		int idx = 0;

		auto [x, y] = g[idx++];
		board[x][y] = 1;

		while (cnt--)
		{
			tie(x, y) = g[idx++];
			board[x][y] = 2;
		}

		tie(x, y) = g[idx++];
		board[x][y] = 3;

		while (idx < g.size())
		{
			tie(x, y) = g[idx++];
			board[x][y] = 4;
		}
	}
}

void Move()
{
	for (auto& g : group)
	{
		g.push_front(g.back());
		g.pop_back();
	}

	Fill();
}

void RoundInfo()
{
	if (cnt == N)
	{
		cnt = 0;
		tDir = (tDir + 1) % 4;
	}

	if (tDir == 0) tie(attX, attY) = make_pair(cnt, 0);
	else if (tDir == 1) tie(attX, attY) = make_pair(N - 1, cnt);
	else if (tDir == 2) tie(attX, attY) = make_pair(N - cnt - 1, N - 1);
	else if (tDir == 3) tie(attX, attY) = make_pair(0, N - cnt - 1);

	cnt++;
}

int Search(int x, int y)
{
	for (int i = 0; i < group.size(); i++)
	{
		for (int j = 0; j < group[i].size(); j++)
		{
			if (group[i][j] == make_pair(x, y))
			{
				ans += (j + 1) * (j + 1);
				return i;
			}
		}
	}
}

void ChangeMember(int num)
{
	auto& g = group[num];
	const int& sz = gCnt[num];

	reverse(g.begin(), g.begin() + sz);
	reverse(g.begin() + sz, g.end());

	Fill();
}

void Attack()
{
	int x = attX;
	int y = attY;

	while (!OOM(x, y))
	{
		if (board[x][y] > 0 && board[x][y] < 4)
		{
			int gNum = Search(x, y);
			ChangeMember(gNum);
			break;
		}
		x += dx[tDir];
		y += dy[tDir];
	}
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> N >> M >> K;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];

	MakeGroup();

	while (K--)
	{
		Move();
		RoundInfo();
		Attack();
	}

	cout << ans;
	return 0;
}