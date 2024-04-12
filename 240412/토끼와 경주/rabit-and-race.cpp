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

struct Rabbit {
	int x, y;
	int pid;
	int d;
	int jump_count;

	bool operator<(const Rabbit& cmp) const {
		if (jump_count != cmp.jump_count) return jump_count > cmp.jump_count;
		if (x + y != cmp.x + cmp.y) return x + y > cmp.x + cmp.y;
		if (x != cmp.x) return x > cmp.x;
		return y > cmp.y;
	}
};

unordered_map<int, Rabbit> rabbits; // (토끼 id, 토끼)
unordered_map<int, LL> score; // (토끼 id, 점수)

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= M; }

void Init() {
	cin >> N >> M >> P;

	for (int pid, d, i = 0; i < P; i++) {
		cin >> pid >> d;
		rabbits[pid] = { 0,0,pid,d,0 };
		score[pid] = 0;
	}
}

pii right(int x, int y, int d) {
	int dist = d % (2 * (M - 1));
	if (M - y - 1 < dist) {
		dist -= M - y - 1;
		if (M - 1 < dist) {
			dist -= M - 1;
			y = dist;
		}
		else y = M - 1 - dist;
	}
	else y += dist;
	return { x,y };
}

pii left(int x, int y, int d) {
	int dist = d % (2 * (M - 1));
	if (y < dist) {
		dist -= y;
		if (M - 1 < dist) {
			dist -= M - 1;
			y = M - 1 - dist;
		}
		else y = dist;
	}
	else y -= dist;
	return { x,y };
}

pii up(int x, int y, int d) {
	int dist = d % (2 * (N - 1));
	if (x < dist) {
		dist -= x;
		if (N - 1 < dist) {
			dist -= N - 1;
			x = N - 1 - dist;
		}
		else x = dist;
	}
	else x -= dist;
	return { x,y };
}

pii down(int x, int y, int d) {
	int dist = d % (2 * (N - 1));
	if (N - x - 1 < dist) {
		dist -= N - x - 1;
		if (N - 1 < dist) {
			dist -= N - 1;
			x = dist;
		}
		else x = N - 1 - dist;
	}
	else x += dist;
	return { x,y };
}

bool Compare(const pii& a, const pii& b) {
	if (a.X + a.Y != b.X + b.Y) return a.X + a.Y < b.X + b.Y;
	if (a.X != b.X) return a.X < b.X;
	return a.Y < b.Y;
}

void Run() {
	cin >> K >> S;

	priority_queue<Rabbit> pq;
	set<int> vis;
	int best;

	for (auto& nxt : rabbits)
		pq.push(nxt.second);
	
	while (K--) {
		pii pos, npos;
		auto [x, y, pid, dist, jump_count] = pq.top();
		pq.pop();

		vis.insert(pid);

		pos = left(x, y, dist);

		npos = right(x, y, dist);
		if (Compare(pos, npos)) pos = npos;

		npos = up(x, y, dist);
		if (Compare(pos, npos)) pos = npos;

		npos = down(x, y, dist);
		if (Compare(pos, npos)) pos = npos;


		for (auto& it : score)
			if (it.first != pid) it.second += pos.X + pos.Y + 2;

		rabbits[pid].x = pos.X;
		rabbits[pid].y = pos.Y;
		rabbits[pid].jump_count++;

		best = pid;
		pq.push(rabbits[pid]);
	}

	for (auto& nxt : vis)
		if (Compare({rabbits[best].x, rabbits[best].y}, {rabbits[nxt].x, rabbits[nxt].y})) best = nxt;

	score[best] += S;
}

void Adjust() {
	int pid, L;
	cin >> pid >> L;
	rabbits[pid].d *= L;
}

void Answer() {
	LL mx = 0;
	for (auto nxt : score)
		mx = max(mx, nxt.second);
	cout << mx;
}

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