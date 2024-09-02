#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using vvi = vector<vector<int>>;

const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };

int K, M, fCnt, ans;
vvi board;
vector<int> filler;

void print(vvi cur)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
			cout << cur[i][j] << ' ';
		cout << '\n';
	}
	cout << "\n\n";
}

vvi rotate90(vvi cur, int sx, int sy)
{
	vvi nxt = cur;
	for (int x = sx; x < sx + 3; x++) {
		for (int y = sy; y < sy + 3; y++) {

			int nx = y - sy;
			int ny = 3 - (x - sx) - 1;

			nxt[nx + sx][ny + sy] = cur[x][y];
		}
	}
	return nxt;
}

bool oom(int x, int y) { return x < 0 || y < 0 || x >= 5 || y >= 5; }

vector<pii> bfs(vvi cboard)
{
	vector<pii> res;
	bool vis[5][5] = { false, };

	for (int i = 0; i < 5; i++) 
	{
		for (int j = 0; j < 5; j++) 
		{
			if (vis[i][j]) continue;

			vector<pii> v;
			queue<pii> q;

			q.push({ i,j });
			vis[i][j] = true;
			v.push_back({ i,j });

			while (!q.empty())
			{
				pii cur = q.front();
				q.pop();

				for (int dir = 0; dir < 4; dir++) 
				{
					int nx = cur.X + dx[dir];
					int ny = cur.Y + dy[dir];

					if (!oom(nx, ny) && !vis[nx][ny] && cboard[i][j] == cboard[nx][ny])
					{
						q.push({ nx, ny });
						vis[nx][ny] = true;
						v.push_back({ nx,ny });
					}
				}
			}

			if (v.size() >= 3)
				for (const pii& p : v)
					res.push_back(p);
		}
	}

	return res;
}

vector<pii> UpdateBoard()
{
	vvi best = board;
	auto bRes = vector<pii>();

	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 3; i++)
		{
			for (int k = 1; k <= 3; k++)
			{
				vvi cmp = board;

				for (int l = 0; l < K; l++)
					cmp = rotate90(cmp, i, j);

				auto cRes = bfs(cmp);
				if (cRes.size() > bRes.size())
				{
					bRes = cRes;
					best = cmp;
				}
			}
		}
	}

	board = best;
	return bRes;
}

bool compare(const pii& a, const pii& b)
{
	if (a.Y != b.Y) return a.Y < b.Y;
	return a.X > b.X;
}

void fill(vector<pii> res)
{
	sort(res.begin(), res.end(), compare);

	for (const auto& nxt : res)
	{
		board[nxt.X][nxt.Y] = filler[fCnt++];
		ans++;
	}
}

int main(void) 
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	board.assign(5, vector<int>(5));

	cin >> K >> M;

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
			cin >> board[i][j];
	for (int x, i = 0; i < M; i++) 
	{
		cin >> x;
		filler.push_back(x);
	}

	while (K--)
	{
		auto res = UpdateBoard();
		if (res.empty()) break;

		while (true)
		{
			fill(res);
			res = bfs(board);
			if (res.empty()) break;
		}
	}
	
	cout << ans;
	return 0;
}