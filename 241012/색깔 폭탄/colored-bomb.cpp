#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#pragma warning(suppress : 6031)
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;
 
enum { BLACK = -1, RED = 0, EMPTY = 6 };
 
const int MAX_N = 25;
const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };
 
int N, M, board[MAX_N][MAX_N], ans;
 
void Print()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (board[i][j] == BLACK) cout << 'B' << ' ';
			else if (board[i][j] == RED) cout << 'R' << ' ';
			else if (board[i][j] == EMPTY) cout << ' ' << ' ';
			else cout << board[i][j] << ' ';
		}
		cout << '\n';
	}
	cout << "\n\n";
}
 
bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }
 
pair<tiii, vector<pii>> BFS(int sx, int sy)
{
	vector<pii> v;
	queue<pii> q;
	bool vis[MAX_N][MAX_N] = { false, };
 
	q.push(make_pair(sx, sy));
	vis[sx][sy] = true;
 
	int red = 0;
	int maxX = -1;
	int minY = MAX_N;
 
	while (!q.empty())
	{
		auto [x, y] = q.front();
		q.pop();
 
		if (board[x][y] == RED) red++;
		else
		{
			maxX = max(maxX, x);
			minY = min(minY, y);
		}
 
		v.push_back(make_pair(x, y));
 
		for (int dir = 0; dir < 4; dir++)
		{
			int nx = x + dx[dir];
			int ny = y + dy[dir];
 
			if (OOM(nx, ny) || vis[nx][ny]) continue;
			if (board[nx][ny] == RED || board[nx][ny] == board[sx][sy])
			{
				q.push(make_pair(nx, ny));
				vis[nx][ny] = true;
			}
		}
	}
 
	return make_pair(make_tuple(red, maxX, minY), v);
}
 
bool Compare(const pair<tiii, vector<pii>>& a, const pair<tiii, vector<pii>>& b)
{
	if (a.second.size() != b.second.size()) return a.second.size() > b.second.size();
	auto [red1, maxX1, minY1] = a.first;
	auto [red2, maxX2, minY2] = b.first;
 
	if (red1 != red2) return red1 < red2;
	if (maxX1 != maxX2) return maxX1 > maxX2;
	return minY1 < minY2;
}
 
bool RemoveBomb()
{
	vector<pair<tiii, vector<pii>>> v;
 
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (board[i][j] > RED && board[i][j] < EMPTY) 
				v.push_back(BFS(i, j));
 
	sort(v.begin(), v.end(), Compare);
 
	if (v.empty() || v[0].second.size() < 2) return false;
 
	for (const auto& [x, y] : v[0].second)
		board[x][y] = EMPTY;
 
	int sz = v[0].second.size();
	ans += sz * sz;

    return true;
}
 
void Gravity()
{
	for (int y = 0; y < N; y++)
	{
		for (int x = N - 1; x >= 0; x--)
		{
			if (board[x][y] >= RED && board[x][y] < EMPTY)
			{
				int nx = x;
				while (!OOM(nx, y) && board[nx + 1][y] == EMPTY)
				{
					swap(board[nx][y], board[nx + 1][y]);
					nx++;
				}
			}
		}
	}
}
 
void Rotate()
{
	int temp[MAX_N][MAX_N];
 
	for (int x = 0; x < N; x++)
		for (int y = 0; y < N; y++)
			temp[x][y] = board[y][N - x - 1];
 
	for (int x = 0; x < N; x++)
		for (int y = 0; y < N; y++)
			board[x][y] = temp[x][y];
}
 
int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);
 
	// auto f = freopen("input.txt", "r", stdin);
 
	cin >> N >> M;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];
 
	while (RemoveBomb())
	{
		Gravity();
		Rotate();
		Gravity();
	}
 
	cout << ans;
	return 0;
}