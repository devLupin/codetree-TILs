/**
 * @author lupin
 * @date 2024-09-07
 * @submit_time 00:45:15
 * @revision1 00:48:15
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

enum { EMPTY, PIT, WALL };

const int MAX_L = 45;
const int MAX_N = 35;
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

int L, N, Q, chess[MAX_L][MAX_L], ans;
int board[MAX_L][MAX_L];
bool out[MAX_N];

struct Info { int r, c, h, w, k; };
vector<Info> knights;

bool OOM(int x, int y) { return x < 1 || y < 1 || x > L || y > L; }

set<int> CanGo(int n, int dir)
{
	set<int> node;
	queue<pii> q;
	bool vis[MAX_L][MAX_L] = { false, };

	auto [r, c, h, w, k] = knights[n];
	for (int i = r; i < r + h; i++)
		for (int j = c; j < c + w; j++)
		{
			q.push({ i, j });
			vis[i][j] = true;
		}

	while (!q.empty())
	{
		pii cur = q.front();
		node.insert(board[cur.X][cur.Y]);
		q.pop();

		int nx = cur.X + dx[dir];
		int ny = cur.Y + dy[dir];

		if (!OOM(nx, ny) && !vis[nx][ny] && board[nx][ny] > 0)
		{
			q.push({ nx, ny });
			vis[nx][ny] = true;
		}
	}

	return node;
}

bool Check(set<int> node, int dir)
{
	for (int n : node)
	{
		auto [r, c, h, w, k] = knights[n];

		for (int i = r; i < r + h; i++)
			for (int j = c; j < c + w; j++)
			{
				int nx = i + dx[dir];
				int ny = j + dy[dir];

				if (OOM(nx, ny) || chess[nx][ny] == WALL)
					return false;
			}
	}

	return true;
}

void Move(set<int> node, int dir, int target)
{
	vector<int> tmpHP(N+1, 0);

	for (int n : node)
	{
		auto& [r, c, h, w, k] = knights[n];

		for (int i = r; i < r + h; i++)
			for (int j = c; j < c + w; j++)
				board[i][j] = 0;
	}

	for (int n : node)
	{
		auto& [r, c, h, w, k] = knights[n];

		for (int i = r; i < r + h; i++)
			for (int j = c; j < c + w; j++)
			{
				int nx = i + dx[dir];
				int ny = j + dy[dir];

				board[nx][ny] = n;
				tmpHP[n] += chess[nx][ny];
			}

		r += dx[dir];
		c += dy[dir];
	}

	for (int n : node)
	{
		if (out[n] || target == n) continue;
		auto& [r, c, h, w, k] = knights[n];
		int diff = k - tmpHP[n];

		if (diff > 0)
		{
			ans += tmpHP[n];
			k = diff;
		}
		else
		{
			out[n] = true;
			for (int i = r; i < r + h; i++)
				for (int j = c; j < c + w; j++)
					board[i][j] = 0;
		}
	}
}

void Print()
{
	for (int i = 1; i <= L; i++) 
	{
		for (int j = 1; j <= L; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << "\n\n";
}

int main(void) {
	FAST_IO();
	// freopen("input.txt", "r", stdin);

	cin >> L >> N >> Q;
	for (int i = 1; i <= L; i++)
		for (int j = 1; j <= L; j++)
			cin >> chess[i][j];

	knights.assign(N + 1, {});

	for (int r, c, h, w, k, n = 1; n <= N; n++)
	{
		cin >> r >> c >> h >> w >> k;
		knights[n] = { r,c,h,w,k };

		for (int i = r; i < r + h; i++)
			for (int j = c; j < c + w; j++)
				board[i][j] = n;
	}

	for (int n, dir; Q--;)
	{
		cin >> n >> dir;
		if (out[n]) continue;

		auto node = CanGo(n, dir);
		if (Check(node, dir)) 
		{
			Move(node, dir, n);
			// Print();
		}
	}

	cout << ans;
	return 0;
}