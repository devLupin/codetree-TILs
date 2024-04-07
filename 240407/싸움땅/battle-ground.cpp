#define _CRT_SECURE_NO_WARNINGS
#include <bits/stdc++.h>
#define X first
#define Y second
using namespace std;
using pii = pair<int, int>;

const int SZ = 25;
// ↑, →, ↓, ←
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };
int N, M, K;
vector<int> board[SZ][SZ];
int pos[SZ][SZ];
vector<int> ans;

struct Info { int x, y, d, s, gun; };
vector<Info> player;

void Input() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // freopen("input.txt", "r", stdin);

    cin >> N >> M >> K;

    ans.assign(M + 1, 0);
    player.assign(M + 1, {});

    for (int i = 0; i < N; i++) {
        for (int x, j = 0; j < N; j++) {
            cin >> x;
            board[i][j].push_back(x);
        }
    }

    for (int x, y, d, s, i = 1; i <= M; i++) {
        cin >> x >> y >> d >> s;
        x--, y--;
        pos[x][y] = i;
        player[i] = { x, y, d, s, 0 };
    }
}

bool OOM(int x, int y) { return x < 0 || y < 0 || x >= N || y >= N; }

void MoveFight(int idx, int nx, int ny) {
    pos[nx][ny] = idx;
    player[idx].x = nx;
    player[idx].y = ny;
}

void GetGun(int idx, int nx, int ny) {
    Info& cur = player[idx];

    if (!board[nx][ny].empty()) {
        int idx = max_element(board[nx][ny].begin(), board[nx][ny].end()) - board[nx][ny].begin();

        if (board[nx][ny][idx] > 0 && cur.gun < board[nx][ny][idx]) {
            if (cur.gun > 0) swap(cur.gun, board[nx][ny][idx]);
            else {
                cur.gun = board[nx][ny][idx];
                board[nx][ny].erase(board[nx][ny].begin() + idx);
            }
        }
    }
}

void Fight(int p1, int p2, int nx, int ny) {
    Info& cur = player[p1];
    Info& nxt = player[p2];
    int diff = 0;
    int winner, losser;

    pos[cur.x][cur.y] = 0;
    pos[nxt.x][nxt.y] = 0;

    if (cur.s + cur.gun > nxt.s + nxt.gun) {
        diff = (cur.s + cur.gun) - (nxt.s + nxt.gun);
        winner = p1;
        losser = p2;
    }
    else if (cur.s + cur.gun < nxt.s + nxt.gun) {
        diff = (nxt.s + nxt.gun) - (cur.s + cur.gun);
        winner = p2;
        losser = p1;
    }
    else {
        if (cur.s > nxt.s) {
            winner = p1;
            losser = p2;
        }
        else {
            winner = p2;
            losser = p1;
        }
    }

    int lx = nx;
    int ly = ny;
    int& dir = player[losser].d;
    int lnx, lny;

    for (int i = 0; i < 4; i++) {
        dir = (dir + i) % 4;
        lnx = lx + dx[dir];
        lny = ly + dy[dir];

        if (!OOM(lnx, lny) && pos[lnx][lny] == 0) break;
    }

    if (player[winner].gun > 0) {
        board[nx][ny].push_back(player[winner].gun);
        player[winner].gun = 0;
    }
    if (player[losser].gun > 0) {
        board[nx][ny].push_back(player[losser].gun);
        player[losser].gun = 0;
    }

    MoveFight(winner, nx, ny);
    GetGun(winner, nx, ny);
    MoveFight(losser, lnx, lny);
    GetGun(losser, lnx, lny);

    ans[winner] += diff;
}

void MovePlayer() {
    for (int i = 1; i <= M; i++) {
        Info& cur = player[i];
        int x = cur.x;
        int y = cur.y;

        int nx = x + dx[cur.d];
        int ny = y + dy[cur.d];

        if (OOM(nx, ny)) {
            cur.d = (cur.d + 2) % 4;
            nx = x + dx[cur.d];
            ny = y + dy[cur.d];
        }

        // fight
        if (pos[nx][ny] > 0)
            Fight(pos[cur.x][cur.y], pos[nx][ny], nx, ny);

        else {
            GetGun(i, nx, ny);
            swap(pos[x][y], pos[nx][ny]);
            cur.x = nx;
            cur.y = ny;
        }
    }
}

int main(void) {
    Input();
    while (K--) MovePlayer();


    for (int i = 1; i <= M; i++) cout << ans[i] << ' ';
    return 0;
}