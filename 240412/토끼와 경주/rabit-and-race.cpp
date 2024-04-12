#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using LL = long long;

const int MAX_P = 2000;
const int DIR_NUM = 4;

const int dx[DIR_NUM] = { -1,1,0,0 };
const int dy[DIR_NUM] = { 0,0,-1,1 };

int Q, N, M, P, K, S;

pii pos[MAX_P];
int id[MAX_P];
unordered_map<int, int> id2idx;
int jump_cnt[MAX_P];
int pw[MAX_P];

LL score[MAX_P];

struct Rabbit {
	int x, y, pid, jcnt;

	Rabbit(int x, int y, int pid, int jcnt) 
		: x(x), y(y), pid(pid), jcnt(jcnt) {}

	bool operator<(const Rabbit& tmp) const {
		if (jcnt != tmp.jcnt) return jcnt > tmp.jcnt;
		if ((x + y) != (tmp.x + tmp.y)) return (x + y) > (tmp.x + tmp.y);
		if (x != tmp.x) return x > tmp.x;
		if (y != tmp.y) return y > tmp.y;
		return pid > tmp.pid;
	}
};


bool OOM(int x, int y) { return x < 1 || y < 1 || x > N || y > M; }

void Init() {
	cin >> N >> M >> P;

	for (int i = 0; i < P; i++) {
		cin >> id[i] >> pw[i];
		id2idx[id[i]] = i;
		pos[i] = { 1,1 };
	}
}

Rabbit Move(Rabbit cur, int dir, int idx) {
	auto [x, y] = pos[idx];
	int d = pw[idx];

	x--, y--;

	if (dir == 0) x = (x + d) % (2 * (N - 1));
	else if (dir == 1) x = (x - d) % (2 * (N - 1));

	if (x >= N) x = (2 * (N - 1)) - x;


	if (dir == 2) y = (y + d) % (2 * (M - 1));
	else if (dir == 3) y = (y - d) % (2 * (M - 1));

	if (y >= M) y = 2 * (M - 1) - y;

	return Rabbit(x + 1, y + 1, id[idx], jump_cnt[idx]);
}

bool Compare(const Rabbit& a, const Rabbit& b) {
	if (a.x + a.y != b.x + b.y) return a.x + a.y < b.x + b.y;
	if (a.x != b.x) return a.x < b.x;
	return a.y < b.y;
}

void Run() {
	priority_queue<Rabbit> pq;
	Rabbit lst(0, 0, 0, 0);
	bool vis[MAX_P] = { false, };

	cin >> K >> S;

	for (int i = 0; i < P; i++) {
		Rabbit new_rabbit = Rabbit(pos[i].X, pos[i].Y, id[i], jump_cnt[i]);
		pq.push(new_rabbit);
	}

	while (K--) {
		Rabbit cur = pq.top();
		pq.pop();

		Rabbit nxt = Rabbit(1, 1, cur.pid, cur.jcnt);
		int idx = id2idx[cur.pid];

		for (int dir = 0; dir < DIR_NUM; dir++) {
			Rabbit cmp = Move(cur, dir, idx);
			if (Compare(nxt, cmp)) nxt = cmp;
		}

		for (int i = 0; i < P; i++)
			score[i] += (nxt.x + nxt.y);
		score[idx] -= (nxt.x + nxt.y);

		nxt.jcnt++;
		pq.push(nxt);
		vis[idx] = true;

		pos[idx] = { nxt.x, nxt.y };
		jump_cnt[idx]++;
	}

	int idx = -1;
	Rabbit cmp = Rabbit(0, 0, -1, -1);

	while (!pq.empty()) {
		Rabbit cur = pq.top();
		int c_idx = id2idx[cur.pid];
		pq.pop();

		if (Compare(cmp, cur) && vis[c_idx]) {
			cmp = cur;
			idx = c_idx;
		}
	}

	score[idx] += S;
}

void Adjust() {
	int pid_t, L;
	cin >> pid_t >> L;
	
	pw[id2idx[pid_t]] *= L;
}

void Answer() {
	cout << *max_element(score, score + P) << '\n';
}

struct cur {
	int x;
	bool operator<(const cur& nxt)const {
		return x < nxt.x;
	}
};

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);

	cin >> Q;
	for (int cmd; Q--;) {
		cin >> cmd;

		if (cmd == 100) Init();
		else if (cmd == 200) Run();
		else if (cmd == 300) Adjust();
		else if (cmd == 400) Answer();
	}

	return 0;
}