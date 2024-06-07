#define _CRT_SECURE_NO_WARNINGS
#include <queue>
#include <iostream>
#include <algorithm>
#include <unordered_map>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int INF = 987654321;
const int SZ = 2000;

struct Info {
	int revenue, dest, dist;
};

unordered_map<int, Info> product;
unordered_map<int, int> pid;

int Q, N, M;
vector<pii> adj[SZ];
vector<int> cost_id;

void dijkstra(int st) {
	vector<int> dist(N, INF);
	priority_queue<pii> pq;

	dist[st] = 0;
	pq.push({ 0, st });

	while (!pq.empty()) {
		int cur_dist = -pq.top().first;
		int cur_node = pq.top().second;
		pq.pop();

		if (cur_dist > dist[cur_node]) continue;

		for (const pii& nxt : adj[cur_node]) {
			int nxt_node = nxt.first;
			int nxt_dist = cur_dist + nxt.second;

			if (nxt_dist < dist[nxt_node]) {
				dist[nxt_node] = nxt_dist;
				pq.push({ -nxt_dist, nxt_node });
			}
		}
	}

	cost_id = dist;
}

void q100() {
	cin >> N >> M;
	for (int u, v, w, i = 0; i < M; i++) {
		cin >> u >> v >> w;
		adj[u].push_back({ v, w });
		adj[v].push_back({ u, w });
	}
}

void q200() {
	int id, revenue, dest;
	cin >> id >> revenue >> dest;
	
	product[id] = { revenue, dest, cost_id[dest] };
	pid[id]++;
}

void q300(int id) { if (pid[id] > 0) pid[id]--; }

bool cmp(const pair<int, Info>& a, const pair<int, Info>& b) {
	int v1 = a.second.revenue - a.second.dist;
	int v2 = b.second.revenue - b.second.dist;

	if (v1 != v2) return v1 > v2;
	return a.first < b.first;
}

void q400() {
	vector<pair<int, Info>> v(product.begin(), product.end());
	sort(v.begin(), v.end(), cmp);

	for (auto& nxt : v) {
		Info info = nxt.second;
		int id = nxt.first;

		if (pid[id] > 0 && info.dest != INF && info.dist <= info.revenue) {
			q300(id);
			cout << id << '\n';
			return;
		}
	}

	cout << -1 << '\n';
}

void q500() {
	int s;
	cin >> s;
	dijkstra(s);

	for (auto& item : product)
		item.second.dist = cost_id[item.second.dest];
}

int main(void) {
	// freopen("input.txt", "r", stdin);
	cin >> Q;

	for (int cmd; Q--;) {
		cin >> cmd;

		if (cmd == 100) {
			q100();
			dijkstra(0);
		}
		else if (cmd == 200) q200();
		else if (cmd == 300) {
			int id; 
			cin >> id;
			q300(id);
		}
		else if (cmd == 400) q400();
		else if (cmd == 500) q500();
	}

	return 0;
}