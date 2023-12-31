#include <iostream>
#include <queue>
#include <cstring>
using namespace std;
using pii = pair<int, int>;

const int SZ = 25, BOTTOM = 2;
const int dy[] = { 0,1,0,-1 };
const int dx[] = { 1,0,-1,0 };
int board[SZ][SZ];
bool vis[SZ][SZ];
int dice[6] = { 1, 2, 6, 5, 4, 3 };
int dir = 0, ans;
pii dice_pos = { 0,0 };

int n, m;

bool oom(int y, int x) { return y < 0 || x < 0 || y >= n || x >= n; }

void move_dice() {
    int tmp;

    if (dir == 0) {
        tmp = dice[5];
        dice[5] = dice[0];
        dice[0] = dice[4];
        dice[4] = dice[2];
        dice[2] = tmp;
    }
    else if (dir == 1) {
        tmp = dice[1];
        dice[1] = dice[0];
        dice[0] = dice[3];
        dice[3] = dice[2];
        dice[2] = tmp;
    }
    else if (dir == 2) {
        tmp = dice[4];
        dice[4] = dice[0];
        dice[0] = dice[5];
        dice[5] = dice[2];
        dice[2] = tmp;
    }
    else {
        tmp = dice[3];
        dice[3] = dice[0];
        dice[0] = dice[1];
        dice[1] = dice[2];
        dice[2] = tmp;
    }
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
        ans += target;

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

void set_dir() {
    int v = board[dice_pos.first][dice_pos.second];

    if (dice[BOTTOM] > v) dir = (dir + 1) % 4;
    else if (dice[BOTTOM] < v) dir = (dir + 3) % 4;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n >> m;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> board[i][j];

    while (m--) {
        if (oom(dice_pos.first + dy[dir], dice_pos.second + dx[dir])) dir = (dir + 2) % 4;
        dice_pos.first += dy[dir];
        dice_pos.second += dx[dir];

        int num = adj();

        move_dice();
        set_dir();
    }
    cout << ans;

    return 0;
}