/**
 * @author         lupin
 * @date           2024-09-28
 *
 * @submit         01:32:55
 */


#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int dx[] = { 0,0,1,-1 };
const int dy[] = { -1,1,0,0 };

const int MAX_N = 100;
const int MAX_M = MAX_N * MAX_N;

int N, M, H, K, ans;
bool tree[MAX_N][MAX_N];
int tx, ty;
bool taggerBackward;
pii runnerPos[MAX_M];
int runnerDir[MAX_M];
bool isCatched[MAX_M];

const int tfdx[] = { -1,0,1,0 };
const int tfdy[] = { 0,1,0,-1 };
const int tbdx[] = { 1,0,-1,0 };
const int tbdy[] = { 0,1,0,-1 };
int forwardDir[MAX_N][MAX_N];
bool forwardDirChanged[MAX_N][MAX_N];
int backwardDir[MAX_N][MAX_N];
bool backwardDirChanged[MAX_N][MAX_N];

int distance(int x, int y) { return abs(tx - x) + abs(ty - y); }

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void Init()
{
	int cnt = 0, cCnt = 1, cChange = 0, dir = 0;
	int x = N / 2, y = N / 2;

	while (make_pair(x, y) != make_pair(0, 0))
	{
		forwardDir[x][y] = dir;
		x += tfdx[dir];
		y += tfdy[dir];

		cnt++;

		if (cnt == cCnt)
		{
			forwardDirChanged[x][y] = true;

			dir = (dir + 1) % 4;
			cnt = 0;
			cChange++;

			if (cChange == 2)
			{
				cChange = 0;
				cCnt++;
			}
		}
	}

	cnt = 0, cCnt = N - 1, cChange = 0, dir = 0;
	x = 0, y = 0;

	while (make_pair(x, y) != make_pair(N / 2, N / 2))
	{
		backwardDir[x][y] = dir;
		x += tbdx[dir];
		y += tbdy[dir];

		cnt++;
		if (cnt == cCnt)
		{
			backwardDirChanged[x][y] = true;

			dir = (dir + 1) % 4;
			cnt = 0;

			if (x == N - 1 && y == 0) continue;
			cChange++;

			if (cChange == 2)
			{
				cChange = 0;
				cCnt--;
			}
		}
	}

	forwardDir[N / 2][N / 2] = 0;
	backwardDir[0][0] = 0;

	tx = N / 2;
	ty = N / 2;
}

void MoveRunner()
{
	for (int i = 1; i <= M; i++)
	{
		if (isCatched[i]) continue;

		auto& [x, y] = runnerPos[i];
		int& dir = runnerDir[i];

		if (distance(x, y) > 3) continue;

		int nx = x + dx[dir];
		int ny = y + dy[dir];

		if (OOM(nx, ny))
		{
			if (dir == 0) dir = 1;
			else if (dir == 1) dir = 0;
			else if (dir == 2) dir = 3;
			else if (dir == 3) dir = 2;
			i--;
			continue;
		}

		if (make_pair(nx, ny) == make_pair(tx, ty)) continue;

		x = nx;
		y = ny;
	}
}

pii MoveTagger()
{
	int dir;

	if (taggerBackward)
	{
		dir = backwardDir[tx][ty];
		tx += tbdx[dir];
		ty += tbdy[dir];

		if (backwardDirChanged[tx][ty]) dir = (dir + 1) % 4;
	}
	else
	{
		dir = forwardDir[tx][ty];
		tx += tfdx[dir];
		ty += tfdy[dir];

		if (forwardDirChanged[tx][ty]) dir = (dir + 1) % 4;
	}

	if (make_pair(tx, ty) == make_pair(0, 0) || make_pair(tx, ty) == make_pair(N / 2, N / 2))
		taggerBackward = !taggerBackward;

	int ddx, ddy;

	if (taggerBackward)
	{
		ddx = tbdx[dir];
		ddy = tbdy[dir];
	}
	else
	{
		ddx = tfdx[dir];
		ddy = tfdy[dir];
	}

	return make_pair(ddx, ddy);
}

void CatchRunner(int turn, int ddx, int ddy)
{
	auto [x, y] = make_pair(tx, ty);
	int cnt = 3;
	int ansCnt = 0;

	while (!OOM(x, y) && cnt--)
	{
		if (!tree[x][y])
		{
			for (int i = 1; i <= M; i++)
			{
				auto& [rx, ry] = runnerPos[i];
				if (isCatched[i] || make_pair(rx, ry) != make_pair(x, y)) continue;

				ans += turn;
				isCatched[i] = true;
				tie(rx, ry) = make_pair(-1, -1);
			}
		}

		x += ddx;
		y += ddy;
	}
}

bool Run()
{
	for (int i = 1; i <= M; i++)
		if (!isCatched[i]) return true;
	return false;
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> N >> M >> H >> K;

	for (int x, y, d, i = 1; i <= M; i++)
	{
		cin >> x >> y >> d;
		x--, y--;
		runnerPos[i] = make_pair(x, y);
		runnerDir[i] = d;
	}

	for (int x, y; H--;)
	{
		cin >> x >> y;
		x--, y--;
		tree[x][y] = true;
	}

	Init();

	for (int turn = 1; turn <= K && Run(); turn++)
	{
		MoveRunner();
		auto [ddx, ddy] = MoveTagger();
		CatchRunner(turn, ddx, ddy);
	}

	cout << ans;
	return 0;
}