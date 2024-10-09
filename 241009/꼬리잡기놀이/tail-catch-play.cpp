/**
 * @author         lupin
 * @date           2024-10-09
 *
 * @submit         01:02:38
 * @revision       00:26:15
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#pragma warning(suppress : 6031)
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int dx[] = { 0,-1,0,1 };
const int dy[] = { 1,0,-1,0 };
const int MAX_N = 25;

int N, M, K, ans;
int board[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];
vector<deque<pii>> group;
vector<int> groupCnt;
int t, tDir, attackX, attackY;


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

void BFS(int sx, int sy)
{
	int cnt = 0;
	deque<pii> pos;
	queue<pii> q;

	q.push(make_pair(sx, sy));
	vis[sx][sy] = true;

	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();

		pos.push_back(make_pair(x, y));
		if (board[x][y] < 4) cnt++;

		int cmp = 5;
		int nDir = -1;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (!OOM(nx, ny) && !vis[nx][ny] && board[nx][ny] > 0)
			{
				if (cmp > board[nx][ny])
				{
					cmp = board[nx][ny];
					nDir = dir;
				}
			}
		}

		if (nDir != -1)
		{
			int nx = x + dx[nDir];
			int ny = y + dy[nDir];

			q.push(make_pair(nx, ny));
			vis[nx][ny] = true;
		}
	}

	group.push_back(pos);
	groupCnt.push_back(cnt);
}

void MakeGroup()
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (board[i][j] == 1)
				BFS(i, j);
}

void Move()
{
	for (int i = 0; i < group.size(); i++)
	{
		auto& cur = group[i];
		cur.push_front(cur.back());
		cur.pop_back();

		int cnt = groupCnt[i] - 2;
		int sz = cur.size();

		auto [x, y] = cur[0];
		board[x][y] = 1;

		for (int j = 1; j < cnt + 1; j++)
		{
			tie(x, y) = cur[j];
			board[x][y] = 2;
		}

		tie(x, y) = cur[cnt + 1];
		board[x][y] = 3;

		for (int j = cnt + 2; j < sz; j++)
		{
			tie(x, y) = cur[j];
			board[x][y] = 4;
		}
	}
}

void Calc()
{
	if (t == N)
	{
		t = 0;
		tDir = (tDir + 1) % 4;
	}

	if (tDir == 0) tie(attackX, attackY) = make_pair(t, 0);
	else if (tDir == 1) tie(attackX, attackY) = make_pair(N - 1, t);
	else if (tDir == 2) tie(attackX, attackY) = make_pair(N - t - 1, N - 1);
	else if (tDir == 3) tie(attackX, attackY) = make_pair(0, N - t - 1);
}

void ChangeHead(int idx)
{
	auto& target = group[idx];
	int sz = groupCnt[idx];
	int cnt = sz - 2;

	reverse(target.begin(), target.begin() + sz);
	reverse(target.begin() + sz, target.end());

	auto [x, y] = target[0];
	board[x][y] = 1;

	for (int j = 1; j < cnt + 1; j++)
	{
		tie(x, y) = target[j];
		board[x][y] = 2;
	}

	tie(x, y) = target[cnt + 1];
	board[x][y] = 3;
}

void Round()
{
	Calc();

	int x = attackX;
	int y = attackY;

	while (!OOM(x, y))
	{
		int val = board[x][y];

		if (val > 0 && val < 4)
		{
			for (int g = 0; g < group.size(); g++) 
			{
				const auto& cur = group[g];
				for (int i = 0; i < cur.size(); i++)
				{
					if (make_pair(x, y) == make_pair(cur[i].X, cur[i].Y))
					{
						ChangeHead(g);
						i++;
						ans += i * i;
						return;
					}
				}
			}
		}

		x += dx[tDir];
		y += dy[tDir];
	}
}

int main(void) {
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
		Round();
		t++;
	}

	cout << ans;
	return 0;
}