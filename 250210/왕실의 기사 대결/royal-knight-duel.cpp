/**
 * @author         lupin
 * @date           2025-02-10
 *
 * @submit         00:59:48
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>

#define X		first
#define Y		second

using namespace std;
using pii = pair<int, int>;

enum {SPACE, PIT, WALL};

const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

const int MAX_L = 45;
const int MAX_N = 35;
int r, c, h, w, k, L, N, Q;

struct _info
{
	int r, c, h, w;
}Info[MAX_N];
int hp[MAX_N];

int ans, knight[MAX_L][MAX_L], board[MAX_L][MAX_L];

void Print()
{
	for (int i = 1; i <= L; i++)
	{
		for (int j = 1; j <= L; j++)
			cout << knight[i][j] << ' ';
		cout << '\n';
	}
	cout << "\n\n";
}

void DrawKnight()
{
	for (int i = 1; i <= N; i++)
	{
		if (hp[i] <= 0) continue;

		const auto& [r, c, h, w] = Info[i];

		for (int x = r; x < r + h; x++)
			for (int y = c; y < c + w; y++)
				knight[x][y] = i;
	}
}

bool OOM(int x, int y) { return x < 1 || y < 1 || x > L || y > L; }

set<int> CanPushKnight(int num, int dir)
{
	set<int> ret;
	queue<pii> q;
	bool vis[MAX_L][MAX_L] = { false, };
	const auto& [r, c, h, w] = Info[num];

	for (int x = r; x < r + h; x++)
		for (int y = c; y < c + w; y++)
		{
			q.push({ x, y });
			vis[x][y] = true;
		}

	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();

		ret.insert(knight[x][y]);
		if (board[x][y] == WALL) return {};
		if (knight[x][y] == 0) continue;

		for (int dd = 0; dd < 4; dd++)
		{
			int nx = x + dx[dd];
			int ny = y + dy[dd];

			if (OOM(nx, ny) || vis[nx][ny]) continue;
			if (knight[nx][ny] > 0 && knight[nx][ny] == knight[x][y])
			{
				q.push({ nx, ny });
				vis[nx][ny] = true;
			}
		}

		int nx = x + dx[dir];
		int ny = y + dy[dir];
		if (!OOM(nx, ny) && !vis[nx][ny])
		{
			q.push({ nx, ny });
			vis[nx][ny] = true;
		}
	}

	return ret;
}

void PushKnight(set<int> st, int dir)
{
	if (st.empty()) return;

	for (const int& nxt : st)
	{
		auto& [r, c, h, w] = Info[nxt];
		r += dx[dir];
		c += dy[dir];
	}
}

void CalcDamage(set<int> st, int num)
{
	if (st.empty()) return;

	for (const int& nxt : st)
	{
		if (nxt == num) continue;
		int temp = hp[nxt];
		const auto& [r, c, h, w] = Info[nxt];
		
		for (int x = r; x < r + h; x++)
			for (int y = c; y < c + w; y++)
				if (board[x][y] == PIT)
				{
					hp[nxt]--;
					if (hp[nxt] >= 0) ans++;
				}
		if (hp[nxt] <= 0) ans -= temp;
	}
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	cin >> L >> N >> Q;
	
	for (int i = 1; i <= L; i++)
		for (int j = 1; j <= L; j++)
			cin >> board[i][j];

	for (int i = 1; i <= N; i++)
	{
		cin >> r >> c >> h >> w >> k;
		Info[i] = { r,c,h,w };
		hp[i] = k;
	}

	while (Q--)
	{
		int i, d;
		cin >> i >> d;
		fill(&knight[1][1], &knight[L + 1][L + 1], 0);

		DrawKnight();
		auto ret = CanPushKnight(i, d);
		PushKnight(ret, d);
		CalcDamage(ret, i);
	}

	cout << ans;
	return 0;
}