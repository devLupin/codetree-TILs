/**
 * @author lupin
 *
 * @submit 2024-09-17 00:34:09
 */

#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int MAX_N = 25;
const int MAX_M = MAX_N * MAX_N;

const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

int N, M, K, ans[MAX_M];
vector<int> board[MAX_N][MAX_N];
int kBoard[MAX_N][MAX_N];

struct Info { int x, y, d, s, g; };
Info knights[MAX_M];

void Print()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << kBoard[i][j] << ' ';
		cout << '\n';
	}
	cout << "\n\n";
}

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

tuple<int, int, int> WhoWinner(int p1, int p2)
{
	int s1, g1, s2, g2, diff;
	s1 = knights[p1].s;
	g1 = knights[p1].g;
	s2 = knights[p2].s;
	g2 = knights[p2].g;
	diff = abs((s1 + g1) - (s2 + g2));

	int winner, losser;

	if (s1 + g1 > s2 + g2)
	{
		winner = p1;
		losser = p2;
	}
	else if (s1 + g1 < s2 + g2)
	{
		winner = p2;
		losser = p1;
	}
	else if (s1 > s2)
	{
		winner = p1;
		losser = p2;
	}
	else
	{
		winner = p2;
		losser = p1;
	}

	return make_tuple(winner, losser, diff);
}

void MoveLosser(int losser)
{
	auto& [x, y, d, s, g] = knights[losser];
	board[x][y].push_back(g);

	for (int i = 0; i < 4; i++)
	{
		int nd = (d + i) % 4;
		int nx = x + dx[nd];
		int ny = y + dy[nd];

		if (!OOM(nx, ny) && kBoard[nx][ny] == 0)
		{
			d = nd;
			tie(x, y) = make_pair(nx, ny);
			sort(board[x][y].begin(), board[x][y].end());

			if (board[x][y].empty()) g = 0;
			else
			{
				g = board[x][y].back();
				board[x][y].pop_back();
			}

			kBoard[x][y] = losser;
			break;
		}
	}
}

void MoveWinner(int winner)
{
	auto& [x, y, d, s, g] = knights[winner];
	board[x][y].push_back(g);

	sort(board[x][y].begin(), board[x][y].end());
	g = board[x][y].back();
	board[x][y].pop_back();

	kBoard[x][y] = winner;
}

void Fight(int p1, int p2)
{
	auto [winner, losser, diff] = WhoWinner(p1, p2);
	MoveLosser(losser);
	MoveWinner(winner);
	ans[winner] += diff;
}

void Move()
{
	for (int i = 1; i <= M; i++)
	{
		auto& [x, y, d, s, g] = knights[i];

		int nx = x + dx[d];
		int ny = y + dy[d];

		if (OOM(nx, ny))
		{
			d = (d + 2) % 4;
			nx = x + dx[d];
			ny = y + dy[d];
		}

		if (kBoard[nx][ny] > 0)
		{
			int cmp = kBoard[nx][ny];
			kBoard[x][y] = kBoard[nx][ny] = 0;
			tie(x, y) = make_pair(nx, ny);
			Fight(i, cmp);
		}
		else
		{
			swap(kBoard[x][y], kBoard[nx][ny]);
			tie(x, y) = make_pair(nx, ny);
			
			board[x][y].push_back(g);
			sort(board[x][y].begin(), board[x][y].end());

			g = board[x][y].back();
			board[x][y].pop_back();
		}
	}
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> K;
	
	for (int i = 0; i < N; i++)
		for (int x, j = 0; j < N; j++)
		{
			cin >> x;
			board[i][j].push_back(x);
		}
	
	for (int i = 1; i <= M; i++)
	{
		int x, y, d, s;
		cin >> x >> y >> d >> s;
		x--, y--;
		knights[i] = { x, y, d, s, 0 };
		kBoard[x][y] = i;
	}

	while (K--) Move();

	for (int i = 1; i <= M; i++)
		cout << ans[i] << ' ';
	
	return 0;
}