/**
 * @author lupin
 *
 * @submit 2024-09-07 01:05:00
 */

#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiiii = tuple<int, int, int, int>;

const int dx[] = { 0,1,0,-1,-1,-1,1,1 };
const int dy[] = { 1,0,-1,0,-1,1,-1,1 };

const int MAX_SZ = 15;
int N, M, K, board[MAX_SZ][MAX_SZ];
int recentAtk[MAX_SZ][MAX_SZ];
bool vis[MAX_SZ][MAX_SZ], isAttack[MAX_SZ][MAX_SZ];
int backX[MAX_SZ][MAX_SZ], backY[MAX_SZ][MAX_SZ];

struct pos { int x, y, atk; };

bool compare(const pos& a, const pos& b)
{
	if (board[a.x][a.y] != board[b.x][b.y]) return board[a.x][a.y] < board[b.x][b.y];
	if (recentAtk[a.x][a.y] != recentAtk[b.x][b.y]) return recentAtk[a.x][a.y] > recentAtk[b.x][b.y];
	if (a.x + a.y != b.x + b.y) return a.x + a.y > b.x + b.y;
	return a.y > b.y;
}

pair<pos, pos> Search(const int& k)
{
	pair<pos, pos> ret;
	vector<pos> v;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			if (board[i][j] > 0)
				v.push_back({ i, j, recentAtk[i][j] });

	sort(v.begin(), v.end(), compare);

	pos weaker = { -1,-1,0 };
	pos stronger = { -1,-1,0 };

	if (v.size() >= 2)
	{
		weaker = v[0];
		stronger = v[v.size() - 1];

		board[weaker.x][weaker.y] += N + M;
		recentAtk[weaker.x][weaker.y] = k;
	}

	return make_pair(weaker, stronger);
}

pii Adjust(const int& x, const int& y)
{
	pii ret = { x, y };
	
	if (x < 0) ret.X = N - 1;
	else if (x >= N) ret.X = 0;
	if (y < 0) ret.Y = M - 1;
	else if (y >= M) ret.Y = 0;

	return ret;
}

bool CanLazerAttack(const pos& src, const pos& dst)
{
	queue<pii> q;

	q.push({ src.x, src.y });
	vis[src.x][src.y] = true;

	while (!q.empty())
	{
		pii cur = q.front();
		q.pop();

		if (cur == make_pair(dst.x, dst.y))
			return true;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = cur.X + dx[dir];
			int ny = cur.Y + dy[dir];

			tie(nx, ny) = Adjust(nx, ny);

			if (!vis[nx][ny] && board[nx][ny] > 0)
			{
				backX[nx][ny] = cur.X;
				backY[nx][ny] = cur.Y;
				q.push({ nx, ny });
				vis[nx][ny] = true;
			}
		}
	}

	return false;
}

void LazerAttack(const pos& src, const pos& dst)
{
	int x = backX[dst.x][dst.y];
	int y = backY[dst.x][dst.y];
	int s = board[src.x][src.y] / 2;

	while (make_pair(x, y) != make_pair(src.x, src.y))
	{
		board[x][y] -= s;
		board[x][y] = max(0, board[x][y]);
		isAttack[x][y] = true;

		int nx = backX[x][y];
		int ny = backY[x][y];
		tie(x, y) = { nx, ny };
	}

	board[dst.x][dst.y] -= board[src.x][src.y];
	board[dst.x][dst.y] = max(0, board[dst.x][dst.y]);
}

void BombAttack(const pos& src, const pos& dst)
{
	int x = dst.x;
	int y = dst.y;
	int s = board[src.x][src.y] / 2;

	for(int dir=0; dir<8; dir++)
	{
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		tie(nx, ny) = Adjust(nx, ny);

		if (make_pair(src.x, src.y) == make_pair(nx, ny)) continue;
		
		board[nx][ny] -= s;
		board[nx][ny] = max(0, board[nx][ny]);
		isAttack[nx][ny] = true;
	}

	board[dst.x][dst.y] -= board[src.x][src.y];
	board[dst.x][dst.y] = max(0, board[dst.x][dst.y]);
}

void Done(const pos& src, const pos& dst)
{
	isAttack[src.x][src.y] = true;
	isAttack[dst.x][dst.y] = true;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			if (!isAttack[i][j] && board[i][j] > 0)
				board[i][j]++;

	fill(&vis[0][0], &vis[N][M], false);
	fill(&isAttack[0][0], &isAttack[N][M], false);
	fill(&backX[0][0], &backX[N][M], 0);
	fill(&backY[0][0], &backY[N][M], 0);
}

void Print()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << "\n\n";
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	// freopen("input.txt", "r", stdin);

	cin >> N >> M >> K;

	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			cin >> board[i][j];

	for (int i = 1; i <= K; i++)
	{
		auto [weaker, stronger] = Search(i);
		if (weaker.x == -1) break;

		if (CanLazerAttack(weaker, stronger))
			LazerAttack(weaker, stronger);
		else BombAttack(weaker, stronger);

		Done(weaker, stronger);

	}
	
	int ans = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			ans = max(ans, board[i][j]);
	
	cout << ans;
	return 0;
}