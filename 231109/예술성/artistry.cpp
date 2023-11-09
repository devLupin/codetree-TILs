#include <iostream>
#include <queue>
#include <cstring>
#include <vector>
using namespace std;
using pii = pair<int, int>;

struct pos {
    int y, x, group_num;
};

vector<int> nums_group;
vector<pos> adj[30*30];
vector<int> group_val;

int group_num, ans;
int N, A[30][30], group[30][30];
const int dy[] = { -1,1,0,0 };
const int dx[] = { 0,0,-1,1 };

void bfs(int y, int x) {
    queue<pii> q;
    bool vis[30][30] = { false, };
    int idx = group_num - 1;

    q.push({ y,x });
    vis[y][x] = true;

    group[y][x] = group_num; 
    nums_group.push_back(1);
    group_val.push_back(A[y][x]);

    while (!q.empty()) {
        pii cur = q.front();
        q.pop();

        for (int dir = 0; dir < 4; dir++) {
            int ny = cur.first + dy[dir];
            int nx = cur.second + dx[dir];

            if (ny < 0 || nx < 0 || ny >= N || nx >= N) continue;

            if (A[ny][nx] == A[y][x]) {
                if (!vis[ny][nx]) {
                    group[ny][nx] = group_num;
                    nums_group[idx]++;
                    q.push({ ny,nx });
                    vis[ny][nx] = true;
                }
            }
            else
                adj[idx].push_back({ ny,nx,-1 });
        }
    }
}

void init() {
    group_val.clear();
    group_num = 1;
    nums_group.clear();
    for (int i = 0; i < 30; i++) {
        adj[i].clear();
        memset(group[i], 0, sizeof(group[i]));
    }
}

void get_info() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (group[i][j] == 0) {
                bfs(i, j);
                group_num++;
            }
        }
    }

    int idx = group_num - 1;

    for (int i = 0; i < idx; i++) {
        for (int j = 0; j < adj[i].size(); j++) {
            auto cur = adj[i][j];
            adj[i][j].group_num = group[cur.y][cur.x];
        }
    }

    bool vis[30*30][30*30] = { false, };

    for (int i = 0; i < idx; i++) {
        for (int j = i + 1; j < idx; j++) {
            if (vis[i][j]) continue;

            int val = -1;
            int cnt = 0;
            for (auto nxt : adj[i]) {
                if (nxt.group_num == j+1) {
                    val = j;
                    cnt++;
                }
            }
            if (cnt > 0) {
                int add = (nums_group[i] + nums_group[val]);
                int mul = group_val[i] * group_val[val] * cnt;
                vis[i][val] = true;

                ans += add * mul;
            }
        }
    }

}

void rotate(int x, int y)
{
    int len = (N - 1) / 2;

    int tmp[30][30] = { 0, };

    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            tmp[j][len - 1 - i] = A[x + i][y + j];
        }
    }

    for (int i = 0; i < len; i++) {
        for (int j = 0; j < len; j++) {
            A[x + i][y + j] = tmp[i][j];
        }
    }

}
void rotate()
{
    int len = (N - 1) / 2 + 1;

    rotate(0, 0);
    rotate(0, len);
    rotate(len, 0);
    rotate(len, len);

    int tmp[30] = { 0, };
    len -= 1;

    for (int i = 0; i < len; i++)  tmp[i] = A[i][len];
    for (int i = 0; i < len; i++)  A[i][len] = A[len][N - 1 - i];
    for (int i = 0; i < len; i++)  A[len][N - 1 - i] = A[N - 1 - i][len];
    for (int i = 0; i < len; i++)  A[N - 1 - i][len] = A[len][i];
    for (int i = 0; i < len; i++)  A[len][i] = tmp[i];
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> A[i][j];

    init();
    get_info();

    for (int i = 0; i < 3; i++) {
        init();
        rotate();
        get_info();
    }
    cout << ans;

    return 0;
}