#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

const int left_adj = 6, right_adj = 2;
deque<char> chair[4];
int k;

void rotate(vector<pii> stat) {
	for (auto nxt : stat) {
		int n = nxt.first;
		int d = nxt.second;
		auto cur = chair[n];
		

		if (d == -1) {
			char tmp = cur.front();
			cur.pop_front();
			cur.push_back(tmp);
		}
		else {
			char tmp = cur.back();
			cur.pop_back();
			cur.push_front(tmp);
		}

		chair[n] = cur;
	}
}

void solution(int n, int d) {
	vector<pii> stat;
	int dir = d;

	stat.push_back({ n, d });

	for (int i = n - 1; i >= 0; i--) {
		if (chair[i + 1][left_adj] != chair[i][right_adj]) {
			dir *= -1;
			stat.push_back({ i, dir });
		}
	}

	dir = d;
	for (int i = n + 1; i < 4; i++) {
		if (chair[i - 1][right_adj] != chair[i][left_adj]) {
			dir *= -1;
			stat.push_back({ i, dir });
		}
	}

	rotate(stat);
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	for (int i = 0; i < 4; i++) {
		string s;
		cin >> s;
		for (auto nxt : s) chair[i].push_back(nxt);
	}
	cin >> k;

	for (int n, d; k--;) {
		cin >> n >> d;
		n--;

		solution(n, d);
	}

	int ans = 0;
	int score[4] = { 1, 2, 4, 8 };
	for (int i = 0; i < 4; i++)
		if (chair[i][0] == '1') ans += score[i];
	
	cout << ans;
	return 0;
}