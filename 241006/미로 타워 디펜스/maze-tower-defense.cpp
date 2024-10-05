/**
 * @author         lupin
 * @date           2024-10-05
 *
 * @submit         01:12:57
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#pragma warning(suppress : 6031)
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int dx[] = { 0,1,0,-1 };
const int dy[] = { 1,0,-1,0 };
const int ddx[] = { 0,1,0,-1 };
const int ddy[] = { -1,0,1,0 };

int N, M, D, P, ans;
int centerX, centerY;
vector<vector<int>> board;
vector<int> vec;

void Print()
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			cout << board[i][j] << ' ';
		cout << '\n';
	}
	cout << "\n\n";
}

void Attack()
{
	auto [x, y] = make_pair(centerX, centerY);

	while (P--)
	{
		x += dx[D];
		y += dy[D];

		ans += board[x][y];
		board[x][y] = 0;
	}
}

void GetVector()
{
	vec.clear();

	auto [x, y] = make_pair(centerX, centerY);
	int cnt = 0;
	int cCnt = 1;
	int dirCnt = 0;
	int ddir = 0;

	while (make_pair(x, y) != make_pair(0, 0))
	{
		x += ddx[ddir];
		y += ddy[ddir];

		if (board[x][y] > 0) vec.push_back(board[x][y]);

		cnt++;
		if (cnt == cCnt)
		{
			ddir = (ddir + 1) % 4;
			cnt = 0;
			dirCnt++;
			if (dirCnt == 2) 
			{
				dirCnt = 0;
				cCnt++;
			}
		}
	}
}

bool RemoveDup()
{
	if (vec.empty()) return false;

	vector<int> v;
	int cmp = vec[0];
	int st = 0;
	int sz = vec.size();

	for (int i = 0; i < sz; i++)
	{
		if (cmp != vec[i])
		{
			if (i - st > 3)
			{
				for (int j = st; j < i; j++)
					ans += vec[j];
			}
			else
			{
				for (int j = st; j < i; j++)
					v.push_back(vec[j]);
			}
			cmp = vec[i];
			st = i;
		}
	}

	if (sz - st > 3)
	{
		for (int i = st; i < sz; i++)
			ans += vec[i];
	}
	else
	{
		for (int i = st; i < sz; i++)
			v.push_back(vec[i]);
	}

	if (vec.size() != v.size())
	{
		vec = v;
		return true;
	}

	return false;
}

void Pairing()
{
	vector<int> v;
	int sz = vec.size();
	int cmp = vec[0];
	int st = 0;

	for (int i = 0; i < sz; i++)
	{
		if (cmp != vec[i])
		{
			v.push_back(i - st);
			v.push_back(cmp);
			cmp = vec[i];
			st = i;
		}
	}

	v.push_back(sz - st);
	v.push_back(vec[sz - 1]);

	vec = v;
}

void FillBoard()
{
	auto [x, y] = make_pair(centerX, centerY);
	int cnt = 0;
	int cCnt = 1;
	int dirCnt = 0;
	int ddir = 0;
	int sz = min(N * N - 1, (int)vec.size());
	int idx = 0;

	board.assign(N, vector<int>(N, 0));

	while (make_pair(x, y) != make_pair(0, 0) && idx < sz)
	{
		x += ddx[ddir];
		y += ddy[ddir];

		board[x][y] = vec[idx++];

		cnt++;
		if (cnt == cCnt)
		{
			ddir = (ddir + 1) % 4;
			cnt = 0;
			dirCnt++;
			if (dirCnt == 2)
			{
				dirCnt = 0;
				cCnt++;
			}
		}
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

	cin >> N >> M;
	board.assign(N, vector<int>(N, 0));
	tie(centerX, centerY) = make_pair(N / 2, N / 2);

	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			cin >> board[i][j];

	while (M--)
	{
		cin >> D >> P;
		Attack();
		GetVector();
		while(RemoveDup());
		Pairing();
		FillBoard();
	}

	cout << ans;
	return 0;
}