/**
 * @author         lupin
 * @date           2024-10-09
 *
 * @submit         00:35:33
 * @revision       01:13:02
 */


#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#pragma warning(suppress : 6031)
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int MAX_N = 2000;
const int MAX_ID = 30001;
const int INF = 987654321;

int N;
struct Info
{
	int id, revenue, dest, d;

	bool operator<(const Info& other) const
	{
		if (revenue - d != other.revenue - other.d)
			return revenue - d < other.revenue - other.d;
		return id > other.id;
	}
};
vector<pii> adj[MAX_N];
priority_queue<Info> product;
int dist[MAX_ID + 2];
unordered_map<int, int> isExist;

void Q100()
{
	int M, u, v, w;
	cin >> N >> M;
	while (M--)
	{
		cin >> u >> v >> w;
		adj[u].push_back(make_pair(v, w));
		adj[v].push_back(make_pair(u, w));
	}
}

void Q200()
{
	int id, revenue, dest;
	cin >> id >> revenue >> dest;
	product.push({ id, revenue, dest, dist[dest] });
	isExist[id] = 1;
}

void Q300(int id)
{
	isExist[id] = 0;
}

void Q400()
{
	while (!product.empty()) 
	{
		Info cur = product.top();

		if (cur.revenue - cur.d < 0) break;
		product.pop();

		if (isExist[cur.id] > 0) {
			Q300(cur.id);
			cout << cur.id << '\n';
			return;
		}
	}

	cout << -1 << '\n';
}

void Dijkstra(int s)
{
	priority_queue<pii> pq;

	fill(&dist[0], &dist[N], INF);
	dist[s] = 0;

	pq.push({ 0, s });

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
}

void Q500()
{
	int s;
	cin >> s;
	Dijkstra(s);

	vector<Info> v;

	while (!product.empty())
	{
		v.push_back(product.top());
		product.pop();
	}

	for (Info& nxt : v)
	{
		nxt.d = dist[nxt.dest];
		product.push(nxt);
	}
}

int main(void)
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	//  auto f = freopen("input.txt", "r", stdin);

	int Q;
	cin >> Q;

	for (int cmd; Q--;)
	{
		cin >> cmd;

		if (cmd == 100)
		{
			Q100();
			Dijkstra(0);
		}
		else if (cmd == 200) Q200();
		else if (cmd == 300)
		{
			int id;
			cin >> id;
			Q300(id);
		}
		else if (cmd == 400) Q400();
		else if (cmd == 500) Q500();
	}

	return 0;
}