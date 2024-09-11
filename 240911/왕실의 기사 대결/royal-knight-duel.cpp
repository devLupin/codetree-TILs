/**
 * @author lupin
 * @date 2024-09-07
 *
 * @submit_time 00:45:15
 * @revision1 00:48:15
 * @revision2 2024-09-11 00:29:55
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

const int MAX_L = 45;
const int MAX_N = 35;
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

int L, N, Q;
int chess[MAX_L][MAX_L], board[MAX_L][MAX_L];
bool vis[MAX_L][MAX_L];
bool die[MAX_N];

struct Info { int r, c, h, w, k; };
vector<Info> knights;

bool OOM(int x, int y) { return x < 1 || y < 1 || x > L || y > L; }

set<int> Search(int n, int dir)
{
	set<int> st;
	queue<pii> q;
	
	fill(&vis[1][1], &vis[L + 1][L + 1], false);

	auto [r, c, h, w, k] = knights[n];

	for (int i = r; i < r + h; i++)
		for (int j = c; j < c + w; j++)
		{
			q.push({ i, j });
			vis[i][j] = true;
		}

	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();

		st.insert(board[x][y]);

		int nx = x + dx[dir];
		int ny = y + dy[dir];

		if (OOM(nx, ny) || chess[nx][ny] == 2) return {};

		if (!vis[nx][ny] && board[nx][ny] > 0)
		{
			q.push({ nx, ny });
			vis[nx][ny] = true;
		}

		for (int ddir = 0; ddir < 4; ddir++)
		{
			nx = x + dx[ddir];
			ny = y + dy[ddir];

			if (!OOM(nx, ny) && !vis[nx][ny] && board[nx][ny] == board[x][y])
			{
				q.push({ nx, ny });
				vis[nx][ny] = true;
			}
		}
	}

	return st;
}

void Move(set<int> st, int dir)
{
	for (int n : st)
	{
		auto& [r, c, h, w, k] = knights[n];

		for (int i = r; i < r + h; i++)
			for (int j = c; j < c + w; j++)
				board[i][j] = 0;

		r += dx[dir];
		c += dy[dir];
	}

	for (int n : st)
	{
		auto [r, c, h, w, k] = knights[n];

		for (int i = r; i < r + h; i++)
			for (int j = c; j < c + w; j++)
				board[i][j] = n;
	}
}

int Calc(set<int> st, int target)
{
	int ret = 0;

	for (int n : st)
	{
		if (n == target) continue;

		int cur = 0;
		auto& [r, c, h, w, k] = knights[n];
		
		for (int i = r; i < r + h; i++)
			for (int j = c; j < c + w; j++)
				cur += chess[i][j];

		k -= cur;

		if (k <= 0)
		{
			for (int i = r; i < r + h; i++)
				for (int j = c; j < c + w; j++)
					board[i][j] = 0;
		
			die[n] = true;
			r = c = h = w = k = 0;
		}
		else
			ret += cur;
	}

	return ret;
}

void Print()
{
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << "\n\n";
}

int main(void)
{
	// freopen("input.txt", "r", stdin);

	cin >> L >> N >> Q;
	for (int i = 1; i <= L; i++)
		for (int j = 1; j <= L; j++)
			cin >> chess[i][j];

	knights.assign(N + 1, {});

	for (int r, c, h, w, k, n = 1; n <= N; n++)
	{
		cin >> r >> c >> h >> w >> k;
		knights[n] = { r, c, h, w, k };

		for (int i = r; i < r + h; i++)
			for (int j = c; j < c + w; j++)
				board[i][j] = n;
	}

	int ans = 0;

	for (int n, dir; Q--;)
	{
		cin >> n >> dir;
		if (die[n]) continue;

		set<int> st = Search(n, dir);
		if (!st.empty())
		{
			Move(st, dir);
			ans += Calc(st, n);
		}

		// Print();
	}
	cout << ans;

	return 0;
}