/**
 * @author         lupin
 * @date           2024-10-05
 *
 * @submit         00:34:03
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#pragma warning(suppress : 6031)
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;


const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

enum {TOP, RIGHT, DOWN, LEFT};

const int MAX_N = 20;
int N, M, board[MAX_N][MAX_N], ans;
vector<int> dice{1, 6, 2, 3, 5, 4};
int diceX, diceY, diceDir = RIGHT;

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void MoveDice(int dir)
{
	vector<int> nxtDice(6, 0);

	if (dir == LEFT)
		nxtDice = { dice[3], dice[5], dice[2], dice[1], dice[4], dice[0] };
	else if (dir == RIGHT)
		nxtDice = { dice[5], dice[3], dice[2], dice[0], dice[4], dice[1] };
	else if (dir == TOP)
		nxtDice = { dice[2], dice[4], dice[1], dice[3], dice[0], dice[5] };
	else if (dir == DOWN)
		nxtDice = { dice[4], dice[2], dice[0], dice[3], dice[1], dice[5] };

	dice = nxtDice;
}

int GetScore(int sx, int sy)
{
	int ret = 0;
	bool vis[MAX_N][MAX_N] = { false, };
	queue<pii> q;
	q.push({ sx, sy });
	vis[sx][sy] = true;

	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();
		ret++;

		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];

			if (!OOM(nx, ny) && !vis[nx][ny] && board[sx][sy] == board[nx][ny])
			{
				q.push({ nx, ny });
				vis[nx][ny] = true;
			}
		}
	}

	return board[sx][sy] * ret;
}

void ChangeDir()
{
	if (dice[1] > board[diceX][diceY])
		diceDir = (diceDir + 1) % 4;
	else if (dice[1] < board[diceX][diceY])
		diceDir = (diceDir + 3) % 4;
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> N >> M;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];

	while (M--)
	{
		MoveDice(diceDir);
		diceX += dx[diceDir];
		diceY += dy[diceDir];

		ans += GetScore(diceX, diceY);
		ChangeDir();
	}
	
	cout << ans;
	return 0;
}