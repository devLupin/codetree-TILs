/**
 * @author         lupin
 * @date           2024-10-12
 *
 * @submit         00:27:14
 */


#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAX_N = 25;
const int MAX_M = 35;
// ↑, →, ↓, ←
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

int N, M, K;
vector<int> guns[MAX_N][MAX_N];
int chess[MAX_N][MAX_N];

struct Info { int x, y, d, s, g; };
Info players[MAX_M];
int ans[MAX_M];

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

bool IsWinnerLeft(int t1, int t2)
{
	auto [s1, g1] = make_pair(players[t1].s, players[t1].g);
	auto [s2, g2] = make_pair(players[t2].s, players[t2].g);

	if (s1 + g1 != s2 + g2) return s1 + g1 > s2 + g2;
	return s1 > s2;
}

void MoveLosser(int num)
{
	auto& [x, y, d, s, g] = players[num];

	guns[x][y].push_back(g);
	g = 0;

	for (int i = 0; i < 4; i++)
	{
		int dir = (d + i) % 4;
		int nx = x + dx[dir];
		int ny = y + dy[dir];

		if (!OOM(nx, ny) && chess[nx][ny] == 0)
		{
			d = dir;
			tie(x, y) = make_pair(nx, ny);
			chess[x][y] = num;

			guns[x][y].push_back(0);
			sort(guns[x][y].begin(), guns[x][y].end());
			g = guns[x][y].back();
			guns[x][y].pop_back();

			break;
		}
	}
}

void MoveWinner(int num)
{
	auto& [x, y, d, s, g] = players[num];

	chess[x][y] = num;
	guns[x][y].push_back(g);
	g = 0;

	sort(guns[x][y].begin(), guns[x][y].end());

	g = guns[x][y].back();
	guns[x][y].pop_back();
}

void Fight(int t1, int t2)
{
	int diff = abs((players[t1].s + players[t1].g) - (players[t2].s + players[t2].g));
	int winner, losser;
	
	if (IsWinnerLeft(t1, t2))
	{
		tie(winner, losser) = make_pair(t1, t2);
		ans[t1] += diff;
	}
	else
	{
		tie(winner, losser) = make_pair(t2, t1);
		ans[t2] += diff;
	}

	MoveLosser(losser);
	MoveWinner(winner);
}

void Move()
{
	for (int i = 1; i <= M; i++)
	{
		auto& [x, y, d, s, g] = players[i];

		int nx = x + dx[d];
		int ny = y + dy[d];

		if (OOM(nx, ny))
		{
			d = (d + 2) % 4;
			i--;
			continue;
		}

		if (chess[nx][ny] == 0)
		{
			guns[nx][ny].push_back(g);
			g = 0;

			sort(guns[nx][ny].begin(), guns[nx][ny].end());
			g = guns[nx][ny].back();
			guns[nx][ny].pop_back();

			swap(chess[x][y], chess[nx][ny]);
			tie(x, y) = make_pair(nx, ny);
		}

		else
		{
			int p1 = i;
			int p2 = chess[nx][ny];

			chess[x][y] = 0;
			chess[nx][ny] = 0;
			tie(x, y) = make_pair(nx, ny);
			Fight(p1, p2);
		}
	}
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> K;
	
	for(int i=0; i<N; i++)
		for (int j = 0; j < N; j++)
		{
			int g; cin >> g;
			guns[i][j].push_back(g);
		}

	for (int x, y, d, s, i = 1; i <= M; i++)
	{
		cin >> x >> y >> d >> s;
		x--, y--;
		players[i] = { x, y, d, s, 0 };
		chess[x][y] = i;
	}

	while (K--)
		Move();

	for (int i = 1; i <= M; i++)
		cout << ans[i] << ' ';

	return 0;
}