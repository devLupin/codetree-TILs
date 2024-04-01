#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
using namespace std;

const int SZ = 25;
// 좌, 위, 우, 아래, 좌측위, 우측위, 우측아래, 좌측아래
const int dx[] = { 0,-1,0,1,-1,-1,1,1 };
const int dy[] = { -1,0,1,0,-1,1,1,-1 };

struct info { int x, y, dir; };
struct pos { int x, y, d; };
vector<info> aircon;
int N, M, K, board[SZ][SZ], A[SZ][SZ], B[SZ][SZ];
bool wall[SZ][SZ][4];
bool vis[SZ][SZ];

bool OOM(int x, int y) { return x < 1 || y < 1 || x > N || y > N; }

vector<int> GetDirs(int d) {
    vector<int> ret;

    if (d == 0) ret = { 0, 4, 7 };
    if (d == 1) ret = { 1, 4, 5 };
    if (d == 2) ret = { 2, 5, 6 };
    if (d == 3) ret = { 3, 6, 7 };

    return ret;
}

// 좌, 위, 우, 아래
// 4
// 좌측상단, 우측상단, 우측아래, 좌측아래
bool CanGo(int x, int y, int dir, int origin_dir) {

    if (dir == 4) {      // 좌측위
        int top_x = x + dx[1];
        int top_y = y + dy[1];

        if (!wall[top_x][top_y][3] && !wall[top_x][top_y][0]) return true;
    }
    else if (dir == 5) {     // 우측위
        int top_x = x + dx[1];
        int top_y = y + dy[1];

        if (!wall[top_x][top_y][3] && !wall[top_x][top_y][2]) return true;
    }
    else if (dir == 6) {     // 우측아래
        int down_x = x + dx[3];
        int down_y = y + dy[3];

        if (!wall[down_x][down_y][1] && !wall[down_x][down_y][2]) return true;
    }
    else if (dir == 7) {      // 좌측아래
        int down_x = x + dx[3];
        int down_y = y + dy[3];

        if (!wall[down_x][down_y][1] && !wall[down_x][down_y][0]) return true;
    }

    return false;
}

void Spread(int x, int y, int d) {
    auto dirs = GetDirs(d);
    queue<pos> q;
    fill(&vis[1][1], &vis[N + 1][N + 1], false);

    q.push({ x, y, 5 });
    A[x][y] += 5;
    vis[x][y] = true;

    while (!q.empty()) {
        auto cur = q.front();
        q.pop();

        if (cur.d == 0) continue;

        for (int dir : dirs) {
            int nx = cur.x + dx[dir];
            int ny = cur.y + dy[dir];

            if (OOM(nx, ny) || vis[nx][ny]) continue;

            if (dir < 4 && !wall[cur.x][cur.y][dir]) {
                q.push({ nx, ny, cur.d - 1 });
                vis[nx][ny] = true;
                A[nx][ny] += (cur.d - 1);
            }
            else if (dir >= 4 && CanGo(cur.x, cur.y, dir, d)) {
                q.push({ nx, ny, cur.d - 1 });
                vis[nx][ny] = true;
                A[nx][ny] += (cur.d - 1);
            }
        }
    }
}

void Mix() {
    fill(&vis[1][1], &vis[N + 1][N + 1], false);

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (vis[i][j]) continue;
            vis[i][j] = true;

            for (int dir : {2, 3}) {
                int nx = i + dx[dir];
                int ny = j + dy[dir];

                if (OOM(nx, ny) || wall[i][j][dir]) continue;

                int diff;

                if (A[i][j] > A[nx][ny]) {
                    diff = A[i][j] - A[nx][ny];
                    diff /= 4;
                    if (diff > 0) {
                        B[i][j] -= diff;
                        B[nx][ny] += diff;
                    }
                }
                else {
                    diff = A[nx][ny] - A[i][j];
                    diff /= 4;
                    if (diff > 0) {
                        B[nx][ny] -= diff;
                        B[i][j] += diff;
                    }
                }
            }
        }
    }

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            A[i][j] += B[i][j];
            B[i][j] = 0;
        }
    }
}

void Decrease() {
    fill(&vis[1][1], &vis[N + 1][N + 1], false);

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (vis[i][j]) continue;

            if (i == 1 || j == 1 || i == N || j == N) {
                if (A[i][j] > 0) A[i][j]--;
                vis[i][j] = true;
            }
        }
    }
}

bool Calc() {
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            if (board[i][j] == 1 && A[i][j] < K) return false;
    return true;
}

void Print() {
    cout << "\n\n";
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++)
            cout << A[i][j] << ' ';
        cout << '\n';
    }
}

int main(void) {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // freopen("input.txt", "r", stdin);

    cin >> N >> M >> K;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            int cur;
            cin >> cur;
            board[i][j] = cur;

            if (cur >= 2)
                aircon.push_back({ i, j, cur - 2 });
        }
    }

    for (int x, y, s; M--;) {
        cin >> x >> y >> s;

        if (s == 0) {
            wall[x][y][1] = true;
            wall[x - 1][y][3] = true;
        }
        if (s == 1) {
            wall[x][y][0] = true;
            wall[x][y - 1][2] = true;
        }
    }

    int cnt = 0, ans = -1;
    while (cnt++ <= 16) {
        for (auto& nxt : aircon) {
            int dir = nxt.dir;
            int nx = nxt.x + dx[dir];
            int ny = nxt.y + dy[dir];

            Spread(nx, ny, dir);
        }
        Mix();
        Decrease();

        if (Calc()) {
            ans = cnt;
            break;
        }
    }

    cout << ans;
    return 0;
}