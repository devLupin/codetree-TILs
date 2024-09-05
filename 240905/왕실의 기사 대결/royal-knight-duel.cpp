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

const int MAX_L = 45;
const int MAX_N = 35;
typedef enum { EMT, PIT, WAL };
int L, N, Q, board[MAX_L][MAX_L], kBoard[MAX_L][MAX_L], ans;
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };
bool out[MAX_N];
typedef struct Knight { int r, c, h, w, k; };
vector<Knight> knight;

bool OOM(int x, int y) { return x < 1 || y < 1 || x > L || y > L; }

set<int> Bfs(int n, int dir)
{
	set<int> nodes;
	queue<pii> q;
	bool vis[MAX_L][MAX_L] = { false, };

	auto [r, c, h, w, k] = knight[n];

	for (int i = r; i < r + h; i++)
		for (int j = c; j < c + w; j++)
		{
			q.push({ i, j });
			vis[i][j] = true;
		}

	while (!q.empty())
	{
		pii cur = q.front();
		q.pop();

		nodes.insert(kBoard[cur.X][cur.Y]);

		int nx = cur.X + dx[dir];
		int ny = cur.Y + dy[dir];

		if (OOM(nx, ny) || vis[nx][ny]) continue;
		if (kBoard[nx][ny] > 0)
		{
			q.push({ nx, ny });
			vis[nx][ny] = true;
		}
	}

	return nodes;
}

bool Check(set<int> nodes, int dir, int target)
{
	vector<int> damage(N + 1);

	for (const int& node : nodes)
	{
		auto [r, c, h, w, k] = knight[node];

		for (int i = r; i < r + h; i++)
			for (int j = c; j < c + w; j++) {
				int nx = i + dx[dir];
				int ny = j + dy[dir];

				if (OOM(nx, ny) || board[nx][ny] == WAL)
					return {};

				if (board[nx][ny] == PIT)
					damage[node]++;
			}
	}

	for (int n = 1; n <= N; n++)
	{
		if (n == target) continue;

		auto& [r, c, h, w, k] = knight[n];
		
		k -= damage[n];

		if (k <= 0)
		{
			out[n] = true;

			for (int i = r; i < r + h; i++)
				for (int j = c; j < c + w; j++)
					kBoard[i][j] = 0;
		}
		else
			ans += min(k, damage[n]);
	}

	return true;
}

void Move(set<int> nodes, int dir)
{
	for (const int& node : nodes)
	{
		if (out[node]) continue;

		auto& [r, c, h, w, k] = knight[node];

		for (int i = r; i < r + h; i++)
			for (int j = c; j < c + w; j++)
				kBoard[i][j] = 0;

		r += dx[dir];
		c += dy[dir];
	}

	for (int n = 1; n <= N; n++)
	{
		if (out[n]) continue;

		auto& [r, c, h, w, k] = knight[n];

		for (int i = r; i < r + h; i++)
			for (int j = c; j < c + w; j++)
				kBoard[i][j] = n;
	}
}

int main(void) {
	FAST_IO();
	// freopen("input.txt", "r", stdin);

	cin >> L >> N >> Q;

	for (int i = 1; i <= L; i++)
		for (int j = 1; j <= L; j++)
			cin >> board[i][j];

	knight.assign(N + 1, {});
	for (int r, c, h, w, k, n = 1; n <= N; n++)
	{
		cin >> r >> c >> h >> w >> k;
		knight[n] = { r, c, h, w, k };

		for (int i = r; i < r + h; i++)
			for (int j = c; j < c + w; j++)
				kBoard[i][j] = n;
	}

	for (int i, d; Q--;)
	{
		cin >> i >> d;
		auto nodes = Bfs(i, d);
		if (Check(nodes, d, i))
			Move(nodes, d);
	}

	cout << ans;
	return 0;
}