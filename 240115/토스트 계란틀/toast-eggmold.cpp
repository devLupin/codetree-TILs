#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

bool vis[50][50];
int n, L, R, A[50][50];

const int dx[] = { -1,1,0,0 };
const int dy[] = { 0,0,-1,1 };

bool oom(int x, int y) { return x < 0 || y < 0 || x >= n || y >= n; }

vector<pii> bfs(int x, int y) {
    vector<pii> ret;

    queue<pii> q;
    q.push({ x, y });
    vis[x][y] = true;
    ret.push_back({ x, y });

    while (!q.empty()) {
        pii cur = q.front();
        q.pop();

        for (int dir = 0; dir < 4; dir++) {
            int nx = cur.X + dx[dir];
            int ny = cur.Y + dy[dir];

            if (oom(nx, ny) || vis[nx][ny]) continue;

            int diff = abs(A[cur.X][cur.Y] - A[nx][ny]);
            if (diff >= L && diff <= R) {
                q.push({ nx, ny });
                vis[nx][ny] = true;
                ret.push_back({ nx, ny });
            }
        }
    }

    return ret;
}

void update(vector<vector<pii>> pos) {
    for (auto cur : pos) {
        int sum = 0;

        for (pii nxt : cur) sum += A[nxt.X][nxt.Y];
        sum /= (int)cur.size();

        for (pii nxt : cur) A[nxt.X][nxt.Y] = sum;
    }
}

bool solve() {
    vector<vector<pii>> pos;

    for (int i = 0; i < n; i++)
        memset(vis[i], false, sizeof(vis[i]));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (!vis[i][j]) {
                vector<pii> tmp = bfs(i, j);
                if (tmp.size() >= 2)
                    pos.push_back(tmp);
            }
        }
    }

    if (pos.empty()) return false;

    update(pos);
    return true;
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> L >> R;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> A[i][j];

    int ans = 0;
    while (solve()) ans++;
    cout << ans;

    return 0;
}