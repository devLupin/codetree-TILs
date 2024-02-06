#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

const int SZ = 35;

int ans, board[SZ], route[SZ], turn[SZ], dice[10], piece[4];
bool vis[SZ];

void init() {
	int num = 2;
	for (int i = 1; i <= 20; i++) {
		board[i] = num;
		num += 2;
		route[i] = i + 1;
	}
	route[0] = 1;

	board[21] = -1;  // 도착점

	// 테두리 외의 값
	board[22] = 13;
	board[23] = 16;
	board[24] = 19;

	board[25] = 28;
	board[26] = 27;
	board[28] = 26;
	
	board[29] = 22;
	board[30] = 24;
	board[31] = 25;
	board[32] = 30;
	board[33] = 35;

	for (int i = 22; i <= 33; i++)
		route[i] = i + 1;
	
	// 테두리 외의 연결점
	route[24] = 31;
	route[28] = 31;
	route[33] = 20;

	// 파란점
	turn[5] = 22;
	turn[10] = 29;
	turn[15] = 25;
}

void solution(int depth, int sum) {
	if (depth == 10) {
		ans = max(ans, sum);
		return;
	}

	for (int i = 0; i < 4; i++) {
		int prev = piece[i];
		int nxt = prev;
		int move = dice[depth];

		if (turn[nxt] > 0) {
			nxt = turn[nxt];
			move--;
		}

		while (move--) nxt = route[nxt];

		if (nxt != 21 && vis[nxt]) continue;

		vis[prev] = false;
		vis[nxt] = true;
		piece[i] = nxt;

		solution(depth + 1, sum + board[nxt]);

		vis[prev] = true;
		vis[nxt] = false;
		piece[i] = prev;
	}
}

int main() {
	ios::sync_with_stdio(0); 
	cin.tie(0);

	// freopen("input.txt", "r", stdin);

	for (int i = 0; i < 10; i++) cin >> dice[i];
	
	init();
	solution(0, 0);

	cout << ans;
	return 0;
}