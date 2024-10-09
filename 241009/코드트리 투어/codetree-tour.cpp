/**
 * @author         lupin
 * @date           2024-10-09
 *
 * @submit         00:35:33
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
const int INF = 987654321;

int N;
typedef struct Info { int revenue, dest; };
vector<pii> adj[MAX_N];
unordered_map<int, Info> product;
int dist[MAX_N];

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
	product[id] = { revenue, dest };
}

void Q300()
{
	int id;
	cin >> id;

	if (product.find(id) != product.end())
		product.erase(id);
}

void Q400()
{
	int id = 0;
	int cmp = -1;

	for (auto [key, value] : product)
	{
		int distance = dist[value.dest];
		int revenue = value.revenue;
		int benefit = revenue - distance;

		if (benefit < 0 || distance == INF) continue;
		if (cmp < benefit)
		{
			cmp = benefit;
			id = key;
		}
		else if (cmp == benefit && id > key)
			id = key;
	}

	if (id > 0)
	{
		cout << id << '\n';
		product.erase(id);
	}
	else cout << -1 << '\n';
}

void Dijkstra(int s)
{
	priority_queue<pii> pq;

	fill(&dist[0], &dist[N], INF);
	dist[s] = 0;

	pq.push(make_pair(0, s));

	while (!pq.empty())
	{
		auto [d, u] = pq.top();
		d *= -1;
		pq.pop();

		if (d > dist[u]) continue;

		for (int v = 0; v < N; v++)
		{
			for (auto [v, w] : adj[u])
			{
				if (dist[u] + w < dist[v])
				{
					dist[v] = dist[u] + w;
					pq.push(make_pair(-dist[v], v));
				}
			}
		}
	}
}

void Q500()
{
	int s;
	cin >> s;
	Dijkstra(s);
}

int main(void) {
	ios::sync_with_stdio(false);
	cin.tie(NULL);

	// auto f = freopen("input.txt", "r", stdin);

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
		else if (cmd == 300) Q300();
		else if (cmd == 400) Q400();
		else if (cmd == 500) Q500();
	}

	return 0;
}