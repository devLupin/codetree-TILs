/**
 * @author         lupin
 * @date           2025-02-17
 *
 * @submit         00:48:06
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>

#define X		first
#define Y		second

using namespace std;
using pii = pair<int, int>;

const int dx[] = { 0,1,0,-1,-1,-1,1,1 };
const int dy[] = { 1,0,-1,0,-1,1,1,-1 };
const int MAX_SZ = 15;

int N, M, K, board[MAX_SZ][MAX_SZ], turn[MAX_SZ][MAX_SZ];
int backX[MAX_SZ][MAX_SZ], backY[MAX_SZ][MAX_SZ];
bool vis[MAX_SZ][MAX_SZ], isAttack[MAX_SZ][MAX_SZ];

typedef struct
{
	int x, y, turn, att;
}Info;

void Print()
{
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= M; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << '\n';
}

bool Compare(const Info& a, const Info& b)
{
	if (a.att != b.att) return a.att < b.att;
	if (a.turn != b.turn) return a.turn > b.turn;
	if (a.x + a.y != b.x + b.y) return a.x + a.y > b.x + b.y;
	return a.y > b.y;
}

bool SortInfo(pii& src, pii& dst, int t)
{
	vector<Info> v;

	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= M; j++)
			if(board[i][j] > 0)
				v.push_back({ i, j, turn[i][j], board[i][j] });

	if (v.size() < 3) return false;

	sort(v.begin(), v.end(), Compare);

	src = make_pair(v[0].x, v[0].y);
	dst = make_pair(v.back().x, v.back().y);
	
	board[src.X][src.Y] += N + M;
	turn[src.X][src.Y] = t;

	return true;
}

void AdjustPos(int& x, int& y)
{
	if (x < 1) x = N;
	else if (x > N) x = 1;
	if (y < 1) y = N;
	else if (y > N) y = 1;
}

bool CanLazerAttack(const pii& src, const pii& dst)
{
	queue<pii> q;
	q.push(src);
	vis[src.X][src.Y] = true;

	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();

		if (make_pair(x, y) == dst)
			return true;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];
			AdjustPos(nx, ny);

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

void LazerAttack(const pii& src, const pii& dst)
{
	auto [x, y] = dst;
	int pt = board[src.X][src.Y] / 2;
	isAttack[src.X][src.Y] = true;

	while (make_pair(x, y) != src)
	{
		isAttack[x][y] = true;
		board[x][y] -= pt;
		board[x][y] = max(0, board[x][y]);

		int nx = backX[x][y];
		int ny = backY[x][y];
		tie(x, y) = make_pair(nx, ny);
	}

	board[dst.X][dst.Y] += pt - board[src.X][src.Y];
	board[dst.X][dst.Y] = max(0, board[dst.X][dst.Y]);
}

void BombAttack(const pii& src, const pii& dst)
{
	const auto& [x, y] = dst;
	const int& pt = board[src.X][src.Y] / 2;

	for (int dir = 0; dir < 8; dir++)
	{
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		AdjustPos(nx, ny);

		if (make_pair(nx, ny) == src) continue;
		
		board[nx][ny] -= pt;
		board[nx][ny] = max(0, board[nx][ny]);
		isAttack[nx][ny] = true;
	}

	board[dst.X][dst.Y] -= board[src.X][src.Y];
	board[dst.X][dst.Y] = max(0, board[dst.X][dst.Y]);

	isAttack[src.X][src.Y] = true;
	isAttack[dst.X][dst.Y] = true;
}

void Update()
{
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= M; j++)
			if (!isAttack[i][j] && board[i][j] > 0) 
				board[i][j]++;
}

void Init()
{
	fill(&backX[1][1], &backX[N + 1][M + 1], 0);
	fill(&backY[1][1], &backY[N + 1][M + 1], 0);
	fill(&vis[1][1], &vis[N + 1][M + 1], false);
	fill(&isAttack[1][1], &isAttack[N + 1][M + 1], false);
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);
	cin >> N >> M >> K;

	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= M; j++)
			cin >> board[i][j];

	for (int i = 1; i <= K; i++)
	{
		Init();

		pii src, dst;
		if(!SortInfo(src, dst, i)) break;

		if (CanLazerAttack(src, dst))
			LazerAttack(src, dst);
		else
			BombAttack(src, dst);
		Update();
	}

	cout << *max_element(&board[1][1], &board[N + 1][M + 1]);
	return 0;
}