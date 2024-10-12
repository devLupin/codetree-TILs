/**
 * @author         lupin
 * @date           2024-10-12
 *
 * @submit         00:54:28
 * @revision       00:05:32
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#pragma warning(suppress : 6031)
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int dx[] = { 0,1,0,-1,-1,-1,1,1 };
const int dy[] = { 1,0,-1,0,-1,1,1,-1 };
const int MAX_N = 15;

int N, M, K, board[MAX_N][MAX_N];
int turn[MAX_N][MAX_N];
bool isAttack[MAX_N][MAX_N];
bool vis[MAX_N][MAX_N];

pii weak, strong;
int backX[MAX_N][MAX_N], backY[MAX_N][MAX_N];

struct turlet { int att, t, x, y; };

void Print()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
			cout << max(0, board[i][j]) << ' ';
		cout << "\n";
	}
	cout << "\n\n";
}

bool Compare(const turlet& a, const turlet& b)
{
	if (a.att != b.att) return a.att < b.att;
	if (a.t != b.t) return a.t > b.t;
	if (a.x + a.y != b.x + b.y) return a.x + a.y > b.x + b.y;
	return a.y > b.y;
}

bool Select(int t)
{
	vector<turlet> v;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			if (board[i][j] <= 0) continue;
			v.push_back({ board[i][j], turn[i][j], i, j });
		}

	sort(v.begin(), v.end(), Compare);

	if (v.size() < 2) return false;

	weak = make_pair(v[0].x, v[0].y);
	strong = make_pair(v.back().x, v.back().y);

	board[weak.X][weak.Y] += N + M;
	turn[weak.X][weak.Y] = t;

	return true;
}

pii Adjust(int x, int y)
{
	if (x == N) x = 0;
	else if (x == -1) x = N - 1;
	if (y == N) y = 0;
	else if (y == -1) y = M - 1;

	return make_pair(x, y);
}

bool CanLazerAttack()
{
	queue<pii> q;
	q.push(make_pair(weak.X, weak.Y));
	vis[weak.X][weak.Y] = true;

	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();

		if (make_pair(x, y) == strong)
			return true;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];
			tie(nx, ny) = Adjust(nx, ny);

			if (!vis[nx][ny] && board[nx][ny] > 0)
			{
				q.push(make_pair(nx, ny));
				vis[nx][ny] = true;
				backX[nx][ny] = x;
				backY[nx][ny] = y;
			}
		}
	}

	return false;
}

void BombAttack(int p)
{
	const auto [x, y] = strong;

	for (int dir = 0; dir < 8; dir++)
	{
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		tie(nx, ny) = Adjust(nx, ny);

		if (board[nx][ny] > 0 && make_pair(nx, ny) != weak)
		{
			board[nx][ny] -= p;
			isAttack[nx][ny] = true;
		}
	}
}

void Attack()
{
	int p = board[weak.X][weak.Y] / 2;

	isAttack[weak.X][weak.Y] = true;
	isAttack[strong.X][strong.Y] = true;

	if (CanLazerAttack())
	{
		int x = backX[strong.X][strong.Y];
		int y = backY[strong.X][strong.Y];

		while (make_pair(x, y) != weak)
		{
			board[x][y] -= p;
			isAttack[x][y] = true;

			int nx = backX[x][y];
			int ny = backY[x][y];
			tie(x, y) = make_pair(nx, ny);
		}
	}
	else BombAttack(p);

	board[strong.X][strong.Y] -= board[weak.X][weak.Y];
}

void Done()
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			if (!isAttack[i][j] && board[i][j] > 0)
				board[i][j]++;

	fill(&backX[0][0], &backX[N][M], 0);
	fill(&backY[0][0], &backY[N][M], 0);
	fill(&vis[0][0], &vis[N][M], false);
	fill(&isAttack[0][0], &isAttack[N][M], false);
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> N >> M >> K;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			cin >> board[i][j];

	for (int i = 1; i <= K; i++)
	{
		if (!Select(i)) break;
		Attack();
		Done();
	}

	int ans = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			ans = max(ans, board[i][j]);

	cout << ans;
	return 0;
}