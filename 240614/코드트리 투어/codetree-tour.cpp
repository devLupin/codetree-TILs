#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
using namespace std;
using pii = pair<int, int>;

const int MAX_N = 2000;
const int INF = 987654321;

struct Info {
	int id, revenue, dest, dist;

	Info(int a, int b, int c, int d) {
		id = a;
		revenue = b;
		dest = c;
		dist = d;
	}

	bool operator<(const Info& cmp) const {
		if (revenue - dist != cmp.revenue - cmp.dist)
			return revenue - dist < cmp.revenue - cmp.dist;
		return id > cmp.id;
	}
};

int Q, cmd, N, M, id;
vector<pii> adj[MAX_N];
unordered_map<int, int> pid;
priority_queue<Info> que;
vector<int> cost;

void dijkstra(int st) {
	priority_queue<pii> pq;

	cost.assign(N, INF);
	cost[st] = 0;

	pq.push({ 0, st });

	while (!pq.empty()) {
		auto [cur_dist, cur_node] = pq.top();
		cur_dist *= -1;
		pq.pop();

		if (cost[cur_node] < cur_dist) continue;

		for (auto& [nxt_node, nxt_dist] : adj[cur_node]) {
			nxt_dist += cur_dist;
			
			if (cost[nxt_node] > nxt_dist) {
				cost[nxt_node] = nxt_dist;
				pq.push({ -nxt_dist, nxt_node });
			}
		}
	}
}

void q100() {
	cin >> N >> M;
	for (int v, u, w, i = 0; i < M; i++) {
		cin >> v >> u >> w;
		adj[v].push_back({ u, w });
		adj[u].push_back({ v, w });
	}
}

void q200() {
	int id, revenue, dest;
	cin >> id >> revenue >> dest;
	que.push({ id, revenue, dest, cost[dest] });
	pid[id]++;
}

void q300(int id) {
	if (pid[id] > 0) pid[id] = 0;
}

void q400() {
	while (!que.empty()) {
		Info cur = que.top();

		if (cur.dist > cur.revenue) break;
		que.pop();

		if (pid[cur.id] > 0) {
			cout << cur.id << '\n';
			q300(cur.id);
			return;
		}
	}
	cout << "-1" << '\n';
}

void q500() {
	vector<Info> v;
	int s;
	cin >> s;

	while (!que.empty()) {
		v.push_back(que.top());
		que.pop();
	}

	dijkstra(s);

	for (auto& nxt : v) {
		nxt.dist = cost[nxt.dest];
		que.push(nxt);
	}
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// freopen("input.txt", "r", stdin);
	
	cin >> Q;
	while (Q--) {
		cin >> cmd;

		if (cmd == 100) {
			q100();
			dijkstra(0);
		}
		else if (cmd == 200) q200();
		else if (cmd == 300) {
			cin >> id;
			q300(id);
		}
		else if (cmd == 400) q400();
		else if (cmd == 500) q500();

	}
	
	return 0;
}