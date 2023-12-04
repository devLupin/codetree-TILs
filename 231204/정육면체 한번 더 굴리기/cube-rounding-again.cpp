#include <iostream>
#include <queue>
#include <cstring>
#include <utility>
#include <tuple>
using namespace std;
using pii = pair<int, int>;

const int SZ = 25;
const int dy[] = { 0,1,0,-1 };
const int dx[] = { 1,0,-1,0 };
int board[SZ][SZ];
bool vis[SZ][SZ];
int dir = 0, ans;
pii dice_pos = { 0,0 };

// 주사위가 놓여있는 상태 
int u = 1, f = 2, r = 3;

int n, m;

bool oom(int y, int x) { return y < 0 || x < 0 || y > n || x > n; }

void move_dice() {
    int y = dice_pos.first;
    int x = dice_pos.second;

    if(dir == 0) // 오른쪽
        tie(u, f, r) = make_tuple(7 - r, f, u);
    else if(dir == 1) // 아래쪽
        tie(u, f, r) = make_tuple(7 - f, u, r);
    else if(dir == 2) // 왼쪽
        tie(u, f, r) = make_tuple(r, f, 7 - u);
    else if(dir == 3) // 위쪽
        tie(u, f, r) = make_tuple(f, 7 - u, r);
    
    int bottom = 7 - u;

    if(bottom > board[y][x])
        dir = (dir + 1) % 4;
    else if(bottom < board[y][x])
        dir = (dir + 3) % 4;
}

int adj() {
    int y = dice_pos.first;
    int x = dice_pos.second;

    queue<pii> q;
    int target = board[y][x];
    int ret = 0;

    for (int i = 0; i < SZ; i++)
        memset(vis[i], false, sizeof(vis[i]));

    q.push({ y,x });
    vis[y][x] = true;

    while (!q.empty()) {
        pii cur = q.front();
        q.pop();
        ret += target;

        for (int dir = 0; dir < 4; dir++) {
            int ny = cur.first + dy[dir];
            int nx = cur.second + dx[dir];

            if (oom(ny, nx)) continue;
            if (vis[ny][nx] || target != board[ny][nx]) continue;

            q.push({ ny,nx });
            vis[ny][nx] = true;
        }
    }

    return ret;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> board[i][j];

    while (m--) {
        if (oom(dice_pos.first + dy[dir], dice_pos.second + dx[dir])) {
            dir = (dir < 2) ? dir+2: dir-2;
        }
        dice_pos.first += dy[dir];
        dice_pos.second += dx[dir];

        int num = adj();
        ans += num;

        move_dice();
    }
    cout << ans;

    return 0;
}