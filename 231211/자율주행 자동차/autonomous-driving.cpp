#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second

using namespace std;
using pii = pair<int, int>;
using tiii = tuple<int, int, int>;

const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

int n, m, sx, sy, sd, A[55][55];
bool vis[55][55];

int bfs() {
    int ans = 1;

    queue<tiii> q;
    q.push({ sx, sy, sd });
    vis[sx][sy] = true;

    while (!q.empty()) {
        auto [x, y, d] = q.front();
        q.pop();

        bool chk = false;
        for (int dir = 0; dir < 4 && !chk; dir++) {
            int nd = (d + 3 - dir) % 4;
            int nx = x + dx[nd];
            int ny = y + dy[nd];

            if (!vis[nx][ny] && A[nx][ny] == 0) {
                q.push({ nx,ny,nd });
                vis[nx][ny] = true;
                chk = true;
                ans++;
            }
        }

        if (!chk) {
            int nd = (d + 2) % 4;
            int nx = x + dx[nd];
            int ny = y + dy[nd];

            if (A[nx][ny] != 1) {
                q.push({ nx, ny, d });
                if (!vis[nx][ny]) {
                    vis[nx][ny] = true;
                    ans++;
                }
            }
        }
    }

    return ans;
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // freopen("input.txt", "r", stdin);

    cin >> n >> m >> sx >> sy >> sd;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> A[i][j];

    cout << bfs();

    return 0;
}