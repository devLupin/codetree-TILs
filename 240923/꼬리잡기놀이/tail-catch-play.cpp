/**
 * @author lupin
 * @date 2024-09-23
 *
 * @submit_time 00:45:15
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

const int dx[] = { 0,-1,0,1 };
const int dy[] = { 1,0,-1,0 };
const int MAX_N = 20;

int N, M, K, score;
int t, tDir;
int board[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];
vector<pii> head;
vector<vector<pii>> groups;
vector<int> ngroup;

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

void MakeGroup()
{
	for (auto& [hx, hy] : head)
	{
		queue<pii> q;
		int cnt = 0;

		groups.push_back({});
		groups.back().push_back({ hx, hy });
		q.push({ hx, hy });
		vis[hx][hy] = true;

		while (!q.empty())
		{
			auto [x, y] = q.front();
			q.pop();

			if (board[x][y] <= 3) cnt++;

			for (int dir = 0; dir < 4; dir++)
			{
				int nx = x + dx[dir];
				int ny = y + dy[dir];

				if (!OOM(nx, ny) && !vis[nx][ny] && board[nx][ny] > 0 && abs(board[nx][ny] - board[x][y]) <= 1)
				{
					vis[nx][ny] = true;
					q.push({ nx, ny });
					groups.back().push_back({ nx, ny });
				}
			}
		}

		ngroup.push_back(cnt);
	}
}

void Move()
{
	for (auto& g : groups)
	{
		vector<pii> nxt;
		int len = g.size();

		nxt.push_back(g[len - 1]);
		for (int i = 1; i < len; i++)
		{
			auto [x1, y1] = g[i - 1];
			auto [x2, y2] = g[i];

			nxt.push_back(g[i - 1]);
			swap(board[x1][y1], board[x2][y2]);
		}

		g = nxt;
	}
}

pii GetAttakPoint()
{
	if (t == N)
	{
		t = 0;
		tDir = (tDir + 1) % 4;
	}

	if (tDir == 0) return make_pair(t, 0);
	if (tDir == 1) return make_pair(N - 1, t);
	if (tDir == 2) return make_pair(N - 1 - t, N - 1);
	if (tDir == 3) return make_pair(0, N - 1 - t);
}

tiii Target(pii pos) 
{
	int gnum = -1, rear = -1, order = -1;

	for (int i = 0; i < groups.size(); i++)
		for (int j = 0; j < groups[i].size(); j++) {
			pii cmp = groups[i][j];

			if (cmp == pos) {
				gnum = i;
				order = j;
			}
			if (gnum != -1 && board[cmp.X][cmp.Y] == 3)
				rear = j;
			if (gnum != -1 && rear != -1)
				return make_tuple(gnum, rear, order + 1);
		}
}

void Attack(int x, int y) 
{

	while (!OOM(x, y)) {
		int val = board[x][y];

		if (val > 0 && val < 4) {
			auto [gnum, ridx, order] = Target({ x,y });
			pii& front = groups[gnum][0];
			pii& rear = groups[gnum][ridx];

			swap(board[front.X][front.Y], board[rear.X][rear.Y]);
			reverse(groups[gnum].begin(), groups[gnum].begin() + ngroup[gnum]);
			reverse(groups[gnum].begin() + ngroup[gnum], groups[gnum].end());

			score += order * order;
			return;
		}

		x += dx[tDir];
		y += dy[tDir];
	}
}

int main(void)
{
	FAST_IO();
	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> K;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) 
		{
			cin >> board[i][j];
			if (board[i][j] == 1) head.push_back({ i, j });
		}
			
	MakeGroup();

	while (K--)
	{
		Move();
		auto [sx, sy] = GetAttakPoint();
		Attack(sx, sy);
		t++;
	}

	cout << score;
	return 0;
}