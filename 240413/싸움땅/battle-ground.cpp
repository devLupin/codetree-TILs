#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <queue>
using namespace std;

// ↑, →, ↓, ←
const int dx[] = { -1,0,1,0 };
const int dy[] = { 0,1,0,-1 };

const int MAX_N = 25;
const int MAX_M = 35;
int N, M, K;

struct Player { int x, y, d, s, g; };

vector<int> gun[MAX_N][MAX_N];
vector<Player> players;
int num[MAX_N][MAX_N];
int score[MAX_M];

bool OOM(int x, int y) { return x < 1 || y < 1 || x > N || y > N; }

void GetGun(int idx) {
    auto& cur = players[idx];
    if (gun[cur.x][cur.y].empty()) return;

    if (cur.g > 0) {
        gun[cur.x][cur.y].push_back(cur.g);
        cur.g = 0;
    }

    sort(gun[cur.x][cur.y].begin(), gun[cur.x][cur.y].end());
    
    cur.g = gun[cur.x][cur.y].back();
    gun[cur.x][cur.y].pop_back();
}

void MoveLosser(int idx) {
    auto& [x, y, d, s, g] = players[idx];
    
    // 총 내려놓고
    if (g > 0) {
        gun[x][y].push_back(g);
        g = 0;
    }

    // 만약 이동하려는 칸에 다른 플레이어가 있거나 격자 범위 밖인 경우에는 
    // 오른쪽으로 90도씩 회전하여 빈 칸이 보이는 순간 이동합니다.
    for (int i = 0; i < 4; i++) {
        int dir = (d + i + 4) % 4;
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if (!OOM(nx, ny) && num[nx][ny] == 0) {
            x = nx, y = ny;
            d = dir;
            num[x][y] = idx;
            break;
        }
    }

    GetGun(idx);
}

void Fight(int p1, int p2) {
    auto& cur = players[p1];
    auto& nxt = players[p2];
    int diff = abs((cur.s + cur.g) - (nxt.s + nxt.g));
    int winn, loss;

    if (cur.s + cur.g > nxt.s + nxt.g) {
        winn = p1;
        loss = p2;
    }
    else if (cur.s + cur.g < nxt.s + nxt.g) {
        winn = p2;
        loss = p1;
    }
    else {
        if (cur.s > nxt.s) {
            winn = p1;
            loss = p2;
        }
        else {
            winn = p2;
            loss = p1;
        }
    }

    num[cur.x][cur.y] = winn;

    MoveLosser(loss);
    GetGun(winn);

    score[winn] += diff;
}

void MoveP() {
    for (int i = 1; i <= M; i++) {
        auto& [x, y, d, s, g] = players[i];

        if (OOM(x + dx[d], y + dy[d])) d = (d + 2) % 4;
        int nx = x + dx[d];
        int ny = y + dy[d];

        num[x][y] = 0;
        x = nx, y = ny;

        if (num[nx][ny] > 0)
            Fight(i, num[nx][ny]);

        else {
            GetGun(i);
            num[x][y] = i;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> M >> K;
    for (int i = 1; i <= N; i++)
        for (int g, j = 1; j <= N; j++) {
            cin >> g;
            gun[i][j].push_back(g);
        }

    players.assign(M + 1, {});
    for (int x, y, d, s, i = 1; i <= M; i++) {
        cin >> x >> y >> d >> s;
        players[i] = { x, y, d, s, 0 };
        num[x][y] = i;
    }

    while (K--) MoveP();

    for (int i = 1; i <= M; i++) cout << score[i] << ' ';
    return 0;
}